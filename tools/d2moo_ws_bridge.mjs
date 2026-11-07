#!/usr/bin/env node
/**
 * Local WebSocket + HTTP Reverse-Proxy Gateway for D2MOO 1.12a (`http://127.0.0.1:8787`)
 *
 * - `/d2net/bnet`: BNCS (port 6112) auth, account logon, realm list & realm logon, lobby chat.
 * - `/d2net/realm`: MCP (port 6113) startup, character list/create/logon (synced with D2MOO `save/*.d2s`),
 *   game list/info/create/join.
 * - `/d2net/game`: Raw binary WebSocket <-> TCP `127.0.0.1:4000` (`Game.D2MOO.exe` D2GS server).
 * - All other HTTP & WebSocket paths: Reverse-proxied to `diablo2-web` Vite server (`http://127.0.0.1:5173`),
 *   with automatic `serverHost = 127.0.0.1:8787` configuration and 1-click/auto-join into the D2MOO C++ server.
 */

import http from 'node:http'
import net from 'node:net'
import crypto from 'node:crypto'
import fs from 'node:fs'
import path from 'node:path'

const BRIDGE_HOST = process.env.D2MOO_BRIDGE_HOST || '127.0.0.1'
const BRIDGE_PORT = Number(process.env.D2MOO_BRIDGE_PORT || process.env.D2MOO_WS_PORT || 8787)
const D2GS_HOST = process.env.D2MOO_D2GS_HOST || '127.0.0.1'
const D2GS_PORT = Number(process.env.D2MOO_D2GS_PORT || 4000)
const VITE_HOST = process.env.D2MOO_VITE_HOST || '127.0.0.1'
const VITE_PORT = Number(process.env.D2MOO_VITE_PORT || 5173)

const SAVE_DIRS = [
  process.env.D2MOO_SAVE_DIR,
  'H:/git/D2MOO-112a/build-112a/bin/Debug/save',
  'H:/git/D2MOO-112a/build-112a/bin/Release/save',
].filter(Boolean)

const WS_GUID = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'

const memoryChars = new Map()
const activeGames = new Map()

activeGames.set('d2moo', {
  index: 1,
  name: 'D2MOO',
  password: '',
  description: 'D2MOO 1.12a C++ Server',
  difficulty: 0,
  maxPlayers: 8,
  levelDiff: 0xff,
  createdAt: Date.now(),
  players: [],
})

function readCString(buf, offset) {
  let end = offset
  while (end < buf.length && buf[end] !== 0) end++
  const str = buf.subarray(offset, end).toString('utf8')
  return { value: str, nextOffset: Math.min(buf.length, end + 1) }
}

function writeCString(str) {
  const bytes = Buffer.from(str || '', 'utf8')
  const out = Buffer.alloc(bytes.length + 1)
  bytes.copy(out, 0)
  out[bytes.length] = 0
  return out
}

function encodeBncsFrame(packetId, payload) {
  const totalLen = 4 + payload.length
  const out = Buffer.alloc(totalLen)
  out[0] = 0xff
  out[1] = packetId & 0xff
  out.writeUInt16LE(totalLen, 2)
  payload.copy(out, 4)
  return out
}

function encodeMcpFrame(packetId, payload) {
  const totalLen = 3 + payload.length
  const out = Buffer.alloc(totalLen)
  out.writeUInt16LE(totalLen, 0)
  out[2] = packetId & 0xff
  payload.copy(out, 3)
  return out
}

function buildCharStatString33(charClass = 1, level = 1, flags = 0x60, progression = 0) {
  const stat = Buffer.alloc(34, 0xff)
  stat[0] = 0x84
  stat[1] = 0x80
  stat[13] = ((charClass & 0x07) + 1) & 0xff
  stat[25] = Math.max(1, Math.min(99, level || 1))
  stat[26] = (flags & 0x7f) | 0x80
  stat[27] = ((progression & 0x1f) << 1) | 0x80
  stat[28] = 0x80
  stat[29] = 0x80
  stat[30] = 0x80
  stat[31] = 0x80
  stat[32] = 0x80
  stat[33] = 0x00
  return stat
}

