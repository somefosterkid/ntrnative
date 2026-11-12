// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/scfg/src/scfg_proc.c: ported as it is,
// with credit (a project decision), with only the functions Black has, its preprocessor's
// choices for Black's final ROM made, and Black's names (libntr's in the comments). The object's ITCM part,
// SCFGi_SwitchCpuSpeed, is scfg_proc_itcm.c; Black lacks SCFGi_SendPxiData, SCFG_ReadFuseDataAsync, SCFG_ReadFuseData
// and SCFGi_Sync: the linker dead-stripped them.
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "twl/scfg.h"

typedef struct {
  BOOL lock;
  u64 fuseData;
  u32 readFlag;
  SCFGCallback callback;
  void *callbackArg;
} SCFGFuseInfo;

// SCFGi_FuseInfo
SCFGFuseInfo data_02153040;

// SCFGi_CommonCallback
static void func_0208c9c4(int tag, uPtr data, BOOL err);

// SCFG_Init
void func_0208c960(void) {
  PXI_SetFifoRecvCallback(PXI_FIFO_TAG_SCFG, func_0208c9c4);

  data_02153040.lock = FALSE;
}

// SCFG_SetCpuSpeed
void func_0208c988(SCFGCpuSpeed cpuSpeed) {
  OSIntrMode enable;

  enable = OS_DisableInterrupts();

  if (cpuSpeed != SCFG_GetCpuSpeed()) {
    func_01ff87d4(cpuSpeed);
  }

  (void)OS_RestoreInterrupts(enable);
}

#define SCFGi_READ_FUSE_DONE 0xf

static void func_0208c9c4(int tag, uPtr pxiData, BOOL err)
{
#pragma unused(tag, err)
  u16 command =
      (u16)((pxiData & SCFG_PXI_COMMAND_MASK) >> SCFG_PXI_COMMAND_SHIFT);
  u16 ordinal =
      (u16)((pxiData & SCFG_PXI_ORDINAL_MASK) >> SCFG_PXI_ORDINAL_SHIFT);
  u16 data = (u16)((pxiData & SCFG_PXI_DATA_MASK) >> SCFG_PXI_DATA_SHIFT);

  switch (command) {
  case SCFGi_PXI_COMMAND_READ:

    data_02153040.fuseData |= (((u64)data) << (ordinal << 4));
    data_02153040.readFlag |= (1 << ordinal);

    if (data_02153040.readFlag == SCFGi_READ_FUSE_DONE) {
      if (data_02153040.callback) {
        (data_02153040.callback)(data_02153040.fuseData,
                                 data_02153040.callbackArg);
        data_02153040.callback = NULL;
      }
      data_02153040.lock = FALSE;
    }
    break;
  case SCFGi_PXI_COMMAND_READ_OP: {
    u64 tempData;

    *((u16 *)&tempData) = data;
    if (data_02153040.callback) {
      (data_02153040.callback)(tempData, data_02153040.callbackArg);
      data_02153040.callback = NULL;
    }
    data_02153040.lock = FALSE;
  } break;
  default:

    // OS_TPanic("illegal SCFG Pxi."), which a final ROM makes OS_Terminate
    OS_Terminate();
  }
}
