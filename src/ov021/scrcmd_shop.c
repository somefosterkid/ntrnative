// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_shop.c, ported by tools/port_file.py.
// The shops: the command that opens a shop's menu over the field, and the menu, which sells items for money (Poké
// Marts) or Battle Points (the BP shop). The name is the ROM's
// own, from GFL_HeapAllocate's file argument. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/arc.h"
#include "constants/field_script.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/event_poke_status.h"
#include "field/field.h"
#include "field/field_camera.h"
#include "field/field_script.h"
#include "field/game_beacon_set.h"
#include "field/musical.h"
#include "field/scrcmd_shop.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/bsubway_save.h"
#include "save/event_work.h"
#include "save/high_link.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/printsys.h"
#include "system/str_tool.h"
#include "system/text_speed.h"
#include "system/vm.h"
#include "system/wordset.h"
#include "text/script/0613.h"

// The command's shop IDs past the Poké Marts', whose items are list file ID + 6 of data_ov021_021de1a8
#define SHOP_ID_SPECIAL_MART_0_FIRST 0xf7
#define SHOP_ID_SPECIAL_MART_0_LAST 0xfa
#define SHOP_ID_BP_TMS 0xfd
#define SHOP_ID_BP_ITEMS 0xfe
// The Poké Mart whose items grow with the badges
#define SHOP_ID_BADGE_MART 0xff

// What the menu lists: items, or TMs with their numbers and moves
#define SHOP_LIST_ITEMS 0
#define SHOP_LIST_TMS 1

// What the menu sells for: money or Battle Points
#define SHOP_CURRENCY_MONEY 0
#define SHOP_CURRENCY_BP 1

// Black 2's; twinned code confirms the offsets of item in Black
typedef struct {
    u16 item;
    u32 price;
} ShopItem;

// Black 2's, not checked against Black
typedef struct {
    u32 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 palette;
    u16 baseTile;
} ShopWindowSetup;

// Black 2's, not checked against Black
typedef struct {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} ShopRect;

// Cell actor resources, as the func_0204a6c8 family returns them
// Black 2's, not checked against Black
typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
} ShopClActRes;

// Black's layout: pokebw2's with 40 entries and none of Black 2's move tutors' fields (ShopScriptWork allocates
// 0x214 bytes)
typedef struct {
    u8 state;
    // The state after a message
    u8 nextState;
    u16 unk2;
    HeapID heapId;
    Font *font;
    TrainerCardSave *trainerCard;
    BagSave *bag;
    GameRecords *records;
    BSubwayScoreData *bsubwayScore;
    // Black City's state (Black 2's KeyDataSave here): its shop's items
    CityState *cityState;
    ShopItem entries[40];
    u16 entryCount;
    u8 listMode;
    u8 currency;
    BmpWin *windows[7];
    ListMenuOption *options;
    BmpMenu *confirmDialog;
    MsgData *msgData;
    MsgData *itemInfoMsgData;
    WordSet *wordSet;
    StrBuf *priceFormat;
    StrBuf *message;
    StrBuf *moveLabelFormat;
    StrBuf *ownedLabel;
    BmpMenuList *list;
    PrintWindow printWindow;
    PrintQueue *printQueue;
    TCBExManager *tcbManager;
    PrintStream *printStream;
    ClActUnit *clactUnit;
    ClActor *actors[5];
    ShopClActRes res[3];
    ArcTool *iconArc;
    u16 item;
    u32 price;
    u32 maxAmount;
    s16 amount;
    u16 listShown;
} ShopUIWork;

// Black's: Black 2's without what the move tutors' party list returns
typedef struct {
    u8 state;
    u8 unk1;
    u8 shopId;
    // The Poké Marts' list, the shop ID itself
    u8 subId;
    BOOL useBoundary;
    ShopUIWork work;
} ShopScriptWork;

void StartShopCameraAnime(ShopScriptWork *shop, Field *field, FieldCamera *camera);
BOOL func_ov021_021b35a4(GameSystem *gsys, ShopUIWork *wk, u8 shopId, u8 subId);

static BOOL ScriptNative_UpdateShopUI(VM *vm, void *env);
static void ShopUI_UpdateItemBrowse(ShopUIWork *wk);
static u8 ShopUI_WaitAB(ShopUIWork *wk);
static void ShopUI_SpendMoney(ShopUIWork *wk, u32 amount);
static BOOL ShopUI_Update(GameSystem *gsys, ShopUIWork *wk, u32 shopId, u32 subId);
BOOL ShopUI_CheckSellingTMHM(ShopItem *items, int count);
void ShopUI_FreeBG(ShopUIWork *wk);
static void ShopUI_CreateCellActors(ShopUIWork *wk);
static void func_ov021_021b3cfc(ShopUIWork *wk);
void ShopUI_LoadItemSprite(ShopUIWork *wk, u16 item);
static void func_ov021_021b3dec(ShopUIWork *wk);
static void ShopUI_SetupSprites(ShopUIWork *wk);
void func_ov021_021b3f3c(ShopUIWork *wk);
static void ShopUI_SetupBmpWin(ShopUIWork *wk);
void ShopUI_FreeSprites(ShopUIWork *wk);
void ShopUI_UpdateMoneyDisp(ShopUIWork *wk);
void func_ov021_021b41ac(ShopUIWork *wk, u16 item);
void func_ov021_021b4264(ShopUIWork *wk, u16 item);
void ShopUI_UpdateItemAmountDisp(ShopUIWork *wk, u16 amount, u32 price);
static void ShopUI_SetupMenuList(ShopUIWork *wk);
void func_ov021_021b44cc(PrintQueue *queue, GFLBitmap *bitmap, int x, int y, const StrBuf *strbuf, Font *font,
                                u16 color);
void func_ov021_021b4510(BmpMenuList *list, intptr_t value, u8 y);
static void func_ov021_021b45a0(BmpMenuList *list, intptr_t value, u8 y);
void func_ov021_021b468c(BmpMenuList *list, intptr_t value, u8 init);
void func_ov021_021b46fc(ShopUIWork *wk);
int func_ov021_021b4710(u32 unused);
void ShopUI_SetStatusDialogue(ShopUIWork *wk, u32 msgId, u16 item, BOOL plural);
void ShopUI_SetBuyConfirmMessage(ShopUIWork *wk, u8 listMode);
void func_ov021_021b4878(BmpWin *window);
void func_ov021_021b4918(ShopUIWork *wk, u32 msgId, u16 item, u32 price, u16 amount);
void func_ov021_021b4984(ShopUIWork *wk, u32 msgId, u16 item, u32 price);
u32 ShopUI_ChangeItemAmount(s16 *amount, u16 max, s32 delta);
u8 ShopUI_UpdateInput(ShopUIWork *wk);
void func_ov021_021b4ba4(u32 index);

// field_wfbc_data_local.c's: the Black City shop's items (its FieldWfbcEntry, a ShopItem) from the city's state
void func_ov020_02186670(CityState *state, ShopItem *out, u16 *count, s32 start, HeapID heapId);
BOOL func_ov021_021b3158(VM *vm, void *env);
void func_ov021_021b3b04(ShopUIWork *wk);
void func_ov021_021b3b84(ShopUIWork *wk);

static const u16 data_ov021_021d9446[3] = { 0x155, 0x160, 0x16d };

