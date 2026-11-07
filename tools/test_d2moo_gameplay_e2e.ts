#!/usr/bin/env npx tsx
/**
 * End-to-End Gameplay & Save-Persistence Verification for D2MOO 1.12a C++ Server <-> diablo2-web Client.
 *
 * Uses the actual `diablo2-web` TypeScript networking (`createD2OnlineFlow`, `createWsBridgeResolver`,
 * `ClientWorld`, `getCanonicalItemDataTables`) against the live 32-bit `Game.D2MOO.exe` server (`127.0.0.1:4000`)
 * and `d2moo_ws_bridge.mjs` (`127.0.0.1:8787`).
 *
 * Verifies:
 *   1. Realm login & character enumeration (including cross-config `kurt.d2s` newbie save discovery).
 *   2. Game join, Act I Rogue Encampment DRLG room streaming, player spawn, stats, skills, starting items, and NPCs.
 *   3. In-game actions: chat, walking/running (`0x01`/`0x03`), weapon swap (`0x60`), cursor item pickup/place (`0x19`/`0x18`),
 *      skill select & cast (`0x3C`/`0x0C`), and town NPC interaction (`0x04`/`0x59`/`0x13`/`0x30`).
 *   4. Zero protocol errors (`adapter.stats.protocolErrors === 0`) and zero unknown opcodes (`adapter.stats.unknownPackets === 0`).
 *   5. Save & Exit (`0x69` `D2CLTSYS_ENDGAME`) writing a full v96 `.d2s` save file to disk (`> 335` bytes),
 *      followed by immediately rejoining a second game and verifying character reload from the saved `.d2s`.
 */

import { spawn, type ChildProcess } from 'node:child_process'
import { copyFileSync, existsSync, mkdirSync, readFileSync, statSync } from 'node:fs'
import net from 'node:net'
import { dirname, resolve } from 'node:path'
import { fileURLToPath } from 'node:url'

import '../../diablo2-web/tools/lib/node-map-loader.ts'
import { ClientWorld } from '../../diablo2-web/src/client/world/client-world.ts'
import { getCanonicalItemDataTables } from '../../diablo2-web/src/client/world/item-tables-provider.ts'
import {
  InMemoryPacketTap,
  createD2OnlineFlow,
  createWsBridgeResolver,
  unitTypeToId,
} from '../../diablo2-web/src/netproto/index.ts'

const __dirname = dirname(fileURLToPath(import.meta.url))
const REPO_ROOT = resolve(__dirname, '..')
const GAME_DIR = resolve(REPO_ROOT, 'build-112a/bin/Release')
const DEBUG_SAVE_DIR = resolve(REPO_ROOT, 'build-112a/bin/Debug/save')
const RELEASE_SAVE_DIR = resolve(GAME_DIR, 'save')
const GAME_EXE = resolve(GAME_DIR, 'Game.D2MOO.exe')
const BRIDGE_MJS = resolve(__dirname, 'd2moo_ws_bridge.mjs')

function sleep(ms: number): Promise<void> {
  return new Promise(r => setTimeout(r, ms))
}

function waitForPort(host: string, port: number, timeoutMs = 15000): Promise<boolean> {
  const start = Date.now()
  return new Promise((resolvePort) => {
    const attempt = () => {
      const sock = net.createConnection({ host, port })
      sock.once('connect', () => {
        sock.destroy()
        resolvePort(true)
      })
      sock.once('error', () => {
        sock.destroy()
        if (Date.now() - start >= timeoutMs) {
          resolvePort(false)
        } else {
          setTimeout(attempt, 200)
        }
      })
    }
    attempt()
  })
}

function assert(cond: unknown, message: string): asserts cond {
  if (!cond) {
    throw new Error(`ASSERTION FAILED: ${message}`)
  }
}

