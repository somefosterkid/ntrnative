#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "overlay_70_gtsapplication_state.h"
s32 ov70_022450B8(void *, String *, s32, s32, s32, s32);

void ov70_0223CC04(BgConfig *arg0, void *arg1, MsgData *arg2, s32 arg3);

s32 ov70_0223CC68(GTSPokemonRequirements *arg0, GTSPokemonRequirements *arg1, s32 arg2, s32 arg3);
void ov70_0223CCA4(GTSApplicationState *arg0);
void ov70_0223CD28(s32 arg0);
s32 ov70_0223CE44(BgConfig *);
s32 ov70_0223CF74(void *);
s32 ov70_0223D058(void *);
s32 ov70_0223D26C(void *);
s32 ov70_0223D3BC(GTSApplicationState *arg0);
s32 ov70_0223E264(void *, s32);
s32 ov70_02241358(void *);
SysTask *ov70_02245124(void *);
s32 ov70_0223CD44(struct GTSApplicationState *arg0);

void ov70_0223CC04(BgConfig *bgConfig, void *arg1, MsgData *msgData, s32 arg3) {
    String *string;
    u8 palette;
    s32 v0;

    v0 = 0x10200;
    if (arg3 != 0) {
        v0 = 0xF0200;
        palette = 0;
    } else {
        palette = 2;
    }
    BgTilemapRectChangePalette(bgConfig, 1U, 0x12U, 2U, 0xEU, 4U, (u8)palette);
    BgCommitTilemapBufferToVram(bgConfig, 1U);
    string = NewString_ReadMsgData(msgData, 0x45);
    ov70_022450B8(arg1, string, 0, 0, 0, v0);
    String_Delete(string);
}

s32 ov70_0223CC68(GTSPokemonRequirements *arg0, GTSPokemonRequirements *arg1, s32 arg2, s32 arg3) {
    if ((arg0->species == arg1->species) && (arg0->gender == arg1->gender) && (arg0->level == arg1->level) && (arg0->level2 == arg1->level2) && (arg2 == arg3)) {
        return 1;
    }
    return 0;
}

void ov70_0223CCA4(GTSApplicationState *arg0) {
    BgSetPosTextAndCommit(arg0->bgConfig, 0U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 1U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 3U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 4U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 6U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 7U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
}

void ov70_0223CD28(s32 brightness) {
    if (brightness != 0) {
        G2_SetBlendBrightness(0xA, brightness);
        return;
    }
    *(s16 *)REG_BLDCNT_ADDR = 0;
}

s32 ov70_0223CD44(GTSApplicationState *arg0) {
    ov70_0223D3BC(arg0);
    *(u16 *)REG_POWCNT_ADDR &= 0xFFFF7FFF;
    ov70_0223CE44(*((void **)&arg0->bgConfig));
    ov70_0223CF74(arg0);
    ov70_0223D26C(arg0);
    ov70_0223D058(arg0);
    ov70_02241358(arg0);
    if (GXS_GetMasterBrightness() == 0) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    }
    ov70_0223E264(arg0, arg0->selectedBoxId);
    ov70_02245124(arg0);
    *((s32 *)&arg0->currentScreenInstruction) = 0;
    return 2;
}
