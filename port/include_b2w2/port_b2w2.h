// Black 2's and White 2's prefix after port_prefix.h (port/meson.build: -include, pokebw2's C only; the maintainers' notes): the
// SDK spellings pokebw2's own lib/nitro and lib/nnsys headers have (the port compiles against libntr's and
// libntrsystem's in their place) and libntr spells otherwise or lacks. What can't be said here is a patch to pokebw2's C
// (port/subprojects/packagefiles/pokebw2-*.patch, tools/pokebw2_fetch.sh).
#ifndef BWPORT_PORT_B2W2_H
#define BWPORT_PORT_B2W2_H

// port_compat.h's renames are for Black's own copies of NitroSystem's OAM size inlines
#undef NNS_G2dGetOamSizeX
#undef NNS_G2dGetOamSizeY

// pokebw2's nitro/gx.h: a BG's screen and character base by its offset (libntr: the GX_BG_SCRBASE_0x* constants)
#define GX_BG_SCRBASE(offset) ((GXBGScrBase)((offset) / 0x800))
#define GX_BG_CHARBASE(offset) ((GXBGCharBase)((offset) / 0x4000))
#define GX_VRAM_NONE 0

// The register-address forms take the address as a u64 in libntr (its registers are host memory): pokebw2 passes the
// REG_*_ADDR pointer
#define G2x_SetBGyAffine_(addr, ...) G2x_SetBGyAffine_((u64)(uintptr_t)(addr), __VA_ARGS__)
#define G2x_SetBlendAlpha_(addr, ...) G2x_SetBlendAlpha_((u64)(uintptr_t)(addr), __VA_ARGS__)
#define G2x_SetBlendBrightness_(addr, ...) G2x_SetBlendBrightness_((u64)(uintptr_t)(addr), __VA_ARGS__)
#define G2x_SetBlendBrightnessExt_(addr, ...) G2x_SetBlendBrightnessExt_((u64)(uintptr_t)(addr), __VA_ARGS__)
#define G2x_ChangeBlendBrightness_(addr, ...) G2x_ChangeBlendBrightness_((u64)(uintptr_t)(addr), __VA_ARGS__)

// NitroSystem's resource signatures by pokebw2's names (libntrsystem's res_struct.h; the same values)
#define NNS_G3D_SIGNATURE_BMD0 NNS_G3D_SIGNATURE_NSBMD
#define NNS_G3D_SIGNATURE_BTX0 NNS_G3D_SIGNATURE_NSBTX
#define NNS_G3D_SIGNATURE_BCA0 NNS_G3D_SIGNATURE_NSBCA
#define NNS_G3D_SIGNATURE_BVA0 NNS_G3D_SIGNATURE_NSBVA
#define NNS_G3D_SIGNATURE_BMA0 NNS_G3D_SIGNATURE_NSBMA
#define NNS_G3D_SIGNATURE_BTP0 NNS_G3D_SIGNATURE_NSBTP
#define NNS_G3D_SIGNATURE_BTA0 NNS_G3D_SIGNATURE_NSBTA

// Declared by pokebw2's nitro headers, defined in its C (src/gfl/rtc_cache.c, src/gfl/calctool.c) or libntr
void RTC_GetCachedDate(RTCDate *date);
void RTC_GetCachedTime(RTCTime *time);
void vecfx_mul(const VecFx32 *v, fx32 scale, VecFx32 *dest);
void CTRDG_DummyInit(void);

// pokebw2's types.h: an address as an integer (a u32 there; the host's pointer size here)
typedef uintptr_t uptr;

// The archive stream (the title's music, src/system/snd_strm.c): the port's own player, port/src/strm_arc.c, in place of
// libntrsystem's (whose work left a sound heap block with a pointer cut to 32 bits: the maintainers' notes, "The title's music")
#define NNS_SndArcStrmInit PORT_SndArcStrmInit
#define NNS_SndArcStrmAllocChannel PORT_SndArcStrmAllocChannel
#define NNS_SndArcStrmFreeChannel PORT_SndArcStrmFreeChannel
#define NNS_SndStrmHandleInit PORT_SndStrmHandleInit
#define NNS_SndStrmHandleRelease PORT_SndStrmHandleRelease
#define NNS_SndArcStrmPrepare PORT_SndArcStrmPrepare
#define NNS_SndArcStrmStartPrepared PORT_SndArcStrmStartPrepared
#define NNS_SndArcStrmStart PORT_SndArcStrmStart
#define NNS_SndArcStrmStop PORT_SndArcStrmStop
#define NNS_SndArcStrmGetCurrentPlayingPos PORT_SndArcStrmGetCurrentPlayingPos
struct NNSSndStrmHandle;
struct NNSSndHeap;
void PORT_SndArcStrmInit(unsigned int threadPrio, struct NNSSndHeap *heap);
int PORT_SndArcStrmAllocChannel(int playerNo);
void PORT_SndArcStrmFreeChannel(int playerNo);
void PORT_SndStrmHandleInit(struct NNSSndStrmHandle *handle);
void PORT_SndStrmHandleRelease(struct NNSSndStrmHandle *handle);
int PORT_SndArcStrmPrepare(struct NNSSndStrmHandle *handle, int strmNo, unsigned int offset);
void PORT_SndArcStrmStartPrepared(struct NNSSndStrmHandle *handle);
int PORT_SndArcStrmStart(struct NNSSndStrmHandle *handle, int strmNo, unsigned int offset);
void PORT_SndArcStrmStop(struct NNSSndStrmHandle *handle, int fadeFrames);
unsigned int PORT_SndArcStrmGetCurrentPlayingPos(struct NNSSndStrmHandle *handle);

#endif // BWPORT_PORT_B2W2_H