static const u16 data_ov021_021d9464[4] = { 0x14e, 0x152, 0x159, 0x16c };

// The list file of the badge Poké Mart for each number of badges
// pokebw2's BADGE_SHOP_DAT_IDS
static const u8 data_ov021_021d946c[] = { 0, 1, 1, 2, 2, 3, 3, 4, 5 };

// pokebw2's sWindowBP
static const ShopWindowSetup data_ov021_021d94a8 = { 1, 2, 2, 8, 2, 11, 38 };

static const u8 data_ov021_021d9430[3] = { 38, 10, 1 };

// pokebw2's sWindowMoney
static const ShopWindowSetup data_ov021_021d94d8 = { 1, 1, 3, 9, 2, 11, 38 };

static const ShopItem data_ov021_021d9454[1] = {
    { 0x2e, 1 },
};

static const ShopItem data_ov021_021d9750[10] = {
#include "asset/scrcmd_shop_data_ov021_021d9750.inc"
};

static const ShopItem data_ov021_021d97a0[38] = {
#include "asset/scrcmd_shop_data_ov021_021d97a0.inc"
};

static const ShopItem *data_ov021_021de164[3] = { data_ov021_021d97a0, data_ov021_021d9750, data_ov021_021d9454 };

static const u16 data_ov021_021d9438[2];
static const u16 data_ov021_021d9490[6];

