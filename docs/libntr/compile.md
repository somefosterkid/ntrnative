# The game's C against libntr's headers

The port phase's first measure (the maintainers' notes, Phase 2): does Black's C compile with the headers of
[libntr](https://github.com/cybervisi0n/libntr) (MIT, a NitroSDK for PC) in place of ours? Only clang -fsyntax-only
for x86_64-linux: nothing is built and no C was changed. `python3 tools/libntr_compile.py` reproduces every number
here (its docstring says how).

Measured at 10d456712 on 1081 C files: every one under src/ (1023), lib/gfl (55) and lib/poke_net (3). clang 21.1.8
(Ubuntu), `--target=x86_64-linux-gnu -std=gnu99 -fsyntax-only -w -fsigned-char`. `-w` as libntr's meson.build has for
linux: warnings off, so a count is of errors, clang's default-error warnings among them (an undeclared call, an
integer passed for a pointer).

## Setup: what works

- Clones (no build needed): libntr f6b3a69, libntrsystem 97fa995 (`git clone --depth 1
  https://github.com/cybervisi0n/<name>.git`). libntrwifi e3a86b0 and libntrdwc c662226 clone too, but are not used
  here (below). ~/ref/libntr is 1555d91, the one lib/nitro/sdk was ported from: 11 of its headers differ from
  f6b3a69's (fx_mtx*.h, mi/memory.h, ...).
- libntr's one generated header, from libntr's root:
  `python3 gen/nitro/fx/gen_fx_const.py gen/nitro/fx/fx_const.csv OUT/nitro/fx/fx_const.h` (866 lines), OUT then an
  include dir after libntr's include/.
- libntr's defines for `build_target=linux`, `sdk_version_major=5`, `hardware_type=twl_hyb` (Black is a TWL hybrid):
  `-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_FINALROM -DNNS_FINALROM -DSDK_VERSION_MAJOR=5UL -DSDK_TWL
  -DSDK_TWLHYB -DSDK_PORT -DSDK_X86 -DSDK_CODE_X86 -DSDK_BUILD_LINUX -DSDK_64BIT -D_NITRO`. libntrsystem's are a
  subset. No SDK_ARM9: libntr's headers take SDK_PORT or SDK_X86 in its place.
