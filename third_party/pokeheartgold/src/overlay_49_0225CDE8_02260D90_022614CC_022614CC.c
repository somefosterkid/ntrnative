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

void ov49_0225A40C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, int param2);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    s32 unk_04;
    ListMenuTemplate unk_08;
    u16 unk_28;
    u16 unk_2A;
} UnkStruct_ov70_022653DC;

extern const ListMenuTemplate ov49_02269C00;

void ov49_022614CC(UnkStruct_ov70_022653DC *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_022614CC(UnkStruct_ov70_022653DC *param0, UnkStruct_ov70_0225DEE8 *param1) {
    int v0;
    const String *v1;

    ov49_0225A10C(param1, 18);

    for (v0 = 1; v0 < 18; v0++) {
        ov49_0225A40C(param1, 0, v0);
        v1 = ov49_0225A30C(param1, 1, 6);
        ov49_0225A144(param1, v1, v0);
    }

    v1 = ov49_0225A30C(param1, 1, 7);
    ov49_0225A144(param1, v1, 0);

    param0->unk_08 = ov49_02269C00;
    param0->unk_08.choices = ov49_0225A154(param1);
}
