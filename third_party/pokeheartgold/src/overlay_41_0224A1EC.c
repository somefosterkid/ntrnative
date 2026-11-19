#include "global.h"

#include "unk_02005D10.h"
#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "touch_hitbox_controller.h"
#include "sprite.h"

void *ov41_0224A258(u8 *);

s32 ov41_0224A264(u8 *);

s32 ov41_0224A270(u8 *);

void ov41_0224A1EC(u8 *arg0, s32 arg1, s32 arg2);

struct SpriteList;
typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;
typedef void (*UnkFuncPtr_ov22_02259D78)(UnkStruct_ov22_02259C58 *param0, void *param1);
void ov41_0224A238(UnkStruct_ov22_02259C58 *param0, UnkFuncPtr_ov22_02259D78 param1, void *param2, int param3);

void ov41_0224A1EC(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    u8 *v1;

    v0 = 2;
    v1 = arg0 + 0x20;
    do {
        if (v0 != arg1) {
            ov41_0224A264(v1);
        } else if (arg2 == 0) {
            ov41_0224A270(v1);
            PlaySE(0x67CU);
        } else if (arg2 == 2) {
            ov41_0224A258(v1);
        } else {
            ov41_0224A270(v1);
        }
        v0 += 1;
        v1 += 0x10;
    } while (v0 <= 3);
}

void ov41_0224A238(UnkStruct_ov22_02259C58 *param0, UnkFuncPtr_ov22_02259D78 param1, void *param2, int param3) {
    ((param0) ? (void)0 : GF_AssertFail());

    param0->unk_04 = param3;
    param0->unk_08 = param2;
    param0->unk_0C = param1;
}
