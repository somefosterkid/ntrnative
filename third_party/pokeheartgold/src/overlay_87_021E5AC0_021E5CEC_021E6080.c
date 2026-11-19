#include "global.h"

#include "overlay_87_scratch_off_card_app.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "struct_defs/resdat_narc_structs.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_printer.h"

typedef struct PaletteAnimator PaletteAnimator;

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;





typedef u16 charcode_t;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
};

struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
};

s32 ov87_021E6AE0(ScratchOffCardApp *);

s32 ov87_021E6AF4(ScratchOffCardApp *);

s32 ov87_021E6B28(ScratchOffCardApp *);

u8 ov87_021E7008(ScratchOffCardApp *);

u8 ov87_021E708C(ScratchOffCardApp *);

s32 ov87_021E70D0(ScratchOffCardApp *);

u8 ov87_021E7134(ScratchOffCardApp *);

u8 ov87_021E717C(ScratchOffCardApp *);

u8 ov87_021E71B4(ScratchOffCardApp *);

u8 ov87_021E71EC(ScratchOffCardApp *, u8);

u8 ov87_021E7490(ScratchOffCardApp *);

s32 ov87_021E74F4(ScratchOffCardApp *, s32);

s32 ov87_021E7550(ScratchOffCardApp *);

s32 ov87_021E75E0(ScratchOffCardApp *, u8);

void ov87_021E7698(ScratchOffCardApp *app);

s32 ov87_021E7734(ScratchOffCardApp *);

s32 ov87_021E788C(ScratchOffCardApp *);

s32 ov87_021E78D8(ScratchOffCardApp *, u8);

s32 ov87_021E7990(ScratchOffCardApp *);

s32 ov87_021E7998(ScratchOffCardApp *);

s32 ov87_021E79A0(ScratchOffCardApp *);

s32 ov87_021E7FD4(ScratchOffCardsAppSprite *, s32);

s32 ov87_021E7FEC(ScratchOffCardsAppSprite *, s32, s32);

s32 ov87_021E803C(ScratchOffCardsAppSprite *, s32);

s32 ov87_021E8084(ScratchOffCardsAppSprite *, s32);

s32 ov87_021E80B4(ScratchOffCardsAppSprite *);

u32 sub_02021280(TouchPadDataBuffer *outBuffer, u32 externalBufferWriteMethod, u32 changeThreshold);

extern TouchscreenHitbox ov87_021E8184;

extern TouchscreenHitbox ov87_021E818C;

extern TouchscreenHitbox ov87_021E8308;

s32 ov87_021E6080(ScratchOffCardApp *app);

