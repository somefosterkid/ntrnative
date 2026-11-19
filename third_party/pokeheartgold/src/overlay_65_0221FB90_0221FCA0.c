#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "list_menu_items.h"

struct BgConfig;

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct ListMenuItem;

struct Window;

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

typedef struct ColoredArrow ColoredArrow;

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

ListMenu *ListMenuInit(const ListMenuTemplate *template, u16 startListPos, u16 startCursorPos, enum HeapID heapID);

void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);

extern void ov65_0221FD20(ListMenu *listMenu, u32 unused, u8 onInit);

extern const ListMenuTemplate ov65_022200EC;

ListMenu *ov65_0221FCA0(ListMenuItem *strList, int friendCount, Window *window, BgConfig *bgConfig);

ListMenu *ov65_0221FCA0(ListMenuItem *strList, int friendCount, Window *window, BgConfig *bgConfig) {
    int maxDisplayHeight = 5;

    AddWindowParameterized(bgConfig, window, 0, 19, 1, 12, maxDisplayHeight * 2, 13, (1024 - (9 + (18 + 12))) - (10 * (maxDisplayHeight + 2) * 2));
    DrawFrameAndWindow1(window, 0, (1024 - 9), 11);

    ListMenuTemplate lmTemplate = ov65_022200EC;
    lmTemplate.count = friendCount + 1;
    lmTemplate.maxDisplay = maxDisplayHeight;
    lmTemplate.choices = strList;
    lmTemplate.window = window;
    lmTemplate.cursorCallback = ov65_0221FD20;

    ListMenu *listMenu = ListMenuInit(&lmTemplate, 0, 0, HEAP_ID_26);

    return listMenu;
}