static const u16 data_ov021_021d95b2[11] = { 0x6, 0x8, 0xd, 0x91, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d95f8[14] = {
#include "asset/scrcmd_shop_data_ov021_021d95f8.inc"
};

static const u16 data_ov021_021d9674[17] = {
#include "asset/scrcmd_shop_data_ov021_021d9674.inc"
};

static const u16 data_ov021_021d9696[18] = {
#include "asset/scrcmd_shop_data_ov021_021d9696.inc"
};

static const u16 data_ov021_021d96ba[19] = {
#include "asset/scrcmd_shop_data_ov021_021d96ba.inc"
};

static const u16 data_ov021_021d94e4[7] = { 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d9540[8] = { 0x3b, 0x39, 0x3a, 0x37, 0x38, 0x3c, 0x3d, 0x3e };

static const u16 data_ov021_021d9574[9] = { 0xb, 0x7, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d95c8[11] = { 0xd, 0xa, 0xf, 0x92, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d94f2[7] = { 0x15c, 0x162, 0x190, 0x191, 0x157, 0x15b, 0x168 };

static const u16 data_ov021_021d9586[11] = { 0x4, 0x3, 0x11, 0x1a, 0x12, 0x16, 0x15, 0x13, 0x14, 0x4e, 0x4f };

static const u16 data_ov021_021d944c[4] = { 0x24, 0x22, 0x23, 0x25 };

// pokebw2's sConfirmDialogSetup
static const ConfirmDialogSetup data_ov021_021d9440 = { 1, 24, 13, 11, 25 };

static const u16 data_ov021_021d959c[11] = { 0xe, 0x6, 0x8, 0x90, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d9500[8] = { 0xe, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d9510[8] = { 0xe, 0x6, 0x8, 0xf, 0xd, 0xa, 0x9, 0xb };

static const u16 data_ov021_021d9550[9] = { 0x13a, 0xfe, 0x13e, 0x13d, 0x13b, 0x140, 0x13f, 0xff, 0x13c };

static const u16 data_ov021_021d9520[8] = { 0x93, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d9530[8] = { 0x94, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

// The price's format for money and for Battle Points
static const u32 SHOP_UI_CURRENCY_MSGIDS[] = { 25, 26 };

static const u16 data_ov021_021d9562[9] = { 0xe, 0x6, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f };

static const u16 data_ov021_021d9614[16] = {
#include "asset/scrcmd_shop_data_ov021_021d9614.inc"
};

static const u16 data_ov021_021d943c[2];

static const u16 data_ov021_021d9634[16] = {
#include "asset/scrcmd_shop_data_ov021_021d9634.inc"
};

// pokebw2's sWindowList
static const ShopWindowSetup data_ov021_021d94cc = { 1, 12, 1, 19, 16, 2, 54 };

static const u8 data_ov021_021d95de[26] = {
#include "asset/scrcmd_shop_data_ov021_021d95de.inc"
};

static const u16 *data_ov021_021de1a8[26] = {
    data_ov021_021d9438,
    data_ov021_021d9586,
    data_ov021_021d95f8,
    data_ov021_021d9674,
    data_ov021_021d9696,
    data_ov021_021d96ba,
    data_ov021_021d94e4,
    data_ov021_021d9500,
    data_ov021_021d9562,
    data_ov021_021d959c,
    data_ov021_021d94f2,
    data_ov021_021d95b2,
    data_ov021_021d9464,
    data_ov021_021d9446,
    data_ov021_021d95c8,
    data_ov021_021d9510,
    data_ov021_021d9520,
    data_ov021_021d9574,
    data_ov021_021d9530,
    data_ov021_021d9540,
    data_ov021_021d944c,
    data_ov021_021d9550,
    data_ov021_021d9614,
    data_ov021_021d943c,
    data_ov021_021d9634,
    data_ov021_021d9490,
};

static const u16 data_ov021_021d9490[6] = { 0x2e, 0x2f, 0x31, 0x34, 0x30, 0x2d };

// pokebw2's data_ov021_021d9478
// pokebw2's sWindowAmount
static const ShopWindowSetup data_ov021_021d9478 = { 1, 18, 15, 13, 2, 11, 447 };

// pokebw2's sWindowInfo
static const ShopWindowSetup data_ov021_021d9484 = { 1, 5, 18, 27, 6, 11, 469 };

// pokebw2's sWindowInBag
static const ShopWindowSetup data_ov021_021d949c = { 1, 1, 15, 15, 2, 11, 415 };

// pokebw2's sWindowMessage
static const ShopWindowSetup data_ov021_021d94b4 = { 1, 1, 19, 30, 4, 11, 631 };

// pokebw2's sWindowTitle
static const ShopWindowSetup data_ov021_021d94c0 = { 1, 1, 1, 9, 2, 11, 20 };

// The areas of BG 1 that the list's windows cover: all of them, and those of the amount
static const ShopRect data_ov021_021d9654[] = {
    { 0, 12, 32, 12 },
    { 0, 12, 32, 6 },
};

// pokebw2's sActorSetups
static const ClActorSetup data_ov021_021d96e0[] = {
#include "asset/scrcmd_shop_data_ov021_021d96e0.inc"
};

// Where the camera looks while the shop is open, from the target, for each way the player faces
// pokebw2's sCameraOffsets
static const VecFx32 data_ov021_021d9708[] = {
    { FX32_CONST(64), 0, FX32_CONST(-16) },
    { FX32_CONST(64), FX32_CONST(-32), 0 },
    { FX32_CONST(64), 0, 0 },
    { FX32_CONST(80), FX32_CONST(-16), 0 },
};

// The windows: the title, the balance, the list, how many the bag holds, the amount, the description and messages
// pokebw2's sMoveTutorWindows
// pokebw2's sMartWindows
static const ShopWindowSetup *data_ov021_021de170[] = {
    &data_ov021_021d94c0, &data_ov021_021d94d8, &data_ov021_021d94cc, &data_ov021_021d949c, &data_ov021_021d9478, &data_ov021_021d9484, &data_ov021_021d94b4,
};

// pokebw2's sBPShopWindows
static const ShopWindowSetup *data_ov021_021de18c[] = {
    &data_ov021_021d94c0, &data_ov021_021d94a8, &data_ov021_021d94cc, &data_ov021_021d949c, &data_ov021_021d9478, &data_ov021_021d9484, &data_ov021_021d94b4,
};

// Turns the camera toward the counter, from the way the player faces

BOOL s0149_CallFriendlyShopBuy(VM *vm, FieldScriptEnv *env) {
    u16 shopId = ScriptReadAny(vm, env);
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    void **heapPtr;
    void *msgBGSys;
    CityState *cityState;
    ShopScriptWork *shop;

    ScriptReadVar(vm, env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    heapPtr = ScriptWork_GetUserHeapPtr(FieldScriptEnv_GetScriptWork(env));
    msgBGSys = Field_GetMsgBGSys(field);
    cityState = func_020127c4(gameData);
    shop = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ShopScriptWork), TRUE, "scrcmd_shop.c", 252);
    *heapPtr = shop;
    shop->work.font = func_ov021_021904ac(msgBGSys);
    shop->work.trainerCard = getTrainerCardDataBlkAddress(gameData);
    shop->work.heapId = HEAPID_FIELDMAP;
    shop->work.bag = GameData_GetBag(gameData);
    shop->work.bsubwayScore = SaveControl_GetBlockPtr(GameData_GetSaveControl(gameData), SAVE_BLOCK_BSUBWAY_SCORE);
    shop->work.records = GameData_GetRecords(gameData);
    shop->work.cityState = cityState;
    switch (shopId) {
    case SHOP_ID_BADGE_MART:
        shop->shopId = shopId;
        break;
    case SHOP_ID_BP_ITEMS:
        shop->shopId = shopId;
        break;
    case SHOP_ID_BP_TMS:
        shop->shopId = shopId;
        break;
    case SHOP_ID_SPECIAL_MART_0_FIRST:
    case 0xf8:
    case 0xf9:
    case SHOP_ID_SPECIAL_MART_0_LAST:
    case 0xfb:
        shop->shopId = shopId;
        break;
    default:
        shop->shopId = shopId;
        shop->subId = shopId;
        break;
    }
    VM_SetNativeCallback(vm, func_ov021_021b3158);
    if (shopId != SHOP_ID_BP_ITEMS && shopId != SHOP_ID_BP_TMS) {
        func_ov010_021655d4();
    }
    return TRUE;
}

void StartShopCameraAnime(ShopScriptWork *shop, Field *field, FieldCamera *camera) {
    FieldEvCameraAnimationSetup setup;
    u32 dir;

    shop->useBoundary = FieldCamera_IsUseBoundaryEnable(camera);
    FieldCamera_SetUseBoundaryEnable(camera, FALSE);
    FieldCameraAnm_EnsureInitDone(camera);
    dir = FieldPlayer_GetFaceDir(Field_GetPlayer(field));
    FieldCamera_CoordsGetEye(camera, &setup.targetCoords.cameraPos);
    FieldCamera_CoordsGetTarget(camera, &setup.targetCoords.targetPos);
    FieldCamera_ClearBind(camera);
    setup.targetCoords.targetPos.x += data_ov021_021d9708[dir].x;
    setup.targetCoords.targetPos.y += data_ov021_021d9708[dir].y;
    setup.targetCoords.targetPos.z += data_ov021_021d9708[dir].z;
    setup.flags.animateExtraTranslation = FALSE;
    setup.flags.animatePitch = FALSE;
    setup.flags.animateYaw = FALSE;
    setup.flags.animateTargetDistance = FALSE;
    setup.flags.animateFOV = FALSE;
    setup.flags.animateTargetPos = TRUE;
    FieldCameraAnm_SetAnimationRealTime(camera, &setup, 10);
}

BOOL func_ov021_021b3158(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    void **heapPtr = ScriptWork_GetUserHeapPtr(work);
    ShopScriptWork *shop = *heapPtr;
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    FieldCamera *camera = Field_GetCameraSystem(field);

    switch (shop->state) {
    case 0:
        FieldCamera_FinishDelay(camera);
        shop->state++;
        break;
    case 1:
        if (!FieldCamera_IsDelayActive(camera)) {
            shop->state++;
        }
        break;
    case 2:
        StartShopCameraAnime(shop, field, camera);
        shop->state++;
        break;
    case 3:
        if (!FieldCamera_IsAnimating(camera)) {
            shop->state++;
            shop->work.state = 0;
            setAlphaBlend_wrapper(FALSE);
        }
        break;
    case 4:
        if (func_ov021_021b35a4(gsys, &shop->work, shop->shopId, shop->subId)) {
            shop->state++;
        }
        break;
    case 5: {
        FieldEvCameraAnimationFlags flags = { FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };

        FieldCameraAnm_SetReturnAnimation(camera, &flags, 8);
        shop->state++;
        shop->unk1 = 0;
        break;
    }
    case 6:
        if (!FieldCamera_IsAnimating(camera)) {
            FieldCamera_ResetBind(camera);
            FieldCamera_SetUseBoundaryEnable(camera, shop->useBoundary);
            FieldCamera_EnableDelay(camera);
            FieldCameraAnm_EVCameraEnd(camera);
            setAlphaBlend_wrapper(TRUE);
            shop->state++;
        }
        break;
    case 7:
        GFL_HeapFree(*heapPtr);
        return TRUE;
    }
    return FALSE;
}

u16 func_ov021_021b3b14(ShopUIWork *work, u16 shopId);
void func_ov021_021b39c4(ShopUIWork *wk, u32 index);
void func_ov021_021b3a64(ShopUIWork *work, u16 shopId);
void func_ov021_021b38e0(ShopUIWork *wk, u32 shopId, u32 fileId, u32 badges);
void func_ov021_021b331c(ShopUIWork *work, u32 a1);
void func_ov021_021b3bd8(ShopUIWork *work);
void func_ov021_021b3258(GameSystem *gsys, ShopUIWork *work, u16 shopId, u32 a4) {
    GameData *gameData = GSYS_GetGameData(gsys);

    // The player's info goes unused
    GetGameDataPlayerInfo(gameData);
    work->listMode = SHOP_LIST_ITEMS;
    shopId = func_ov021_021b3b14(work, shopId);
    switch (shopId) {
    case SHOP_ID_BADGE_MART:
        func_ov021_021b38e0(work, shopId, a4, getBadgeCount(getTrainerCardDataBlkAddress(gameData)));
        break;
    case SHOP_ID_BP_ITEMS:
        func_ov021_021b39c4(work, 0);
        work->currency = SHOP_CURRENCY_BP;
        break;
    case SHOP_ID_BP_TMS:
        work->currency = SHOP_CURRENCY_BP;
        work->listMode = SHOP_LIST_TMS;
        func_ov021_021b39c4(work, 1);
        break;
    case SHOP_ID_SPECIAL_MART_0_FIRST:
    case SHOP_ID_SPECIAL_MART_0_FIRST + 1:
    case SHOP_ID_SPECIAL_MART_0_FIRST + 2:
    case SHOP_ID_SPECIAL_MART_0_FIRST + 3:
    case SHOP_ID_SPECIAL_MART_0_FIRST + 4:
        func_ov021_021b3a64(work, shopId);
        break;
    default:
        func_ov021_021b38e0(work, shopId, a4, 0);
        break;
    }
    func_ov021_021b331c(work, work->currency);
    func_ov021_021b3bd8(work);
    ShopUI_SetupBmpWin(work);
    ShopUI_SetupSprites(work);
    ShopUI_UpdateMoneyDisp(work);
    ShopUI_SetupMenuList(work);
}

// Corrected for Black: msgData/itemInfoMsgData/wordSet come right after entries; entryCount/listMode/currency/windows/options/confirmDialog moved after ownedLabel
void func_ov021_021b331c(ShopUIWork *work, u32 a1)
{
    if (a1 == 1)
        work->priceFormat = GFL_MsgDataLoadStrbufNew(work->msgData, 0xd);
    else
        work->priceFormat = GFL_MsgDataLoadStrbufNew(work->msgData, 0xc);
    work->ownedLabel = GFL_MsgDataLoadStrbufNew(work->msgData, 0xe);
}

u32 func_ov021_021b3350(ShopUIWork *wk) {
    if (wk->currency == 0) {
        return getCash(wk->trainerCard);
    }
    return (u32)func_0200dabc(wk->bsubwayScore);
}

void func_ov021_021b3370(ShopUIWork *wk) {
    u32 balance = func_ov021_021b3350(wk);
    u16 owned = BagSave_GetItemCountByID(wk->bag, wk->item, wk->heapId);

    if (!BagSave_CheckAvailItemSpace(wk->bag, wk->item, 1, wk->heapId)) {
        ShopUI_SetStatusDialogue(wk, 10, wk->item, FALSE);
        wk->state = 10;
        wk->nextState = 4;
        return;
    }
    if (balance < wk->price) {
        if (wk->currency == SHOP_CURRENCY_MONEY) {
            ShopUI_SetStatusDialogue(wk, 3, wk->item, FALSE);
        } else {
            ShopUI_SetStatusDialogue(wk, 16, wk->item, FALSE);
        }
        wk->state = 10;
        wk->nextState = 4;
        return;
    }
    if (balance >= wk->price && balance < wk->price * 2) {
        ShopUI_SetBuyConfirmMessage(wk, wk->listMode);
        wk->state = 10;
        wk->nextState = 5;
        return;
    }
    if (wk->listMode == SHOP_LIST_ITEMS) {
        ShopUI_SetStatusDialogue(wk, 4, wk->item, FALSE);
        wk->state = 10;
        wk->nextState = 2;
        wk->maxAmount = balance / wk->price;
        if (wk->maxAmount > 99) {
            wk->maxAmount = 99;
        }
        if (wk->maxAmount + owned > PML_ItemGetMaxStorageCount(wk->item)) {
            wk->maxAmount = PML_ItemGetMaxStorageCount(wk->item) - owned;
        }
    } else {
        ShopUI_SetBuyConfirmMessage(wk, wk->listMode);
        wk->state = 10;
        wk->nextState = 6;
    }
}

void func_ov021_021b3370(ShopUIWork *wk);
// Picks an item of the list: its price and an amount of 1 to buy
void func_ov021_021b3470(ShopUIWork *wk) {
    intptr_t index;

    if (!func_0201c6b4(wk->printQueue)) {
        return;
    }
    if (wk->listShown == 0) {
        BmpWin_TransferNow(wk->windows[2]);
        wk->listShown++;
    }
    index = BmpMenuList_Update(wk->list);
    if (index == BMPMENULIST_NULL) {
        return;
    }
    if (index == BMPMENULIST_CANCEL) {
        wk->state = 11;
        return;
    }
    wk->item = wk->entries[index].item;
    wk->price = wk->entries[index].price;
    wk->amount = 1;
    func_ov021_021b3370(wk);
}

u8 func_ov021_021b34f4(ShopUIWork *wk) {
    u32 state = func_0201ce30(wk->printStream);

    if (state == PRINT_STREAM_DONE) {
        func_0201ce48(wk->printStream);
        return wk->nextState;
    } else if (state == PRINT_STREAM_RUNNING) {
        if ((GCTX_HIDGetHeldKeys() & PAD_BUTTON_A) || (GCTX_HIDGetHeldKeys() & PAD_BUTTON_B)) {
            func_0201ce5c(wk->printStream, 0);
        }
    } else if (state == PRINT_STREAM_PAUSED) {
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            func_0201ce38(wk->printStream);
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
        }
    }
    return wk->state;
}

void func_ov021_021b3568(ShopUIWork *wk, u32 param_2) {
    GameRecords *records;
    u32 id;

    if (wk->currency == 0) {
        subCashFromTotal(wk->trainerCard, param_2);
        records = wk->records;
        id = 0x16;
    }
    else {
        func_0200da98(wk->bsubwayScore, (u16)param_2);
        records = wk->records;
        id = 0x22;
    }
    RecordAdd(records, id, param_2);
    RecordAddOne(wk->records, 0x15);
}

BOOL func_ov021_021b35a4(GameSystem *gsys, ShopUIWork *wk, u8 shopId, u8 subId) {
    static const u8 sUnk021d091e[4] = { 0 };

    u32 result;

    switch (wk->state) {
    case 0:
        func_ov021_021b3258(gsys, wk, shopId, subId);
        wk->state++;
        break;
    case 1:
        func_ov021_021b3470(wk);
        break;
    case 2:
        func_ov021_021b41ac(wk, wk->item);
        ShopUI_UpdateItemAmountDisp(wk, wk->amount, wk->price);
        func_0204b3dc(wk->actors[3], TRUE);
        wk->state = 3;
        break;
    case 3:
        wk->state = ShopUI_UpdateInput(wk);
        break;
    case 4:
        func_ov021_021b4ba4(0);
        BmpWin_FlushMap(wk->windows[2]);
        BmpWin_FlushMap(wk->windows[5]);
        GFL_BGSysQueueScrLoad(1);
        func_0204b3dc(wk->actors[3], FALSE);
        func_02020234(wk->list);
        wk->state = 1;
        break;
    case 5:
        func_ov021_021b4ba4(1);
        BmpWin_FlushMap(wk->windows[2]);
        func_0204b3dc(wk->actors[3], FALSE);
        wk->confirmDialog = ShopUI_CreateConfirmDialog(&data_ov021_021d9440, 1, 9, 0, wk->heapId);
        if (wk->listMode != SHOP_LIST_TMS) {
            func_ov021_021b41ac(wk, wk->item);
        }
        wk->state = 7;
        break;
    case 6:
        func_ov021_021b4ba4(1);
        BmpWin_FlushMap(wk->windows[2]);
        func_0204b3dc(wk->actors[3], FALSE);
        wk->confirmDialog = ShopUI_CreateConfirmDialog(&data_ov021_021d9440, 1, 9, 0, wk->heapId);
        wk->state = 7;
        break;
    case 7:
        result = func_0201febc(wk->confirmDialog);
        if (result == BMPMENU_NULL) {
            break;
        }
        if (result == 0) {
            wk->state = 8;
            func_ov021_021b4ba4(1);
            func_02020234(wk->list);
            BmpWin_FlushMap(wk->windows[2]);
            GFL_BGSysQueueScrLoad(1);
        } else if (result == BMPMENU_CANCEL) {
            wk->state = 4;
        }
        break;
    case 8:
        GFL_SndSEPlay(SEQ_SE_SYS_22);
        ShopUI_SetStatusDialogue(wk, 9, wk->item, wk->amount > 1);
        func_ov021_021b3568(wk, wk->price * wk->amount);
        ShopUI_UpdateMoneyDisp(wk);
        BagSave_AddItem(wk->bag, wk->item, wk->amount, wk->heapId);
        wk->state = 10;
        wk->nextState = 9;
        break;
    case 9:
        if (wk->item == ITEM_POKE_BALL && wk->amount >= 10) {
            ShopUI_SetStatusDialogue(wk, 15, wk->item, wk->amount > 1);
            BagSave_AddItem(wk->bag, ITEM_PREMIER_BALL, 1, wk->heapId);
            RecordAddOne(wk->records, 31);
            wk->state = 10;
            wk->nextState = 4;
        } else {
            wk->state = 4;
        }
        break;
    case 10:
        wk->state = func_ov021_021b34f4(wk);
        break;
    case 11:
        func_ov021_021b46fc(wk);
        func_ov021_021b3f3c(wk);
        ShopUI_FreeSprites(wk);
        ShopUI_FreeBG(wk);
        func_ov021_021b3b04(wk);
        func_ov021_021b3b84(wk);
        return TRUE;
    }
    func_0201c4e4(wk->printQueue);
    {
        PrintQueue *queue = wk->printQueue;

        if (wk->printWindow.flushPending) {
            if (func_0201c6c4(queue, BmpWin_GetBitmap(wk->printWindow.window)) == 0) {
                BmpWin_FlushChar(wk->printWindow.window);
                wk->printWindow.flushPending = 0;
            }
        }
    }
    GFL_TCBExMgrUpdate(wk->tcbManager);
    GFL_TCBExMgrUpdate(wk->tcbManager);
    return FALSE;
}

BOOL ShopUI_CheckSellingTMHM(ShopItem *items, int count) {
    BOOL tmhm = PML_ItemIsTMHM(items[0].item);
    int i;

    for (i = 0; i < count; i++) {
        PML_ItemIsTMHM(items[i].item);
    }
    return tmhm;
}

static const u16 data_ov021_021d943c[2] = { 0x156, 0x18b };

static const u16 data_ov021_021d9438[2] = { 0x4, 0x11 };

void func_ov021_021b385c(ShopUIWork *work, u32 index, u32 type, u32 move, MsgData *msgData) {
    if (type == 1) {
        WordSetNumber(work->wordSet, 0, move - 0x147, 2, 2, 1);
        loadMoveNameToStrbuf(work->wordSet, 1, (u32)PML_ItemGetTMWazaID((u16)move));
        GFL_WordSetFormatStrbuf(work->wordSet, work->message,
                work->moveLabelFormat);
        ListMenuCore_AppendStrBufOption(&work->options[index],
                work->message, index, work->heapId);
        return;
    }
    ListMenuCore_AppendMsgOption(&work->options[index], msgData, move, index,
            work->heapId);
}

void func_ov021_021b38e0(ShopUIWork *wk, u32 shopId, u32 fileId, u32 badges) {
    int i;
    int count;
    u32 file;
    MsgData *msgData;

    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ITEM_NAMES, wk->heapId);
    if (shopId == 0xff) {
        file = data_ov021_021d946c[badges];
    } else {
        file = fileId + 6;
    }
    if (PML_ItemIsTMHM(data_ov021_021de1a8[file][0])) {
        wk->listMode = SHOP_LIST_TMS;
    }
    count = data_ov021_021d95de[file];
    wk->options = ListMenuCore_CreateOptionList(count + 1, wk->heapId);
    for (i = 0; i < count; i++) {
        u16 item = data_ov021_021de1a8[file][i];

        func_ov021_021b385c(wk, i, wk->listMode, item, msgData);
        wk->entries[i].item = item;
        wk->entries[i].price = PassPower_ApplyBargain(GetItemParam(item, ITEM_PARAM_PRICE, wk->heapId));
    }
    wk->entryCount = i;
    ShopUI_CheckSellingTMHM(wk->entries, wk->entryCount);
    ListMenuCore_AppendMsgOption(&wk->options[i], wk->msgData, 11, BMPMENULIST_CANCEL, wk->heapId);
    GFL_MsgDataFree(msgData);
}

void func_ov021_021b39c4(ShopUIWork *wk, u32 index) {
    int i;
    int count;
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ITEM_NAMES, wk->heapId);
    const ShopItem *items = data_ov021_021de164[index];

    count = data_ov021_021d9430[index];
    wk->options = ListMenuCore_CreateOptionList(count + 1, wk->heapId);
    for (i = 0; i < count; i++) {
        func_ov021_021b385c(wk, i, wk->listMode, items[i].item, msgData);
        wk->entries[i].item = items[i].item;
        wk->entries[i].price = items[i].price;
    }
    wk->entryCount = i;
    ListMenuCore_AppendMsgOption(&wk->options[i], wk->msgData, 11, BMPMENULIST_CANCEL, wk->heapId);
    GFL_MsgDataFree(msgData);
}

void func_ov021_021b3a64(ShopUIWork *work, u16 shopId) {
    MsgData *msgData;
    u32 index;
    index = 0;
    msgData = GFL_MsgSysLoadData(0, 2, 0x36, work->heapId);
    func_ov020_02186670(work->cityState, work->entries, &work->entryCount, shopId - SHOP_ID_SPECIAL_MART_0_FIRST,
                        work->heapId);
    work->options = ListMenuCore_CreateOptionList(work->entryCount + 1, work->heapId);
    while ((s32)index < (s32)(u32)work->entryCount) {
        func_ov021_021b385c(work, index, work->listMode, work->entries[index].item, msgData);
        index = index + 1;
    }
    ListMenuCore_AppendMsgOption(&work->options[index], work->msgData, 11, BMPMENULIST_CANCEL, work->heapId);
    GFL_MsgDataFree(msgData);
}

void func_ov021_021b3b04(ShopUIWork *wk) {
    ListMenuCore_FreeOptionList(wk->options);
}

u16 func_ov021_021b3b14(ShopUIWork *work, u16 shopId) {
    MsgData *pMVar1;
    WordSet *pWVar2;
    StrBuf *pSVar3;
    TCBExManager *pTVar4;

    pMVar1 = GFL_MsgSysLoadData(0, 3, 0x1a7, work->heapId);
    work->msgData = pMVar1;
    pMVar1 = GFL_MsgSysLoadData(0, 2, 0x35, work->heapId);
    work->itemInfoMsgData = pMVar1;
    pWVar2 = GFL_WordSetSystemCreateDefault(work->heapId);
    work->wordSet = pWVar2;
    pSVar3 = GFL_StrBufCreate(200, work->heapId);
    work->message = pSVar3;
    pTVar4 = GFL_TCBExMgrCreate(work->heapId, work->heapId, 0x20, 0x20);
    work->tcbManager = pTVar4;
    pSVar3 = GFL_MsgDataLoadStrbufNew(work->msgData, 0x1d);
    work->moveLabelFormat = pSVar3;
    return shopId;
}

void func_ov021_021b3b84(ShopUIWork *wk) {
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_StrBufFree(wk->moveLabelFormat);
    GFL_StrBufFree(wk->message);
    GFL_StrBufFree(wk->ownedLabel);
    GFL_StrBufFree(wk->priceFormat);
    GFL_MsgDataFree(wk->msgData);
    GFL_MsgDataFree(wk->itemInfoMsgData);
}

void func_ov021_021b3bd8(ShopUIWork *work) {
    ArcTool *arc;

    reg_G2_BLDCNT = 0;
    arc = GFL_ArcSysCreateFileHandle(0x36, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 0x60, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 2, 100, 0, 0, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 2, 2, 0, 100, 0, 0, work->heapId);
    GFL_ArcToolFree(arc);
    LoadSysMsgBox(1, 0xe4, 9, 0, work->heapId);
    GFL_BGSysClearScr(1);
    GFL_BGSysSetBGPriority(2, 1);
}

void ShopUI_FreeBG(ShopUIWork *wk) {
    GFL_BGSysClearScr(1);
    GFL_BGSysClearScr(2);
    GFL_BGSysSetBGPriority(2, 0);
    setAlphaBlend_wrapper(1);
}

static void ShopUI_CreateCellActors(ShopUIWork *wk) {
    ArcTool *arc;
    u32 palette;

    arc = GFL_ArcSysCreateFileHandle(0x36, wk->heapId);
    palette = func_0204ac18(arc, 10, 0, 0, 0, 1, wk->heapId);
    wk->res[0].palette = palette;
    palette = func_0204a6c8(arc, 7, 0, 0, wk->heapId);
    wk->res[0].chars = palette;
    palette = func_0204a6c8(arc, 4, 0, 0, wk->heapId);
    wk->res[1].chars = palette;
    palette = func_0204af28(arc, 8, 9, wk->heapId);
    wk->res[0].cellAnims = palette;
    palette = func_0204af28(arc, 5, 6, wk->heapId);
    wk->res[1].cellAnims = palette;
    GFL_ArcToolFree(arc);
}

static void func_ov021_021b3cfc(ShopUIWork *wk) {
    u16 fileId;
    u32 palette;

    fileId = GetItemGraphicsDatID(1, 2);
    palette = func_0204ac18(wk->iconArc, (u32)fileId, 0, 0x20, 0, 1, wk->heapId);
    wk->res[2].palette = palette;
    fileId = GetItemGraphicsDatID(1, 1);
    palette = func_0204a6c8(wk->iconArc, (u32)fileId, 0, 0, wk->heapId);
    wk->res[2].chars = palette;
    palette = func_0204af28(wk->iconArc, 1, 0, wk->heapId);
    wk->res[2].cellAnims = palette;
}

void ShopUI_LoadItemSprite(ShopUIWork *wk, u16 item) {
    NNSG2dCharacterData *character;
    NNSG2dPaletteData *palette;
    void *nclr;
    void *ncgr;

    if (item <= ITEM_LAST) {
        nclr = GFL_G2DIOReadNCLRArc(wk->iconArc, GetItemGraphicsDatID(item, 2), &palette, wk->heapId);
        ncgr = GFL_G2DIOReadOBJNCGRArc(wk->iconArc, GetItemGraphicsDatID(item, 1), FALSE, &character, wk->heapId);
        func_0204adec(wk->res[2].palette, palette, 1);
        func_0204a9d0(wk->res[2].chars, character);
        GFL_HeapFree(nclr);
        GFL_HeapFree(ncgr);
        func_0204b3dc(wk->actors[4], TRUE);
    } else {
        func_0204b3dc(wk->actors[4], FALSE);
    }
}

static void func_ov021_021b3dec(ShopUIWork *wk) {
    wk->actors[0] = func_0204b294(wk->clactUnit, wk->res[0].chars, wk->res[0].palette, wk->res[0].cellAnims,
                                  &data_ov021_021d96e0[0], 0, wk->heapId);
    wk->actors[1] = func_0204b294(wk->clactUnit, wk->res[1].chars, wk->res[0].palette, wk->res[1].cellAnims,
                                  &data_ov021_021d96e0[1], 0, wk->heapId);
    wk->actors[2] = func_0204b294(wk->clactUnit, wk->res[1].chars, wk->res[0].palette, wk->res[1].cellAnims,
                                  &data_ov021_021d96e0[2], 0, wk->heapId);
    wk->actors[3] = func_0204b294(wk->clactUnit, wk->res[1].chars, wk->res[0].palette, wk->res[1].cellAnims,
                                  &data_ov021_021d96e0[3], 0, wk->heapId);
    func_0204b92c(wk->actors[3], TRUE);
    wk->actors[4] = func_0204b294(wk->clactUnit, wk->res[2].chars, wk->res[2].palette, wk->res[2].cellAnims,
                                  &data_ov021_021d96e0[4], 0, wk->heapId);
    func_0204b3dc(wk->actors[1], FALSE);
    func_0204b3dc(wk->actors[2], FALSE);
    func_0204b3dc(wk->actors[3], FALSE);
}

static void ShopUI_SetupSprites(ShopUIWork *wk) {
    ClActUnit *unit;
    ArcTool *pAVar1;

    unit = func_0204b100(5, 1, wk->heapId);
    wk->clactUnit = unit;
    func_0204b22c(unit, 1);
    ShopUI_CreateCellActors(wk);
    pAVar1 = GFL_ArcSysCreateFileHandle(0x19, wk->heapId);
    wk->iconArc = pAVar1;
    func_ov021_021b3cfc(wk);
    func_ov021_021b3dec(wk);
}

void func_ov021_021b3f3c(ShopUIWork *wk) {
    func_0204afd8(wk->res[2].cellAnims);
    func_0204afd8(wk->res[1].cellAnims);
    func_0204afd8(wk->res[0].cellAnims);
    func_0204a8d4(wk->res[2].chars);
    func_0204a8d4(wk->res[1].chars);
    func_0204a8d4(wk->res[0].chars);
    func_0204ada4(wk->res[2].palette);
    func_0204ada4(wk->res[0].palette);
    GFL_ArcToolFree(wk->iconArc);
    func_0204b1cc(wk->clactUnit);
}

static void ShopUI_SetupBmpWin(ShopUIWork *wk) {
    const ShopWindowSetup **setups;
    StrBuf *strbuf;
    int i;

    if (wk->currency == SHOP_CURRENCY_BP) {
        setups = data_ov021_021de18c;
    } else {
        setups = data_ov021_021de170;
    }
    for (i = 0; i < 7; i++) {
        const ShopWindowSetup *setup = setups[i];

        wk->windows[i] =
            BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, 1);
    }
    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0613_Text_020);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->windows[0]), 0, 0, strbuf, wk->font, PRINT_COLOR(15, 2, 15));
    GFL_StrBufFree(strbuf);
    PrintWindow_Init(&wk->printWindow, wk->windows[2]);
    wk->printQueue = func_0201c440(wk->heapId);
    if (wk->currency == SHOP_CURRENCY_BP) {
        BmpWin_TransferNow(wk->windows[1]);
    } else {
        BmpWin_TransferNow(wk->windows[0]);
        BmpWin_TransferNow(wk->windows[1]);
    }
}

