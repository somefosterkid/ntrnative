#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "string_util.h"
#include "fashion_case.h"
#include "bag_types_def.h"
#include "msgdata.h"
#include "sprite_resources_header.h"
#include "overlay_72_wfc_connect.h"

struct AccessoryShopItem;

struct AccessoryShopItemList;

struct AccessoryShopMessageBox;

struct Bag;

struct FashionCase;

struct MessageBank;

struct MsgData;

struct NARC;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct Window;

typedef struct MessageBank MessageBank;

String *NewString_ReadMsgData();

typedef struct StringTemplateArg StringTemplateArg;

MessageFormat *MessageFormat_New();

void MessageFormat_Delete(MessageFormat *template);

void BufferItemName(MessageFormat *template, u32 idx, u32 item);

void StringTemplate_SetItemNamePlural(MessageFormat *template, u32 idx, u32 item);

void StringTemplate_SetContestAccessoryName(MessageFormat *template, u32 idx, u32 accessory);

typedef struct AccessoryShopItem {
    u32 accessoryID;
    u32 itemBerryID;
    u32 totalAmount;
} AccessoryShopItem;

typedef struct AccessoryShopMessageBox {
    BOOL active;
    Window *window;
    u32 renderDelay;
    u32 printerID;
    String *string;
} AccessoryShopMessageBox;

typedef struct AccessoryShopItemList AccessoryShopItemList;

void AccessoryShop_PrintStringToMsgBox(AccessoryShopMessageBox *msgbox, String *string, enum HeapID heapID);

void AccessoryShop_LoadSuccessfulPurchaseMsg(AccessoryShopMessageBox *msgbox, MsgData *msgLoader, enum HeapID heapID, const AccessoryShopItem *items, u32 idx);

void AccessoryShop_LoadScrollArrowGfx(SpriteResourcesHeader *scrollArrowResource, AccessoryShopItemList *itemList, enum HeapID heapID);

BOOL AccessoryShop_HasEnoughBerries(Bag *bag, const AccessoryShopItem *items, u32 idx, enum HeapID heapID);

void AccessoryShop_DoPurchase(FashionCase *fashionCase, Bag *bag, const AccessoryShopItem *items, u32 idx, enum HeapID heapID);

void ov72_0223B0C4(NintendoWfcYesNoMenu *arg0);

void ov72_0223B0C4(NintendoWfcYesNoMenu *arg0) {
    s32 i;
    NARC *narc;
    void *scrnData;
    NNSG2dScreenData *scrnHdr;
    u16 *src;
    u16 *dst;

    narc = NARC_New((enum NarcId)239, arg0->heapID);

    if (arg0->bgLayer <= 3) {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(arg0->bgPltt << 5), 0x20, arg0->heapID);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(arg0->fontPltt << 5), arg0->heapID);
    } else {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(arg0->bgPltt << 5), 0x20, arg0->heapID);
        LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(arg0->fontPltt << 5), arg0->heapID);
    }

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, arg0->bgConfig, (enum GFBgLayer)arg0->bgLayer, 0, 0, 0, arg0->heapID);
    scrnData = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 10, 0, &scrnHdr, arg0->heapID);
    dst = GetBgTilemapBuffer(arg0->bgConfig, arg0->bgLayer);
    i = 0;
    src = (u16 *)((u8 *)scrnHdr + 0xc);

    for (; i < 0x300; i++) {
        *dst = (arg0->bgPltt << 12) | (*src & 0xfff);
        src++;
        dst++;
    }

    BgCommitTilemapBufferToVram(arg0->bgConfig, arg0->bgLayer);
    Heap_Free(scrnData);
    NARC_Delete(narc);
    ToggleBgLayer(arg0->bgLayer, 1);
    ToggleBgLayer(arg0->textBgLayer, 1);
}
