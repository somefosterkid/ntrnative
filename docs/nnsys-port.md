# Our NitroSystem on the PC (dropping libntrsystem)

Goal (owner 2026-11-06): the port compiles lib/nnsys (our NitroSystem, re-ported from pokebw2, GPL-3) on libntr and
the libntrsystem subproject (no licence) leaves every build. This file is the plan and the state; round 95b (nn1)
did the inventory and the first piece; round 95c (nn2a, nn2b, nn2c) wired it behind the meson option `nnsys`
(default off then); the NitroSystem switch made it the only NitroSystem: libntrsystem's wrap and patches are gone, there is
no `nnsys` option. State: "The switch" and "Parity, the switch, libntrsystem removed" below.

## What there is

- lib/nnsys: 75 C files, 5 public headers (lib/nnsys/include/nnsys/{fnd,gfd,g2d,g3d,snd}.h; the game includes them by
  those names). 35 files byte-match Black (`complete` in config/b_us/arm9/delinks.txt); 40 are pokebw2's newer
  NitroSystem (no delinks status: the DS build links the ROM's objects there, so an edit to one of those 40 cannot
  change the ROM, but it must still compile in the DS build):
  fnd/allocator expheap frameheap heapcommon; g2d/g2d_Animation CharCanvas Image MultiCellAnimation OAM
  OamSoftwareSpriteDraw Renderer RendererCore internal/g2di_Mtx32 load/g2d_NCG_load; g3d/anm anm/nsbca nsbta
  binres/res_struct_accessor cgtool/3dsmax maya xsi gecom kernel model sbc util; gfd FramePltt FrameTex LinkedListPltt
  LinkedListTex LinkedListVramMan_Common; snd/nns_snd_main player resource_mgr snd_heap sndarc sndarc_loader
  sndarc_player sndarc_stream stream.
  PARITY RISK: those 40 are a newer NitroSystem than the ROM's; libntrsystem (NitroSystem 071126) is what the port
  shows today. Every one needs a behaviour diff against libntrsystem's version (and the ROM's disassembly where they
  differ) before the switch: heap (expheap), animation, OAM/renderer, G3D kernel/model/sbc, VRAM managers, sound.
- Black 2 / White 2 (port/meson.build `b2w2`) also compile against libntrsystem (`nitrosystem_dep` in b2_kw) and link
  it: the switch covers all four games. pokebw2-*.patch fix pokebw2's game C only, not its NitroSystem: there is no
  proven 64-bit NitroSystem path to reuse.

## The compile (done: port/include_nnsys/, tools/nnsys_cc.py)

`python3 tools/nnsys_cc.py -Iport/include_nnsys -include port/include_nnsys/nnsys_lib.h -ferror-limit=0` compiles
every lib/nnsys file with a game file's PC line: 74 of 75 compile (2026-11-07).
- port/include_nnsys/nnsys.h: the umbrella `<nnsys.h>` port_prefix.h includes (now libntrsystem's), our five headers.
- nnsys_sdk.h (game and library): pokebw2's lib/nitro names libntr lacks, as the DS defines them: MtxFx32,
  GX_OAM_ATTR0_SHAPE_MASK, GXi_DmaFill32, SND_CHANNEL_VOLUME(v), SND_CHANNEL_DATASHIFT(v), SNDSequenceData.
- nnsys_lib.h (library only, -include): member renames onto libntr's GXOamAttr (charName -> charNo, affineParam -> _3).
- Left: g3d/glbstate.c:95 `invBaseRot.rt.rot` (MtxFx43's `rt` view: rotation 3x3 + translation, which libntr's
  MtxFx43 union lacks). Fix: libntr-nnsys.patch (new, last in diff_files) adds `struct { MtxFx33 rot; VecFx32 trans; } rt;`
  to MtxFx43's union (MtxFx33 must be declared first; same layout, 48 bytes).

## The 2D loaders (done: port/src/nnsys_load64.c, round 95c nn2b)

- The nnsys build leaves lib/nnsys/src/g2d/load/*.c out (nn2a's `tools/port_sources.py --nnsys`, once
  port/src/nnsys_load64.c is there) and compiles lib/nnsys's files through port/extern/lib/nnsys/ where it has one.
- port/src/nnsys_load64.c (all of it under `#ifdef BWPORT_NNSYS`: a libntrsystem build compiles it empty) has their
  entry points: NNS_G2dFindBinaryBlock, NNS_G2dGetUnpacked{Character,BGCharacter,Palette,PaletteCompressInfo,Screen,
  CellBank,AnimBank,MCAnimBank,MultiCellBank}Data/..., NNSi_G2dGetUnpackedFont, NNS_G2dGet{CellData,AnimSequence,
  MultiCellData}ByIdx (the NNS_G2dGetUserEx* are inlines of g2d.h, on the host structs). Not provided: the in-place
  NNS_G2dUnpack*/NNSi_G2dUnpack* (no caller outside the loaders; a call links to an abort stub).
  Design: a table per struct (Desc: its size in the file and on the host; fields copied, or an offset made a pointer from
  the struct itself or from a base its parent gives, or NULL when 0) and a builder per format that walks the file with
  them twice (size, then fill) into ONE malloc'd block; raw data (characters, colours, OAM attributes, frame contents,
  hierarchy data, attribute values, glyphs) is pointed at in the file, which is never changed. The font's width tables
  and code map info are copied (lib/nnsys's headers have them inline after the struct; the whole of their block, no
  fixed tables: ios-nftbounds). The screen data is in place.