void ShopUI_FreeSprites(ShopUIWork *wk) {
    s32 i;

    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    i = 0;
    do {
        BmpWin_Free(wk->windows[i]);
        i = i + 1;
    } while (i < 7);
}

void ShopUI_UpdateMoneyDisp(ShopUIWork *wk) {
    StrBuf *format;
    StrBuf *strbuf;
    GFLBitmap *bitmap;

    if (wk->currency == SHOP_CURRENCY_BP) {
        format = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0613_Text_022);
        strbuf = GFL_StrBufCreate(10, wk->heapId);
        WordSetNumber(wk->wordSet, 0, func_0200dabc(wk->bsubwayScore), 4, NUM_PAD_SPACE, 1);
    } else {
        format = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0613_Text_021);
        strbuf = GFL_StrBufCreate(10, wk->heapId);
        WordSetNumber(wk->wordSet, 0, getCash(wk->trainerCard), 7, NUM_PAD_SPACE, 1);
    }
    GFL_WordSetFormatStrbuf(wk->wordSet, strbuf, format);
    bitmap = BmpWin_GetBitmap(wk->windows[1]);
    GFL_BitmapFill(bitmap, 0);
    GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 2, 15));
    BmpWin_TransferNow(wk->windows[1]);
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(format);
}

// How many of the item the bag holds
void func_ov021_021b41ac(ShopUIWork *wk, u16 item) {
    u8 bg;
    StrBuf *src;
    StrBuf *dest;
    GFLBitmap *bitmap;
    BmpWin *window;

    src = GFL_MsgDataLoadStrbufNew(wk->msgData, 0x17);
    dest = GFL_StrBufCreate(0x14, wk->heapId);
    BmpWin_DrawFrame(wk->windows[3], 2, 0xe4, 9);
    WordSetNumber(wk->wordSet, 0, (u32)BagSave_GetItemCountByID(wk->bag, item, wk->heapId), 3, 0, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, dest, src);
    bitmap = BmpWin_GetBitmap(wk->windows[3]);
    GFL_BitmapFill(bitmap, 0xf);
    GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, dest, wk->font, 0x44f);
    window = wk->windows[3];
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
    GFL_StrBufFree(dest);
    GFL_StrBufFree(src);
}

