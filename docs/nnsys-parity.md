# NitroSystem parity: the 40 non-matching lib/nnsys files (round 95c, nn2c)

Question: for the 40 lib/nnsys files that are pokebw2's newer NitroSystem (no `complete` in config/b_us/arm9/delinks.txt, so
the DS build links the ROM's objects), does our C behave like (a) Black's ROM and (b) libntrsystem 071126, which the PC runs
today? Method (a): `tools/compiler_probe.py lib/nnsys/src/<file>.c` per file (instruction diff against Black's delinked
object; 3429 lines of output, kept outside the repo on the build host, regenerate with the loop in this file's last section),
then every differing function read by hand; register-allocation and instruction-order differences (the large majority) are
"same". Method (b): reading libntrsystem's source (its `#ifdef SDK_PORT` branches in snd; algorithms in g3d/g2d/fnd/gfd).
Functions the probe cannot name (no symbol in symbols.txt: pokebw2's statics) are "not compared" and listed per file.

## Result in one paragraph

Of ~330 functions probed, ~290 match Black's ROM byte for byte; ~30 differ only by register allocation or instruction order;
~15 differ in the instruction stream because of struct layout (field order of a struct private to the file) or call targets
named differently (a pokebw2 static against the ROM's `func_xxxx`). **One real behaviour difference from Black was found and
fixed** (NNS_G3dMdlSetMdlCullMode). **One family differs numerically from Black and equals libntrsystem**: the texture matrix
makers for 3ds Max / Softimage (xsi) / Maya models in g3d/cgtool/*.c (rounding of the translation, see below). Nothing in
heap order/alignment, animation timing, OAM order, the VRAM managers or the sound envelope/volume maths differs from the ROM.
The 64-bit problems the files carry (not behaviour differences on DS) are listed last, as the PC build needs them.

## Risky ones first

| file / function | vs Black's ROM | vs libntrsystem | player-visible? | status |
|---|---|---|---|---|
| g3d/cgtool/3dsmax.c texmtxCalc_flag_, _flagS_, _flagR_, _flagT_, _flagTS_, _flagTR_ | differs: Black computes the translation as `origW * (...<<3) - origW * ((scale*trans)>>8)` (the 64-bit product shifted by 8 first, then multiplied by the width); pokebw2 turns the 12.4 offsets about the centre first and shifts the sum by 8 (`((cos*offS - sin*offT) >> 8) + (origW<<15)`). Same formula, other factoring: the low bits of the translation (about 1/4096 of a texel at most) can differ | not compared (3dsmax.c algorithm to read) | only a texture-animated material of a 3ds Max model; sub-texel | NOT FIXED: seven functions per tool, each needs the ROM's factoring written out; do it with a scan of the ROM's NSBTA/NSBMA tool flag first, to see whether the game uses 3dsmax at all |
| g3d/cgtool/xsi.c texmtxCalc_flag_, _flagS_, _flagR_, _flagRS_, _flagT_, _flagTS_, _flagTR_ | differs: Black uses `>>8` on the 64-bit products (12.4 results), ours `>>12` then `<<4` (rounds the product earlier: up to 15/16 of a 12.4 unit lost in the translation) | SAME as ours (libntrsystem xsi.c: `(origW * (ss_sin - p)) << 4`): libntrsystem is equally off Black | sub-texel; only xsi models | NOT FIXED; same remark. The PC shows this today, so it is not a regression of the switch |
| g3d/cgtool/maya.c texmtxCalc_flagS_, _flagTS_ | same value (`rsb 0x1000` against `rsb 0; add 0x1000`: 0x1000 - x) | n/a | no | same |
| g3d/model.c NNS_G3dMdlSetMdlCullMode | WAS different: ours cleared only the back-face bit (0x40) of the material's polyAttr, Black (and libntrsystem) clear back AND front bits (0xc0) before setting the mode; a material that had the front bit set kept it | same as Black | yes if the game calls it on a material with front-face rendering (cull mode changes of a model) | FIXED in this commit (now MATCH under compiler_probe; the file is one of the 40, so the ROM is unchanged) |
| snd/snd_internal.h NNSSndArcFileInfo (sndarc.c, sndarc_loader.c) | same on DS | libntrsystem builds a second FAT (WIN_NNSSndArcFat, malloc'd, 64-bit `mem`) and reads the FAT into a 2x buffer; ours has `void *mem` inside the file's layout: 24 bytes per entry instead of 16 on a 64-bit host, so the FAT read from the SDAT is mis-parsed | yes: no sound at all | 64-bit item for nn2a (see below) |

## Per file (same = matches Black's ROM, register/ordering noise only)

fnd (heap allocation order, alignment, group ids, 0-clear): all four files same as the ROM.
- allocator.c: all MATCH. heapcommon.c: all MATCH.
- expheap.c: AllocFromHead, AllocFromTail, NNS_FndResizeForMBlockExpHeap differ by register numbers only; the rest MATCH.
  Same algorithm as the ROM, so first-fit/best-fit mode, alignment (negative = from the tail), 0x10-byte block header and
  the fill byte are Black's. (libntrsystem: same algorithm.)
- frameheap.c: "AllocFromHead/AllocFromTail" show size mismatches because compiler_probe looks the static names up in
  symbols.txt and finds expheap's statics of the same name (Black's frame-heap ones are `AllocFromHeadForFrmHeap`); the
  bodies match by reading (head/tail bump allocation, align first, 0-clear on the opt flag). NNS_FndAllocFromFrmHeapEx,
  NNS_FndRecordStateForFrmHeap: call-target name only. same.

g2d (OAM order, animation timing):
- g2d_Animation.c: NNS_G2dResetAnimCtrlState, NNS_G2dTickAnimCtrl: registers only. same. (the zero-frame loop guard of the ios-animloop patch is Black's code: no read past the last frame)
- g2d_CharCanvas.c: ClearAreaLine, ClearChar, LetterChar, CharCanvasInitForOBJ1D: registers only; ClearArea1D/DrawGlyph1D: call target name (GetCharIndex1D = ROM's func_020681e4); NNS_G2dArrangeOBJ1D: stack slot order. same.
- g2d_Image.c, g2d_OAM.c (NNS_G2dApplyOamManagerToHW, i.e. OAM write order), g2d_Mtx32.c: registers only / MATCH. same.
- g2d_MultiCellAnimation.c: NNS_G2dTickMCInstance, NNS_G2dSetMCAnimationSpeed: instruction order (loop variable initialisation hoisted), SetMCDataToMCInstanceImpl_, SetAnimSequence/Tick/SetCurrentFrame: registers. same.
- g2d_OamSoftwareSpriteDraw.c: CalcSpriteParams_, NNS_G2dDrawOneOam3DDirectWithPosFast not compared (no symbol); 3D-OAM path only. The file's register stores are a port problem (nn1's list).
- g2d_Renderer.c, g2d_RendererCore.c: all named functions differ only in the names of their callees (DrawCellImpl_ = func_02065438, DrawCellAnimationImpl_ = func_02065878, DoRenderByRndCore2D_ = func_02067340, DoRenderByRndCore3D_ = func_02067998, RndCoreCBFuncBeforeOBJ_ = func_020651b8, NNSi_G2dIdentity = func_02064e3c); NNS_G2dDrawMultiCell/MultiCellAnimation, DrawNode_, NNS_G2dRndCoreDrawCellVramTransfer: registers/order. NOT COMPARED: the bodies of those six statics (and DoAffineTransforme_, DrawOamToSurface3D_, NNSi_G2dMCMStoreCurrentMtxToMtxCache, NNS_G2dBeginRenderingEx): no symbol names. Their callers match, and the sizes of the file's text range (0x020643a0..) are what pokebw2 builds in the DS build's link; a run of the probe with those statics given names in symbols.txt would close this.
- g2d_NCG_load.c: NNS_G2dGetUnpackedBGCharacterData: call name only; NNS_G2dUnpackBGNCG not compared. (64-bit loader: nn2b.)

g3d (kernel/model/sbc/animation):
- anm.c NNSi_G3dAnmBlendVis: ours is compiled as Thumb (`#pragma thumb on`), Black's is ARM: same C, same result (probe shows thumb bytes as .byte). same.
- anm/nsbca.c: getMdlRot_, getRotDataEx_, getRotData_, getRotDataByIdx_: registers; getJntSRTAnmResult_ (constant 0xfff against `0x1000 - 1` after the flag was materialised: same value), getTransDataEx_ (conditional load placement), NNSi_G3dAnmObjInitNsBca (post-increment against indexed load of the tag array): same.
- anm/nsbta.c: GetTexSRTAnmVectorVal_ registers; GetTexSRTAnmSinCosVal_ push/pop against bx (interworking of the epilogue only): same.
- binres/res_struct_accessor.c: NNS_G3dGetResDataByName / NNS_G3dGetResDictIdxByName load the dict header with a 3-register ldm plus a separate ldr against a 4-register ldm: same.
- gecom.c: field offsets of the command buffer struct (`ldr [r,#4]` against ours `[r]`: pokebw2's buffer struct has no leading word; the ROM's index/size words sit one word further) and `.word 0x0` literals (our statics are in .bss/.data at link time): same behaviour, a different private layout. NNS_G3dGeFlushBuffer: one instruction more in ours (resets the index after the send, as libntrsystem-geflush.patch did).
- kernel.c: NNS_G3dRenderObjAddAnmObj call name (updateHintVec_ = ROM's updateHintVec__kernel); NNS_G3dTexLoad, BindMdlTex, BindMdlPltt registers. same.
- model.c: see CullMode above; NNS_G3dMdlSetMdlFogEnableFlag, GetMdlAlpha, SetMdlAlphaAll, SetMdlFogEnableFlagAll not compared (no symbol: Black has none of them linked; unused by the game).
- sbc.c: G3dDrawInternal_: ours reads the model info through the NULL-checking accessor (`p ? p + 0x14 : NULL`) the ROM folds into direct offsets; behaviour the same unless the pointer is NULL. NNSi_G3dFuncSbc_NODEMIX, _NODEDESC: instruction order, predicated against branched (same). _BBY: literal pool symbol name (sBBYCmd = data_020a8370). _BB: registers. same.
- util.c NNS_G3dWorldPosToScrPos: registers. same.
- cgtool/*.c: see the table above.

gfd (VRAM managers): FramePlttVramMan, FrameTexVramMan (NNS_GfdAllocFrm*Vram), LinkedList*VramMan, gfdi_LinkedListVramMan_Common (NNSi_GfdAllocLnkVramAligned): registers only or callee names (NNS_GfdResetLnk*VramState = ROM's func_020625d8 / func_02062270). Allocation order and alignment same as the ROM.

snd (envelope/volume maths live in the SDK's SND_* and the ROM's player.c, which matches):
- player.c: 40 of 41 functions MATCH (NNS_SndPlayerGetTick has no symbol). sndarc_player.c: NNS_SndArcPlayerSetup registers. resource_mgr.c: NNS_SndLockChannel/UnlockChannel/FreeAlarm/InitResourceMgr differ in the field offsets of the file's private resource-manager struct (ours puts the channel/alarm masks at offset 0, the ROM at 8 and 4; same operations). snd_heap.c: NNS_SndHeapClear, LoadState registers.
- nns_snd_main.c: BeginSleep/EndSleep differ: ours calls NNSi_SndCaptureBeginSleep/EndSleep (capture.c, which is a matching file) and nothing more; the ROM inlines the capture sleep handling in main (checks a flag word of the capture state, stops/starts the alarm). Only on the DS sleep (lid closed); on the PC the PM callbacks never run. different, no player effect.
- sndarc.c: NNS_SndArcInit call name; NNS_SndArcReadFile registers; the other functions (Setup, the three dispose callbacks, GetStrmInfo, GetFileOffset, GetFileID, GetFilePath) have no symbol: not compared. The 64-bit width problems are the issue (below).
- sndarc_loader.c: all MATCH except LoadWaveArcTable (a literal pool entry order) and DisposeCallback (RestoreInterrupts placement). same.
- sndarc_stream.c, stream.c: Black's ROM links only NNSi_SndArcStrmMain from sndarc_stream.c (its text range is 0x2fc bytes, ours is the whole 1058-line file): the rest is dead-stripped, so Black never runs it except through that function and NNS_SndStrm* of stream.c (used by src/ov013/dwc_rap_vchat.c only: voice chat). The probe's size mismatches for ForceStopStrm/DisposeCallback/StrmCallback/ShutdownPlayer compare against other functions of the same names in symbols.txt (capture/waveout statics): meaningless. NNS_SndStrmSetup, SetVolume: registers. unknown, only voice chat can reach it.

## 64-bit and host differences libntrsystem handled (the PC build of our files needs them)

1. sndarc.c: the FAT. libntrsystem (SDK_PORT) allocates `fatSize*2`, reads the file's FAT into it, then builds a malloc'd
   WIN_NNSSndArcFat with 64-bit file-info entries (`mem` is a pointer: 16 bytes in the file, 24 in a host struct). Ours
   uses the file layout with `void *mem` inside it: wrong on 64-bit. Needs a host copy of the FAT (the same table-driven
   unpack as nn2b's loaders, or `u32 mem` handle in the FAT entries plus a side table).
2. sndarc.c, sndarc_loader.c, snd_heap.c, player.c: dispose callbacks carry `NNSSndArc *` in `u32 data1` and the heap block
   holds a pointer: libntrsystem widened data1 to u64 (NNSSndHeapDisposeCallback, NNS_SndHeapAlloc) and made the heap block
   header 0x20 + 28 bytes (SDK_PORT padding). Ours is `u32`: a pointer above 4 GB truncates. (nn2a's item.)
3. sndarc.c: libntrsystem opens the SDAT by path (FS_OpenFile) because FS_ConvertPathToFileID/FS_OpenFileFast don't exist in
   its libntr; heap.c rounds the heap start to 4 with a u64 round-up. Our files use the DS calls: libntr has them (nn1:
   nnsys_cc compiles the files); check the path-to-id works on the PC filesystem image.
4. player.c: SND_SetTrackMuteEx stub under `#ifndef SDK_PORT`: ours has the SDK's real one? (not checked: SNDi in libntr)
5. sbc.c, gecom.c, g2d_OamSoftwareSpriteDraw.c: G3 register stores (nn2a).

## Fixes made in this branch

- lib/nnsys/src/g3d/model.c NNS_G3dMdlSetMdlCullMode: clears BK and FR mask bits as Black does (compiler_probe: MATCH).

## Not done / next

- Name the statics of g2d_Renderer.c, g2d_RendererCore.c, g2d_OamSoftwareSpriteDraw.c, sndarc.c, model.c in the probe (a
  symbols.txt entry per static from its address in the delinks range) to compare their bodies. Highest value: the Renderer
  statics (every 2D sprite goes through DrawCellImpl_/DoRenderByRndCore2D_).
- Scan the ROM's NSBTA/NSBMA files for the tool id (3ds Max / xsi / Maya) to see which texmtxCalc family the game exercises;
  if 3ds Max or xsi: write Black's factoring into the six/seven functions (they cannot change the ROM).
- Clean room: the author of this table read libntrsystem's snd sources (SDK_PORT blocks of heap.c, sndarc.c, player.c) and
  g3d/cgtool/xsi.c: never assign this worker to port/acore, port/vcore or port/gpu.