function getCharacterList() {
  const result = new Map()
  for (const [k, v] of memoryChars.entries()) {
    result.set(k, v)
  }
  for (const dir of SAVE_DIRS) {
    try {
      if (!fs.existsSync(dir)) continue
      const files = fs.readdirSync(dir)
      for (const file of files) {
        if (!file.toLowerCase().endsWith('.d2s')) continue
        const fullPath = path.join(dir, file)
        const stat = fs.statSync(fullPath)
        if (!stat.isFile() || stat.size < 48) continue
        const buf = fs.readFileSync(fullPath)
        const magic = buf.readUInt32LE(0)
        if (magic !== 0xaa55aa55) continue
        const fallbackName = path.basename(file, path.extname(file))
        const parsedName = readCString(buf.subarray(20, 36), 0).value || fallbackName
        const name = /^[A-Za-z_-]+$/.test(parsedName) ? parsedName : fallbackName
        if (!/^[A-Za-z_-]+$/.test(name)) continue
        const flags = buf[36] || 0x20
        const progression = buf[37] || 0
        const charClass = buf[40] !== undefined && buf[40] <= 6 ? buf[40] : 1
        const level = buf[43] || 1
        result.set(name.toLowerCase(), {
          name,
          charClass,
          level,
          flags: flags | 0x20,
          progression,
        })
      }
    } catch (err) {
      console.error('[Bridge] Error scanning save dir:', err)
    }
  }
  if (result.size === 0) {
    const defaultChar = {
      name: 'WebHero',
      charClass: 1,
      level: 1,
      flags: 0x60,
      progression: 0,
    }
    memoryChars.set('webhero', defaultChar)
    result.set('webhero', defaultChar)
  }
  return Array.from(result.values())
}

class RawWebSocket {
  constructor(socket) {
    this.socket = socket
    this.rxBuf = Buffer.alloc(0)
    this.closed = false
    this.messageHandlers = []
    this.closeHandlers = []

    socket.setNoDelay(true)
    socket.on('data', chunk => this._onData(chunk))
    socket.on('close', () => this._emitClose(1000, 'socket closed'))
    socket.on('error', err => {
      this._emitClose(1006, err.message)
    })
  }

  onMessage(cb) {
    this.messageHandlers.push(cb)
  }

  onClose(cb) {
    this.closeHandlers.push(cb)
  }

  sendBinary(payload) {
    if (this.closed || this.socket.destroyed) return
    const buf = Buffer.isBuffer(payload) ? payload : Buffer.from(payload)
    const len = buf.length
    let header
    if (len < 126) {
      header = Buffer.alloc(2)
      header[0] = 0x82
      header[1] = len
    } else if (len < 65536) {
      header = Buffer.alloc(4)
      header[0] = 0x82
      header[1] = 126
      header.writeUInt16BE(len, 2)
    } else {
      header = Buffer.alloc(10)
      header[0] = 0x82
      header[1] = 127
      header.writeBigUInt64BE(BigInt(len), 2)
    }
    this.socket.write(Buffer.concat([header, buf]))
  }

  close(code = 1000, reason = '') {
    if (this.closed) return
    this.closed = true
    try {
      const reasonBuf = Buffer.from(reason, 'utf8')
      const payload = Buffer.alloc(2 + reasonBuf.length)
      payload.writeUInt16BE(code, 0)
      reasonBuf.copy(payload, 2)
      const frame = Buffer.alloc(2 + payload.length)
      frame[0] = 0x88
      frame[1] = payload.length
      payload.copy(frame, 2)
      this.socket.end(frame)
    } catch {
      this.socket.destroy()
    }
    this._emitClose(code, reason)
  }

  _emitClose(code, reason) {
    if (this._closeEmitted) return
    this._closeEmitted = true
    this.closed = true
    for (const cb of this.closeHandlers) {
      try {
        cb(code, reason)
      } catch {}
    }
  }