// The description of the item at the cursor
void func_ov021_021b4264(ShopUIWork *wk, u16 item) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(wk->windows[5]);

    GFL_BitmapFill(bitmap, 0);
    if (item <= ITEM_LAST) {
        StrBuf *strbuf;

        strbuf = GFL_MsgDataLoadStrbufNew(wk->itemInfoMsgData, item);
        GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, strbuf, wk->font, PRINT_COLOR(15, 2, 0));
        GFL_StrBufFree(strbuf);
    }
    BmpWin_TransferNow(wk->windows[5]);
}

void ShopUI_UpdateItemAmountDisp(ShopUIWork *wk, u16 amount, u32 price) {
    StrBuf *amountFormat = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0613_Text_024);
    StrBuf *priceFormat = GFL_MsgDataLoadStrbufNew(wk->msgData, SHOP_UI_CURRENCY_MSGIDS[wk->currency]);
    StrBuf *strbuf = GFL_StrBufCreate(20, wk->heapId);
    GFLBitmap *bitmap = BmpWin_GetBitmap(wk->windows[4]);

    GFL_BitmapFill(bitmap, 15);
    BmpWin_DrawFrame(wk->windows[4], WINFRAME_TRANSFER_NONE, 228, 9);
    WordSetNumber(wk->wordSet, 0, amount, 2, NUM_PAD_NONE, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, strbuf, amountFormat);
    GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, strbuf, wk->font, PRINT_COLOR(1, 2, 15));
    WordSetNumber(wk->wordSet, 0, amount * price, 7, NUM_PAD_SPACE, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, strbuf, priceFormat);
    if (wk->currency == SHOP_CURRENCY_MONEY) {
        GFL_TextRendererDrawToBitmapEx(bitmap, 35, 0, strbuf, wk->font, PRINT_COLOR(1, 2, 15));
    } else {
        GFL_TextRendererDrawToBitmapEx(bitmap, 29, 0, strbuf, wk->font, PRINT_COLOR(1, 2, 15));
    }
    BmpWin_TransferNow(wk->windows[4]);
    GFL_StrBufFree(strbuf);
    GFL_StrBufFree(priceFormat);
    GFL_StrBufFree(amountFormat);
}

