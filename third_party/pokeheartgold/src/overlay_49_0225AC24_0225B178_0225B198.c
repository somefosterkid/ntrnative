#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "message_format.h"

struct ColoredArrow;

struct ListMenu;

struct MsgData;

struct ListMenuItem;

struct MessageFormat;

typedef struct ColoredArrow ColoredArrow;

typedef struct MsgData MsgData;

typedef struct ListMenuItem ListMenuItem;

ListMenuItem *ListMenuItems_New(u32 capacity, u32 heapID);

void ListMenuItems_AddItem(ListMenuItem *list, const String *string, u32 index);

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

typedef struct {
    ListMenuItem *unk_00;
    ListMenuTemplate unk_04;
} UnkStruct_ov70_0225F098;

String *ov49_0225B388(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);

extern const ListMenuTemplate ov49_022697AC;

void ov49_0225B198(UnkStruct_ov70_0225F098 *param0, UnkStruct_ov70_0225F208 *param1, u32 heapID);

void ov49_0225B198(UnkStruct_ov70_0225F098 *param0, UnkStruct_ov70_0225F208 *param1, u32 heapID) {
    String *v0;

    GF_ASSERT(param0->unk_00 == NULL);
    param0->unk_00 = ListMenuItems_New(2, heapID);

    v0 = ov49_0225B388(param1, 1, 67);
    ListMenuItems_AddItem(param0->unk_00, v0, 0);
    v0 = ov49_0225B388(param1, 1, 66);
    ListMenuItems_AddItem(param0->unk_00, v0, 1);

    param0->unk_04 = ov49_022697AC;
    param0->unk_04.count = 2;
    param0->unk_04.choices = param0->unk_00;
}
