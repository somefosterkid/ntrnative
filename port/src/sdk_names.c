// The decomp's names for SDK and NitroSystem functions that libntr and libntrsystem have under Nintendo's names: each
// forwards to libntr's (sdk_black.h has the list and how each name is known). The ones a macro can give are in
// port_compat.h instead (hw_isDSi, GetFileSize, func_0208c030 PM_GetAmp, func_0208c988 SCFG_SetCpuSpeed,
// NNS_G3DExecOp, NNS_SndPlayerSetTrackModDepth_player and ..._ModSpeed_player). The game reaches these from
// src/system/machine_use.c and lib/gfl/src/clact.c (GetCRC16's caller, src/main/main_02087f60.c, is excluded: libntr
// has its DeliverArg API).
#include "sdk_black.h"

// Defined for SDK_PORT in libntr's ctrdg_proc.c, declared for SDK_ARM9 only (nitro/ctrdg/common/ctrdg_common.h)
void CTRDG_DummyInit(void);

// Black's os_init is TwlSDK 5's OS_Init (OSi_InitCommon). On the DS it also starts the cartridge library through the
// game's func_02005438 (CTRDG_DummyInit); libntr's OS_Init calls its own CTRDG_Init
void os_init(void)
{
    OS_Init();
}

u16 GetCRC16(u32 init, void *data, u32 size)
{
    return SVC_GetCRC16(init, data, size);
}

void func_020911a0(void)
{
    CTRDG_DummyInit();
}

void func_027314f0(void)
{
    PM_ReadyToExit();
}

void func_0273154c(PMExitCallbackInfo *info)
{
    PM_AppendPreExitCallback(info);
}

void func_027315c0(BOOL enabled)
{
    PM_SetAutoExit(enabled);
}

BOOL func_027315d0(void)
{
    return PM_GetAutoExit();
}

void func_02065ce4(const NNSG2dCellData *cell)
{
    NNS_G2dDrawCell(cell);
}

void func_02065d4c(NNSG2dCellAnimation *pCellAnim)
{
    NNS_G2dDrawCellAnimation(pCellAnim);
}

void func_02066748(NNSG2dRendererInstance *renderer, const NNSG2dImageProxy *imgProxy,
                   const NNSG2dImagePaletteProxy *pltProxy)
{
    NNS_G2dSetRendererImageProxy(renderer, imgProxy, pltProxy);
}
