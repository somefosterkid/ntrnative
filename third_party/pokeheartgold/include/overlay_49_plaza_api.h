#ifndef POKEHEARTGOLD_OVERLAY_49_PLAZA_API_H
#define POKEHEARTGOLD_OVERLAY_49_PLAZA_API_H

#include "global.h"

#include "overlay_49_unk_struct_ov70_0226da18.h"

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
struct UnkStruct_ov70_022630A4_t;
struct UnkStruct_ov70_02260AD4_t;
struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

UnkStruct_ov70_0225CC54 *ov49_02258C5C(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2, u32 param3);
void ov49_02258E7C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);
UnkStruct_ov70_02260AD4 *ov49_0225A008(const UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225CC40(UnkStruct_ov70_02260AD4 *param0, const UnkStruct_ov70_0225CC54 *param1);
void ov49_02258D54(UnkStruct_ov70_0225CC54 *param0);
UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);
BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);
u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);
void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);
void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);
struct String;
void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const struct String *param1);
void ov49_0225A0BC(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A0CC(UnkStruct_ov70_0225DEE8 *param0);
struct String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);
void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
struct String *ov49_02264C04(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);
u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);
void ov49_0225A53C(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);
void ov49_02258EAC(struct UnkStruct_ov70_0225CA20_t *param0, const struct UnkStruct_ov70_0225CC54_t *param1, int param2, int param3);
void ov49_02258EEC(struct UnkStruct_ov70_0225CA20_t *param0, struct UnkStruct_ov70_0225CC54_t *param1, int param2);
void ov49_0225EF98(struct UnkStruct_ov70_022630A4_t *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

#endif
