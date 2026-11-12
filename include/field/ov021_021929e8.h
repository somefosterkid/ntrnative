#ifndef BWPORT_FIELD_OV021_021929E8_H
#define BWPORT_FIELD_OV021_021929E8_H

#include "types.h"
#include "gfl/heap.h"
#include "field/field_msgbg.h"

void func_ov021_02192fa4(u16 bgId, HeapID heapId);
void func_ov021_02192fec(u16 bgId, HeapID heapId, s32 param_3);
void func_ov021_02192e0c(FieldMsgBG *param_1);
void func_ov021_02192f74(FieldMsgBG *param_1);
BmpWin *func_ov021_021930c0(u16 param_1, HeapID param_2, u16 param_3, u16 param_4, u16 param_5, u16 param_6, u16 param_7);
void func_ov021_02193144(u32 heapId);
void setAlphaBlend(BOOL param_1);
void loadPltt(u32 heapId);
// The fifth parameter, the heap, is passed by the callers and not read
BmpWin *func_ov021_02192b20(u8 param_1, u8 param_2, u8 param_3, u8 param_4, HeapID param_5);

void func_ov021_02193200(MsgBGInput *param_1);
u32 func_ov021_02193228(MsgBGInput *param_1);
BOOL func_ov021_02193260(MsgBGInput *param_1, u32 param_2);
void func_ov021_021932a0(PrintStream *param_1, BOOL param_2);
void func_ov021_02193208(MsgBGInput *param_1, u8 param_2);
s32 func_ov021_02193210(MsgBGInput *param_1);
void func_ov021_02193068(FieldMsgBG *param_1);
#endif // BWPORT_FIELD_OV021_021929E8_H
