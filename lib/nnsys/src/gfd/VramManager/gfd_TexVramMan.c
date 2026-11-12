// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nnsys/src/gfd/gfd_TexVramMan.c.
#include "nnsys/gfd.h"

// NitroSystem's gfd_TexVramMan.c: the texture VRAM manager that NNS_GfdAllocTexVram and NNS_GfdFreeTexVram use until
// a manager's init replaces it, which allocates nothing. pokediamond's link map has the name as gfd_texvramman, the
// only one of its GFD names shorter than the 15 characters it cuts them to. swan calls NNS_GfdDefaultFuncAllocTexVram
// and NNS_GfdDefaultFuncFreeTexVram g_TexVRAMAllocFunc and g_TexVRAMFreeFunc

static NNSGfdTexKey AllocTexVram_(u32 szByte, BOOL is4x4comp, u32 opt);
static int FreeTexVram_(NNSGfdTexKey key);

NNSGfdFuncAllocTexVram NNS_GfdDefaultFuncAllocTexVram = AllocTexVram_;
NNSGfdFuncFreeTexVram NNS_GfdDefaultFuncFreeTexVram = FreeTexVram_;

static NNSGfdTexKey AllocTexVram_(u32 szByte, BOOL is4x4comp, u32 opt) {
    return NNS_GFD_ALLOC_ERROR_TEXKEY;
}

static int FreeTexVram_(NNSGfdTexKey key) {
    return -1;
}
