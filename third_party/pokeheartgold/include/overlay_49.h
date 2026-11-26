#ifndef POKEHEARTGOLD_OVERLAY_49_H
#define POKEHEARTGOLD_OVERLAY_49_H

#include "global.h"
#include "overlay_42.h"
#include "overlay_manager.h"
#include "list_menu.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"
#include "filesystem.h"
#include "unk_struct_ov49_02268db0.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov70_02260BB8_t;
struct UnkStruct_ov70_022610B8_t;
struct UnkStruct_ov70_02261E10_t;

void ov49_02258800(const UnkStruct_ov44_02232914 *param0, VecFx32 *param1);
void ov49_02258814(const VecFx32 *param0, UnkStruct_ov44_02232914 *param1);

s32 ov49_02258A30(s32 arg0);
s32 ov49_02258A50(s32 arg0);
s32 ov49_02258A70(s32 arg0);
s32 ov49_02258A90(s32 arg0);

void ov49_0225940C(void);

s32 ov49_02259EF8(OverlayManager *arg0, int *state);

void ov49_0225A044(struct UnkStruct_ov70_0225DEE8_t *arg0);

u8 ov49_0225A064(const struct UnkStruct_ov70_0225DEE8_t *arg0, s32 arg1);

void ov49_0225A10C(struct UnkStruct_ov70_0225DEE8_t *arg0, u32 arg1);
void ov49_0225A120(struct UnkStruct_ov70_0225DEE8_t *arg0, u32 arg1, u32 arg2);

void ov49_0225A204(struct UnkStruct_ov70_0225DEE8_t *param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5);

void ov49_0225A23C(struct UnkStruct_ov70_0225DEE8_t *arg0, struct String *arg1, u32 arg2, u32 arg3);

void ov49_0225A428(struct UnkStruct_ov70_0225DEE8_t *arg0, s32 arg1, s32 arg2);
void ov49_0225A478(struct UnkStruct_ov70_0225DEE8_t *arg0, s32 arg1);

s32 ov49_0225A55C(struct UnkStruct_ov70_0225DEE8_t *arg0);

void ov49_0225B058(ListMenu *param0, u32 param1, u8 param2);

void ov49_0225B444(struct UnkStruct_ov70_0225DEE8_t *arg0);

void ov49_0225BBCC(void);

s32 ov49_0225CB70(struct UnkStruct_ov70_0225DEE8_t *arg0);

struct UnkStruct_ov70_02260AD4_t {
    struct Camera *camera;
    void *unk_4;
    VecFx32 pos;
};

void ov49_0225CC28(struct UnkStruct_ov70_02260AD4_t *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov49_0225CDEC(void *arg0, s8 arg1, s8 arg2, enum HeapID arg3, enum HeapID arg4);

void ov49_0225D394(struct UnkStruct_ov70_02260BB8_t *arg0, struct UnkStruct_ov70_022610B8_t *arg1);

void ov49_0225D528(Field3dModel *param0, NARC *param1, u32 param2, u32 heapID);

void ov49_0225E47C(struct UnkStruct_ov70_02261E10_t *arg0, u32 arg1);
void ov49_0225E4A4(struct UnkStruct_ov70_02261E10_t *arg0, u32 arg1);

void ov49_0225EFF0(u8 *arg0, u32 arg1, s32 arg2);

BOOL ov49_0225FA14(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2);
BOOL ov49_0225FB5C(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2);
BOOL ov49_0225FCA8(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2);

u32 ov49_02260230(struct UnkStruct_ov70_02263344_t *param0, struct UnkStruct_ov70_0225DEE8_t *param1);

s32 ov49_02260C58(u32 arg0);
s32 ov49_02260CC0(u32 arg0);
s32 ov49_02260D28(u32 arg0);

BOOL ov49_022618C0(struct UnkStruct_ov70_02263344_t *param0, struct UnkStruct_ov70_0225DEE8_t *param1, u32 param2);

BOOL ov49_02262AC4(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2);

void ov49_02264C50(struct UnkStruct_ov70_0225DEE8_t *param0, u32 param1, u32 param2);
BOOL ov49_02264CA8(UnkStruct_ov49_02264CF8 *arg0, struct UnkStruct_ov66_0222DFF8_t *arg1, u8 arg2);
void ov49_02264CF8(void);

void ov49_02264D14(UnkStruct_ov49_02264CF8 *arg0, void *arg1);
void ov49_02264D30(UnkStruct_ov49_02264CF8 *arg0, void *arg1, void *arg2);

void ov49_02264D9C(void *arg0, s32 arg1, s32 arg2, void *arg3);

void ov49_02264EC8(UnkStruct_ov49_02264CF8 *arg0, s32 arg1);
void ov49_02264F10(UnkStruct_ov49_02264CF8 *arg0);
void ov49_02264F1C(UnkStruct_ov49_02264CF8 *arg0);
void ov49_02264F24(UnkStruct_ov49_02264CF8 *arg0, s32 arg1);
void ov49_02264F60(UnkStruct_ov49_02264CF8 *arg0);
struct WiFiPlazaProfile;
void ov49_02264F78(UnkStruct_ov49_02264CF8 *arg0, const struct WiFiPlazaProfile *profile);

void ov49_0226526C(s16 *arg0);
void ov49_02265274(void *arg0, void *arg1, s16 arg2, s16 arg3);
void ov49_0226529C(void *arg0, void *arg1);
s32 ov49_022652D0(u16 *arg0);
u16 ov49_022652E0(void *arg0);

void ov49_02265C68(void *arg0, void *arg1);
void ov49_02265C74(void *arg0, void *arg1);
void ov49_02265C80(void *arg0, void *arg1);
void ov49_02265C8C(void *arg0, void *arg1);
void ov49_02265C98(void *arg0, void *arg1);
void ov49_02265CA4(void *arg0, void *arg1);

void ov49_02266354(void *arg0, void *arg1);
void ov49_02266360(void *arg0, void *arg1);
void ov49_0226636C(void *arg0, void *arg1);
void ov49_02266378(void *arg0, void *arg1);
void ov49_02266384(void *arg0, void *arg1);
void ov49_02266390(void *arg0, void *arg1);

void ov49_0226639C(s32 arg0, s32 arg1);

void ov49_02266748(void *arg0, void *arg1);
void ov49_02266754(void *arg0, void *arg1);
void ov49_02266760(void *arg0, void *arg1);
s32 ov49_0226676C(void *arg0, void *arg1);

s32 ov49_02266AB0(void *arg0, void *arg1);

void ov49_022686C0(s32 arg0, s32 arg1);

void ov49_022686E4(u8 *arg0);

void ov49_02268B04(void);
void ov49_02268B08(void);

void ov49_02268CEC(UnkStruct_ov49_02268DB0 *arg0);

void ov49_022693A4(BOOL param0, BOOL param1, BOOL param2, u32 seqID);
void ov49_022693D4(BOOL param0, BOOL param1, BOOL param2, u32 species);

void ov49_02269474(void *arg0);
void ov49_02269494(void *arg0);

#endif // POKEHEARTGOLD_OVERLAY_49_H
