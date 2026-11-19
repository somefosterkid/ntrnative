#include "global.h"

#include "constants/heap.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "fashion_case.h"
#include "list_menu.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct AccessoryShopItem;
struct Menu;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
typedef struct Menu Menu;
BOOL sub_02014C40(SpriteList *collection);
enum AccessoryShopState {
    ACCESSORY_SHOP_STATE_LOAD_GREET_MSG = 0,
    ACCESSORY_SHOP_STATE_INIT_ITEM_LIST,
    ACCESSORY_SHOP_STATE_INIT_INTERFACE,
    ACCESSORY_SHOP_STATE_LOAD_BUY_MSG,
    ACCESSORY_SHOP_STATE_SELECT_ITEM_LIST,
    ACCESSORY_SHOP_STATE_INIT_YES_NO_CHOICE,
    ACCESSORY_SHOP_STATE_SELECT_YES_NO_CHOICE,
    ACCESSORY_SHOP_STATE_CONFIRM_PURCHASE,

    ACCESSORY_SHOP_STATE_LOAD_SHOCKED_MSG,
    ACCESSORY_SHOP_STATE_LOAD_THANKS_MSG,
    ACCESSORY_SHOP_STATE_WAIT_MSGBOX,
    ACCESSORY_SHOP_STATE_WAIT_AB_INPUT,
    ACCESSORY_SHOP_STATE_FREE_INTERFACE,
};
enum AccessoryShopSpriteResourceIdx {
    ACCESSORY_SHOP_RES_IDX_TILES = 0,
    ACCESSORY_SHOP_RES_IDX_PALETTE,
    ACCESSORY_SHOP_RES_IDX_CELL,
    ACCESSORY_SHOP_RES_IDX_ANIM,

    ACCESSORY_SHOP_RES_IDX_COUNT,
};
enum AccessoryShopStaticSprite {
    ACCESSORY_SHOP_SPRITE_SCROLL_TOP_ARROW = 0,
    ACCESSORY_SHOP_SPRITE_SCROLL_BOTTOM_ARROW,

    ACCESSORY_SHOP_SPRITE_COUNT,
};
typedef struct AccessoryShopItem AccessoryShopItem;
typedef void (*AccessoryShopUpdateFunc)(void *, u32);
typedef struct AccessoryShopMessageBox {
    BOOL active;
    Window *window;
    u32 renderDelay;
    u32 printerID;
    String *string;
} AccessoryShopMessageBox;
typedef struct AccessoryShopDescBox {
    BOOL active;
    Window *window;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString[3];
} AccessoryShopDescBox;
typedef struct AccessoryShopItemList {
    BOOL active;
    Window *window;
    ListMenu *listMenu;
    ListMenuItem strList[22 + 1];
    String *string[22 + 1];
    s32 maxListItems;
    MessageFormat *strTemplate;
    String *tempString;
    u16 cursorPos;
    u16 unused;
    void *unk_130;
    AccessoryShopUpdateFunc updateFunc;
    SpriteResourceCollection *spriteResCollections[ACCESSORY_SHOP_RES_IDX_COUNT];
    SpriteResource *spriteResources[ACCESSORY_SHOP_RES_IDX_COUNT];
    Sprite *sprites[ACCESSORY_SHOP_SPRITE_COUNT];
} AccessoryShopItemList;
typedef struct AccessoryShopYesNoChoice {
    BOOL active;
    Menu *menu;
    enum HeapID heapID;
} AccessoryShopYesNoChoice;
typedef struct AccessoryShopDataPtrs {
    enum HeapID heapID;
    BgConfig *bgConfig;
    AccessoryShopDescBox *descBox;
    const AccessoryShopItem *items;
    Bag *bag;
} AccessoryShopDataPtrs;
typedef struct AccessoryShop {
    enum AccessoryShopState state;
    enum AccessoryShopState nextState;
    SaveData *saveData;
    FashionCase *fashionCase;
    Bag *bag;
    BgConfig *bgConfig;
    enum HeapID heapID;
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    MsgData *msgLoader;
    u32 msgBoxFrame;
    u32 renderDelay;
    AccessoryShopMessageBox msgbox;
    AccessoryShopDescBox descBox;
    AccessoryShopItemList itemList;
    AccessoryShopYesNoChoice yesNoChoice;
    AccessoryShopDataPtrs dataPtr;
} AccessoryShop;

void ov07_02222CC0(AccessoryShop *shop);

void ov07_02222CC0(AccessoryShop *shop) {
    sub_02014C40(shop->spriteList);
}
