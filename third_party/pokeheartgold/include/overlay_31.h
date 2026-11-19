#ifndef POKEHEARTGOLD_OVERLAY_31_H
#define POKEHEARTGOLD_OVERLAY_31_H

#include "global.h"
#include "bg_window.h"
#include "field_types_def.h"
#include "overlay_03.h"
#include "sys_task.h"
#include "pm_string.h"
#include "message_format.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

enum {
    MART_SUBSCREEN_WIN_ITEM_0, // 0 - 5: the six listed items
    MART_SUBSCREEN_WIN_SELECTED_ITEM = 6,
    MART_SUBSCREEN_WIN_UNK_7,
    MART_SUBSCREEN_WIN_UNK_8,
    MART_SUBSCREEN_WIN_UNK_9,
    MART_SUBSCREEN_WIN_OWNED,
    MART_SUBSCREEN_WIN_TOTAL,
    MART_SUBSCREEN_WIN_MESSAGE_BOX,
    MART_SUBSCREEN_WIN_COUNT,
};

// Touch-screen half of the Pokeathlon mart; lives in the SysTask environment.
typedef struct MartSubScreen {
    u32 unk_0;
    BgConfig *bgConfig;
    SysTask *sysTask;
    s32 state;
    u8 unk_10[4];
    MartData *martData;
    u16 *lastTouchMenuInput;
    FieldSystem *fieldSystem;
    u8 unk_20[0x10];
    u32 unk_30;
    Window unk_34;
    Window msgWindow;
    Window pageWindow;
    Window unk_64;
    Window moneyWindow;
    Window windows[MART_SUBSCREEN_WIN_COUNT];
    MessageFormat *messageFormat;
    MsgData *msgData;
    MsgData *itemMsgData;
    u8 unk_160[4];
    Options *options;
    PlayerProfile *playerProfile;
    PokeathlonSave *pokeathlonSave;
    YesNoPrompt *yesNoPrompt;
    u8 unk_174[0x10];
    u32 unk_184;
    String *string;
    u8 unk_18C[4];
} MartSubScreen;

void ov31_0225D684(MartSubScreen *screen, s32 arg1);
SysTask *ov31_0225D520(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, MartData *martData);
void ov31_0225D710(BgConfig *arg0, SysTask *arg1);

u32 ov31_0225D75C(MartSubScreen *screen, s32 param_2);
void ov31_0225D7A0(SysTask *task, void *arg1);
void ov31_0225D83C(MartSubScreen *screen, u32 arg1);
void ov31_0225D9D4(MartSubScreen *screen, s32 arg1);

s32 ov31_0225DAA4(MartSubScreen *screen, s32 param_2);
void ov31_0225DAC4(MartSubScreen *screen);
void ov31_0225DB38(MartSubScreen *screen);
void ov31_0225DBA0(MartSubScreen *screen);
void ov31_0225DBD4(MartSubScreen *screen);
void ov31_0225DCA8(MartSubScreen *screen);
void ov31_0225DCF4(MartSubScreen *screen);
void ov31_0225DD14(MartSubScreen *screen);
void ov31_0225DE84(MartSubScreen *screen);
void ov31_0225DF98(MartSubScreen *screen);

u8 ov31_0225DE00(MartSubScreen *screen, Window *param_2, String *param_3, s32 unused);

void ov31_0225DE24(MessageFormat *arg0, MsgData *arg1, Window *arg2, s32 arg3, s32 arg4);

void ov31_0225E060(BgConfig *arg0, s32 arg1, s32 arg2);
void ov31_0225E0E4(MartSubScreen *screen, s32 arg1);

s32 ov31_0225E12C(MartData *data, s32 arg1, s32 arg2);

void ov31_0225E184(MartSubScreen *screen);
void ov31_0225E20C(MartSubScreen *screen, u16 unused);
void ov31_0225E2D4(MartSubScreen *screen, s32 arg1);
void ov31_0225E474(MartSubScreen *screen);
void ov31_0225E4BC(u32 arg0, MessageFormat *arg1, u32 arg2, u32 arg3);
void ov31_0225E4EC(u32 arg0, MessageFormat *arg1, u32 arg2, u32 arg3);
void ov31_0225E51C(u32 arg0, MessageFormat *arg1, u32 arg2, u32 arg3);

void ov31_0225E54C(MartSubScreen *screen);
void ov31_0225E5FC(MartSubScreen *screen);
void ov31_0225E7D4(MartSubScreen *screen);

void ov31_0225E700(MartSubScreen *screen);

s32 ov31_0225E774(MartSubScreen *screen);

u32 ov31_0225E948(u32 param_1, s32 param_2);

void ov31_0225EA08(MartSubScreen *screen);

void ov31_0225EA9C(MartSubScreen *screen);

void ov31_0225EB30(MartSubScreen *screen);
void ov31_0225EBC4(MartSubScreen *screen);

void ov31_0225EC58(MartSubScreen *screen);

void ov31_0225EDA0(MartSubScreen *screen);

#endif // POKEHEARTGOLD_OVERLAY_31_H
