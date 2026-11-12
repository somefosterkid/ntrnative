# The PC build's runtime: how Black runs on libntr

How libntr (docs/libntr/) starts a game and runs its frame, and what Black needs from it: the entry and init order,
threads, fixed addresses, the game's files, the save and the keys. Measured 2026-10-28 on libntr f6b3a69 (sdk 5,
twl_hyb, linux, debug), the commit port/subprojects/libntr.wrap pins, with a probe program on it (below). The PC
code is port/src/main_start.c, port/src/main_threads.c, port/include/port_runtime.h and
port/subprojects/packagefiles/libntr-port-runtime.patch.

**To wire in** (port/meson.build and port_prefix.h are the build's): port_prefix.h's last line
`#include "port_runtime.h"`, and `libntr-port-runtime.patch` in libntr.wrap's `diff_files` (after
libntr-clang.patch). Without the first the game's `main` clashes with libntr's and the threads run unguarded;
without the second the game's first FS_OpenFile segfaults and its save identify hangs.

## How libntr starts a game

libntr's `main()` (libraries/sim/src/sim_main.cpp) is the program's entry, and the game gives `NitroMain()`:

1. debug log, `SIM_Config_LoadDefaults`, then `./sim_config.ini` (written with libntr's defaults when missing);
2. the simulated registers and memory (arrays: `s_HW_MAIN_MEM` 8 MB, `s_HW_MAIN_MEM_SYSTEM`, VRAM, OAM, palettes),
   `header.bin` (first 0x160 bytes of a ROM) into `HW_ROM_HEADER_BUF`;
3. the ARM7 side: `SIM_pxiInit`, two SDL threads (PXI 9->7: SOUND, WM, OS, WVR only; 7->9: callbacks), WM, the
   sound mixer, the NVRAM config from sim_config.ini;
4. SDL window + GL 4.2 (`SIM_RenderInit`), the network thread;
5. `NitroMain()` on the main thread. libntr never returns from it; the window's close button calls `exit(0)`.

The frame is the game's own V-blank wait: `OS_WaitIrq(TRUE, OS_IE_V_BLANK)` (and `OS_WaitVBlankIntr`, which calls
it) with exactly the V-blank mask draws both engines, then calls the game's H-blank handler once and its V-blank
handler, adds 1 to the V-blank count, polls SDL input (keys to `REG_KEYINPUT` and the X/Y word, the mouse to the
touch panel), caps the frame at 60 fps (CapFrameRate) and swaps. So the IRQ handlers run on the main thread, inside
the wait, after the frame is drawn: a register the game writes during its frame shows in the same frame (one frame
earlier than on the DS), what its V-blank handlers write shows in the next, as on the DS. The H-blank handler runs
once a frame, not per line (Black's H-blank TCBs, gfl_use.c func_02005610, get one call a frame).

Threads: `OS_CreateThread` is `OS_CreateThreadDebug`, which starts an SDL thread at once (not at
`OS_WakeupThreadDirect`); `OS_SleepThread` sets the state and returns after `SDL_Delay(1)`; `OS_SleepThreadDirect`,
`OS_WakeupThreadDirect` only set the state; `OS_DestroyThread` of another thread marks it and lets the host thread
run on; `OS_Disable/Enable/RestoreInterrupts` return 0 and do nothing; mutexes are SDL mutexes; message queues poll a
semaphore every millisecond. `OS_GetCurrentThread` on the main thread is a stand-in OSThread of priority 0.

## Black's entry and init order

crt0 (lib/nitro/src/init/crt0.c, not compiled for PC) jumps to `main` in src/system/main.c, pokebw2's name for the
SDK's NitroMain. port_runtime.h has `#define main NitroMain` for the game's C (after libntr's headers; every `main`
of the game's files is renamed alike, struct fields too), so libntr's main() calls it. (A rename in the decomp,
main.c, main.h, crt0.c's `ldr r1, =main` and symbols.txt, is the alternative; it keeps the ROM byte-identical.)

`main`: `SystemInit` (machine_use.c: os_init, OS_InitTick, OS_InitAlarm, RTC_Init, GX_Init, FX_Init, the dev-console
SCFG write, FS_Init(1) + FS_TryLoadTable, NNS_GfdInitVramTransferManager, the DSi exit callback under
func_0202eb0c, the amp off), `GFLAppInit` (gfl_use.c: the NTR heaps, root heap 1 of 0x199500 bytes since
OS_IsRunOnTwl is FALSE; the arc table of 235 paths; HID, overlays, procs, TCBs, fade, backup system, text),
H-blank and V-blank handlers, `GameInit` (GFL_OvlLoad(10), (11): FS_LoadOverlay is p43c's; the first proc
data_020a0480 queued with OVERLAY_NONE; save area, net heaps, sound init, PokeVoice), then the loop: HID/proc
update, `GameUpdate`, BGM loader put to sleep, `OS_WaitIrq` (the frame), `GameUpdateVBlank` (VRAM transfers, BG,
fade), loader woken.

What the probe measured on the boot path's SDK calls (libntr as it is): `OS_GetConsoleType` 0x80000001 (the dev
mask 0x3000000 is clear, so SystemInit's raw SCFG writes at 0x04004008 / 0x02fffdf0 don't run), `OS_IsRunOnTwl` 0,
`OS_GetBootType` 0, `MB_IsMultiBootChild` 0, main arena about 7.7 MB (heap 1's 1.6 MB fits; x64's bigger records
may need more than the DS's 1.6 MB: the knob is HEAP_SETUP_NTR), RTC synchronous on host time (callbacks run in
the caller), TP_Init / TP_GetUserInfo / auto sampling / TP_WaitRawResult return at once, PAD_DetectFold 0,
PM_SetAmp / PM_SetBackLight 1, PM_GetBattery 1 and PM_GetBatteryLevel 2 (errors: ui.c keeps batteryLevel 5),
PM_GetBackLight leaves its output unset (ui.c stores garbage in ui->backlight, used only after sleep), MIC_Init ok.

libntr f6b3a69's own link gaps on this configuration: `FSi_InitOverlay` (fs_api.c's FS_Init calls it; p43c's
overlay model) and `RTCEXi_GetCounter`/`GetFout`/`SetFout` and their Async forms (the TWL RTC); the probe stubs them.

## Threads

Black's own threads: the BGM loader `g_GFLSndThread` (src/system/snd_sys.c, priority 17 against main's 16: one
`NNS_SndArcLoadSeqEx` per thread, two per BGM: sequence+bank, then waves; destroyed by StopLoadThread when the BGM
changes before it is done and made again in the same OSThread; main.c's loop sleeps it around each V-blank wait),
and lib/poke_net's two (online only). What the DS gives them that libntr doesn't:

- one CPU: the loader only runs while main blocks (snd_sys.c's waits call `OS_Sleep(1)` / `OS_Sleep(2)` to give it
  the CPU), never during main's frame or the V-blank. On libntr it runs alongside: its seeks and reads of the sound
  archive's one FSFile (`sSndArc.file`), the sound heap and the archive's file table race main's sound calls;
- a created thread waits for its wakeup; a slept thread doesn't run;
- `OS_DestroyThread` stops it. On libntr the host thread runs on, and when it ends `SIM_OS_RunThread` writes
  TERMINATED into the OSThread the game has made again, so the next load looks done before it ran;
- OS_DisableInterrupts as a critical section (main_02083644.c's NVRAM state, the SDK's own uses).

The PC build's model (port/include/port_runtime.h, port/src/main_threads.c): **one game lock**, the DS's CPU. The
main thread holds it from the start (main_start.c's constructor) and lets it go only where a DS thread blocks:
`OS_Sleep`, `OS_SleepThread`, `OS_WaitIrq`, `OS_WaitVBlankIntr`, `OS_JoinThread`, a held `OS_LockMutex`, a blocking
`OS_ReceiveMessage`/`OS_SendMessage` (port_runtime.h's macros route the game's calls to `PORT_*`). A game thread
is the game's OSThread (state only; libntr never sees it) run by a kept host thread of a 16-slot table: made at
`OS_WakeupThreadDirect`, it takes the lock only while its thread is awake, runs the game's function to its end or
its next block, and sets TERMINATED itself; `OS_SleepThreadDirect` keeps it off the lock; `OS_DestroyThread` keeps a
thread that hasn't run from running (one destroyed inside a sleep parks its host for good: poke_net's receive thread
only); the V-blank and H-blank handlers take the lock as an interrupt takes the CPU. Callbacks libntr runs on its
own threads (WM, PXI 7->9) don't take it.

Measured with a stand-in for main.c's loop and the loader (port/src's files built with the probe, `rtg`): 300
frames at 60 fps, a load every 20 frames (2 MB in 32 KB reads of wb_sound_data.sdat each) and a create, wakeup,
destroy, remake every 60: 10 runs of this code, 0 hangs, 0 loader reads during main's frame or the V-blank handler, every load
run once, no destroyed thread run. The 11 game files that call the wrapped functions (main.c, snd_sys.c,
poke_net.c, ui.c, net_err.c, pm_voice.c, screentex.c, field_transition.c, battle_examination.c, mb_parent_sys.c,
main_02083644.c) compile with port_runtime.h after port_prefix.h with the build's flags (clang, 0 errors).