  _onData(chunk) {
    this.rxBuf = Buffer.concat([this.rxBuf, chunk])
    while (this.rxBuf.length >= 2) {
      const byte0 = this.rxBuf[0]
      const byte1 = this.rxBuf[1]
      const opcode = byte0 & 0x0f
      const masked = (byte1 & 0x80) !== 0
      let payloadLen = byte1 & 0x7f
      let offset = 2

      if (payloadLen === 126) {
        if (this.rxBuf.length < 4) return
        payloadLen = this.rxBuf.readUInt16BE(2)
        offset = 4
      } else if (payloadLen === 127) {
        if (this.rxBuf.length < 10) return
        payloadLen = Number(this.rxBuf.readBigUInt64BE(2))
        offset = 10
      }

      const maskLen = masked ? 4 : 0
      if (this.rxBuf.length < offset + maskLen + payloadLen) return

      const maskKey = masked ? this.rxBuf.subarray(offset, offset + 4) : null
      offset += maskLen
      const payload = Buffer.from(this.rxBuf.subarray(offset, offset + payloadLen))
      this.rxBuf = this.rxBuf.subarray(offset + payloadLen)

      if (masked && maskKey) {
        for (let i = 0; i < payload.length; i++) {
          payload[i] ^= maskKey[i & 3]
        }
      }

      if (opcode === 0x08) {
        this.close(1000, 'client closed')
        return
      } else if (opcode === 0x09) {
        if (!this.closed && !this.socket.destroyed) {
          const pong = Buffer.alloc(2 + payload.length)
          pong[0] = 0x8a
          pong[1] = payload.length
          payload.copy(pong, 2)
          this.socket.write(pong)
        }
      } else if (opcode === 0x01 || opcode === 0x02 || opcode === 0x00) {
        for (const cb of this.messageHandlers) {
          cb(payload)
        }
      }
    }
  }
}

function handleBnetConnection(ws) {
  let sawGreeting = false
  let buf = Buffer.alloc(0)
  let username = 'D2MOO'
  let serverToken = 0xdeadbeef

  ws.onMessage(chunk => {
    buf = Buffer.concat([buf, chunk])
    if (!sawGreeting && buf.length >= 1 && buf[0] === 0x01) {
      sawGreeting = true
      buf = buf.subarray(1)
    }

    while (buf.length >= 4) {
      if (buf[0] !== 0xff) {
        buf = buf.subarray(1)
        continue
      }
      const id = buf[1]
      const totalLen = buf.readUInt16LE(2)
      if (totalLen < 4 || buf.length < totalLen) break
      const payload = buf.subarray(4, totalLen)
      buf = buf.subarray(totalLen)

      switch (id) {
        case 0x00:
        case 0x25:
          break

        case 0x50: {
          const pingPayload = Buffer.alloc(4)
          pingPayload.writeUInt32LE(0x12345678, 0)
          ws.sendBinary(encodeBncsFrame(0x25, pingPayload))

          const fixed = Buffer.alloc(20)
          fixed.writeUInt32LE(0, 0)
          fixed.writeUInt32LE(serverToken, 4)
          fixed.writeUInt32LE(0, 8)
          fixed.writeBigUInt64LE(0n, 12)
          const mpqName = writeCString('ver-IX86-1.mpq')
          const formula = writeCString('A=1 B=2 C=3 4 A=A^S B=B^C C=C^A A=A^B')
          ws.sendBinary(encodeBncsFrame(0x50, Buffer.concat([fixed, mpqName, formula])))
          break
        }

        case 0x51: {
          const res = Buffer.alloc(5)
          res.writeUInt32LE(0, 0)
          res[4] = 0
          ws.sendBinary(encodeBncsFrame(0x51, res))
          break
        }

        case 0x3d: {
          if (payload.length > 20) {
            username = readCString(payload, 20).value || username
          }
          const res = Buffer.alloc(5)
          res.writeUInt32LE(0, 0)
          res[4] = 0
          ws.sendBinary(encodeBncsFrame(0x3d, res))
          break
        }

        case 0x29:
        case 0x3a: {
          if (payload.length > 28) {
            username = readCString(payload, 28).value || username
          }
          const res = Buffer.alloc(4)
          res.writeUInt32LE(0, 0)
          ws.sendBinary(encodeBncsFrame(0x3a, res))
          break
        }

        case 0x40: {
          const hdr = Buffer.alloc(12)
          hdr.writeUInt32LE(0, 0)
          hdr.writeUInt32LE(1, 4)
          hdr.writeUInt32LE(1, 8)
          const title = writeCString('D2MOO')
          const desc = writeCString('D2MOO 1.12a C++ Server')
          ws.sendBinary(encodeBncsFrame(0x40, Buffer.concat([hdr, title, desc])))
          break
        }

        case 0x3e: {
          const clientToken = payload.length >= 4 ? payload.readUInt32LE(0) : 1
          const fixed = Buffer.alloc(72)
          fixed.writeUInt32LE(clientToken, 0)
          fixed.writeUInt32LE(0, 4)
          fixed[16] = 127
          fixed[17] = 0
          fixed[18] = 0
          fixed[19] = 1
          fixed[20] = 0x17
          fixed[21] = 0xe1
          const uniqueName = writeCString(username)
          ws.sendBinary(encodeBncsFrame(0x3e, Buffer.concat([fixed, uniqueName])))
          break
        }

        case 0x0a: {
          const parsedChar = readCString(payload, 0).value
          const unique = writeCString(parsedChar || username)
          const stat = writeCString('PX2D')
          const acct = writeCString(username)
          ws.sendBinary(encodeBncsFrame(0x0a, Buffer.concat([unique, stat, acct])))
          break
        }

        case 0x0b: {
          const ch1 = writeCString('Diablo II')
          const end = Buffer.from([0])
          ws.sendBinary(encodeBncsFrame(0x0b, Buffer.concat([ch1, end])))
          break
        }

        case 0x0c: {
          const channel = payload.length > 4 ? readCString(payload, 4).value : 'Diablo II'
          const sendChatEvt = (eventId, user, text) => {
            const hdr = Buffer.alloc(24)
            hdr.writeUInt32LE(eventId, 0)
            hdr.writeUInt32LE(0, 4)
            hdr.writeUInt32LE(10, 8)
            ws.sendBinary(
              encodeBncsFrame(0x0f, Buffer.concat([hdr, writeCString(user), writeCString(text)])),
            )
          }
          sendChatEvt(0x07, username, channel)
          sendChatEvt(0x01, username, 'PX2D')
          break
        }

        case 0x0e: {
          const text = readCString(payload, 0).value
          if (text && !text.startsWith('/')) {
            const hdr = Buffer.alloc(24)
            hdr.writeUInt32LE(0x05, 0)
            hdr.writeUInt32LE(0, 4)
            hdr.writeUInt32LE(10, 8)
            ws.sendBinary(
              encodeBncsFrame(0x0f, Buffer.concat([hdr, writeCString(username), writeCString(text)])),
            )
          }
          break
        }
      }
    }
  })
}

