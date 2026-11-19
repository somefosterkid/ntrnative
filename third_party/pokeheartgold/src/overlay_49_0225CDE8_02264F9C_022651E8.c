#include "global.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "bg_window.h"

struct Background;

struct BgConfig;

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct String;

struct ListMenuItem;

struct UnkStruct_ov70_0225DEE8_t;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef u16 charcode_t;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

typedef u32 TextColor;

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;

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

void ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

typedef struct {
    ListMenuTemplate unk_00;
    u32 unk_20;
} UnkStruct_ov70_02268E44;

extern const ListMenuTemplate ov49_02269E24;

void ov49_022651E8(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov49_022651E8(UnkStruct_ov70_02268E44 *param0, UnkStruct_ov70_0225DEE8 *param1) {
    int v0;
    String *v1;

    ov49_0225A10C(param1, 4);
    param0->unk_20 = 4;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        ov49_0225A37C((struct UnkStruct_ov70_0225DEE8_t *)((enum PlazaMinigame)(param1)), (enum PlazaMinigame)(UnkEnum_ov66_022324D0_00 + v0), 0);
        v1 = ov49_0225A30C(param1, 1, 38);
        ov49_0225A144(param1, v1, v0);
    }

    v1 = ov49_0225A30C(param1, 1, 37);

    ov49_0225A144(param1, v1, 4 - 1);

    param0->unk_00 = ov49_02269E24;
    param0->unk_00.count = param0->unk_20;
    param0->unk_00.maxDisplay = param0->unk_20;
    param0->unk_00.choices = ov49_0225A154(param1);
}
