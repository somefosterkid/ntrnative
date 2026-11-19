#include "global.h"

#include "constants/heap.h"

#include "assert.h"
#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02037C94.h"

typedef enum HeapID HeapID;

struct Background;

struct BgConfig;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct ListMenu;

struct ListMenuCursor;

struct ListMenuItem;

struct ListMenuTemplate;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct String;

struct UnkStruct_ov49_0225AC_Gfx;

struct UnkStruct_ov49_0225AC_List;

struct Window;

typedef struct UnkStruct_ov49_0225AC_Gfx {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    u8 unk8[0x128];
    GF_2DGfxResMan *resMan[4];
} UnkStruct_ov49_0225AC_Gfx;

typedef struct UnkStruct_ov49_0225AC_List {
    ListMenuTemplate menuTemplate;
    Window window;
    struct ListMenu *list;
    ListMenuItem *items;
    u16 itemCount;
    u16 templateCount;
    BOOL unk3C;
    SpriteResource *unk40[4];
    Sprite *sprites[2];
} UnkStruct_ov49_0225AC_List;

void ov49_0225B058(struct ListMenu *list, s32 index, u8 onInit);

void ov49_0225AF30(UnkStruct_ov49_0225AC_List *param0, const ListMenuTemplate *param1, UnkStruct_ov49_0225AC_Gfx *param2, u16 param3, u16 param4, u32 heapID, u8 param6, u8 param7, u8 param8);

void ov49_0225AF30(UnkStruct_ov49_0225AC_List *param0, const ListMenuTemplate *param1, UnkStruct_ov49_0225AC_Gfx *param2, u16 param3, u16 param4, u32 heapID, u8 param6, u8 param7, u8 param8) {
    ((param0->list == 0L) ? (void)0 : GF_AssertFail());
    (((param1->maxShowed * 2) < 18) ? (void)0 : GF_AssertFail());

    param0->menuTemplate = *param1;
    param0->menuTemplate.window = &param0->window;
    param0->templateCount = param1->totalItems;
    param0->menuTemplate.moveCursorFunc = ov49_0225B058;

    AddWindowParameterized(param2->bgConfig, &param0->window, 1, param6, param7, param8, (u8)(param1->maxShowed * 2), 5, 0xCA);
    FillWindowPixelBuffer(&param0->window, 15);
    DrawFrameAndWindow1(&param0->window, 1, 0x55, 3);

    param0->list = (struct ListMenu *)(ListMenuInit(&param0->menuTemplate, param3, param4, (HeapID)((u8)heapID)));

    ScheduleWindowCopyToVram(&param0->window);
}
