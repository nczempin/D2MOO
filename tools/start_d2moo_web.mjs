#!/usr/bin/env node
/**
 * Streamlined launcher for D2MOO 1.12a + diablo2-web TypeScript Client:
 *  - Normal mode (`node tools/start_d2moo_web.mjs`):
 *      Starts headless `Game.D2MOO.exe` (TCP :4000) + Bridge (:8787) + Vite (:5173) and opens browser.
 *  - Attach mode (`node tools/start_d2moo_web.mjs --attach --char <Name> --class <Id>`):
 *      Called automatically by `Game.D2MOO.exe` when `CLIENT: TS WEB` is selected in `D2Launch.dll`.
 *      Attaches to the already-running in-process `Game.D2MOO.exe` server on TCP :4000,
 *      ensures Bridge (:8787) + Vite (:5173) are running, and opens the browser straight into the game!
 */

import { spawn } from 'node:child_process'
import { existsSync } from 'node:fs'
import { dirname, resolve } from 'node:path'
import { fileURLToPath } from 'node:url'
import net from 'node:net'
import http from 'node:http'

const __dirname = dirname(fileURLToPath(import.meta.url))
const repoRoot = resolve(__dirname, '..')
const releaseBinDir = resolve(repoRoot, 'build-112a/bin/Release')
const debugBinDir = resolve(repoRoot, 'build-112a/bin/Debug')
const binDir = existsSync(resolve(releaseBinDir, 'Game.D2MOO.exe')) ? releaseBinDir : debugBinDir
const gameExe = resolve(binDir, 'Game.D2MOO.exe')
const bridgeScript = resolve(__dirname, 'd2moo_ws_bridge.mjs')
const webRepoDir = process.env.DIABLO2_WEB_DIR || 'H:/git/diablo2-web'

const args = process.argv.slice(2)
const isAttach = args.includes('--attach')
const noBrowser = args.includes('--no-browser')
const charIdx = args.indexOf('--char')
const charName =
  (charIdx >= 0 && args[charIdx + 1] ? args[charIdx + 1] : process.env.D2MOO_CHAR_NAME) || 'kurt'
const classIdx = args.indexOf('--class')
const charClass =
  classIdx >= 0 && args[classIdx + 1] !== undefined
    ? Number.parseInt(args[classIdx + 1], 10)
    : Number.parseInt(process.env.D2MOO_CHAR_CLASS || '1', 10)

function isPortListening(host, port, timeoutMs = 600) {
  return new Promise(resolvePromise => {
    const sock = net.createConnection({ host, port })
    const timer = setTimeout(() => {
      sock.destroy()
      resolvePromise(false)
    }, timeoutMs)
    sock.once('connect', () => {
      clearTimeout(timer)
      sock.destroy()
      resolvePromise(true)
    })
    sock.once('error', () => {
      clearTimeout(timer)
      sock.destroy()
      resolvePromise(false)
    })
  })
}

function waitForTcpPort(host, port, timeoutMs = 20000) {
  const start = Date.now()
  return new Promise((resolvePromise, rejectPromise) => {
    const tryConnect = () => {
      const sock = net.createConnection({ host, port })
      sock.once('connect', () => {
        sock.destroy()
        resolvePromise(true)
      })
      sock.once('error', () => {
        sock.destroy()
        if (Date.now() - start >= timeoutMs) {
          rejectPromise(new Error(`Timed out waiting for ${host}:${port}`))
        } else {
          setTimeout(tryConnect, 250)
        }
      })
    }
    tryConnect()
  })
}

function registerCharWithBridge(name, cls) {
  return new Promise(resolvePromise => {
    const body = JSON.stringify({ name, charClass: cls })
    const req = http.request(
      {
        hostname: '127.0.0.1',
        port: 8787,
        path: '/api/register-char',
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Content-Length': Buffer.byteLength(body),
        },
      },
      res => {
        res.resume()
        res.on('end', () => resolvePromise(true))
      },
    )
    req.on('error', () => resolvePromise(false))
    req.write(body)
    req.end()
  })
}

function openBrowser(url) {
  if (noBrowser) return
  if (process.platform === 'win32') {
    spawn('cmd.exe', ['/c', 'start', '', url], { detached: true, stdio: 'ignore' }).unref()
  }
}

const children = []
function cleanup() {
  for (const child of children) {
    try {
      child.kill()
    } catch {}
  }
}
process.on('SIGINT', () => {
  cleanup()
  process.exit(0)
})
process.on('SIGTERM', () => {
  cleanup()
  process.exit(0)
})

async function main() {
  // 1. D2MOO 1.12a C++ Server on TCP 127.0.0.1:4000
  if (!isAttach && !(await isPortListening('127.0.0.1', 4000))) {
    console.log(`[1/3] Starting D2MOO 1.12a Headless C++ Server (${gameExe})...`)
    const d2Server = spawn(gameExe, ['-w'], {
      cwd: binDir,
      env: {
        ...process.env,
        D2MOO_HEADLESS_SERVER: '1',
        D2MOO_USE_TS_CLIENT: '0',
        D2MOO_CHAR_NAME: charName,
        D2MOO_CHAR_CLASS: String(charClass),
      },
      stdio: 'inherit',
    })
    children.push(d2Server)
  }
  await waitForTcpPort('127.0.0.1', 4000, 15000)

  // 2. WebSocket + HTTP Reverse Proxy Bridge on 127.0.0.1:8787
  if (!(await isPortListening('127.0.0.1', 8787))) {
    console.log('[2/3] Starting D2MOO Gateway Bridge on http://127.0.0.1:8787...')
    const bridge = spawn(process.execPath, [bridgeScript], {
      cwd: repoRoot,
      env: {
        ...process.env,
        D2MOO_BRIDGE_PORT: '8787',
        D2MOO_D2GS_HOST: '127.0.0.1',
        D2MOO_D2GS_PORT: '4000',
      },
      stdio: 'inherit',
    })
    children.push(bridge)
    await waitForTcpPort('127.0.0.1', 8787, 10000)
  }

  await registerCharWithBridge(charName, charClass)

  // 3. diablo2-web Vite Server on 127.0.0.1:5173
  const viteBin = resolve(webRepoDir, 'node_modules/vite/bin/vite.js')
  if (existsSync(viteBin) && !(await isPortListening('127.0.0.1', 5173))) {
    console.log(`[3/3] Starting diablo2-web Vite server in ${webRepoDir}...`)
    const vite = spawn(process.execPath, [viteBin, '--host', '127.0.0.1', '--port', '5173'], {
      cwd: webRepoDir,
      stdio: 'inherit',
      windowsHide: true,
    })
    children.push(vite)
    await waitForTcpPort('127.0.0.1', 5173, 20000)
  }

  const targetUrl = `http://127.0.0.1:8787/play.html?autojoin=1&char=${encodeURIComponent(charName)}&class=${charClass}`
  console.log(`\n[D2MOO] Launching TS Web Client: ${targetUrl}\n`)
  openBrowser(targetUrl)

  // Keep running until interrupted or parent process Job Object closes
  setInterval(() => {}, 60000)
}

main().catch(err => {
  console.error(err)
  cleanup()
  process.exit(1)
})
