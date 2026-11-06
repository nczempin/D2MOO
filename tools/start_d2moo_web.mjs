#!/usr/bin/env node
/**
 * One-command launcher for:
 *  1. D2MOO 1.12a C++ Server (Game.D2MOO.exe, headless on TCP 127.0.0.1:4000)
 *  2. D2MOO WebSocket Bridge (ws://127.0.0.1:8787/d2net/{bnet,realm,game} -> TCP 127.0.0.1:4000)
 *  3. diablo2-web TypeScript Client (http://127.0.0.1:5173/play.html)
 */

import { spawn } from 'node:child_process'
import { existsSync } from 'node:fs'
import { dirname, resolve } from 'node:path'
import { fileURLToPath } from 'node:url'
import net from 'node:net'

const __dirname = dirname(fileURLToPath(import.meta.url))
const repoRoot = resolve(__dirname, '..')
const releaseBinDir = resolve(repoRoot, 'build-112a/bin/Release')
const debugBinDir = resolve(repoRoot, 'build-112a/bin/Debug')
const binDir = existsSync(resolve(releaseBinDir, 'Game.D2MOO.exe')) ? releaseBinDir : debugBinDir
const gameExe = resolve(binDir, 'Game.D2MOO.exe')
const bridgeScript = resolve(__dirname, 'd2moo_ws_bridge.mjs')
const webRepoDir = process.env.DIABLO2_WEB_DIR || 'H:/git/diablo2-web'

function waitForTcpPort(host, port, timeoutMs = 15000) {
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

const children = []
function cleanup() {
  for (const child of children) {
    try {
      child.kill()
    } catch {
      // ignore
    }
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
  console.log(`[1/3] Starting D2MOO 1.12a Headless C++ Server (${gameExe})...`)
  const d2Server = spawn(gameExe, ['-w'], {
    cwd: binDir,
    env: {
      ...process.env,
      D2MOO_HEADLESS_SERVER: '1',
      D2MOO_CHAR_NAME: process.env.D2MOO_CHAR_NAME || 'D2WebHero',
      D2MOO_CHAR_CLASS: process.env.D2MOO_CHAR_CLASS || '1',
    },
    stdio: 'inherit',
  })
  children.push(d2Server)

  await waitForTcpPort('127.0.0.1', 4000, 15000)
  console.log('[1/3] D2MOO 1.12a C++ Server listening on TCP 127.0.0.1:4000')

  console.log('[2/3] Starting D2MOO WebSocket Bridge on ws://127.0.0.1:8787/d2net/{bnet,realm,game}...')
  const bridge = spawn(process.execPath, [bridgeScript], {
    cwd: repoRoot,
    env: {
      ...process.env,
      D2MOO_WS_PORT: '8787',
      D2MOO_D2GS_HOST: '127.0.0.1',
      D2MOO_D2GS_PORT: '4000',
    },
    stdio: 'inherit',
  })
  children.push(bridge)

  await waitForTcpPort('127.0.0.1', 8787, 10000)
  console.log('[2/3] WebSocket Bridge ready on ws://127.0.0.1:8787')

  if (existsSync(webRepoDir)) {
    console.log(`[3/3] Starting diablo2-web Vite server in ${webRepoDir}...`)
    const npmCmd = process.platform === 'win32' ? 'npm.cmd' : 'npm'
    const vite = spawn(npmCmd, ['run', 'dev'], {
      cwd: webRepoDir,
      stdio: 'inherit',
      shell: true,
    })
    children.push(vite)
    console.log('\n====================================================================')
    console.log('Open in browser: http://127.0.0.1:5173/play.html')
    console.log('In the top toolbar Server box, enter: 127.0.0.1:8787')
    console.log('Then log in with any username/password and click Create/Join Game.')
    console.log('====================================================================\n')
  } else {
    console.log(`diablo2-web directory not found at ${webRepoDir}; Server + WS Bridge are running.`)
  }
}

main().catch((err) => {
  console.error(err)
  cleanup()
  process.exit(1)
})
