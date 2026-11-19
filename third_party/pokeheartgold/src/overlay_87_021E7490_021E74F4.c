#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_41_sprite_resource_collection.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct OverlayManager OverlayManager;

typedef struct OverlayManagerTemplate OverlayManagerTemplate;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct YesNoPrompt YesNoPrompt;

typedef struct PaletteAnimator PaletteAnimator;

typedef u16 charcode_t;

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

struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
};

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

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

struct FontSpecialCharsContext {
    void *ncgrBuffer;
    NNSG2dCharacterData *charData;
    charcode_t charcodes[16];
    u32 bgColor;
};

enum YesNoTouchButton {
    BUTTON_YES = 0,
    BUTTON_NO,
    BUTTON_COUNT,
    BUTTON_NONE,
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

struct YesNoPrompt {
    TouchHitboxController *buttonAction;
    TouchscreenHitbox hitTable[BUTTON_COUNT];
    YesNoButtonSprite buttonSprites[BUTTON_COUNT];
    BgConfig *bgConfig;
    u32 bgLayer;
    enum HeapID heapID;
    fx32 frameThresholds[4];
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 pressedButton;
    u8 paramsApplied : 4;
    u8 buttonState : 4;
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

void ov87_021E7A44(ScratchOffCardApp *app, int touchX, int touchY);

extern s32 ov87_021E829C;

void ov87_021E74F4(ScratchOffCardApp *app, s32 cellIdx);

void ov87_021E74F4(ScratchOffCardApp *app, s32 cellIdx) {
    s32 sp0;
    ScratchOffCardApp *var_r4;
    s32 var_r6;
    u16 temp_r1;
    u16 temp_r2;
    void *temp_r5;

    var_r6 = 0;
    if ((s32) app->tpBuffer.bufferSize > 0) {
        temp_r5 = ((u8 *)&ov87_021E829C) + (cellIdx * 4);
        var_r4 = app;
        sp0 = (s32) * (u8 *)((u8 *)temp_r5 + 2);
        do {
            temp_r1 = var_r4->tpBuffer.buffer[0].x;
            temp_r2 = var_r4->tpBuffer.buffer[0].y;
            if ((sp0 <= (s32)temp_r1) && ((s32)temp_r1 <= (s32) * (u8 *)((u8 *)temp_r5 + 3)) && ((s32) * (u8 *)temp_r5 <= (s32)temp_r2) && ((s32)temp_r2 <= (s32) * (u8 *)((u8 *)temp_r5 + 1))) {
                ov87_021E7A44(app, (s32)temp_r1, (s32)temp_r2);
            }
            var_r6 += 1;
            var_r4 = (void *)((u8 *)var_r4 + (8));
        } while (var_r6 < (s32) app->tpBuffer.bufferSize);
    }
}
