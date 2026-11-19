#include "global.h"
struct UnkStruct_ov14_boxapp;
struct UnkStruct_ov70_022630A4_t_14;

#include "sys_task.h"
#include "constants/heap.h"
#include "overlay_14_unk_struct_ov70_022630_a4_t_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"
#include "overlay_103_unk_struct_ov70_022630_a4_t.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0F0C(void *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F0CD8(UnkStruct_ov14_boxapp *);
void ov14_021F2718(void *arg0);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F40E8(void *, s32);
void ov14_021F263C(UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E76B8();
void ov14_021F2690(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021F0B34(UnkStruct_ov14_boxapp *);
void ov14_021F26F0(void *arg0);

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_022630A4_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);
typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;
typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;
void ov14_021F2728(UnkStruct_ov70_022630A4 *param0);

void ov14_021F2634(void *arg0) {
    ov14_021F0AAC(arg0);
}

s32 ov14_021EA180(UnkStruct_ov14_boxapp *app);

void ov14_021F263C(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8248(((u8 *)arg0->sys->unk_2F0));
    ov14_021E82A8(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8328(arg0->sys->unk_2F0);
    ov14_021E884C(arg0->sys);
    ov14_021F40E8(arg0, 0);
    ov14_021F0234(arg0, ov14_021EA180, 0x4A);
}

void ov14_021F2690(UnkStruct_ov14_boxapp *arg0) {
    if (arg0->unk2B == 0) {
        ov14_021E76B8();
    } else {
        ov14_021F40E8(arg0, 0);
    }
    ov14_021F0C0C(arg0);
}

void ov14_021F26B4(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8328(arg0->sys->unk_2F0);
    ov14_021E884C(arg0->sys);
    ov14_021F40E8(arg0, 0);
    ov14_021F0234(arg0, ov14_021EA180, 0x4C);
}

void ov14_021F26F0(void *arg0) {
    ov14_021E76B8();
    ov14_021F0B34(arg0);
}

void ov14_021F2700(UnkStruct_ov70_022630A4_t_14 *arg0) {
    ov14_021F2A18(*((void **)&arg0->unk_08[1].unk_04.unk_04), 9, 0);
    ov14_021F0F0C(arg0, 3);
}

void ov14_021F2718(void *arg0) {
    ov14_021E76B8();
    ov14_021F0CD8(arg0);
}

void ov14_021F2728(UnkStruct_ov70_022630A4 *param0) {
    ov14_021F0F0C(param0, 1);
}