function handleRealmConnection(ws) {
  let sawGreeting = false
  let buf = Buffer.alloc(0)
  let activeCharName = 'WebHero'

  ws.onMessage(chunk => {
    buf = Buffer.concat([buf, chunk])
    if (!sawGreeting && buf.length >= 1 && buf[0] === 0x01) {
      sawGreeting = true
      buf = buf.subarray(1)
    }

    while (buf.length >= 3) {
      const totalLen = buf.readUInt16LE(0)
      const id = buf[2]
      if (totalLen < 3 || buf.length < totalLen) break
      const payload = buf.subarray(3, totalLen)
      buf = buf.subarray(totalLen)

      switch (id) {
        case 0x01: {
          const res = Buffer.alloc(4)
          res.writeUInt32LE(0, 0)
          ws.sendBinary(encodeMcpFrame(0x01, res))
          break
        }

        case 0x19: {
          const requested = payload.length >= 4 ? payload.readUInt32LE(0) : 8
          const chars = getCharacterList().slice(0, requested)
          const hdr = Buffer.alloc(8)
          hdr.writeUInt16LE(requested, 0)
          hdr.writeUInt32LE(chars.length, 2)
          hdr.writeUInt16LE(chars.length, 6)

          const entries = [hdr]
          const expiresAt = Math.floor(Date.now() / 1000) + 90 * 86400
          for (const c of chars) {
            const expBuf = Buffer.alloc(4)
            expBuf.writeUInt32LE(expiresAt, 0)
            const nameBuf = writeCString(c.name)
            const statBuf = buildCharStatString33(c.charClass, c.level, c.flags, c.progression)
            entries.push(expBuf, nameBuf, statBuf)
          }
          ws.sendBinary(encodeMcpFrame(0x19, Buffer.concat(entries)))
          break
        }

        case 0x02: {
          const charClass = payload.length >= 4 ? payload.readUInt32LE(0) : 1
          const flags = payload.length >= 6 ? payload.readUInt16LE(4) : 0x60
          const rawName = payload.length > 6 ? readCString(payload, 6).value : 'WebHero'
          const cleanName = rawName.replace(/[^A-Za-z_-]/g, '') || 'WebHero'
          memoryChars.set(cleanName.toLowerCase(), {
            name: cleanName,
            charClass: Math.max(0, Math.min(6, charClass)),
            level: 1,
            flags: flags | 0x20,
            progression: 0,
          })
          activeCharName = cleanName
          const res = Buffer.alloc(4)
          res.writeUInt32LE(0, 0)
          ws.sendBinary(encodeMcpFrame(0x02, res))
          break
        }

        case 0x07: {
          const name = readCString(payload, 0).value
          if (name) activeCharName = name
          const res = Buffer.alloc(4)
          res.writeUInt32LE(0, 0)
          ws.sendBinary(encodeMcpFrame(0x07, res))
          break
        }

        case 0x0a: {
          const reqId = payload.length >= 2 ? payload.readUInt16LE(0) : 0
          const name = payload.length > 2 ? readCString(payload, 2).value : ''
          if (name) {
            memoryChars.delete(name.toLowerCase())
          }
          const res = Buffer.alloc(6)
          res.writeUInt16LE(reqId, 0)
          res.writeUInt32LE(0, 2)
          ws.sendBinary(encodeMcpFrame(0x0a, res))
          break
        }

        case 0x18: {
          const res = Buffer.alloc(4)
          res.writeUInt32LE(0, 0)
          ws.sendBinary(encodeMcpFrame(0x18, res))
          break
        }

        case 0x12: {
          const unk = Buffer.from([0])
          const headline = writeCString('D2MOO 1.12a Server')
          const body = writeCString('Connected to local D2MOO C++ D2GS server on port 4000.')
          ws.sendBinary(encodeMcpFrame(0x12, Buffer.concat([unk, headline, body])))
          break
        }

        case 0x05: {
          const reqId = payload.length >= 2 ? payload.readUInt16LE(0) : 1
          let idx = 1
          for (const g of activeGames.values()) {
            const fixed = Buffer.alloc(11)
            fixed.writeUInt16LE(reqId, 0)
            fixed.writeUInt32LE(idx++, 2)
            fixed.writeUInt8(Math.max(1, g.players.length), 6)
            fixed.writeUInt32LE(0x00000004, 7)
            ws.sendBinary(
              encodeMcpFrame(
                0x05,
                Buffer.concat([fixed, writeCString(g.name), writeCString(g.description)]),
              ),
            )
          }
          const endFixed = Buffer.alloc(11)
          endFixed.writeUInt16LE(reqId, 0)
          endFixed.writeUInt32LE(0, 2)
          endFixed.writeUInt8(0, 6)
          endFixed.writeUInt32LE(0xffffffff, 7)
          ws.sendBinary(
            encodeMcpFrame(0x05, Buffer.concat([endFixed, writeCString(''), writeCString('')])),
          )
          break
        }

        case 0x06: {
          const reqId = payload.length >= 2 ? payload.readUInt16LE(0) : 1
          const gameName = payload.length > 2 ? readCString(payload, 2).value : 'D2MOO'
          const g = activeGames.get(gameName.toLowerCase()) || activeGames.get('d2moo')
          const fixed = Buffer.alloc(46)
          fixed.writeUInt16LE(reqId, 0)
          fixed.writeUInt32LE(0x00000004, 2)
          fixed.writeUInt32LE(Math.max(1, Math.floor((Date.now() - (g?.createdAt ?? Date.now())) / 1000)), 6)
          fixed.writeUInt8(1, 10)
          fixed.writeUInt8(0xff, 11)
          fixed.writeUInt8(8, 12)
          fixed.writeUInt8(1, 13)
          fixed[14] = 1
          fixed[30] = 1
          ws.sendBinary(
            encodeMcpFrame(
              0x06,
              Buffer.concat([
                fixed,
                writeCString(g?.description || 'D2MOO 1.12a C++ Server'),
                writeCString(activeCharName || 'WebHero'),
              ]),
            ),
          )
          break
        }

        case 0x03: {
          const reqId = payload.length >= 2 ? payload.readUInt16LE(0) : 1
          const diffFlag = payload.length >= 6 ? payload.readUInt32LE(2) : 0
          const levelDiff = payload.length >= 8 ? payload[7] : 0xff
          const maxPlayers = payload.length >= 9 ? payload[8] : 8
          let off = 9
          const rName = readCString(payload, off)
          off = rName.nextOffset
          const rPass = readCString(payload, off)
          off = rPass.nextOffset
          const rDesc = readCString(payload, off)

          const gameName = rName.value || 'D2MOO'
          activeGames.set(gameName.toLowerCase(), {
            index: activeGames.size + 1,
            name: gameName,
            password: rPass.value || '',
            description: rDesc.value || 'D2MOO 1.12a Game',
            difficulty: diffFlag === 0x2000 ? 2 : diffFlag === 0x1000 ? 1 : 0,
            maxPlayers,
            levelDiff,
            createdAt: Date.now(),
            players: [activeCharName],
          })

          const res = Buffer.alloc(10)
          res.writeUInt16LE(reqId, 0)
          res.writeUInt16LE(1, 2)
          res.writeUInt16LE(0, 4)
          res.writeUInt32LE(0, 6)
          ws.sendBinary(encodeMcpFrame(0x03, res))
          break
        }

        case 0x04: {
          const reqId = payload.length >= 2 ? payload.readUInt16LE(0) : 1
          const res = Buffer.alloc(18)
          res.writeUInt16LE(reqId, 0)
          res.writeUInt16LE(1, 2)
          res.writeUInt16LE(0, 4)
          res[6] = 127
          res[7] = 0
          res[8] = 0
          res[9] = 1
          res.writeUInt32LE(1, 10)
          res.writeUInt32LE(0, 14)
          ws.sendBinary(encodeMcpFrame(0x04, res))
          break
        }
      }
    }
  })
}