static void ShopUI_SetupMenuList(ShopUIWork *wk) {
    BmpMenuListHeader header;

    header.options = wk->options;
    header.cursorCallback = func_ov021_021b468c;
    if (wk->listMode == SHOP_LIST_ITEMS) {
        header.printCallback = func_ov021_021b4510;
    } else {
        header.printCallback = func_ov021_021b45a0;
    }
    header.window = wk->windows[2];
    header.count = ListMenuCore_GetFirstFreeIndex(wk->options);
    header.maxShown = 7;
    header.labelX = 0;
    header.itemX = 0;
    header.cursorX = 0;
    header.y = 8;
    header.fgColor = 12;
    header.bgColor = 0;
    header.shadowColor = 13;
    header.letterSpacing = 0;
    header.lineSpacing = 0;
    header.pageSkip = BMPMENULIST_SKIP_LR_KEY;
    header.fontId = 0;
    header.cursorDisplay = BMPMENULIST_CURSOR_HIDE;
    header.work = wk;
    header.fontSizeX = 12;
    header.fontSizeY = 16;
    header.msgData = NULL;
    header.printWindow = &wk->printWindow;
    header.queue = wk->printQueue;
    header.font = wk->font;
    wk->list = BmpMenuList_Create(&header, 0, 0, wk->heapId);
}

