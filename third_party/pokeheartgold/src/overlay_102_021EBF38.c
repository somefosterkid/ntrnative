#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

typedef struct UnkStruct_ov102_021EBF38 {
    u8 unk_0[0x30];
    Sprite *unk_30;
} UnkStruct_ov102_021EBF38;

void ov102_021EBF38(UnkStruct_ov102_021EBF38 *param_1, u32 param_2);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct UnkStruct_ov20_021D4210_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;
BOOL ov102_021E9050(const EasyChatApp *easyChatApp);
BOOL ov102_021E9064(const EasyChatApp *easyChatApp);
typedef struct UnkStruct_ov20_021D4210_t UnkStruct_ov20_021D4210;
typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;
BOOL ov102_021EC2EC(UnkStruct_ov20_021D4E8C *param0);
struct UnkStruct_ov20_021D4210_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    BgConfig *unk_0C;
    Window unk_10;
    Window unk_20;
    Sprite *unk_30;
    Sprite *unk_34;
    Sprite *unk_38;
    String *unk_3C;
    UnkStruct_ov20_021D4FF0 unk_40;
    UnkStruct_ov20_021D4E8C unk_60;
    u32 unk_80;
    GXWndPlane unk_84;
    int unk_88;
    int unk_8C;
    int unk_90;
};
BOOL ov102_021EC05C(UnkStruct_ov20_021D4210 *param0);

void ov102_021EBF38(UnkStruct_ov102_021EBF38 *param_1, u32 param_2) {
    VecFx32 VStack_18;

    if (param_2 == 0xffffffff) {
        VStack_18.x = 0xe0000;
        VStack_18.y = 0xb0000;
        VStack_18.z = 0;
        Sprite_SetMatrix(param_1->unk_30, &VStack_18);
        Sprite_SetAnimCtrlSeq(param_1->unk_30, 0x10);
        return;
    }
    VStack_18.x = ((param_2 & 1) * 0x70 + 0x48) * 0x1000;
    VStack_18.y = ((param_2 >> 1) * 0x18 + 0x38) * 0x1000;
    VStack_18.z = 0;
    Sprite_SetMatrix(param_1->unk_30, &VStack_18);
    Sprite_SetAnimCtrlSeq(param_1->unk_30, 8);
    return;
}

u32 ov102_021E8FD8(const EasyChatApp *);

s32 ov102_021EC090(UnkStruct_ov20_021D4210 *param0, int param1);

s32 ov102_021EC13C(void *, s32, u32);

s32 ov102_021EC298(void *, BgConfig *, s32, s32, s32, s32);

void ov102_021EBFA0(void *arg0, s32 arg1);

void ov102_021EBFA0(void *arg0, s32 arg1) {
    s32 v2;
    s32 v1;
    s32 v0;
    s32 v4;
    s32 v3;
    s32 v5;
    s32 v6;
    u32 v7;
    s32 v9;
    s32 v10;
    s32 v12;
    u8 v8;
    u8 v11;

    ov102_021EC090(arg0, arg1);
    v5 = arg1 * 2;
    v3 = v5;
    v9 = *(s32 *)((u8 *)arg0 + 0x90);
    v1 = v9 + v5;
    v4 = 0x18 * arg1;
    v6 = *(s32 *)((u8 *)arg0 + 0x8C);
    v8 = v6 + v4;
    v2 = (s32)v8;
    if (arg1 < 0) {
        v11 = v8;
        v7 = 0 - v3;
        v12 = v1;
        v0 = v7;
        v3 = v7;
    } else {
        v11 = v6 + 0x78;
        v12 = v9 + 0xA;
        v0 = v3;
        if ((u32)(v3 + v12) > ov102_021E8FD8(*(void **)((u8 *)arg0 + 4))) {
            v0 -= 1;
        }
    }
    v10 = 0;
    if (v0 > 0) {
        do {
            ov102_021EC13C(arg0, v12 + v10, v11);
            if (1 & v10) {
                v11 += 0x18;
            }
            v10 += 1;
        } while (v10 < v0);
    }
    *(s32 *)((u8 *)arg0 + 0x8C) = v2;
    *(s32 *)((u8 *)arg0 + 0x90) = v1;
    CopyWindowPixelsToVram_TextMode(arg0 + 0x10);
    ov102_021EC298(arg0 + 0x60, *(void **)((u8 *)arg0 + 0xC), 2, 1, v4, v3);
}

BOOL ov102_021EC05C(UnkStruct_ov20_021D4210 *param0) {
    if (ov102_021EC2EC(&param0->unk_60)) {
        Sprite_SetDrawFlag(param0->unk_34, ov102_021E9050(param0->unk_04));
        Sprite_SetDrawFlag(param0->unk_38, ov102_021E9064(param0->unk_04));
        return 1;
    }

    return 0;
}