- The include dirs: the build's (configure.py's INCLUDE_DIRS) with `libntr/include` and the generated dir where
  lib/nitro/include was, `libntrsystem/include` where lib/nnsys/include was, lib/twl/include dropped (libntr's
  include/twl/ has TwlSDK's headers).
- The host needs SDL2's headers: libntr's nitro/os/common/thread.h includes `<SDL2/SDL_thread.h>` for SDK_PORT
  (libsdl2-dev 2.32.10 is installed here).
- libntr's own umbrella headers compile alone with these flags with no error: `<nitro.h>` (SDK 4 and SDK 5),
  `<twl.h>` (SDK 5) and libntrsystem's `<nnsys.h>` and each of nnsys/g2d.h, g3d.h, gfd.h, fnd.h, snd.h.
- `python3 tools/libntr_compile.py --libntr DIR --libntrsystem DIR` runs every stage below (3.5 minutes on the
  worker slice, about 50 s a stage); `--stage pch --causes` groups one stage's errors by cause; `--stage pch FILE...`
  lists a file's errors.

## Files that compile

Each stage is the one before plus one step (the tool's `--stage`):

| stage | what changes | files that compile (of 1081) |
|---|---|---|
| `ours` | the control: our own headers (lib/nitro, lib/twl, lib/nnsys, MSL), same target and flags | 994 |
| `raw` | libntr's and libntrsystem's include dirs and defines in place of ours | **21** |
| `msl` | lib/msl/include after the host's headers (`-idirafter`) | 204 |
| `shim` | a header for each of the 15 SDK header names libntr lacks, each including `<nitro.h>` (`<nnsys.h>`) | 437 |
| `pch` | `-include nitro.h -include nnsys.h`, as libntr's include/pch/nitro_pch.h has its users do | **450** |

- **raw, 1060 fail.** 839 stop at a header libntr lacks: `nitro/fx.h` in 828 (reached through game headers:
  include/gfl/clact.h first in 240, field/field.h in 139, gfl/bg_sys.h in 113), `nitro/hw.h` in 6, `nitro/mic.h` in 4,
  `nitro/tp.h` in 1. 221 stop in the host's /usr/include/wchar.h (`unknown type name '__gnuc_va_list'`): libntr's
  thread.h includes SDL2, SDL2 includes the host's libc, and MSL's stdarg.h, stdio.h... answer in its place.
- **The header names.** 14 of the 40 headers of lib/nitro/include and lib/twl/include are pokebw2's names that libntr
  has not: nitro/fx.h (NitroSDK has nitro/fx/fx.h, fx_const.h, fx_trig.h ...), nitro/hw.h, nitro/mic.h, nitro/pm.h,
  nitro/tp.h (libntr: nitro/spi/ARM9/*.h), nitro/svc.h (nitro/os/common/systemCall.h), nitro/crypto/rc4.h,
  nitro/card_rom_header_twl.h, twl/mic.h, twl/photo_sign.h, twl/pm.h, twl/sndex.h (libntr: twl/snd/ARM9/sndex.h),
  twl/ssp.h (libntr has only ssp_jpeg_type.h), twl/syscall.h; and 1 of lib/nnsys's 6:
  nnsys/gfdi_LinkedListVramMan_Common.h. The other 26 and 5 have the same path in libntr and libntrsystem.
- **pch.** The prefix header adds 13 files: those that used a name their own includes didn't bring with libntr's
  headers (ours include more of the SDK each). All 450 that compile also compile in the control.

## What stops the other 631 (stage pch)

30 of the 631 fail in the control alike (below); 601 have an error the control lacks: libntr's causes, one error
counted once per file and line. "Alone" is the files that compile once that cause alone is gone (they have no other
libntr cause and compile in the control). From `--stage pch --causes`:

| cause | files | alone | places | what it is, with examples |
|---|---|---|---|---|
| a prototype of ours that differs from libntr's | 428 | 312 | 8 | **one line**, include/gfl/g3d.h:273 `void NNS_G3dMdlSetMdlDiffAll(NNSG3dResMdl *model, int polygonId);`, against libntrsystem's `(NNSG3dResMdl *pMdl, GXRgb col)`, in 423 files (it is the only libntr error of 311 of them); the rest a file's own prototype: MI_CpuFill8's `u32 value` (libntr `u8 data`, 2 files), NNS_G2dSetRndCoreFlipMode, NNS_SndStrmSetup, GXS_GetDispCnt |
| an SDK name our headers have and libntr's lack or spell otherwise (a macro, constant, type or function) | 160 | 44 | 720 | 40 names: pokebw2's `GX_BG_SCRBASE(0x7000)` and `GX_BG_CHARBASE(0)` macros (110 files; libntr has NitroSDK's enums GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000; with them the 71 files' static BG templates also fail "not a compile-time constant"), GX_VRAM_NONE (35; libntr has one per bank, GX_VRAM_BG_NONE...), DEG_TO_IDX (31; FX_DEG_TO_IDX), hw_isDSi (15; OS_IsRunOnTwl()), GX_PLANEMASK_ALL (15; none), GetFileSize (3; FS_GetLength), NNSG2dVRamType (2; NNS_G2D_VRAM_TYPE), NNS_G3D_SIGNATURE_*, the DMA internals of main_020821c0.c (MI_DMA_SET_WAIT, MIDmaRegs), os_init, sys_memset_fast |
| an SDK internal G2x_*_ called with a `REG_*_ADDR` | 94 | 17 | 208 | the decomp's spelling of the SDK's inline G2_SetBlendAlpha and kin: `G2x_SetBlendAlpha_(REG_BLDCNT_ADDR, ...)` (134 calls), G2x_SetBlendBrightness_ (61), G2x_SetBlendBrightnessExt_ (6), G2x_ChangeBlendBrightness_ (2). libntr's X86 registers are variables (nitro/hw/X86/ioreg_G2S.h: `#define REG_DB_BLDCNT_ADDR (&s_reg_G2S_DB_BLDCNT)`) and the internals take a `u64` address: passing a pointer for the integer is an error |
| an SDK struct field our headers name otherwise | 90 | 26 | 331 | 34 fields, pokebw2's names for NitroSystem's: NNSG2dCharacterData and NNSG2dPaletteData `rawData`, `size` (libntrsystem `pRawData`, `szByte`; 52 and 41 files), NNSG2dScreenData `width`, `height`, `size` (`screenWidth`, `screenHeight`, `szByte`), MICAutoParam `loop`, `fullCallback`, `fullArg` (`loop_enable`, `full_callback`, `full_arg`), WMGameInfo `gameId` (`ggid`), FSOverlayInfo, OSDeliverArgInfo, MtxFx43 `rt` |
| an `int` prototype where libntr returns an enum | 19 | 6 | 2 | include/system/rtc.h:32 and 34, `int RTC_GetDate(RTCDate *)` and RTC_GetTime against `RTCResult`: MWCC's `-enum int` makes them one type, clang gives the enum `unsigned int` |
| a header that relied on our SDK headers' includes | 12 | 3 | 14 | include/field/field_map_chunk.h:125 uses HeapID (gfl/heap.h) unincluded (5 files), lib/nitrowifi/include/nitrowifi/soc.h:266 uses SOCInAddr, whose typedef is include/struct_decls.h's, which our nitro/os.h includes (4), GFL_ASSERT, GFL_STD_MemCmp (2) |
| a TWL part libntr lacks | 12 | 2 | 35 | 26 names: MIC_StartLimitedSampling and MIC_StopLimitedSampling (TWL's MIC, 3 files), SSP_StartJpegEncoder, SSP_StartJpegDecoder (the SSP JPEG codec; lib/twl/src has Black's own copy of it), CAMERA_SIZE_DSI_VGA, MI_FreeWram, the camera library's unnamed func_027xxxxx (ov172), OSTWLParentalControl's fields |
| the file defines what libntr's headers define | 3 | 0 | 14 | src/system/net_err.c redefines GXDispCnt and GX_GetDispCnt, GXS_GetVisibleWnd, G2_GetWndOBJInsidePlane (libntr's inlines); src/ov107/mbp.c's static MB_Init, MB_RegisterFile...; lib/gfl/src/clact.c's NNS_G2dGetOamSizeX |
| an SDK internal only in libntr's sources | 1 | 0 | 20 | src/main/main_020821c0.c (MI's DMA): MIi_DmaSetParameters, MIi_CheckDma0SourceAddress |
| other | 2 | 0 | 3 | lib/gfl/src/gfx_loader_data.c:12 and 13, a follow-on of NNSG2dVRamType (clang reads its typedef with the name it suggests, NNSG2dOamType), and src/ov172/camera_system.c:221 (a `void *` for a BOOL) |

Fixed in turn, the causes bring the files that compile to: prototypes 762, names 832, register addresses 883, fields
951, enum-int 968, includes 980, TWL 990, clashes 992, internals 993, other 994 (the control's count). Where the 1345
places are: 1339 on the files' own lines, 5 in game headers (g3d.h:273, rtc.h:32 and 34, field_map_chunk.h:125 and
129), 1 in lib/nitrowifi's soc.h. Every one is the C or our headers, not libntr's: libntr's headers have no error.
Fixing one is a change to the ROM's C or to a header, so each is judged as a respelling is (it must still match).

By area, at stage pch: src/system 132 of 172 compile, src/main 7 of 9, the overlays 273 of 842, lib/gfl 37 of 55,
lib/poke_net 1 of 3.

## The control: our headers for x86_64-linux

994 of 1081 compile. 86 fail on `static declaration of 'X' follows non-static declaration` (165 places, func_02039764
in lib/gfl/src/net_handle.c and kin): a function prototyped in a header and defined `static`. MWCC takes it, and so
does clang for the Windows target the maintainers' notes uses (an MSVC extension); for Linux it is an error. 1 more,
src/ov021/field_task_manager.c:93 and 96, passes an `s32` for a `FieldTaskManager *`. These are the C's own on a
Linux target, with any SDK's headers, and are not counted against libntr above.

## Variants

- **libntr's default SDK 4** (`--sdk 4`: `SDK_VERSION_MAJOR=4UL`, no SDK_TWL, NTR only), stage pch: 447 compile,
  3 fewer than SDK 5. The causes are the same; OS_IsRunOnTwl joins the missing names.
- **libntr with our lib/nnsys** in place of libntrsystem (`--our-nnsys`, pch): 523 compile, but 491 files then fail
  on `MtxFx32`, a pokebw2 type name our nnsys/g2d.h uses and NitroSDK has not (libntr has MtxFx22, MtxFx33, MtxFx43,
  MtxFx44). The NNS_G3dMdlSetMdlDiffAll conflict goes (our lib/nnsys doesn't declare it), and the NNSG2d field names
  with it. libntrsystem's headers are the ones to take: one line blocks 423 files there, a type of our own 491 here.
- **libntrwifi and libntrdwc** are not swapped in: their headers are Nintendo's layout (`dwc.h` with ac/, auth/,
  base/, gs/ ...; `nitroWiFi.h`, `ninet.h`), not pokebw2's `dwc/dwc.h`, `dwc/ext.h`, `nitrowifi/soc.h` (dwc/dwc.h
  alone has 22 #include lines in the game). lib/dwc, lib/nitrowifi, lib/dpw, lib/spl and lib/dsprot stay ours in
  every stage.

## What it says for the port

- libntr's headers drop in at the include level: the 15 header names (or the game's includes respelled to
  `<nitro.h>`), MSL out of the host's way (`-idirafter`), libntr's prefix header. That is 450 files with no change.
- The rest is our side: three header lines (g3d.h:273, rtc.h:32 and 34) alone make 325 more files compile (775),
  the 6 header places 334; then pokebw2's SDK spellings (40 names, 34 fields) where libntr has Nintendo's, and the
  G2x_*_ calls with register addresses, the decomp's spelling of SDK inlines.
- TWL is small at this level: 12 files, all of them DSi-only features (camera, MIC, SSP JPEG, parental control).
- What compiles is not yet what runs: 32-bit assumptions (pointer casts, raw offsets; the maintainers' notes) are
  warnings here, under `-w`.
