// Black's code at ov021 0x021b7400-0x021b7884: pokebw2's src/ov033/pdw_postman.c, the window that lists the items the
// Dream World sent and the lookup of the mystery cards (its asserts pass "pdw_postman.c"). The mystery-gift script
// command and the gift handlers that pokebw2's file has too are Black's ov021_021b69b0.c: their table sorts before
// scrcmd_misc.c's .rodata.

#include "types.h"
#include "field/field.h"
#include "field/pdw_postman.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "save/bag.h"
#include "save/dream_world.h"
#include "save/mystery_gift.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/wordset.h"

typedef struct PdwPostmanItem {
    u16 item;
    u16 quantity;
} PdwPostmanItem;

// The window that lists up to 7 of the items (pokebw2's)
typedef struct PdwPostmanItemWindow {
    u16 heapId;
    Field *field;
    MsgData *messages;
    MsgBGSysWin *window;
    MsgBGCursor *unk10;
    WordSet *wordSet;
    StrBuf *first;
    StrBuf *second;
    s32 count;
    PdwPostmanItem *items;
} PdwPostmanItemWindow;

// The event that shows the items 7 at a time (pokebw2's)
typedef struct PdwPostmanItemEvent {
    Field *field;
    PdwPostmanItemWindow *window;
    PdwPostmanItem *items;
    u32 count;
    u16 index;
} PdwPostmanItemEvent;

GameEventReturnCode func_ov021_021b7574(GameEvent *unused, u32 *state, void *data);
PdwPostmanItem *func_ov021_021b7640(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld, u32 *count);

PdwPostmanItemWindow *func_ov021_021b7400(Field *field, PdwPostmanItem *items, s32 count);
void func_ov021_021b74cc(PdwPostmanItemWindow *work);
MsgBGCursor *func_ov021_02192788(FieldMsgBG *param_1);
void func_ov021_021928c0(MsgBGCursor *param_1, GFLBitmap *param_2, u8 param_3);
void func_ov021_021927b0(MsgBGCursor *param_1);

PdwPostmanItemWindow *func_ov021_021b7400(Field *field, PdwPostmanItem *items, s32 count) {
    u16 heapId;
    PdwPostmanItemWindow *work;
    s32 height;

    heapId = Field_GetHeapID(field);
    work = GFL_HeapAllocate(heapId, sizeof(PdwPostmanItemWindow), TRUE, "pdw_postman.c", 0x73);
    work->heapId = heapId;
    work->field = field;
    work->items = items;
    work->count = count;
    work->messages = GFL_MsgSysLoadData(FALSE, 3, 0x13b, heapId);
    work->wordSet = GFL_WordSetSystemCreateDefault(heapId);
    work->first = GFL_StrBufCreate(0x80, heapId);
    work->second = GFL_StrBufCreate(0x80, heapId);
    height = ((14 * count + 7) & ~7) / 8;
    work->window = FieldMsgBG_CreateMoneyWin(Field_GetMsgBGSys(field), work->messages, 1, 1, 0x15, height);
    return work;
}

void func_ov021_021b7494(PdwPostmanItemWindow *param_1) {
    func_ov021_02190788(param_1->window);
    func_ov021_02190728(param_1->window);
    GFL_BGSysLoadScr(1);
    GFL_StrBufFree(param_1->first);
    GFL_StrBufFree(param_1->second);
    GFL_WordSetSystemFree(param_1->wordSet);
    GFL_MsgDataFree(param_1->messages);
    GFL_HeapFree(param_1);
}

void func_ov021_021b74cc(PdwPostmanItemWindow *work) {
    MsgData *messages;
    s32 i;

    messages = GFL_MsgSysLoadData(FALSE, 2, 0x36, HEAPID_TAIL(work->heapId));
    for (i = 0; i < work->count; i++) {
        GFL_MsgDataLoadStrbuf(messages, work->items[i].item, work->first);
        func_ov021_02190758(work->window, 0, 14 * i, work->first);
        GFL_MsgDataLoadStrbuf(work->messages, 5, work->first);
        WordSetNumber(work->wordSet, 0, work->items[i].quantity, 2, 1, 1);
        GFL_WordSetFormatStrbuf(work->wordSet, work->second, work->first);
        func_ov021_02190758(work->window, 0x6c, 14 * i, work->second);
    }
    GFL_MsgDataFree(messages);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(func_ov021_021907c0(work->window)));
}

GameEventReturnCode func_ov021_021b7574(GameEvent *unused, u32 *state, void *data) {
    PdwPostmanItemEvent *event = data;
    PdwPostmanItemWindow *ptr;
    FieldMsgBG *pHVar2;
    BOOL BVar4;
    BmpWin *window;
    GFLBitmap *pGVar5;
    u32 val;
    switch (*state) {
    case 0:
        val = event->count - (u32)event->index & 0xffff;
        if (7 < val) {
            val = 7;
        }
        ptr = func_ov021_021b7400(event->field, &event->items[event->index], val);
        event->window = ptr;
        func_ov021_021b74cc(ptr);
        pHVar2 = Field_GetMsgBGSys(event->field);
        event->window->unk10 = func_ov021_02192788(pHVar2);
        (*state)++;
        break;
    case 1:
        BVar4 = func_ov021_0219077c(event->window->window);
        if (BVar4 == 1) {
            (*state)++;
        }
        break;
    case 2:
        window = func_ov021_021907c0(event->window->window);
        pGVar5 = BmpWin_GetBitmap(window);
        func_ov021_021928c0(event->window->unk10, pGVar5, 0xf);
        BmpWin_FlushChar(window);
        if ((GCTX_HIDGetPressedKeys() & 1) != 0) {
            GFL_SndSEPlay(0x547);
            (*state)++;
        }
        break;
    case 3:
        func_ov021_021927b0(event->window->unk10);
        func_ov021_021b7494(event->window);
        (*state)++;
        break;
    case 4:
        if (event->index + 7 >= event->count) {
            GFL_HeapFree(event->items);
            return GAMEEVENT_DONE;
        }
        event->index = event->index + 7;
        *state = 0;
        break;
    }
    return GAMEEVENT_CONTINUE;
}

