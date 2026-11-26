#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct UnkStruct_ov20_021D4210_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
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

void ov102_021EC090(UnkStruct_ov20_021D4210 *param0, int param1);

void ov102_021EC11C(struct UnkStruct_ov20_021D4210_t *arg0);

void ov102_021EC090(UnkStruct_ov20_021D4210 *param0, int param1) {
    int v0, v1;

    if (param1 > 0) {
        v0 = (param0->unk_8C + 120) & 0xff;
        v1 = (v0 + (param1 * 24)) & 0xff;
    } else {
        v1 = param0->unk_8C;
        v0 = (v1 + (param1 * 24)) & 0xff;
    }

    if (v0 < v1) {
        FillWindowPixelRect(&param0->unk_10, 0, 0, v0, 26 * 8, (v1 - v0));
    } else {
        FillWindowPixelRect(&param0->unk_10, 0, 0, v0, 26 * 8, 256 - v0);
        FillWindowPixelRect(&param0->unk_10, 0, 0, 0, 26 * 8, v1);
    }
}

void ov102_021EC11C(struct UnkStruct_ov20_021D4210_t *arg0) {
    *((s32 *)&arg0->unk_8C) = 0x30;
    *((s32 *)&arg0->unk_90) = 0;
    BgSetPosTextAndCommit(arg0->unk_0C, 2U, BG_POS_OP_SET_XSCALE, 0);
}