function handleGameConnection(ws) {
  const pendingC2S = []
  let tcpConnected = false

  const tcp = net.createConnection({ host: D2GS_HOST, port: D2GS_PORT }, () => {
    tcpConnected = true
    tcp.setNoDelay(true)
    while (pendingC2S.length > 0) {
      const frame = pendingC2S.shift()
      tcp.write(frame)
    }
  })

  tcp.on('data', chunk => {
    ws.sendBinary(chunk)
  })

  tcp.on('close', () => {
    ws.close(1000, 'D2GS TCP closed')
  })

  tcp.on('error', err => {
    console.error('[Bridge /d2net/game] TCP error:', err.message)
    ws.close(1011, err.message)
  })

  ws.onMessage(chunk => {
    if (tcpConnected && !tcp.destroyed) {
      tcp.write(chunk)
    } else if (!tcp.destroyed) {
      pendingC2S.push(Buffer.from(chunk))
    }
  })

  ws.onClose(() => {
    if (!tcp.destroyed) {
      tcp.end()
      setTimeout(() => {
        if (!tcp.destroyed) {
          tcp.destroy()
        }
      }, 150)
    }
  })
}

const INJECTED_D2MOO_BOOTSTRAP = `
<script>
  (() => {
    try {
      const url = new URL(window.location.href);
      if (!url.searchParams.has('sw')) {
        url.searchParams.set('sw', '0');
        window.history.replaceState(null, '', url.toString());
      }
      if (navigator.serviceWorker && typeof navigator.serviceWorker.getRegistrations === 'function') {
        navigator.serviceWorker.getRegistrations().then(regs => {
          for (const r of regs) r.unregister();
        }).catch(() => {});
      }
      const host = window.location.host || '127.0.0.1:8787';
      const raw = window.localStorage.getItem('d2web.settings.v1');
      const parsed = raw ? JSON.parse(raw) : {};
      parsed.serverHost = host;
      if (!parsed.lang) parsed.lang = 'en';
      window.localStorage.setItem('d2web.settings.v1', JSON.stringify(parsed));
      window.localStorage.setItem('d2_last_bnet_account', 'D2MOO');
      for (let i = window.localStorage.length - 1; i >= 0; i--) {
        const k = window.localStorage.key(i);
        if (k && k.startsWith('d2_active_char_lease_v1:')) {
          window.localStorage.removeItem(k);
        }
      }
    } catch {}
  })();
</script>
<script type="module">
  import { applyOnlineCharactersToFlowState } from '/src/client/frontend/flow-state.ts';

  const params = new URLSearchParams(window.location.search);
  const autoJoin = params.get('autojoin') !== '0';
  const requestedChar = (params.get('char') || '').trim();
  const requestedClass = Number.parseInt(params.get('class') || '1', 10);
  const host = window.location.host || '127.0.0.1:8787';

  async function enterD2MooGame(ctrl) {
    const st = ctrl.flowState;
    st.stage = 'door_loading';
    st.stageTicks = 0;
    st.doorFrame = 0;
    st.sceneReady = false;
    ctrl.frontendRenderer.render(performance.now());
    await ctrl.assetsReadyPromise;
    await ctrl.session.enterGame({
      mode: 'create',
      spec: {
        name: 'D2MOO',
        password: '',
        description: 'D2MOO 1.12a C++ Server',
        difficulty: 0,
        maxPlayers: 8,
        levelDiff: 0xff,
      },
    });
    st.sceneReady = true;
    st.stage = 'in_game';
    st.stageTicks = 0;
    st.tickAccumulatorMs = 0;
    st.lastFrameTimeMs = null;
    ctrl.renderFrame(performance.now());
  }

  async function bootstrapD2Moo() {
    while (!window.__d2Play || !window.__d2Play.ready) {
      await new Promise(r => setTimeout(r, 50));
    }
    const ctrl = window.__d2Play;
    ctrl.settingsStore.setServerHost(host);
    const serverInput = document.getElementById('d2-toolbar-server-input');
    if (serverInput) serverInput.value = host;

    ctrl.flowState.playMode = 'bnet';
    const res = await ctrl.session.login({
      username: 'D2MOO',
      password: 'password',
      host,
    });
    let chars = res.characters;
    if (requestedChar && !chars.some(c => c.name.toLowerCase() === requestedChar.toLowerCase())) {
      chars = await ctrl.session.createCharacter({
        name: requestedChar,
        classId: Number.isFinite(requestedClass) ? requestedClass : 1,
        hardcore: false,
        expansion: true,
        ladder: true,
      });
    }
    applyOnlineCharactersToFlowState(ctrl.flowState, chars, {
      preserveSelectedName: requestedChar || chars[0]?.name,
    });
    ctrl.frontendRenderer.render(performance.now());

    if (autoJoin) {
      const targetName =
        (requestedChar && chars.find(c => c.name.toLowerCase() === requestedChar.toLowerCase())?.name) ||
        chars[0]?.name ||
        'WebHero';
      await ctrl.session.selectCharacter(targetName);
      await enterD2MooGame(ctrl);
    }

    // Also hook Character Select -> OK so selecting any character enters D2MOO directly without the chat lobby
    let enteringFromLobby = false;
    setInterval(() => {
      if (!enteringFromLobby && ctrl.flowState.stage === 'bnet_lobby' && ctrl.session.phase === 'lobby') {
        enteringFromLobby = true;
        enterD2MooGame(ctrl).finally(() => {
          enteringFromLobby = false;
        });
      }
    }, 100);
  }

  bootstrapD2Moo().catch(err => console.error('[D2MOO Auto-Connect]', err));
</script>
`