PdwPostmanItem *func_ov021_021b7640(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld, u32 *count) {
    PdwPostmanItem *result;
    BagSave *bag;
    s32 n;
    s32 i;
    u16 item;
    u16 quantity;

    bag = GameData_GetBag(gameData);
    result = GFL_HeapAllocate(heapId, sizeof(PdwPostmanItem) * 20, TRUE, "pdw_postman.c", 0x14d);
    i = 0;
    n = 0;
    for (; i < 20; i++) {
        item = func_02009310(dreamWorld, i);
        quantity = func_02009330(dreamWorld, i);
        if (item != 0 && BagSave_CheckAvailItemSpace(bag, item, quantity, heapId) == TRUE) {
            result[n].item = item;
            result[n].quantity = quantity;
            n++;
        }
    }
    *count = n;
    return result;
}

void func_ov021_021b76b4(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld) {
    u8 count;
    u16 item;
    BagSave *bag;
    BOOL BVar2;
    u32 index;

    bag = GameData_GetBag(gameData);
    index = 0;
    do {
        item = func_02009310(dreamWorld, index);
        count = func_02009330(dreamWorld, index);
        if ((item != 0) && (BVar2 = BagSave_AddItem(bag, item, (u16)count, heapId), BVar2 == 1)) {
            func_02009364(dreamWorld, index);
        }
        index = index + 1;
    } while (index < 0x14);
}

u32 func_ov021_021b76f8(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld) {
    u8 count;
    u16 item;
    BagSave *bag;
    BOOL BVar2;
    u32 index;
    u32 i;

    bag = GameData_GetBag(gameData);
    index = 0;
    i = 0;
    do {
        item = func_02009310(dreamWorld, index);
        count = func_02009330(dreamWorld, index);
        if ((item != 0) &&(BVar2 = BagSave_CheckAvailItemSpace(bag, item, (u16)count, heapId), BVar2 == 0)) {
            i = i + 1;
        }
        index = index + 1;
    } while (index < 0x14);
    return i;
}

u16 func_ov021_021b7740(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld, u32 position) {
    u8 count;
    u16 item;
    BagSave *bag;
    BOOL BVar2;
    s32 index;
    u32 i;

    bag = GameData_GetBag(gameData);
    index = 0;
    i = 0;
    do {
        item = func_02009310(dreamWorld, index);
        count = func_02009330(dreamWorld, index);
        if ((item != 0) &&(BVar2 = BagSave_CheckAvailItemSpace(bag, item, (u16)count, heapId), BVar2 == 0)) {
            if (i == position) {
                return item;
            }
            i = i + 1;
        }
        index = index + 1;
    } while (index < 0x14);
    return 0;
}

GameEvent *func_ov021_021b7794(GameSystem *gsys) {
    GameData *gameData;
    SaveControl *save;
    DreamWorldSave *pDVar2;
    GameEvent *event;
    PdwPostmanItemEvent *data;
    Field *field;

    gameData = GSYS_GetGameData(gsys);
    save = GameData_GetSaveControl(gameData);
    pDVar2 = getDreamWorldStuffAddress(save);
    event = GameEvent_Create(gsys, NULL, func_ov021_021b7574, sizeof(PdwPostmanItemEvent));
    data = GameEvent_GetData(event);
    field = GSYS_GetField(gsys);
    data->field = field;
    data->index = 0;
    data->items = func_ov021_021b7640(gameData, Field_GetHeapID(field), pDVar2, &data->count);
    return event;
}

MysteryGift *func_ov021_021b77e4(MysteryGiftSave *param_1, u32 param_2, MysteryGift *param_3) {
    BOOL BVar1;

    BVar1 = func_0200a0e4(param_1, param_2);
    if (BVar1 == 0) {
        return (MysteryGift *)0x0;
    }
    BVar1 = func_0200a104(param_1, param_2);
    if (BVar1 == 1) {
        return (MysteryGift *)0x0;
    }
    BVar1 = func_0200a000(param_1, param_2, param_3);
    if (BVar1 == 0) {
        return (MysteryGift *)0x0;
    }
    if (param_3->kind == 0) {
        return (MysteryGift *)0x0;
    }
    if (3 < param_3->kind) {
        param_3 = (MysteryGift *)0x0;
    }
    return param_3;
}

MysteryGift *func_ov021_021b7834(MysteryGiftSave *param_1, s32 *param_2, MysteryGift *param_3) {
    MysteryGift *pMVar1;
    u32 i;

    i = 0;
    do {
        pMVar1 = func_ov021_021b77e4(param_1, i, param_3);
        if (pMVar1 != (MysteryGift *)0x0) {
            *param_2 = i;
            return param_3;
        }
        i = i + 1;
    } while ((s32)i < 0xc);
    return (MysteryGift *)0x0;
}

void func_ov021_021b785c(MysteryGiftSave *save, u32 slot) {
    func_0200a13c(save, slot);
}

u8 func_ov021_021b7864(MysteryGiftSave *param_1) {
    MysteryGift *pMVar1;
    s32 sStack_d8;
    MysteryGift MStack_d4;

    pMVar1 = func_ov021_021b7834(param_1, &sStack_d8, &MStack_d4);
    if (pMVar1 == (MysteryGift *)0x0) {
        return 0;
    }
    return pMVar1->kind;
}