s32 ov87_021E6080(ScratchOffCardApp *app) {
    ScratchOffCardApp *var_r5_2;
    enum YesNoResponse temp_r0_6;
    enum YesNoResponse temp_r0_7;
    s32 temp_r0_2;
    s32 var_r6;
    u8 temp_r0;
    u8 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r0_5;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 var_r0;
    s32 var_r5;
    temp_r1 = app->subState;
    switch ((u32)temp_r1) {
    case 0:
        if (app->currentCard == 2) {
            ClearWindowTilemapAndCopyToVram(&app->windows[0xD]);
            ov87_021E71B4(app);
            ov87_021E7FEC(app->skipCardButtonSprite, 0x58, 0xA0);
            ov87_021E803C(app->skipCardButtonSprite, 0);
        } else {
            ClearWindowTilemapAndCopyToVram(&app->windows[0xE]);
            ov87_021E717C(app);
        }
        app->subStateTimer = 0x1E;
        ov87_021E7FD4(app->centerMessageBoxSprite, 1);
        ov87_021E708C(app);
        var_r5 = 0;
        do {
            ov87_021E71EC(app, (u8)var_r5);
            var_r5 += 1;
        } while (((s32)var_r5) < 4);
        ov87_021E7FD4(app->skipCardButtonSprite, 1);
        ov87_021E8084(app->skipCardButtonSprite, 0);
        PlaySE(0x562U);
        app->subState = 1;
        break;

    case 1:
        GfGfx_EngineATogglePlanes(4U, 1U);
        GfGfx_EngineBTogglePlanes(1U, 1U);
        app->subState = 2;
        break;

    case 2:
        temp_r0 = app->subStateTimer;
        if (temp_r0 == 0) {
            ov87_021E7FD4(app->centerMessageBoxSprite, 0);
            FillWindowPixelBuffer(&app->windows[0xA], 0U);
            ScheduleWindowCopyToVram(&app->windows[0xA]);
            app->subState = 3;
        } else {
            app->subStateTimer = temp_r0 - 1;
        }
        break;

    case 3:
        sub_02021280(&app->tpBuffer, 4, 2);
        temp_r0_2 = TouchscreenHitbox_FindRectAtTouchHeld(&ov87_021E8308);
        if (temp_r0_2 != (-1)) {
            if (ov87_021E7490(app) < 3U) {
                app->cellIsScratched[temp_r0_2] = 1;
            }
            if (app->cellIsScratched[temp_r0_2] == 1) {
                ov87_021E7550(app);
                ov87_021E75E0(app, (u8)temp_r0_2);
                ov87_021E74F4(app, temp_r0_2);
                if (IsSEPlaying(0x55FU) == 0) {
                    PlaySE(0x55FU);
                }
            }
            if (ov87_021E7734(app) == 1) {
                StopSE(0x55FU, 0);
                ov87_021E7FD4(app->centerMessageBoxSprite, 1);
                app->wonItems[app->currentCard] = app->winnableItems[app->winningCellValue];
                if (app->winnableItems[app->winningCellValue] == 0x5C) {
                    app->wonItemsCount[app->currentCard] = 1;
                } else {
                    app->wonItemsCount[app->currentCard] = 3;
                }
                if (ov87_021E788C(app) == 1) {
                    var_r0 = 0x1E;
                } else {
                    var_r0 = 0;
                }
                app->subStateTimer = var_r0;
                ov87_021E70D0(app);
                app->subState = 4;
                return 0;
            }
            if (app->numRevealedCells >= 3) {
                StopSE(0x55FU, 0);
                app->subStateTimer = 0x3C;
                ov87_021E7FD4(app->centerMessageBoxSprite, 1);
                ov87_021E7134(app);
                app->subState = 7;
                return 0;
            }
            goto block_30;
        }
        StopSE(0x55FU, 0);
block_30:
        if (app->pairFound == 1) {
            ov87_021E7698(app);
        }

        if (app->currentCard == 2) {
            if (TouchscreenHitbox_FindRectAtTouchNew(&ov87_021E8184) != (-1)) {
                ov87_021E8084(app->skipCardButtonSprite, 1);
                ov87_021E7008(app);
                PlaySE(0x5E4U);
                ov87_021E803C(app->skipCardButtonSprite, 0);
                ov87_021E6AE0(app);
                ov87_021E6AF4(app);
                app->subState = 0xA;
            }
        } else if (TouchscreenHitbox_FindRectAtTouchNew(&ov87_021E818C) != (-1)) {
            ov87_021E8084(app->skipCardButtonSprite, 1);
            ov87_021E7008(app);
            PlaySE(0x5E4U);
            ov87_021E803C(app->skipCardButtonSprite, 1);
            ov87_021E6AE0(app);
            ov87_021E6AF4(app);
            app->subState = 9;
        } else {
block_44:;

            ;
            ;
        }
        break;

    case 4:
        temp_r1_2 = app->subStateTimer;
        if (temp_r1_2 == 0) {
            if (ov87_021E78D8(app, 0) == 0) {
                app->subState = 5;
            }
        } else {
            app->subStateTimer = temp_r1_2 - 1;
        }
        break;

    case 5:
        if (ov87_021E78D8(app, 1) == 0) {
            app->subStateTimer = 0;
            app->subState = 6;
        }
        break;

    case 6:
        if (ov87_021E80B4(app->winPopupSprite) == 1) {
            goto block_44;
        }
        temp_r0_3 = app->subStateTimer;
        if (temp_r0_3 == 0) {
            var_r6 = 0;
            var_r5_2 = app;
            do {
                ov87_021E7FD4(var_r5_2->winMonSprites[0], 0);
                var_r5_2 = (void *)(((u8 *)var_r5_2) + 4);
                var_r6 += 1;
            } while (var_r6 < 3);
            ov87_021E7FD4(app->winPopupSprite, 0);
            ov87_021E7998(app);
            ov87_021E79A0(app);
            return 1;
        }
        app->subStateTimer = temp_r0_3 - 1;
        break;

    case 7:
        temp_r0_4 = app->subStateTimer;
        if (temp_r0_4 == 0) {
            GfGfx_EngineATogglePlanes(1U, 0U);
            ov87_021E7998(app);
            ov87_021E79A0(app);
            app->subStateTimer = 0x1E;
            app->subState = 8;
        } else {
            app->subStateTimer = temp_r0_4 - 1;
        }
        break;

    case 8:
        temp_r0_5 = app->subStateTimer;
        if (temp_r0_5 == 0) {
            return 1;
        }
        app->subStateTimer = temp_r0_5 - 1;
        break;

    case 9:
        temp_r0_6 = YesNoPrompt_HandleInputForSave(app->yesNoMenu);
        if (temp_r0_6 == YESNORESPONSE_YES) {
            ov87_021E8084(app->skipCardButtonSprite, 0);
            ClearWindowPixelBuffer(app->windows, 0);
            ClearWindowTilemapAndCopyToVram(app->windows);
            ov87_021E7998(app);
            ov87_021E6B28(app);
            return 1;
        }
        if (temp_r0_6 == YESNORESPONSE_NO) {
            ov87_021E8084(app->skipCardButtonSprite, 0);
            ClearWindowPixelBuffer(app->windows, 0);
            ClearWindowTilemapAndCopyToVram(app->windows);
            ov87_021E6B28(app);
            app->subState = 0xB;
        }
        break;

    case 10:
        temp_r0_7 = YesNoPrompt_HandleInputForSave(app->yesNoMenu);
        if (temp_r0_7 == YESNORESPONSE_YES) {
            ov87_021E8084(app->skipCardButtonSprite, 0);
            ClearWindowPixelBuffer(app->windows, 0);
            ClearWindowTilemapAndCopyToVram(app->windows);
            ov87_021E7990(app);
            ov87_021E6B28(app);
            return 1;
        }
        if (temp_r0_7 == YESNORESPONSE_NO) {
            ov87_021E8084(app->skipCardButtonSprite, 0);
            ClearWindowPixelBuffer(app->windows, 0);
            ClearWindowTilemapAndCopyToVram(app->windows);
            ov87_021E6B28(app);
            app->subState = 0xB;
        }
        break;

    case 11:
        if (System_GetTouchHeld() == 0) {
            app->subState = 3;
        }
        break;
    }

    return 0;
}
