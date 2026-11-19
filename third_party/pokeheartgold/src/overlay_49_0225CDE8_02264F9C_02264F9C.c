#include "global.h"
#include "pm_string.h"
#include "bg_window.h"

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct String;

struct ListMenuItem;

struct UnkStruct_ov70_0225DEE8_t;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct ColoredArrow ColoredArrow;

typedef struct ListMenuItem ListMenuItem;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

void ov49_0225A10C(UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225A144(UnkStruct_ov70_0225DEE8 *param0, const String *param1, u32 param2);

const ListMenuItem *ov49_0225A154(const UnkStruct_ov70_0225DEE8 *param0);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

typedef struct {
    ListMenuTemplate unk_00;
    u32 unk_20;
} UnkStruct_ov70_02268E44;

extern const ListMenuTemplate ov49_02269E24;

void ov49_02264F9C(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4);

void ov49_02264F9C(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4) {
    int v0;
    const String *v1;

    ov49_0225A10C(param1, param2);
    param0->unk_20 = param2;

    for (v0 = 0; v0 < param2; v0++) {
        if ((param4 != 0) && (v0 == (param2 - 1))) {
            if (param4 == 1) {
                v1 = ov49_0225A30C(param1, 3, 517);
            } else {
                v1 = ov49_0225A30C(param1, 3, 235);
            }
        } else {
            v1 = ov49_0225A30C(param1, 3, param3 + v0);
        }

        ov49_0225A144(param1, v1, v0);
    }

    param0->unk_00 = ov49_02269E24;
    param0->unk_00.count = param0->unk_20;

    if (param0->unk_00.maxDisplay > param0->unk_20) {
        param0->unk_00.maxDisplay = param0->unk_20;
    }

    param0->unk_00.choices = ov49_0225A154(param1);
}

void ov49_0225A120(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

BOOL ov49_0225A164(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);

u32 MTRandom(void);

void ov49_02265044(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4, u32 param5);

void ov49_02265044(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, u32 param4, u32 param5) {
    int v0;
    u32 v1;
    u32 v2;
    const String *v3;
    BOOL v4;

    ov49_0225A120(param1, param3, param2);
    param0->unk_20 = param3;

    for (v0 = 0; v0 < param3; v0++) {
        if ((param5 != 0) && (v0 == (param3 - 1))) {
            v2 = param2;

            if (param5 == 1) {
                v3 = ov49_0225A30C(param1, 3, 517);
            } else {
                v3 = ov49_0225A30C(param1, 3, 235);
            }
        } else {
            do {
                v1 = MTRandom() % param2;
                v2 = v1;
                v4 = ov49_0225A164(param1, v2);
            } while (v4 == 1);

            v3 = ov49_0225A30C(param1, 3, param4 + v1);
        }

        ov49_0225A144(param1, v3, v2);
    }

    param0->unk_00 = ov49_02269E24;
    param0->unk_00.count = param0->unk_20;

    if (param0->unk_00.maxDisplay > param0->unk_20) {
        param0->unk_00.maxDisplay = param0->unk_20;
    }

    param0->unk_00.choices = ov49_0225A154(param1);
}
