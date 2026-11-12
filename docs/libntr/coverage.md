# libntr API coverage for Black

Measured 2026-10-28 on WSL (gcc 15.2, meson 1.10.1, SDL2 2.32, enet 1.3.18). Nothing here changes the decomp's C. Clones and builds are in <scratch>, scripts and tables in <scratch> (need.py, classify.py, body.py, scen.py; *_class.tsv, *_body.tsv).

## What was built

| repo | commit | license | PC build |
|---|---|---|---|
| libntr (github.com/cybervisi0n/libntr) | f6b3a69 (2026-10-27; ~/ref/libntr is 4 commits older, 1555d91) | MIT | 38 static libs, no errors |
| libntrsystem | 97fa995 (NitroSystem 071126) | **none** (no LICENSE file; GitHub reports no license) | fnd g2d g3d gfd mcs snd |
| libntrwifi | e3a86b0 (NitroWiFi 2.1) | **none** | cps soc ssl wcm (+2 stub libs) |
| libntrdwc | c662226 (NitroDWC 2.2) | **none**; its GameSpy SDK files say "Copyright GameSpy Industries" | ac account auth base bm gs/* nd nhttp |

No license means all rights reserved: the three sister libraries can be read and measured, but a port can't ship them without the author's permission. Their prebuilt/*.a (libcps, libssl, libdwcutil, libdwcauth ...; libntr's libcht, libdgt, libe3s) are ARM binaries that only build_target=arm uses; the PC targets build C sources in their place, except dwc's ranking, util (dwcutil), gs/pinger and gs/pt, which have no PC build at all.

Commands that work:

    git clone https://github.com/cybervisi0n/libntr.git          # and libntrsystem, libntrwifi, libntrdwc
    cd libntr && meson setup ../b-ntr-linux5 -Dbuild_target=linux -Dsdk_version_major=5 -Dhardware_type=twl_hyb -Dbuildtype=debug
    ninja -C ../b-ntr-linux5                                     # 378 steps, about a minute
    cd libntrdwc/subprojects && ln -s ../../libntr libntr && ln -s ../../libntrsystem libntrsystem && ln -s ../../libntrwifi libntrwifi
    cd .. && meson setup ../b-dwc-linux5 -Dbuild_target=linux -Dlibntr:sdk_version_major=5 -Dlibntr:hardware_type=twl_hyb -Dbuildtype=debug
    ninja -C ../b-dwc-linux5                                     # 660 steps, all four projects

sdk_version_major=4 (the NitroSDK 4.2 API) builds too: 8,179 defined symbols against 9,089 for 5 (TWL). Black's SDK is TwlSDK, so the numbers below are the SDK 5 twl_hyb build's. build_target is arm, linux, win64 or nx (the sister libraries add prospero): **every PC target is 64-bit** (-DSDK_64BIT, x86_64). There is no 32-bit target, so port-survey.md's first target (i686, /Zp4) can't use libntr: the 3,490 pointer/int casts have to become uintptr_t first.

## How it was measured

- **Needs**: tools/port_survey.py's elf() over every C object of the ROM build (build/b_us): a symbol one library's object references and another library defines. 563 SDK symbols (lib/nitro, lib/twl) are referenced from outside the SDK (port-survey.md's number), 233 NitroSystem (lib/nnsys), 147 DWC (lib/dwc, lib/dwcutil), 70 NitroWiFi.
- **Names**: the decomp's unnamed functions get their SDK name from the comment above them ("pokebw2's CAMERA_InitCore") or from reading them (alias_manual.tsv, 18: hw_isDSi = OS_IsRunOnTwl, NNS_G3DExecOp = NNS_G3dGeBufferOP_N, func_02065ce4 = NNS_G2dDrawCell ...).
- **Defined**: nm --defined-only over the PC build's archives (9,089 libntr symbols; 828 libntrsystem, 297 libntrwifi, 1,487 libntrdwc), then the headers preprocessed with the PC flags (-DSDK_PORT -DSDK_64BIT -DSDK_TWL -DSDK_TWLHYB ...) for macros and static inlines (OS_CreateThread is a macro for OS_CreateThreadDebug; MATH_MD5/SHA1 and SNDEX_* are inlines over DGT_* and SNDEXi_*, which libntr defines).
- **Stubs**: each defined function's body from its source preprocessed with its own compile_commands.json line (-E -P -C): empty, a constant return, or a "Not implemented"/TODO marker. One level deep: a real body that calls a stub isn't caught.

## (1) NitroSDK/TwlSDK against libntr

| module | used | on PC | stub | missing |
|---|---|---|---|---|
| nitro/gx | 97 | 97 | 0 | 0 |
| nitro/os | 90 | 89 | 0 | 1 |
| nitro/fx | 58 | 54 | 4 | 0 |
| nitro/snd | 51 | 51 | 0 | 0 |
| nitro/wm | 48 | 47 | 0 | 1 |
| nitro/spi | 41 | 38 | 0 | 3 |
| nitro/fs | 28 | 25 | 1 | 2 |
| twl/camera | 23 | 22 | 0 | 1 |
| twl/nwm | 20 | 0 | 0 | 20 |
| nitro/math | 18 | 18 | 0 | 0 |
| nitro/mi | 18 | 13 | 0 | 5 |
| nitro/card, mb, rtc, pxi, init | 12, 12, 8, 4, 1 | all | 0 | 0 |
| twl/mi | 10 | 7 | 0 | 3 |
| nitro/std | 8 | 7 | 0 | 1 |
| twl/dsp | 6 | 0 | 0 | 6 |
| twl (SSP JPEG) | 3 | 0 | 0 | 3 |
| nitro/crypto | 2 | 0 | 0 | 2 |
| twl/snd (SNDEX), twl/scfg | 2, 1 | all | 0 | 0 |
| nitro/syscall | 1 | 0 | 1 | 0 |
| nitro/ctrdg | 1 | 0 | 0 | 1 |
| **all** | **563** | **508** | **6** | **49** |

By caller (scen.py): the game itself (src, lib/gfl, lib/poke_net) uses 375 of them: 350 on PC, 6 stubs, 19 missing. With the libraries a port keeps as decomp C (spl, dpw, dsprot, msl): 384, 359 / 6 / 19. The decomp's own nnsys adds 2 missing (MI_SendNDmaGXCommandAsync, ...Fast); its dwc and nitrowifi add the other 28 (all 20 NWM, CRYPTO_RC4Init/Encrypt, PM and FS helpers), which go away when libntrsystem, libntrdwc and libntrwifi replace them.

The game's 6 stubs and 19 missing:
- **Stubs**: MTX_Copy33To44_, MTX_Copy43To44_ and MTX_Scale33_ are SIM_assert_always_msg("Not implemented"), which raise(SIGTRAP)s on Linux (the process dies without a debugger) and prints and goes on on Windows. MTX_Scale22_ is empty (//TODO). Their callers: src/system/mcss.c, src/ov112/musical_mcss.c, src/ov094/btlv_effvm.c and btlv_input.c, src/ov010/building_enter_effect.c, src/ov021/field_3dbg.c. SVC_GetCRC16 returns 0 (// TODO: stubbed), which src/main/main_02087f60.c's checksum uses. FS_LoadOverlay is empty on PC, where the overlays are linked in (lib/gfl/src/gf_overlay.c). libntr has 8 "Not implemented" fx functions in all (Transpose33/43/44, Copy33To43 and Scale44 too).
- **Trivial ones that are Black's own SDK additions** (libntr has no such name; each is a libc call): sys_memcpy_ex, sys_memcpy_fast, sys_memset_fast, MIi_CpuClearFast (asm only, 25 game files), GetFileSize (FS_GetFileLength), func_02086164 (os_mutex.c), func_0208c030 (pm.c), func_020911a0 (ctrdg_proc.c).
- **TWL hardware**, all from src/ov172 (the C-Gear Xtransceiver video chat): SSP_StartJpegEncoder, SSP_StartJpegDecoder, SSP_GetJpegEncoderBufferSize (header only in libntr); DSP_PlayShutterSoundCore, DSP_StopSoundCore, DSP_IsShutterSoundPlayingCore, DSP_OpenStaticComponentG711Core, DSP_LoadG711Core, DSP_UnloadG711Core (libntr's dsp has none of them); the camera stop func_02700bd0; MI_Camera_NDmaRecvAsync (the source exists but isn't built for PC).

What "on PC" means at run time (from reading the code; nothing was run):
- OS threads are SDL host threads that run at the same time. OS_DisableInterrupts and OS_RestoreInterrupts return 0 and do nothing, so the DS's single-core priority scheduling isn't reproduced (src/system/snd_sys.c and lib/poke_net create threads).
- The ARM7 is simulated (libraries/sim7) only for the PXI tags OS, SOUND, TOUCHPANEL, MIC, NVRAM, PM, WM and WVR. Nothing answers CAMERA, SNDEX, MI or DSP: CAMERA_InitCore returns FATAL_ERROR, and SNDEXi_Init loops forever on PXI_IsCallbackReady(PXI_FIFO_TAG_SNDEX). The game doesn't reach them by default: OS_IsRunOnTwl (the game's hw_isDSi, 16 files) reads a byte of the zeroed s_HW_PRV_WRAM and returns FALSE, so the game runs as on a DS (NTR) and skips its DSi paths.
- Registers, VRAM and main memory are arrays (s_HW_MAIN_MEM is 8 MB; s_reg_GX_*). HW_IOREG stays 0x04000000, so a raw register address faults on PC: the game's 280 register casts, such as src/ov231/ir_check.c's (vu16 *)0x040001a0.
- FS: with SDK 4, FS_OpenFile fopen's the path in the working directory (the extracted ROM's files and header.bin; the sim's ImGui dialog extracts a .nds). With SDK 5, FS_OpenFile and FS_OpenFileFast go through the archive commands instead. Not run here.
- The SDK's structs are bigger on PC (clang AST: the decomp's headers for ARM against libntr's for x86_64): OSThread 192 / 424 bytes, FSFile 72 / 136, FSArchive 92 / 152, OSMutex 24 / 56, OSAlarm 44 / 72, OSMessageQueue 32 / 64, CARDRomHeader 352 / 360. GXOamAttr (8), MtxFx43 (48), RTCDate (16) and WMBssDesc (192) are the same. The game's records that embed these, and its raw offsets into them, have to follow.

## (2) NitroSystem, DWC and NitroWiFi against the sister libraries

| library | used by the game + kept libs (by all callers) | defined on PC | missing |
|---|---|---|---|
| nnsys (fnd g2d g3d gfd snd) | 219 (233) | **219 (233)** | 0 |
| nitrowifi (soc cps ssl wcm) | 11 (70) | 11 | 0 (the decomp's dwc's own calls: 9 WCM functions) |
| dwc + dwcutil | 147 | 48 | 9 named, 90 unnamed |

NitroSystem: all 233 are in libntrsystem once the decomp's names NNS_SndPlayerSetTrackModDepth_player and ..._ModSpeed_player, NNS_G3DExecOp and four func_ ones are mapped to libntrsystem's. No stub among them (NNS_G2dGetUnpackedPaletteData only has a TODO comment).

NitroWiFi: libntrwifi's PC CPS (cps/src/network.c, 4,614 lines) maps the DS socket calls to host BSD/Winsock sockets (socket, connect, select, recv, gethostbyname); its SSL is the DS's own C (ssl.c, biginteger.c). Its WCM is the NTR one: the 9 WCM functions Black's own DSi-era dwc calls (WCM_GetState, WCM_ContinueSearchAsync, WCM_FillApEntry, WCM_InitCore ...) aren't there, so the decomp's dwc can't sit on libntrwifi as it is.

DWC, by module (libntrdwc is NitroDWC 2.2; Black's DWC has more):
- Modules libntrdwc doesn't have: **sc** (20 used), **vct** voice chat (18), **gdb**/sake (10), **gamestats** (6), **crypt** (2: Crypt_VerifySignature, Crypt_SetAllocator), **dwcutil** on PC (2: DWC_StartUtility, Calloc; ARM prebuilt only), and wds: 58 of the 147.
- Named and missing in modules it has: DWCi_GetBssDescParseWorkSize, DWCi_BssDescToApInfo (base), DWC_CheckProfanityProcess (auth: the source is there, not built).
- 38 used functions are still unnamed in the decomp in modules libntrdwc has (account 9, base 12 in dwc_nd/nasfunc/transport, the d_nhttp wrapper 14, auth 2, bm 1): they need names before they can be matched.

