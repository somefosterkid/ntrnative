// Black's SDK names that libntr and libntrsystem don't have, for the PC build (port/): Black's own additions to the
// SDK (the sys_mem* helpers, MIi_CpuClearFast on PC) and the decomp's names for SDK functions libntr has under
// Nintendo's name (os_init is OS_Init, func_027314f0 PM_ReadyToExit ...). port/src/sdk_mi.c and sdk_names.c define
// them, with the decomp's prototypes (lib/nitro/include, lib/twl/include, lib/nnsys/include, include/gfl/std.h, the
// callers' own declarations). port_compat.h (port_prefix.h) declares the ones the game's files need and has the names
// a macro can give (hw_isDSi, GetFileSize, func_0208c030 ...): none of those may be declared here.
#ifndef PORT_SDK_BLACK_H
#define PORT_SDK_BLACK_H

#include <nitro.h>
#include <nnsys.h>

#ifdef __cplusplus
extern "C" {
#endif

// mi_memory.c: Black's memory helpers (the source first, as the SDK's MI_ functions have it)
void sys_memset_fast(void *dest, u32 value, u32 size);       // memset: value's low byte in size bytes
void sys_memcpy_fast(const void *src, void *dest, u32 size); // memcpy, forwards
void sys_memcpy_ex(const void *src, void *dest, u32 size);   // memmove
// NitroSDK's MIi_CpuClearFast (libntr declares it in nitro/mi/memory.h, defines it only for ARM): size bytes of words
// of data
void MIi_CpuClearFast(u32 data, void *destp, u32 size);

// The decomp's names for SDK functions libntr has (port/src/sdk_names.c forwards each)
void os_init(void);                                    // OS_Init
u16 GetCRC16(u32 init, void *data, u32 size);          // SVC_GetCRC16
void func_020911a0(void);                              // CTRDG_DummyInit
void func_027314f0(void);                              // PM_ReadyToExit
void func_0273154c(PMExitCallbackInfo *info);          // PM_AppendPreExitCallback
void func_027315c0(BOOL enabled);                      // PM_SetAutoExit
BOOL func_027315d0(void);                              // PM_GetAutoExit

// NitroSystem: the decomp's names for libntrsystem's functions
void func_02065ce4(const NNSG2dCellData *cell);                // NNS_G2dDrawCell
void func_02065d4c(NNSG2dCellAnimation *pCellAnim);            // NNS_G2dDrawCellAnimation
void func_02066748(NNSG2dRendererInstance *renderer, const NNSG2dImageProxy *imgProxy,
                   const NNSG2dImagePaletteProxy *pltProxy);    // NNS_G2dSetRendererImageProxy

#ifdef __cplusplus
}
#endif

#endif
