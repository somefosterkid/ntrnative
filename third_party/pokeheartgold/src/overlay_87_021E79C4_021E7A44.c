#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"
#include "yes_no_prompt.h"
#include "gf_gfx_planes.h"

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct PaletteAnimator PaletteAnimator;

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

void ov87_021E7AB0(ScratchOffCardApp *app, u32 x, u32 y);

void ov87_021E7A44(ScratchOffCardApp *app, s32 touchX, s32 touchY);

void ov87_021E7A44(ScratchOffCardApp *app, s32 touchX, s32 touchY) {
    s32 v2;
    s32 v1;
    s32 v0;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 v6;

    v0 = touchY;
    v1 = -3;
    v2 = -3;
    do {
        v6 = v2;
        v5 = v0 + v1;
loop_2:
        v4 = touchX + v6;
        if ((v4 > 0) && (v4 < 0x100) && (v5 > 0) && (v5 < 0xC0)) {
            ov87_021E7AB0(app, (u32)v4, (u32)v5);
        }
        v6 += 1;
        if (v6 < 3) {
            goto loop_2;
        }
        v3 = v1 + 1;
        v1 = v3;
    } while (v3 < 3);
    BG_LoadCharTilesData(app->bgConfig, 0U, app->scratchCellsDataBackup, app->scratchCellsCharData->szByte, 0U);
    BgCommitTilemapBufferToVram(app->bgConfig, 0U);
}