// Prints straight to the bitmap in the first row, and through the print queue below it
void func_ov021_021b44cc(PrintQueue *queue, GFLBitmap *bitmap, int x, int y, const StrBuf *strbuf, Font *font,
                         u16 color) {
    if (y <= 8) {
        GFL_TextRendererDrawToBitmapEx(bitmap, x, y, strbuf, font, color);
    } else {
        func_0201c724(queue, bitmap, x, y, strbuf, font, color);
    }
}

// Prints an item's price at the right of its row
void func_ov021_021b4510(BmpMenuList *list, intptr_t value, u8 y) {
    ShopUIWork *wk = func_02020d0c(list);
    s32 width;
    u32 windowWidth;

    if (value == BMPMENULIST_CANCEL) {
        return;
    }
    WordSetNumber(wk->wordSet, 1, wk->entries[value].price, 5, NUM_PAD_NONE, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->message, wk->priceFormat);
    width = GFL_FontGetBlockWidth(wk->message, wk->font, 0);
    windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[2]));
    func_ov021_021b44cc(wk->printQueue, BmpWin_GetBitmap(wk->windows[2]), windowWidth - width, y, wk->message, wk->font,
                        PRINT_COLOR(12, 13, 0));
}

// The same for a TM, or that the bag holds it
static void func_ov021_021b45a0(BmpMenuList *list, intptr_t value, u8 y) {
    ShopUIWork *wk = func_02020d0c(list);
    s32 width;
    u32 windowWidth;

    if (value == BMPMENULIST_CANCEL) {
        return;
    }
    if (BagSave_CheckAmount(wk->bag, wk->entries[value].item, 1, wk->heapId)) {
        width = GFL_FontGetBlockWidth(wk->ownedLabel, wk->font, 0);
        windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[2]));
        func_ov021_021b44cc(wk->printQueue, BmpWin_GetBitmap(wk->windows[2]), windowWidth - width, y, wk->ownedLabel,
                            wk->font, PRINT_COLOR(12, 13, 0));
        return;
    }
    WordSetNumber(wk->wordSet, 1, wk->entries[value].price, 5, NUM_PAD_SPACE, 1);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->message, wk->priceFormat);
    width = GFL_FontGetBlockWidth(wk->message, wk->font, 0);
    windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[2]));
    func_ov021_021b44cc(wk->printQueue, BmpWin_GetBitmap(wk->windows[2]), windowWidth - width, y, wk->message, wk->font,
                        PRINT_COLOR(12, 13, 0));
}

