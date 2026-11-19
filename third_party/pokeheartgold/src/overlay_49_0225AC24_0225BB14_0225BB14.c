#include "global.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

struct BgConfig;

struct ListMenuCursor;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct UnkStruct_ov70_0225DEE8_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

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

void LoadRectToBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, const void *src, u8 destX, u8 destY, u8 destWidth, u8 destHeight);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

void *GfGfxLoader_GetScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, NNSG2dScreenData **outScreenData, enum HeapID heapID);

typedef u32 TextColor;

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

void Heap_Free(void *ptr);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchHitboxController {
    const TouchscreenHitbox *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchHitboxController;

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225BB14(UnkStruct_ov70_0225E4EC *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 heapID);

void ov49_0225BB14(UnkStruct_ov70_0225E4EC *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 heapID) {
    void *v0;
    NNSG2dScreenData *v1;

    v0 = GfGfxLoader_GetScrnDataFromOpenNarc((struct NARC *)((u8 *)((struct NARC *)((u8 *)(param1)))), (u32)((struct NNSG2dScreenData **)(param2)), (int)((struct NNSG2dScreenData **)(0)), (struct NNSG2dScreenData **)((u8 *)(&v1)), (enum HeapID)(heapID));

    {
        int v2;
        u16 *v3;
        int v4;

        v4 = v1->szByte / 2;
        v3 = (u16 *)v1->rawData;

        for (v2 = 0; v2 < v4; v2++) {
            v3[v2] += param4;
        }
    }

    LoadRectToBgTilemapRect(param0->unk_00, param3, v1->rawData, 0, 0, v1->screenWidth / 8, v1->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(param0->unk_00, param3);
    Heap_Free(v0);
}