## (3) What none of them has: what a port writes

- **IR** (the C-Gear's infrared battles and trades): src/ov015/irc_spi.c, irc_link.c, net_irc.c, net_devirc.c and src/ov231/ir_check.c drive the card's IR chip themselves, through the card SPI registers 0x040001a0/0x040001a2. libntr has no IR device. A port replaces irc_spi.c, for example with LAN like libntr's WM over enet.
- **DSi camera, DSP and JPEG** (src/ov172, the Xtransceiver): libntr builds the camera and dsp libraries but nothing serves their ARM7/DSP side. It has no SSP (JPEG encoder/decoder) and no DSP shutter sound or G711 component. A port needs a host camera, a JPEG codec and G711, or keeps the feature off (OS_IsRunOnTwl FALSE).
- **NWM** (DSi wireless; 20 functions, all from the decomp's nitrowifi and dwc): not in libntr (header only). Not needed with libntrwifi's NTR WCM, which in turn lacks Black's DSi-only WCM calls.
- **The DSi sound extension** (SNDEX_Init, SNDEX_SetIirFilter; src/system/snd_sys.c, snd_mic.c, src/ov172/ctvt_mic.c): libntr has SNDEXi_* but no ARM7 handler, harmless only while OS_IsRunOnTwl is FALSE. SCFG_SetCpuSpeed and MI_CancelWram are in libntr.
- **dsprot** (lib/dsprot, the anti-piracy: ROM integrity, RC4, MAC owner; the game calls 6 entry points, func_ov230_021882a0 to 021883cc): nothing to emulate. A port makes the 6 calls report "check passed".
- **CRYPTO_RC4Init/Encrypt** (dwc): not in libntr; a few lines of C.
- Online play: Nintendo WFC is gone. libntrwifi's sockets reach whatever server the DWC host names resolve to.