// Moves the cursor's arrow, and shows the description and icon of the entry at the cursor
void func_ov021_021b468c(BmpMenuList *list, intptr_t value, u8 init) {
    ShopUIWork *wk;
    u16 listTop;
    u16 cursorRow;
    ClActorPos pos;

    wk = func_02020d0c(list);
    func_020202fc(list, &listTop, &cursorRow);
    pos.x = 172;
    pos.y = cursorRow * 16 + 22;
    func_0204b404(wk->actors[0], &pos, 0);
    if (value < NELEMS(wk->entries)) {
        func_ov021_021b4264(wk, wk->entries[value].item);
        ShopUI_LoadItemSprite(wk, wk->entries[value].item);
    } else {
        func_ov021_021b4264(wk, 0xffff);
        ShopUI_LoadItemSprite(wk, 0xffff);
    }
}

void func_ov021_021b46fc(ShopUIWork *wk) {
    u8 cursorRow;
    u16 listTop;

    BmpMenuList_Free(wk->list, &listTop, &cursorRow);
}

int func_ov021_021b4710(u32 unused) {
    return 0;
}

void ShopUI_SetStatusDialogue(ShopUIWork *wk, u32 msgId, u16 item, BOOL plural) {
    StrBuf *strbuf;
    BOOL flag = msgId == 9 && plural;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[6]), 15);
    BmpWin_DrawFrame(wk->windows[6], WINFRAME_TRANSFER_NOW, 228, 9);
    loadItemText(wk->wordSet, 0, item, flag, FALSE);
    loadBagPocketNameToStrbuf(wk->wordSet, 1, GetItemParam(item, 5, wk->heapId));
    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->message, strbuf);
    GFL_StrBufFree(strbuf);
    wk->printStream = func_0201cd10(wk->windows[6], 0, 0, wk->message, wk->font, func_02012ffc(), wk->tcbManager, 0,
                                    wk->heapId, 0xffff, func_ov021_021b4710);
    BmpWin_TransferNow(wk->windows[6]);
}

// Asks whether to buy the item or the TM. The callers pass the list mode, which the menu's own replaces
void ShopUI_SetBuyConfirmMessage(ShopUIWork *wk, u8 listMode) {
    if (wk->listMode == SHOP_LIST_TMS) {
        if (wk->currency == SHOP_CURRENCY_BP) {
            func_ov021_021b4984(wk, 8, wk->item, wk->price);
        } else {
            func_ov021_021b4984(wk, 7, wk->item, wk->price);
        }
    } else {
        if (wk->currency == SHOP_CURRENCY_BP) {
            func_ov021_021b4918(wk, 6, wk->item, wk->price, wk->amount);
        } else {
            func_ov021_021b4918(wk, 5, wk->item, wk->price, wk->amount);
        }
    }
}

void func_ov021_021b4878(BmpWin *window) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_DrawFrame(window, 0, 0xe4, 9);
}

void func_ov021_021b4894(ShopUIWork *wk, u32 param_2) {
    u8 bg;
    StrBuf *src;
    s32 wait;
    PrintStream *pPVar1;
    BmpWin *window;

    src = GFL_MsgDataLoadStrbufNew(wk->msgData, param_2);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->message, src);
    GFL_StrBufFree(src);
    wait = func_02012ffc();
    pPVar1 = func_0201cd10(wk->windows[6], 0, 0, wk->message,
            wk->font, wait, wk->tcbManager, 0,
            wk->heapId, 0xffff, (void *)0x0);
    wk->printStream = pPVar1;
    window = wk->windows[6];
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
}

// Asks whether to buy an amount of an item
void func_ov021_021b4918(ShopUIWork *wk, u32 msgId, u16 item, u32 price, u16 amount) {
    func_ov021_021b4878(wk->windows[6]);
    loadItemText(wk->wordSet, 0, item, FALSE, FALSE);
    WordSetNumber(wk->wordSet, 1, amount, 2, NUM_PAD_NONE, 1);
    WordSetNumber(wk->wordSet, 2, (u32)amount * price, 7, NUM_PAD_NONE, 1);
    func_ov021_021b4894(wk, msgId);
}

// Asks whether to buy a TM
void func_ov021_021b4984(ShopUIWork *wk, u32 msgId, u16 item, u32 price) {
    func_ov021_021b4878(wk->windows[6]);
    WordSetNumber(wk->wordSet, 0, item - ITEM_TM01 + 1, 2, NUM_PAD_ZERO, 1);
    loadMoveNameToStrbuf(wk->wordSet, 1, PML_ItemGetTMWazaID(item));
    WordSetNumber(wk->wordSet, 2, price, 5, NUM_PAD_NONE, 1);
    func_ov021_021b4894(wk, msgId);
}

// Changes the amount by delta, wrapping around by one and stopping at the ends by ten. Returns 1 for more, 2 for less
// and 0 if it didn't change
u32 ShopUI_ChangeItemAmount(s16 *amount, u16 max, s32 delta) {
    s16 old = *amount;

    switch (delta) {
    case -1:
        *amount = old - 1;
        if (*amount <= 0) {
            *amount = max;
        }
        if (*amount == old) {
            return 0;
        }
        return 2;
    case -10:
        *amount = old - 10;
        if (*amount <= 0) {
            *amount = 1;
        }
        if (*amount == old) {
            return 0;
        }
        return 2;
    case 1:
        *amount = old + 1;
        if (*amount > max) {
            *amount = 1;
        }
        if (*amount == old) {
            return 0;
        }
        return 1;
    case 10:
        *amount = old + 10;
        if (*amount > max) {
            *amount = max;
        }
        if (*amount == old) {
            return 0;
        }
        return 1;
    }
    return 0;
}

u8 ShopUI_UpdateInput(ShopUIWork *wk) {
    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (ShopUI_ChangeItemAmount(&wk->amount, wk->maxAmount, 1)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            ShopUI_UpdateItemAmountDisp(wk, wk->amount, wk->price);
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (ShopUI_ChangeItemAmount(&wk->amount, wk->maxAmount, -1)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            ShopUI_UpdateItemAmountDisp(wk, wk->amount, wk->price);
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        if (ShopUI_ChangeItemAmount(&wk->amount, wk->maxAmount, -10)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            ShopUI_UpdateItemAmountDisp(wk, wk->amount, wk->price);
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        if (ShopUI_ChangeItemAmount(&wk->amount, wk->maxAmount, 10)) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            ShopUI_UpdateItemAmountDisp(wk, wk->amount, wk->price);
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        ShopUI_SetBuyConfirmMessage(wk, wk->listMode);
        wk->nextState = 6;
        return 10;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return 4;
    }
    return 3;
}

void func_ov021_021b4ba4(u32 index) {
    GFL_BGSysFillScrArea(1, 0, data_ov021_021d9654[index].x, data_ov021_021d9654[index].y, data_ov021_021d9654[index].width,
                         data_ov021_021d9654[index].height, 0);
}
