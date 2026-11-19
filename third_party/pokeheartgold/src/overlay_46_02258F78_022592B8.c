#include "global.h"
#include "bg_window.h"
#include "heap.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_46_unk_struct_ov67_0225d210.h"
#include "overlay_46_unk_struct_ov67_0225d154.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
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

extern void *_02259598;

void ov46_022592B8(UnkStruct_ov67_0225D154 *param0);

void ov46_022592B8(UnkStruct_ov67_0225D154 *param0) {
  s32 iVar1;
  u32 *puVar2;
  
  puVar2 = (u32 *)&_02259598;
  iVar1 = 0;
  do {
    FreeBgTilemapBuffer(param0->unk_0C,(u8)*puVar2);
    iVar1 = iVar1 + 1;
    puVar2 = puVar2 + 1;
  } while (iVar1 < 3);
  Heap_Free(param0->unk_0C);
  return;
}