async function main(): Promise<void> {
  mkdirSync(RELEASE_SAVE_DIR, { recursive: true })

  // Seed a clean 335-byte newbie kurt.d2s in Debug/save if needed so we test cross-config discovery + newbie initialization
  const debugKurt = resolve(DEBUG_SAVE_DIR, 'kurt.d2s')
  const releaseKurt = resolve(RELEASE_SAVE_DIR, 'kurt.d2s')
  if (existsSync(debugKurt)) {
    copyFileSync(debugKurt, releaseKurt)
  }

  let serverProc: ChildProcess | null = null
  let bridgeProc: ChildProcess | null = null

  try {
    const d2gsAlreadyUp = await waitForPort('127.0.0.1', 4000, 600)
    if (!d2gsAlreadyUp) {
      console.log(`[e2e] Starting headless C++ 1.12a server: ${GAME_EXE}`)
      serverProc = spawn(GAME_EXE, [], {
        cwd: GAME_DIR,
        env: {
          ...process.env,
          D2MOO_HEADLESS_SERVER: '1',
          D2MOO_SAVE_DIR: RELEASE_SAVE_DIR,
        },
        stdio: 'ignore',
        windowsHide: true,
      })
      const ready4000 = await waitForPort('127.0.0.1', 4000, 15000)
      assert(ready4000, 'Game.D2MOO.exe did not listen on 127.0.0.1:4000 within 15s')
    }

    const bridgeAlreadyUp = await waitForPort('127.0.0.1', 8787, 600)
    if (!bridgeAlreadyUp) {
      console.log(`[e2e] Starting WebSocket gateway: ${BRIDGE_MJS}`)
      bridgeProc = spawn(process.execPath, [BRIDGE_MJS], {
        cwd: REPO_ROOT,
        env: {
          ...process.env,
          D2MOO_SAVE_DIR: RELEASE_SAVE_DIR,
        },
        stdio: 'ignore',
        windowsHide: true,
      })
      const ready8787 = await waitForPort('127.0.0.1', 8787, 10000)
      assert(ready8787, 'd2moo_ws_bridge.mjs did not listen on 127.0.0.1:8787 within 10s')
    }

    const tables = getCanonicalItemDataTables()
    const tap = new InMemoryPacketTap()
    const resolver = createWsBridgeResolver({
      host: '127.0.0.1:8787',
      scheme: 'ws',
      basePath: '/d2net',
      timeoutMs: 10000,
    })

    const flow = createD2OnlineFlow({
      resolver,
      bnetHost: '127.0.0.1',
      defaultRealm: 'D2MOO',
      itemTables: tables,
      tap,
    })

    try {
      // --- PHASE 1: Login, Realm Entry, Character Discovery, and First Game Join ---
      console.log('[e2e] Phase 1: Logging in & entering realm D2MOO...')
      await flow.loginOrRegister('d2moo', 'd2moo', { register: true })
      const chars = await flow.enterRealm('D2MOO')
      console.log(
        `[e2e] Realm characters (${chars.length}):`,
        chars.map(c => `${c.name}(class=${c.charClass},lvl=${c.level},exp=${c.expansion},ladder=${c.ladder})`),
      )

      const charName = 'kurt'
      const hasKurt = chars.some(c => c.name.toLowerCase() === charName)
      if (!hasKurt) {
        await flow.createCharacter({
          name: charName,
          classId: 1,
          expansion: true,
          hardcore: false,
          ladder: false,
        })
      }
      await flow.selectCharacter(charName)
      await flow.createGame({
        name: 'D2MOO1',
        password: '',
        description: 'E2E Game 1',
        difficulty: 0,
        maxPlayers: 8,
      })

      const adapter1 = await flow.joinGame('D2MOO1', '')
      const world1 = new ClientWorld()
      adapter1.onEvent(ev => world1.apply(ev))

      const waitStart1 = Date.now()
      while (world1.loading && Date.now() - waitStart1 < 6000) {
        await sleep(50)
      }
      await sleep(500)

      console.log(
        `[e2e] Game 1 entered: loading=${world1.loading} act=${world1.act} area=${world1.areaId} self=${world1.self.name}(#${world1.selfId},class=${world1.self.charClass},lvl=${world1.self.level}) pos=(${world1.self.x},${world1.self.y}) hp=${world1.self.life}/${world1.self.maxLife} mp=${world1.self.mana}/${world1.self.maxMana} skills=${world1.self.skills.size} eq=${world1.containers.equipment.size} inv=${world1.containers.inventory.size} belt=${world1.containers.belt.size} units=${world1.units.size} rooms=${world1.revealedRooms.size} pktsIn=${adapter1.stats.packetsIn} errs=${adapter1.stats.protocolErrors} unk=${adapter1.stats.unknownPackets}`,
      )

      assert(!world1.loading, 'world1.loading should be false after entering game')
      assert(world1.selfId > 0, 'world1.selfId should be assigned')
      assert(world1.self.x > 0 && world1.self.y > 0, 'world1.self position should be valid (> 0)')
      assert(world1.self.maxLife > 0 && world1.self.maxMana > 0, 'world1.self HP/Mana stats should be populated')
      assert(world1.self.skills.size > 0, 'world1.self.skills should contain starting skill (Fire Bolt)')
      assert(
        world1.containers.equipment.size + world1.containers.inventory.size + world1.containers.belt.size > 0,
        'Starting items (staff, potions, scrolls) should be populated via 0x9C/0x9D',
      )
      assert(world1.getUnitsByType('monster').length > 0, 'Rogue Encampment NPCs should be spawned')
      assert(world1.getUnitsByType('object').length > 0, 'Rogue Encampment objects (stash/waypoint/fire) should be spawned')
      assert(adapter1.stats.protocolErrors === 0, `Expected 0 protocolErrors in Game 1, got ${adapter1.stats.protocolErrors}`)
      assert(adapter1.stats.unknownPackets === 0, `Expected 0 unknownPackets in Game 1, got ${adapter1.stats.unknownPackets}`)

      // --- PHASE 2: In-Game Gameplay Actions (Move, Chat, Skill Cast, Items, NPC) ---
      console.log('[e2e] Phase 2: Executing in-game actions (Move, Chat, Skill, Item Cursor, NPC Interact)...')
      const startX = world1.self.x
      const startY = world1.self.y

      adapter1.send({ type: 'OverheadChat', text: 'D2MOO 1.12a E2E!' })
      await sleep(200)
      adapter1.send({ type: 'Chat', text: 'Testing 1.12a C++ server gameplay' })
      await sleep(200)

      // Walk & Run
      adapter1.send({ type: 'Move', run: false, to: { x: startX + 3, y: startY } })
      await sleep(450)
      adapter1.send({ type: 'Move', run: true, to: { x: startX + 3, y: startY + 3 } })
      await sleep(450)

      // Skill select & cast
      const firstSkill = Array.from(world1.self.skills.keys()).find(id => id > 0) ?? 36
      adapter1.send({ type: 'SelectSkill', hand: 'right', skill: firstSkill })
      await sleep(250)
      adapter1.send({
        type: 'SkillAt',
        hand: 'right',
        at: { x: startX + 5, y: startY + 5 },
        hold: false,
      })
      await sleep(350)

      // Weapon swap x2
      adapter1.send({ type: 'SwapWeapons' })
      await sleep(300)
      adapter1.send({ type: 'SwapWeapons' })
      await sleep(300)

      // Pick up an inventory item to cursor and place it back
      const invItem = Array.from(world1.containers.inventory.values())[0]
      if (invItem) {
        const origX = invItem.gridX
        const origY = invItem.gridY
        adapter1.send({ type: 'ItemToCursor', itemId: invItem.id })
        await sleep(400)
        if (world1.containers.cursor?.id === invItem.id) {
          adapter1.send({
            type: 'ItemPlace',
            itemId: invItem.id,
            x: origX,
            y: origY,
            buffer: 0,
          })
          await sleep(400)
        }
      }

      // Interact with nearest town NPC
      const monsters = world1.getUnitsByType('monster')
      let nearestNpc = monsters[0]
      let bestDist = Infinity
      for (const m of monsters) {
        if (m.x > 0 && m.y > 0) {
          const d = Math.hypot(m.x - world1.self.x, m.y - world1.self.y)
          if (d < bestDist) {
            bestDist = d
            nearestNpc = m
          }
        }
      }
      if (nearestNpc) {
        console.log(`[e2e] Interacting with town NPC #${nearestNpc.id} (txt=${nearestNpc.txtFileNo}, dist=${bestDist.toFixed(1)})...`)
        adapter1.send({
          type: 'MoveToUnit',
          run: true,
          target: { unitType: unitTypeToId('monster'), id: nearestNpc.id },
        })
        await sleep(500)
        adapter1.send({
          type: 'NpcInit',
          target: { unitType: unitTypeToId('monster'), id: nearestNpc.id },
        })
        await sleep(300)
        adapter1.send({
          type: 'Interact',
          target: { unitType: unitTypeToId('monster'), id: nearestNpc.id },
        })
        await sleep(400)
        adapter1.send({
          type: 'NpcCancel',
          target: { unitType: unitTypeToId('monster'), id: nearestNpc.id },
        })
        await sleep(300)
      }

      assert(adapter1.stats.protocolErrors === 0, `Expected 0 protocolErrors after actions, got ${adapter1.stats.protocolErrors}`)
      assert(adapter1.stats.unknownPackets === 0, `Expected 0 unknownPackets after actions, got ${adapter1.stats.unknownPackets}`)

      // --- PHASE 3: Save & Exit (`0x69` LeaveGame) and Verify `.d2s` Save Persistence ---
      console.log('[e2e] Phase 3: Leaving Game 1 (0x69 LeaveGame) and verifying kurt.d2s persistence...')
      await flow.leaveToLobby()
      await sleep(700)

      assert(existsSync(releaseKurt), `Expected ${releaseKurt} to exist after leaving game`)
      const savedSize = statSync(releaseKurt).size
      const savedBytes = readFileSync(releaseKurt)
      const savedMagic = savedBytes.readUInt32LE(0)
      const savedVersion = savedBytes.readUInt32LE(4)
      console.log(
        `[e2e] Saved kurt.d2s: size=${savedSize} bytes, magic=0x${savedMagic.toString(16)}, version=${savedVersion}`,
      )
      assert(savedMagic === 0xaa55aa55, 'Saved kurt.d2s should have 0xAA55AA55 magic')
      assert(savedVersion === 96, `Expected version 96 .d2s save, got ${savedVersion}`)
      assert(savedSize > 335, `Expected full .d2s save (> 335 bytes), got ${savedSize} bytes`)

      // --- PHASE 4: Immediately Rejoin Game 2 to Verify Reload from Saved `.d2s` ---
      console.log('[e2e] Phase 4: Rejoining Game 2 as kurt to verify reload from saved v96 .d2s...')
      await flow.createGame({
        name: 'D2MOO2',
        password: '',
        description: 'E2E Game 2',
        difficulty: 0,
        maxPlayers: 8,
      })
      const adapter2 = await flow.joinGame('D2MOO2', '')
      const world2 = new ClientWorld()
      adapter2.onEvent(ev => world2.apply(ev))

      const waitStart2 = Date.now()
      while (world2.loading && Date.now() - waitStart2 < 6000) {
        await sleep(50)
      }
      await sleep(500)

      console.log(
        `[e2e] Game 2 entered: loading=${world2.loading} self=${world2.self.name}(#${world2.selfId},class=${world2.self.charClass},lvl=${world2.self.level}) pos=(${world2.self.x},${world2.self.y}) hp=${world2.self.life}/${world2.self.maxLife} mp=${world2.self.mana}/${world2.self.maxMana} skills=${world2.self.skills.size} eq=${world2.containers.equipment.size} inv=${world2.containers.inventory.size} belt=${world2.containers.belt.size} pktsIn=${adapter2.stats.packetsIn} errs=${adapter2.stats.protocolErrors} unk=${adapter2.stats.unknownPackets}`,
      )

      assert(!world2.loading, 'world2.loading should be false after rejoining Game 2')
      assert(world2.self.x > 0 && world2.self.y > 0, 'world2.self position should be valid (> 0)')
      assert(world2.self.skills.size > 0, 'world2.self.skills should be preserved on reload')
      assert(
        world2.containers.equipment.size + world2.containers.inventory.size + world2.containers.belt.size > 0,
        'Player items should be preserved on reload from kurt.d2s',
      )
      assert(adapter2.stats.protocolErrors === 0, `Expected 0 protocolErrors in Game 2, got ${adapter2.stats.protocolErrors}`)
      assert(adapter2.stats.unknownPackets === 0, `Expected 0 unknownPackets in Game 2, got ${adapter2.stats.unknownPackets}`)

      await flow.leaveToLobby()
      await sleep(400)
      console.log('[e2e] ALL 4 E2E GAMEPLAY & SAVE-PERSISTENCE PHASES PASSED!')
    } finally {
      await flow.close()
    }
  } finally {
    if (bridgeProc) {
      bridgeProc.kill()
    }
    if (serverProc) {
      serverProc.kill()
    }
  }
}

main().catch((err) => {
  console.error('[e2e] FAILED:', err)
  process.exit(1)
})