function proxyHttpToVite(req, res) {
  const urlPath = (req.url || '/').split('?')[0]
  if (urlPath === '/' || urlPath === '/index.html') {
    const query = (req.url || '').includes('?') ? req.url.slice(req.url.indexOf('?')) : '?autojoin=1'
    res.writeHead(302, { Location: `/play.html${query}` })
    res.end()
    return
  }

  const isPlayHtml = urlPath === '/play.html'
  const headers = { ...req.headers, host: `${VITE_HOST}:${VITE_PORT}` }
  if (isPlayHtml) {
    delete headers['accept-encoding']
  }

  const proxyReq = http.request(
    {
      hostname: VITE_HOST,
      port: VITE_PORT,
      path: req.url,
      method: req.method,
      headers,
    },
    proxyRes => {
      if (isPlayHtml && proxyRes.statusCode === 200) {
        const chunks = []
        proxyRes.on('data', c => chunks.push(c))
        proxyRes.on('end', () => {
          let html = Buffer.concat(chunks).toString('utf8')
          html = html.replace('</head>', `${INJECTED_D2MOO_BOOTSTRAP}\n</head>`)
          const outHeaders = { ...proxyRes.headers }
          delete outHeaders['content-length']
          delete outHeaders['content-encoding']
          outHeaders['cache-control'] = 'no-store'
          res.writeHead(200, outHeaders)
          res.end(html)
        })
        return
      }
      res.writeHead(proxyRes.statusCode || 200, proxyRes.headers)
      proxyRes.pipe(res)
    },
  )

  proxyReq.on('error', err => {
    res.writeHead(502, { 'Content-Type': 'text/plain; charset=utf-8' })
    res.end(`Waiting for diablo2-web Vite server on ${VITE_HOST}:${VITE_PORT} (${err.message})...\n`)
  })

  req.pipe(proxyReq)
}

