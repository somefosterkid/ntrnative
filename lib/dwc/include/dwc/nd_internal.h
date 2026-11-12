// Black overlay 114 ND code: the prototypes of its functions as Black defines them (the file layout of pokebw2 overlay 189).
#ifndef BWPORT_DWC_ND_INTERNAL_H
#define BWPORT_DWC_ND_INTERNAL_H

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/nd.h"

// A request in flight, which the HTTP client's callback gets back (pokebw2's NdTask)
typedef struct NdTask {
    int kind;
    DWCNdCallback callback;
    // A queue to wake when the request has ended
    OSMessageQueue *waiter;
    // What the answer fills: the list of files, the number, or the name of the file
    union {
        DWCNdFileInfo *files;
        int *number;
        char *fileName;
    } target;
    int count;
    char *buffer;
    char *offsetParam;
    char *countParam;
} NdTask;

void *func_ov114_021c569c(u32 param_1, int param_2);
void func_ov114_021c56b4(void *param_1);
void func_ov114_021c56cc(void);
void func_ov114_021c5724(void *arg);
void func_ov114_021c58c8(u32 param_1, u32 param_2, s32 param_3);
void func_ov114_021c5e78(void);
u32 func_ov114_021c5ebc(s32 param_1, s8 *param_2);
u32 func_ov114_021c5f90(s8 *param_1, s8 *param_2, s8 *param_3);
s32 func_ov114_021c6004(s8 *param_1, s8 *param_2, s32 param_3);
s32 decodeChar(u32 param_1);
u32 DWCi_Ndi_getDecodeLenBASE64(s32 param_1);
s32 DWCi_Ndi_getEncodeLenBASE64(s32 param_1);
s32 DWCi_Ndi_encodeBASE64(u8 *param_1, s8 *param_2, s32 param_3);
s32 DWCi_Ndi_decodeBASE64(u8 *param_1, s8 *param_2, s32 param_3);
void *strAlloc(s8 *param_1);
BOOL func_ov114_021c6348(NHttpAllocFunc alloc, NHttpFreeFunc free, s8 *url, s8 *token, s8 *gameCode, s8 *password, void (*onResult)(u32, u32, s32));
void func_ov114_021c64e0(void);
void func_ov114_021c651c(void (*callback)(void));
BOOL func_ov114_021c654c(s32 param_1);
void func_ov114_021c659c(u32 param_1, s32 param_2);
BOOL func_ov114_021c666c(void);
u32 DWCi_Ndi_getApInfo(u8 *param_1);
BOOL func_ov114_021c67fc(s8 *gamecd, s8 *userId);
void DWCi_Ndi_getUserAgent(s8 *param_1);
int func_ov114_021c68bc(int request, int event, void *arg);
void func_ov114_021c6b44(s32 *param_1);
void freeHttp(NdTask *task, s32 request);
BOOL func_ov114_021c6c0c(NdTask **taskOut, s32 *requestOut, const char *action, void *buffer, u32 bufferSize, s8 *logData);
BOOL func_ov114_021c70d4(NdTask *task, s32 request);
BOOL func_ov114_021c7124(DWCNdFileInfo *files, u32 offset, u32 count);
BOOL func_ov114_021c73c0(DWCNdFileInfo *file, void *buffer, u32 size, s8 *logData);
BOOL func_ov114_021c757c(u32 *param_1, s32 *param_2);
BOOL DWCi_Ndi_response_count(s32 param_1, void *param_2);
BOOL func_ov114_021c7614(s32 param_1, s32 param_2);
BOOL DWCi_Ndi_response_list(s32 request, NdTask *task);
u32 DWCi_Ndi_initLockGlobal(void);
void DWCi_Ndi_exitLockGlobal(void);
void DWCi_Ndi_lockGlobal(void);
void DWCi_Ndi_unlockGlobal(void);
u32 DWCi_Ndi_initHttpCallback(void);
void DWCi_Ndi_exitHttpCallback(void);
BOOL DWCi_Ndi_prepareHttpCallback(void);
void DWCi_Ndi_unprepareHttpCallback(void);
void DWCi_Ndi_kickThread(OSMessageQueue *param_1);
void func_ov114_021c79c0(u8 *param_1);
u32 DWCi_Ndi_strcmp(s8 *param_1);
s32 DWCi_Ndi_strncmp(s8 *param_1, s8 *param_2, s32 param_3);
void DWCi_Ndi_memclr(void *param_1, u32 param_2);
void DWCi_Ndi_memcpy(void *param_1, void *param_2, u32 param_3);
s32 DWCi_Ndi_strToInt(s8 *param_1, s32 param_2);
s32 DWCi_Ndi_intToStr(s8 *param_1, u32 param_2);
s32 DWCi_Ndi_intToStrFigure(s8 *p, u64 n, s32 figure);
s32 DWCi_Ndi_hexToStrFigure(s8 *p, u32 n, s32 figure);

#endif
