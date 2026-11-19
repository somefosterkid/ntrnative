#ifndef POKEHEARTGOLD_OVERLAY_90_H
#define POKEHEARTGOLD_OVERLAY_90_H

struct SysTask;
struct UnkStruct_ov114_0225D084;
struct UnkStruct_ov114_0225F270;
struct UnkStruct_ov114_0225CEF0;
struct UnkStruct_ov114_0225F578;

#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
struct UnkStruct_ov114_0225E854_t;

void ov90_02258914(void *arg0, s32 arg1);
void ov90_02258920(void *arg0, s32 arg1);

s32 ov90_022589BC(u8 *arg0);

void ov90_022589E0(void *arg0, void *arg1, s32 arg2);
void ov90_022589EC(void *arg0, void *arg1, s32 arg2);
void ov90_022589F8(void *arg0, void *arg1, s32 arg2);

void *ov90_02258BD4(s32 arg0, enum HeapID arg1);
typedef struct UnkStruct_ov90_02258C38_ptr UnkStruct_ov90_02258C38_ptr;
void ov90_02258C38(UnkStruct_ov90_02258C38_ptr *arg0);
void ov90_02258C74(void *arg0);
void ov90_02258C8C(void *arg0);
void ov90_02258CB0(void *arg0);
s32 ov90_02258CE0(void *arg0);

void ov90_02259184(void *arg0, enum HeapID arg1);

void ov90_02259250(struct UnkStruct_ov114_0225D084 *arg0, s32 arg1);

void ov90_02259464(u8 *param_1, u32 param_2, u32 param_3, s32 param_4);

s32 ov90_02259538(s32 arg0, s32 arg1);

typedef struct UnkStruct_ov90_0225C178 UnkStruct_ov90_0225C178;
void ov90_022596C8(UnkStruct_ov90_0225C178 *arg0, enum HeapID arg1);

void ov90_02259B18(u32 param_1, s32 param_2);

int ov90_02259B38(const u8 *param0, u32 param1);

void ov90_02259F44(void *arg0, s16 arg1);

u32 ov90_0225A050(u32 param0, u32 param1, u32 param2);

struct UnkStruct_ov114_0225DFFC;
struct UnkStruct_ov114_0225CEF0;
void ov90_0225A088(struct UnkStruct_ov114_0225DFFC *arg0, struct UnkStruct_ov114_0225CEF0 *arg1, enum HeapID heapID);

void ov90_0225A850(struct UnkStruct_ov114_0225E854_t *arg0, enum HeapID arg1);

void ov90_0225B230(struct SysTask *task, void *data);

void ov90_0225B274(struct UnkStruct_ov114_0225E854_t *arg0);

void ov90_0225B340(struct UnkStruct_ov114_0225F270 *arg0, struct UnkStruct_ov114_0225CEF0 *arg1, s8 arg2, enum HeapID arg3);

u8 ov90_0225B538(struct UnkStruct_ov114_0225F270 *arg0);

void ov90_0225B6B0(struct UnkStruct_ov114_0225F578 *arg0, s8 arg1);

s32 ov90_0225B978(struct UnkStruct_ov114_0225E854_t *arg0);

void *ov90_0225BA14(struct UnkStruct_ov114_0225E854_t *arg0);

void ov90_0225BAA0(struct UnkStruct_ov114_0225E854_t *arg0);

void ov90_0225BE08(void *param0);

void ov90_0225C15C(void *arg0);
void ov90_0225C178(void *arg0);
s32 ov90_0225C194(void);
s32 ov90_0225C198(void);

void ov90_0225C19C(int param0, int param1, void *param2, void *param3);
void ov90_0225C1A8(int param0, int param1, void *param2, void *param3);
void ov90_0225C1B8(int param0, int param1, void *param2, void *param3);
void ov90_0225C1C8(int param0, int param1, void *param2, void *param3);
void ov90_0225C1D4(int param0, int param1, void *param2, void *param3);

#endif // POKEHEARTGOLD_OVERLAY_90_H