const server = http.createServer((req, res) => {
  res.setHeader('Access-Control-Allow-Origin', '*')
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
  res.setHeader('Access-Control-Allow-Headers', '*')
  if (req.method === 'OPTIONS') {
    res.writeHead(204)
    res.end()
    return
  }
  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' })
    res.end(
      JSON.stringify({
        ok: true,
        bridge: `ws://${BRIDGE_HOST}:${BRIDGE_PORT}/d2net`,
        d2gs: `${D2GS_HOST}:${D2GS_PORT}`,
        characters: getCharacterList().map(c => c.name),
      }),
    )
    return
  }
  if (req.url?.startsWith('/api/register-char') && req.method === 'POST') {
    let body = ''
    req.on('data', c => {
      body += c
    })
    req.on('end', () => {
      try {
        const data = JSON.parse(body || '{}')
        const cleanName = String(data.name || 'WebHero').replace(/[^A-Za-z_-]/g, '') || 'WebHero'
        const charClass = Math.max(0, Math.min(6, Number(data.charClass ?? 1)))
        memoryChars.set(cleanName.toLowerCase(), {
          name: cleanName,
          charClass,
          level: 1,
          flags: 0x60,
          progression: 0,
        })
        res.writeHead(200, { 'Content-Type': 'application/json' })
        res.end(JSON.stringify({ ok: true, name: cleanName, charClass }))
      } catch (e) {
        res.writeHead(400)
        res.end(String(e))
      }
    })
    return
  }
  proxyHttpToVite(req, res)
})