- Lifetime: blocks registered by (file, block kind) and freed, 64 frees late, when the expanded heap block holding the
  file is freed: port/extern/lib/nnsys/src/fnd/expheap.c wraps NNS_FndFreeToExpHeap and calls WIN_G2dFreeUnpacked
  (a child heap's memory freed to its parent frees everything in it). The same file unpacked again rebuilds its block in
  place when it fits (a pointer kept from the first unpack sees the new data, as the DS's in-place unpack of a re-read
  file gives; libntrsystem's NMC loader replaced its copy, its other loaders kept both). WIN_G2dResetUnpacked (same
  name as libntrsystem's: port/src/machine.c calls it) frees all for the in-process restart.
- Results as libntrsystem's (boot, load64 patches): pStringBank NULL; pExtendedData / pVramTransferData NULL when the
  file has none; each animation sequence gets its own frames, after the sequence array (pFrameArrayHead = the first's).
- CHECK: `python3 port/tests/res_load_test.py --nnsys` (lib/nnsys's headers, nnsys_load64.c, lib/nnsys's G3D accessors)
  and the default (libntrsystem's), each with RES_DUMP=FILE (every result field by field: pointers as file offsets, H or
  0): Black and White (BWPORT_ROM), 25.7k files of 15 kinds, all OK on both, the dumps identical (122474 lines), the
  same file unpacked twice gives the same structs, nothing left registered after the frees; NSBMD 408 of 408 models bind
  on lib/nnsys's g3d. NFTR: the ROM's a/0/2/3 fonts are GFL's (both loaders crash on them, as before); the DWC
  settings' NitroSystem fonts (dwc/utility.bin msg/*.NFTR.l, version 1.0) on both: OK, dumps identical.
  `python3 port/tests/res_load_test.py --nmc`: port/tests/nmc_test.c on lib/nnsys's expanded heap with the free hook
  (blocks freed with their heap block and with a child heap's memory, a file unpacked again): 273 checks OK.
- The nnsys build needs (nn2a's port/meson.build option): `-DBWPORT_NNSYS` in game_args, port/src/nnsys_*.c built
  only there (`port_sources.py --nnsys-port`), the library from `port_sources.py --nnsys` (with -include
  port/include_nnsys/nnsys_lib.h), port/include_nnsys and lib/nnsys/include in place of libntrsystem's include dirs.

## The 64-bit and hardware spots (288 pointer-width warnings; nnsys_cc's all.txt)

1. G2D loaders (g2d/load/*.c; NAN 28, NOB 15, NFT 15, NMC 11, NCL 6, NCG 6, Load 6): unpack in place, 4-byte offsets
   turned into pointers. On 64-bit a struct with pointers is not the file's layout. DONE (round 95c, nn2b): see "The 2D
   loaders" below.
2. fnd (expheap 11, frameheap 5, allocator 2, heapcommoni.h, fnd.h:154): pointer math through `(u32)`
   (heapcommoni.h's NNSiGetUIntPtr returns u32). All four .c files are among the 40, so a plain-C change (a
   pointer-sized integer type) is ROM-neutral; or a port-side heapcommoni.h (the wrapper predefines its include guard).
   fnd.h:154 (NNS_FndGetMemBlockSizeForUnitHeap, a public inline: `(u32)heap + ...`) is a real truncation too;
   allocator.c's `(int)` casts round-trip an OS heap handle kept in a pointer (harmless). Must be exact: every GFL heap is
   an NNS expheap.
3. snd (sndarc_loader 12, sndarc 6, stream 2, sndarc_stream 2, snd_heap 2): NNSSndHeap's dispose callbacks carry a
   pointer in `u32 data1` (libntrsystem widened it to u64 on the PC); `(u32)arc + offset` math. All among the 40.
   libntrsystem's PC branches also open the SDAT by path (FS_OpenFile) where the DS uses the file id, round the heap
   start to 4, and read ADPCM state without `*(T*)p++`.
4. G3 register writes: sbc.c (45), gecom.c (3), g2d_OamSoftwareSpriteDraw.c (2) store to reg_G3X_GXFIFO and the
   command ports (reg_G3_MTX_MODE ...); on libntr those are plain variables and the stores are lost (libntr's own G3
   functions send draw messages: SIM_HandleG3Command, DRAW_CMD_G3_CMD_LIST = a packed command list). DESIGN: nnsys_lib.h
   redefines reg_G3X_GXFIFO and the command-port macros as `(*PORT_G3Stage(op))`: each use commits the word staged by
   the previous use into a packed-list buffer (a port write becomes op word + params, counted by the GX command's
   parameter count; a FIFO write is appended as is) and returns the staging word; MIi_CpuSend32 to the FIFO appends the
   words. The buffer goes out as one DRAW_CMD_G3_CMD_LIST before any other G3 traffic: a libntr patch calls a hook
   (commit + flush) at the top of SIM_HandleG3Command and in the G3X read functions (clip/vector matrix, busy).
   gecom.c's NNS_G3dGeFlushBuffer then needs nothing more (libntrsystem-geflush.patch's idx reset is the DS code).

## libntrsystem's patches: where each behaviour goes

| patch | behaviour | new place |
|---|---|---|
| boot | multi-cell bank into host structs | nnsys_load64.c's BuildMCBank (done) |
| load64 | host copies of NCL/NCE/NAN/NCG, one block per file, freed with the file's heap block | nnsys_load64.c + port/extern/lib/nnsys/src/fnd/expheap.c (done) |
| geflush | flush empties the buffer | lib/nnsys gecom.c's DS code (with the G3 stage, item 4) |
| sp77 | SIM_Snd_SeqPlayerAssign(slot, NNS player) before SND_PrepareSeq | port/extern/lib/nnsys/src/snd/player.c: `#define SND_PrepareSeq(n, ...)` calling it first with the call site's seqPlayer/player |
| o2flags | -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks | the nnsys library takes the game's c_args |
| ios-animloop | no read past a sequence's last frame | lib/nnsys g2d_Animation.c has no such read (DS code); check the zero-frame loop guard |
| ios-nftbounds | font loader without fixed tables | nnsys_load64.c's BuildFont (done) |
| ios-reset | Apple: NitroSystem data in the machine sections; WIN_G2dResetUnpacked | the nnsys library built with the game's section flags; nnsys_load64.c's WIN_G2dResetUnpacked (done; a host file: its registry is emptied, not restored) |

## The switch (round 95c, nn2a)

`meson setup build/port/nnsys port --native-file port/machine/linux.ini -Dnnsys=true` (then `BWPORT_BUILD=build/port/nnsys
python3 tools/port_run.py ...`; tools/lockstep and the other port_run users take BWPORT_BUILD too).
- port/meson.build: `nitrosystem_dep` = our headers (port/include_nnsys, lib/nnsys/include) for every game's C (Black, White,
  B2/W2's b2_kw: one library for all, as libntrsystem was); libnnsys.a = `tools/port_sources.py --nnsys` (lib/nnsys/src
  through port/extern/lib/nnsys/, without g2d/load/ when port/src/nnsys_load64.c exists) with the game's flags and
  `-include port/include_nnsys/nnsys_lib.h`; libnnsys_port.a = `--nnsys-port` (port/src/nnsys_*.c, never in the game's
  list); `-DBWPORT_NNSYS`. libnnsys.a is in tools/port_overlays.py's MACH_ARCHIVES (the restart puts its data back).
- The G3 stage (libntr-nnsys.patch + port/src/nnsys_g3stage.c): nnsys_lib.h makes reg_G3X_GXFIFO, the seven command ports
  lib/nnsys writes and MIi_CpuSend32 to the FIFO words of one packed command list, sent as DRAW_CMD_G3_CMD_LIST by
  SIM_G3PendingHook before libntr's next geometry command or G3X read (the list only grows in between: a command never
  splits). PORT_NnsReset (machine.c) drops it at a restart.
- Widths (plain C, the same code on the DS: uintptr_t is MSL's unsigned long): NNSiGetUIntPtr, the sound heap's dispose
  data1 and its block head (ROUNDUP(sizeof, 32): 32 on the DS, 64 on the PC), the 2D animation callback's param (the game
  passes pointers: the intro's sprites crashed at f1848 with a u32). The SDAT file table keeps its 16-byte entries (mem a
  u32): port/extern's sndarc.c keeps the addresses in a host table, opens the archive by path (libntr's FS has no file-ID
  table), sndarc_stream.c reopens it by path; sndarc_loader.c moves a loaded bank into libntr's 64-bit SNDBankData
  layout (the rest 0x20 further, inside the load's 32-byte padding; libntr's snd_bank.c reads it so).
- sp77: port/extern's player.c calls SIM_Snd_SeqPlayerAssign before NNSi_SndPlayerStartSeq's SND_PrepareSeq.

Checks (Linux, 2026-11-07): boots through the intro movie, title and menu to the professor at 60 fps, Sound acore.
Lockstep sweep with flat run dirs (C:\stress\nn2a\sweep.sh: port_run's --save writes the flat save.bin, which the
launcher ignores once a run dir has its game folder, so the stock sweep's copies all fail with "exit 3"): libntrsystem
6 SAME + the 2 known; nnsys room, bianca, route1 SAME, townmap/caught the known two, and NEW: downstairs f211, town f210,
lab f190 = the field menu's selected POKEMON icon (bottom screen, x13-46): libntrsystem and melonDS draw the pokeball on
its dark square, nnsys without the square (the selected state's picture), every frame while the menu is up. The 2D
pipeline's suspects: the cell's VRAM-transfer animation (g2d_CellAnimation.c's NNS_G2dSetCellTransferStateRequested,
g2d_RendererCore.c's bDrawn, the NCER's transfer data from nnsys_load64.c, GFL's task callback) or the renderer's
unnamed static functions nn2c did not compare (DrawCellImpl_, DoRenderByRndCore2D_). Not yet run: fps interleaved,
the title's 3D pictures, the acore corpus and a title WAV, restart T1-T2, White, win64, Android.

## Next steps (in order)

1. libntr-nnsys.patch (MtxFx43 rt; the G3 hook) and the G3 stage (port/src/nnsys_g3stage.c + nnsys_lib.h macros).
2. fnd and snd pointer widths (item 2, 3), then a `nnsys` meson option: lib/nnsys as a static library (game c_args,
   -Iport/include_nnsys, -include nnsys_lib.h, the loader files left out), port_prefix.h's `<nnsys.h>` resolved to
   port/include_nnsys, nitrosystem_dep dropped from the game and B2/W2 kwargs; Linux boot to the title.
3. nnsys_load64.c + res_load_test.c on it (done, round 95c nn2b); the 40 files' behaviour diff against libntrsystem (nn2c).
4. The brief's checks (sweep, fps interleaved, pics_ab corpus, acore corpus + title WAV, restart T1-T2, White switch),
   then win64, Android, docs/ios-handoff.md, and the removal (`git grep -i libntrsystem`).

## Parity, the switch, libntrsystem removed

- The field menu's icon (the sweep's one difference): NNS_G2dGetOAMSize and three more spots in g2d_RendererCore.c and
  g2d_OamSoftwareSpriteDraw.c took an OAM's shape from attr01 & GX_OAM_ATTR01_SHAPE_MASK alone (every OAM 8x8 there); the
  ROM masks 0xc000c000 (shape and size). The 32x32 icon the clact actor turns double-size affine sat 12 px off. Found by
  a BWPORT_VCAPTURE of both builds (sub OAM 15's position differed, nothing else) and a trace of DoAffineTransforme_'s
  inputs (equal) in both NitroSystems.
- Pointer-in-u32 spots: the multi-cell traverse callback's param (Black's only caller, btlv_mcss.c, passes an index),
  the char canvas param (a width or packed OBJ bits; no game caller), fnd's heap visitor param (GFL_HeapDumpProc, 0):
  safe as they are.
- White: the snd stand-ins push/pop their macros (a rename header's definitions survive).
- B2/W2: their C compiles on lib/nnsys's headers (zero-byte stand-in ROMs, b2.o and w2.o relinked, 3129 steps, 0 failed)
  and every NitroSystem name they call (225) is defined in libnnsys.a / libnnsys_port.a: they share the one library.
  Not run here (no ROMs).
- Checks against the libntrsystem build (Linux): lockstep sweep identical (6 SAME + the known townmap f78 and caught f464,
  the same renderer-only and beam-race counts per save); a sound capture of black/boot-room (BWPORT_ACAPTURE 0-17000):
  the same 778 commands with the same arguments (heap addresses ranked; READ_DRIVER_INFO and the per-frame timing
  left out: those jitter between two runs of one build), the same MEM kinds and sizes; the capture corpus
  (tools/vcap_corpus.sh) is not deterministic between two runs of the same build (libntrsystem against itself: 0 of 18
  captures equal), so it says nothing here: the sweep is the parity check. On the final build (no libntrsystem): plays
  black/route1-battle, black/town-lab, white/room-smoke, white/boot-room; three in-process restarts from the room save
  (1 ms each, back in the room); Windows exe (hidden, win_smoke room 30 s, 59.8 fps).
- Not run: fps interleaved (--realtime and uncapped), Android, the Black -> White -> Black switch test, the Mac and iOS
  (docs/ios-handoff.md "Our NitroSystem on the Mac"). port/tests' libntrsystem modes (res_load_test.py's default,
  run_nmc_test.sh, run_sdk_test.sh, run_sdk_names_test.sh) need the libntrsystem checkout: run them with --nnsys or retire
  them.