## Fixed addresses in the game's code

the maintainers' notes's 143 `fixed-address` casts are 137 in lib/nitro, lib/twl, lib/dwc*, lib/nitrowifi (not
compiled for PC; libntr's headers map `HW_*` to its arrays), 6 in the game. Read from the game's own text (its
macros unexpanded, so the SDK's `HW_*`/`reg_*` it uses go to libntr's variables):

| place | what | on PC |
|---|---|---|
| src/system/machine_use.c:49-50, SystemInit | `reg_SCFG_EXT` 0x04004008, `SCFG_EXT_COPY` 0x02fffdf0, under `OS_GetConsoleType() & 0x3000000` | not run (0x80000001) |
| src/main/main_02087f60.c | TwlSDK's DeliverArg: 0x02ffe010, 0x02000000 | excluded (port/exclude.txt: libntr has os_argument.c) |
| src/main/main_02083644.c | NVRAM over PXI tag 4: the words 0x02002200 ... are commands, not addresses | not on the boot path; libntr has its own NVRAM library: the two define the same names |
| lib/gfl/src/gf_overlay.c:18 | `HW_ITCM_END` 0x02000000, a bound | p43c's overlay model |
| src/ov204/intro_graphic.c:144-150 | `*(u16 *)0x4000050`, `0x4001050` (BLDCNT), `(u32 *)0x4000000`, `0x4001000` (DISPCNT) | fixed: `reg_G2_BLDCNT`, `reg_G2S_DB_BLDCNT`, `GX_SetVisibleWnd`/`GXS_SetVisibleWnd(GX_WNDMASK_NONE)` |
| src/ov088/startmenu.c:429 | `*(u16 *)0x4001050` | fixed: `reg_G2S_DB_BLDCNT` |
| src/ov021/shortcut_menu.c:207 | `*(u16 *)0x4000050` | fixed: `reg_G2_BLDCNT` |
| 11 `G2x_SetBlendAlpha_(0x4000050 ...)` (shortcut_menu, c_gear_sw, btlv_effvm x3, ircbattlematch, gsync_disp, ctvt_talk, psel x3) | the register as a number | fixed before (`G2_SetBlendAlpha`, `G2S_SetBlendAlpha`) |
| palette/VRAM: intrude_subdisp.c (4), field_transition.c:542, fld3d_ci.c:1586, ov071_021b6100.c:531-534, plist_battle.c:164, cg_help.c:444 | 0x05000xxx, 0x06xxxxxx | fixed: `HW_DB_BG_PLTT`, `(u16 *)HW_DB_OBJ_PLTT + n`, `HW_BG_VRAM` ..., `HW_LCDC_VRAM(_D)` |

The fix for each is the SDK's spelling (`reg_G2_BLDCNT`, `reg_GX_DISPCNT`, `G2_SetBlendAlpha`, `HW_BG_PLTT + x`,
`HW_LCDC_VRAM_C` ...), which libntr maps to its arrays and MWCC compiles to the same bytes (every file
matches, the ROM is byte-identical). A text scan of src/, lib/gfl, lib/poke_net and include/ for numbers in the DS's
IO, palette, VRAM, OAM, WRAM and system-area ranges, and a scan of the PC build's preprocessed C for casts of such
numbers, leave only code that doesn't run on PC: machine_use.c's `reg_SCFG_EXT` and its copy at 0x02fffdf0 (under the
dev console mask; libntr's own reg_SCFG_EXT is a raw address too), ov172 (DSi, excluded), main_02087f60.c and
main_020821c0.c (excluded SDK copies), and main_02083644.c's PXI words (commands, not addresses).

libntr's side (port/subprojects/packagefiles/libntr-hw.patch): its HW_DB_OBJ_VRAM_END/_SIZE said 256 KB of a 128 KB
array; its renderer looked a direct-colour bitmap BG's pixels up in a palette pointer it hadn't set (the opening movie
faulted); G3_MultMtx33/44, G3_MultTransMtx33, G3_Shininess and MIi_CpuSend32 copied 36 to 128 bytes over the 4-byte
geometry FIFO variable and the registers after it (GXSTAT's busy bit then held the title screen's 3D draw in
G3X_GetVectorMtx forever): they go to the G3 simulator as commands, or to the one port. HW_PLTT_SIZE (BG+OBJ
palettes as one 1 KB area) holds only because sim_vars.c defines the four palette arrays in a row (the ELF and the PE
link keep them so).

The VRAM banks (port/subprojects/packagefiles/libntr-vram.patch). On the DS a bank (A-I) is one memory: the
CPU sees it at its LCDC address (`HW_LCDC_VRAM_*`), the 2D engines wherever VRAMCNT maps it. libntr has separate
arrays: s_HW_LCDC_VRAM (also where its 3D engine reads textures and texture palettes, and its 2D engine the BG
extended palettes) and the main/sub BG and OBJ arrays (what `HW_BG_VRAM` ... and its 2D renderer use), and a
VRAMCNT write only set a register. So a bank written or cleared through its LCDC address and then mapped to BG drew
the BG array's old data: GFL_BGSysInitVRAM's clear never reached the BG array, and the start menu showed the opening
movie's bitmap behind BG0/BG1 (port-boot.md, stop 9). Now a bank's data lives in one place, its home: the BG/OBJ
array at the bank's offset while VRAMCNT maps it there, else its part of s_HW_LCDC_VRAM (LCDC, texture, palette,
ARM7, disabled). gx_vramcnt.c's register writes all end in GX_VRAMCNT_SetLCDC_ or disableBankForX_, which call
GXi_SimSyncVramBanks: every bank whose home changed goes back to its LCDC part (its old place in the array cleared,
as unmapped VRAM reads 0, unless a bank that stays covers it), then every arriving bank is copied to its new place
(field_transition.c's clear of bank D, fld3d_ci.c's darkening through HW_LCDC_VRAM_D then shown as BG). Copies, not
one store with aliased views: the game writes the BG/OBJ arrays at any offset, which only page aliasing (a memfd
mapped twice) could share, and Windows maps views at 64 KB granularity while banks F, G and I are 16 KB. Not
modelled: two banks mapped over each other (the DS ORs them; here the later copy wins), VRAMCNT written outside
gx_vramcnt.c (only libntr's wvr.c, wireless, not used), and display capture into a bank: libntr's simulator has no
DISPCAPCNT, so what title.c, demo3d_capture.c, screentex.c, lib/gfl double3Ddisp.c (3D on both screens),
field_transition.c, fld3d_ci.c and encounter_effect_capture.c capture stays empty.

## The game's files

The game reads its files by path (lib/gfl/src/arc_tool.c: "a/0/0/0" ... from gfl_use.c's 235-path table;
"wb_sound_data.sdat", "gfl_font.dat") with FS_OpenFile, FS_SeekFile, FS_ReadFile, FS_GetFileLength (Black's
GetFileSize), and NARCs in memory with FS_CreateFileFromMemory. libntr as it is, sdk 5: **FS_OpenFile segfaults**
(TwlSDK's path goes to the "rom" archive: FS_LoadArchiveTables keeps the table pointer in a u32, and the reads would
go to a card that isn't there), and FS_CreateFileFromMemory truncates its address to 32 bits. libntr's sdk 4 path
(fopen of the path under the working directory) is what the PC needs: `libntr-port-runtime.patch` gives sdk 5 the
same, in fs_file.c: a file FS_OpenFile opens is a host file (`rom:` and a leading `/` dropped, a lowercase retry),
a FS_CreateFileFromMemory file is the memory block, both read, seeked, sized and closed by a small port layer; a
missing file returns FALSE once the ROM is extracted (header.bin is there), else libntr's dialog (below) asks for
a ROM. Probe: a/0/0/0 (NARC, 2924 bytes), rom:/a/0/0/0, /a/0/0/0, wb_sound_data.sdat (51,533,312), gfl_font.dat,
a/0/0/7 seek to end/back and read, a memory file's seek and short read, a missing file FALSE.

Where the files go: the working directory. libntr's extractor (sim_RomExtractor.cpp: an SDL message box, then an
ImGui file dialog in the game window, `SIM_NDSExtract` from fireflower's nds-extract.cpp) writes the ROM's file tree
(a/, dl_rom/, dwc/, the 5 root files), header.bin, arm9.bin, banner.bin, overlay9/, overlay7/ into it, and runs
inside the FS_OpenFile that missed. main_start.c does it first and without a window: when a/0/0/0 or header.bin is
missing it extracts `BWPORT_ROM` or the first `*.nds` of the directory (the FNT/FAT walk, header.bin as the first
0x200 bytes): Black's 247 files in about 2 s, byte-identical to extract/b_us/files. `BWPORT_DIR` sets the
directory. So: `BWPORT_ROM=pokeblack.nds ./bwport` once, or the .nds next to bwport.exe.

## The save

libntr's backup is the host file `save.bin` in the working directory: nitro/card/backup.h's PC inlines of
CARDi_ReadBackup, CARDi_WriteBackup and CARDi_WriteAndVerifyBackup open it per call (lib/gfl/src/flash_access.c's
CARD_ReadBackupAsync, CARD_ReadBackup, CARD_WriteAndVerifyBackupAsync are those; the callback runs in the call;
CARD_WaitBackupAsync / CARD_TryWaitBackupAsync return TRUE). But `CARD_IdentifyBackup` (flash_access.c's
identifyGameCardBackup, at boot) **hangs**: CARD_Init doesn't enable the card (OS_GetBootType 0), so it first
panics (OS_Terminate), and enabled it waits forever for the ARM7's CARD PXI callback, which libntr's sim7 never
registers. The patch's CARD_IdentifyBackup takes the type as given (CARD_BACKUP_TYPE_FLASH_4MBITS_EX: 512 KB) and
makes save.bin the chip's size, its new bytes 0xFF (an erased flash; a fresh save.bin of libntr's own would be
zeros of the first read's size, and the first write's fopen("rb+") of a missing file crashes); main_start.c enables
the card. Probe: identify TRUE, result 0, save.bin 524,288 bytes of 0xFF, reads and writes land at their offsets.
A DeSmuME or melonDS .sav of Black is the same 512 KB image (DeSmuME's with a footer to cut): copy it to save.bin.
libntr's synchronous `CARD_WriteAndVerifyBackup` returns CARD_RESULT_SUCCESS (0) as its BOOL, so it says FALSE;
Black only uses the Async one and CARD_GetResultCode.

## Keys and settings

libntr's keys (sim_config.c, SDL keycodes in sim_config.ini's [PAD]): A Z, B X, X C, Y V, arrows, L Q, R E, Start
Enter, Select right Shift; ` the debug GUI, Tab Frame rate Unlimited and back, Space fast forward; a joystick's buttons too. The mouse
is the touch screen: any button down inside the bottom screen's rectangle touches at the mapped point (the rectangle
follows the layout, vertical by default). Its 3D scale default is 10. main_start.c writes sim_config.ini when there
is none, and libntr then reads it instead of its defaults: A X, B Z, X S, Y A (the DS's layout, and DeSmuME's and
melonDS's), L Q, R W, Start Enter, Select right Shift, the same extra keys, InternalResolutionScale 2 (llvmpipe, the
software GL of WSL and of the Mesa DLLs on this Windows host, keeps up with it). The rest of the file is libntr's
(MacAddr random each run, the OSNVRAM owner settings default: nickname "Player").

## Test cheat (BWPORT_TESTCHEAT=win)

ON BY DEFAULT in tests (runs are with the cheat; cheat-off only when a test is about a battle itself):
`route.py` legs and `port_run.py --play` runs have it unless `--no-cheat`; a plain `port_run.py` run without --play takes `--cheat`
(or the env var BWPORT_TESTCHEAT=win). The game and the DS build never see it otherwise. The battles still really play (menus, animations, messages, EXP, trainer switch-ins, the
end); only the player's side (battle mon IDs 0-11) changes: it moves first (the CALC_SPEED event's ratio: player 32x, opponents 0.1x),
its attacks do 3000 damage (they one-shot) and the attacks on it do 0 (CALC_DAMAGE). The stand-ins wrap BattleEvent_CallHandlers:
port/extern/src/ov093/battle_event.c (Black, White) and port/extern_b2w2/src/ov167/battle_event.c (Black 2, White 2); each rewrite
prints `cheat: speed mon N ratio ..` / `cheat: damage atk A def D -> N` to the log. Proven: tools/plays/black/route1-fight.txt,
tools/plays/b2/hugh-fight.txt (Hugh's battle). A new stand-in file is only picked up after `touch port/meson.build` (the source list is
made at configure time).
Not covered: a move of the opponent's with priority (Quick Attack) still goes first, but does 0 damage; moves that do not go through
CALC_DAMAGE (fixed damage: Dragon Rage, Seismic Toss, Super Fang), status moves, poison/burn, weather, recoil, confusion; the
opponent can still cause status conditions and stat drops.

## The probe

<scratch> (scratch): a meson project on a clone of libntr f6b3a69 with the patch applied; `rt` (rt.c: console
type, threads, FS, CARD, the boot SDK calls; `RT_TEST=thread|fs|card|sdk`) and `rtg` (rt_game.c, main.c's loop
and the loader with -include port_runtime.h, plus main_threads.c and main_start.c). Run under xvfb-run with
`SDL_AUDIODRIVER=dummy` in a directory with the base ROM.

## Left

- Wire in the two lines above; then the first boot shows what's next.
- FS_LoadOverlay / FSi_InitOverlay (p43c), the TWL RTC stubs, PM_GetBackLight's unset output (p43d's area).
- main_02083644.c (Black's NVRAM over PXI tag 4) against libntr's NVRAM library: the same names, one of them goes.
- Heap 1 is the DS's 1.6 MB; x64 records are bigger: watch GFL_HeapAllocate failures at boot.
- The game lock covers the game's threads and the IRQ handlers, not callbacks libntr calls from its PXI threads (WM,
  MB: wireless, not on the boot path).