server.on('upgrade', (req, socket, head) => {
  const urlPath = (req.url || '').split('?')[0]
  const isD2Net =
    urlPath.endsWith('/bnet') || urlPath.endsWith('/realm') || urlPath.endsWith('/game')

  if (!isD2Net) {
    // Proxy Vite HMR WebSocket upgrades directly to Vite
    const viteSock = net.createConnection({ host: VITE_HOST, port: VITE_PORT }, () => {
      const reqLines = [`${req.method || 'GET'} ${req.url || '/'} HTTP/${req.httpVersion || '1.1'}`]
      for (let i = 0; i < req.rawHeaders.length; i += 2) {
        reqLines.push(`${req.rawHeaders[i]}: ${req.rawHeaders[i + 1]}`)
      }
      reqLines.push('\r\n')
      viteSock.write(reqLines.join('\r\n'))
      if (head && head.length > 0) viteSock.write(head)
      socket.pipe(viteSock).pipe(socket)
    })
    viteSock.on('error', () => socket.destroy())
    socket.on('error', () => viteSock.destroy())
    return
  }

  const key = req.headers['sec-websocket-key']
  if (!key) {
    socket.destroy()
    return
  }
  const accept = crypto
    .createHash('sha1')
    .update(key + WS_GUID)
    .digest('base64')

  const headers = [
    'HTTP/1.1 101 Switching Protocols',
    'Upgrade: websocket',
    'Connection: Upgrade',
    `Sec-WebSocket-Accept: ${accept}`,
    '\r\n',
  ]
  socket.write(headers.join('\r\n'))

  const ws = new RawWebSocket(socket)
  if (head && head.length > 0) {
    ws._onData(head)
  }

  if (urlPath.endsWith('/bnet')) {
    handleBnetConnection(ws)
  } else if (urlPath.endsWith('/realm')) {
    handleRealmConnection(ws)
  } else if (urlPath.endsWith('/game')) {
    handleGameConnection(ws)
  }
})

server.listen(BRIDGE_PORT, BRIDGE_HOST, () => {
  console.log(
    `[D2MOO Bridge] Listening on http://${BRIDGE_HOST}:${BRIDGE_PORT}/play.html (WS /d2net/{bnet,realm,game} -> D2GS ${D2GS_HOST}:${D2GS_PORT})`,
  )
})
