// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/gsync_state.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Game Sync itself: it asks the server for the account's state, sends the Pokémon to the Dream World or gets it back
// with what it brought, downloads the C-Gear and Pokédex skins and the musical the Dream World sent, and saves. The
// name is the ROM's string, from GFL_HeapAllocate's calls and an assert. Function names are ours.

#include "types.h"
#include "app/gsync.h"
#include "app/gsync/gsync_disp.h"
#include "app/gsync/gsync_download.h"
#include "app/gsync/gsync_message.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/nhttp_rap.h"
#include "gfl/clact.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/rtc_cache.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "nitro/rtc.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/adventure.h"
#include "save/box.h"
#include "save/c_gear_savedata.h"
#include "save/dream_world.h"
#include "save/medal_box.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_taskmenu.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/tpoke_data.h"
#include "system/wipe.h"
#include "dwc/nhttp.h"

#define HEAPID_GSYNC 0x62

// Errors of Game Sync's own, beside the server's
#define GSYNC_ERROR_DOWNLOAD 0xfff0
#define GSYNC_ERROR_SERVICE_UNAVAILABLE 0xfff1
#define GSYNC_ERROR_BAD_GATEWAY 0xfff2
#define GSYNC_ERROR_HTTP 0xfff3

// The kinds of file Game Sync downloads
enum {
    GSYNC_DOWNLOAD_CGEAR,
    GSYNC_DOWNLOAD_MUSICAL,
    GSYNC_DOWNLOAD_ZUKAN,
};

// The requests Game Sync makes of the server, by their URLs in overlay 189's table
enum {
    GSYNC_REQUEST_PLAY_STATUS,         // account.playstatus
    GSYNC_REQUEST_SPECIES_FLAGS,       // sleepily.bitlist, the species that can be sent
    GSYNC_REQUEST_DOWNLOAD,            // savedata.download
    GSYNC_REQUEST_UPLOAD,              // savedata.upload
    GSYNC_REQUEST_DOWNLOAD_FINISH = 8, // savedata.download.finish
    GSYNC_REQUEST_CREATE = 9,          // account.create.upload
};

// The largest item ID the Dream World sends
#define GSYNC_ITEM_MAX 626

// The sizes of the downloaded skins, before their CRC
#define GSYNC_CGEAR_SKIN_SIZE 0x2600
#define GSYNC_ZUKAN_SKIN_SIZE 0x6200
#define GSYNC_MUSICAL_SIZE_MAX 0x1fc0e

typedef struct GSyncWork GSyncWork;
typedef void (*GSyncStateFunc)(GSyncWork *wk);

// A Pokémon the Dream World sends to the Entree Forest
// Black 2's; twinned code confirms the offsets of species, unk2, form, sex and slot in Black
typedef struct {
    u16 species;
    u16 unk2;
    u8 form;
    u8 sex;
    u8 slot;
    u8 unk7;
} GSyncResultPokemon;

// What the Dream World sends back with the Pokémon
// Black's size is 0x11c (the sizeof of the reply the Dream World sends back, copied whole); the people are Black 2's
typedef struct {
    u32 id;
    GSyncResultPokemon pokemon[10];
    u16 levels;
    u8 unk56;
    u8 musical;
    u8 cgear;
    u8 zukan;
    u8 unk5A;
    u8 unk5B;
    u16 items[20];
    u8 itemCounts[20];
    u8 entries[5][26];
    u8 unk11A[2];
} GSyncResult;

// The server's answer: its status, and after the header the body of the request's answer
// Black 2's, not checked against Black
typedef struct {
    u32 status;
    u8 unk4[0x7c];
    union {
        GSyncResult result;
        GSyncAccountInfo accountInfo;
        u8 sendableSpecies[0x80];
    } body;
} GSyncResponse;

// Black 2's; twinned code confirms the offsets of download, http, disp, msg, yesNo, param, boxes, save, gameData,
// dreamWorldCopy, boxTray, boxPos, state, pkmTaken, receiving, saving and busy in Black
struct GSyncWork {
    HeapID heapId;
    u8 request[4];
    GSyncDownload *download;
    PartyPkm *pkm;
    NHttpRap *http;
    GSyncDisp *disp;
    GSyncMessage *msg;
    AppTaskMenu *yesNo;
    EventGameSync *param;
    BoxSaveAccessor *boxes;
    SaveControl *save;
    GameData *gameData;
    void *musical;
    void *dreamWorldCopy;
    GSyncResult *result;
    void *zukanSkin;
    void *cgearSkin;
    void *unk44;
    int boxTray;
    int boxPos;
    GSyncStateFunc state;
    u8 unk54[0xc];
    int timer;
    int seTimer;
    int idleProgress;
    int levels;
    int progress;
    int frames;
    int wait;
    u32 resultId;
    int error;
    u8 unk84[0x20];
    u8 getMusical;
    u8 getCGear;
    u8 getZukan;
    u8 downloadKind;
    u8 downloadFlags;
    u8 hasItems;
    u8 pkmTaken;
    u8 receiving;
    // Whether the Game Sync ID was shown before, from the save, and in this session
    u8 idShownSaved;
    u8 saving;
    u8 busy;
    u8 idShown;
};

BOOL func_ov123_021d6e08(GSyncWork *wk, int status);
BOOL func_ov123_021d6e70(GSyncWork *wk);
BOOL func_ov123_021d6ea4(GSyncWork *wk);
BOOL func_ov123_021d6ebc(GSyncWork *wk);
void func_ov123_021d6edc(GSyncWork *wk, GSyncStateFunc state);
void func_ov123_021d6ee0(GSyncWork *wk, GSyncStateFunc state, int line);
void func_ov123_021d6ee8(GSyncWork *wk);
void func_ov123_021d6f04(GSyncWork *wk);
void func_ov123_021d6f44(GSyncWork *wk);
void func_ov123_021d6f70(GSyncWork *wk);
void func_ov123_021d6ff0(GSyncWork *wk);
void func_ov123_021d706c(GSyncWork *wk);
void func_ov123_021d7088(GSyncWork *wk);
void func_ov123_021d70bc(GSyncWork *wk);
void func_ov123_021d7130(GSyncWork *wk);
void func_ov123_021d7180(GSyncWork *wk);
void func_ov123_021d7204(uintptr_t param, fx32 frame);
void func_ov123_021d7224(GSyncWork *wk);
void func_ov123_021d72d8(GSyncWork *wk);
void func_ov123_021d7390(GSyncWork *wk);
void func_ov123_021d73d8(GSyncWork *wk);
void func_ov123_021d7430(GSyncWork *wk);
void func_ov123_021d746c(GSyncWork *wk);
void func_ov123_021d751c(GSyncWork *wk);
void func_ov123_021d7560(GSyncWork *wk);
void func_ov123_021d758c(GSyncWork *wk);
void func_ov123_021d75d0(GSyncWork *wk);
void func_ov123_021d768c(GSyncWork *wk);
void func_ov123_021d76b8(GSyncWork *wk);
void func_ov123_021d76f0(GSyncWork *wk);
void func_ov123_021d7798(GSyncWork *wk);
void func_ov123_021d77c4(GSyncWork *wk, int size);
void func_ov123_021d7858(GSyncWork *wk);
void func_ov123_021d78c0(GSyncWork *wk);
void func_ov123_021d7910(GSyncWork *wk);
void func_ov123_021d7960(GSyncWork *wk);
void func_ov123_021d79a4(GSyncWork *wk);
void func_ov123_021d7a18(GSyncWork *wk);
void func_ov123_021d7a5c(GSyncWork *wk);
void func_ov123_021d7b68(GSyncWork *wk);
void func_ov123_021d7bdc(GSyncWork *wk);
void func_ov123_021d7c54(DreamWorldSave *dreamWorld, GSyncResult *result);
BOOL func_ov123_021d7cb4(DreamWorldSave *dreamWorld, GSyncResult *result);
BOOL func_ov123_021d7cec(DreamWorldSave *dreamWorld, GSyncResult *result);
void func_ov123_021d7d18(GSyncWork *wk, DreamWorldSave *dreamWorld, int species, int sex, int form, int unk2,
                                   int slot);
void func_ov123_021d7de8(GSyncWork *wk, DreamWorldSave *dreamWorld, GSyncResult *result);
void func_ov123_021d7e20(GSyncWork *wk, GSyncResult *result);
void func_ov123_021d7e58(GSyncWork *wk, DreamWorldSave *dreamWorld, GSyncResult *result, GSyncResponse *response);
BOOL func_ov123_021d7f5c(GSyncWork *wk);
void func_ov123_021d7f9c(GSyncWork *wk);
void func_ov123_021d80b8(GSyncWork *wk);
void func_ov123_021d8130(GSyncWork *wk);
void func_ov123_021d8154(GSyncWork *wk);
void func_ov123_021d81b4(GSyncWork *wk);
void func_ov123_021d81e4(GSyncWork *wk);
void func_ov123_021d8210(GSyncWork *wk);
void func_ov123_021d8244(GSyncWork *wk);
void func_ov123_021d8270(GSyncWork *wk);
void func_ov123_021d8304(GSyncWork *wk);
void func_ov123_021d834c(GSyncWork *wk);
void func_ov123_021d83a4(GSyncWork *wk);
void func_ov123_021d83c8(GSyncWork *wk);
void func_ov123_021d840c(GSyncWork *wk);
void func_ov123_021d8448(GSyncWork *wk);
void func_ov123_021d8490(GSyncWork *wk);
void func_ov123_021d8520(GSyncWork *wk);
void func_ov123_021d8590(GSyncWork *wk);
void func_ov123_021d85c4(GSyncWork *wk);
void func_ov123_021d85f0(GSyncWork *wk);
void func_ov123_021d8624(GSyncWork *wk);
void func_ov123_021d8650(GSyncWork *wk, u16 status, GSyncResponse *response);
void func_ov123_021d8790(GSyncWork *wk);
void func_ov123_021d87f8(GSyncWork *wk);
void func_ov123_021d885c(GSyncWork *wk);
void func_ov123_021d88a0(GSyncWork *wk);
void func_ov123_021d88c0(GSyncWork *wk);
void func_ov123_021d8958(GSyncWork *wk);
void func_ov123_021d8a38(GSyncWork *wk);
void func_ov123_021d8a7c(GSyncWork *wk);
void func_ov123_021d8a9c(GSyncWork *wk);
void func_ov123_021d8b0c(GSyncWork *wk);
void func_ov123_021d8b7c(GSyncWork *wk);
void func_ov123_021d8c04(GSyncWork *wk);
void func_ov123_021d8d08(GSyncWork *wk);
void func_ov123_021d8d98(GSyncWork *wk);
void func_ov123_021d8dfc(GSyncWork *wk);
void func_ov123_021d8e28(GSyncWork *wk);
void func_ov123_021d8e68(GSyncWork *wk);
void func_ov123_021d8e98(GSyncWork *wk);
void func_ov123_021d8ec4(GSyncWork *wk);
void func_ov123_021d8f3c(void *work, int a1, int code, int error);
void func_ov123_021d8f58(GSyncWork *wk);
BOOL func_ov123_021d8fbc(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov123_021d9158(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov123_021d920c(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's GSYNC_PROC_FUNCTIONS
const GameProcFunctions data_ov123_021da838 = {
    func_ov123_021d8fbc,
    func_ov123_021d9158,
    func_ov123_021d920c,
};

BOOL func_ov114_021b95d4(u32 a0, NHttpRap *a1);
u32 func_02009930(void);

// Returns whether the HTTP status is an error, and shows it
// pokebw2's GSync_CheckHttpStatus
BOOL func_ov123_021d6e08(GSyncWork *wk, int status) {
    switch (status) {
    case 503:
        wk->error = GSYNC_ERROR_SERVICE_UNAVAILABLE;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 211);
        return TRUE;
    case 502:
        wk->error = GSYNC_ERROR_BAD_GATEWAY;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 215);
        return TRUE;
    default:
        if (status >= 400) {
            wk->error = GSYNC_ERROR_HTTP;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 221);
            return TRUE;
        }
        return FALSE;
    }
}

// pokebw2's GSync_IsOnline
BOOL func_ov123_021d6e70(GSyncWork *wk) {
    EventGameSync *param = wk->param;

    if (param->gsyncResult == GSYNC_RESULT_CONNECT) {
        return FALSE;
    }
    if (param->hasAccountInfo && param->accountInfo.status == 4) {
        return FALSE;
    }
    if (func_0203cd98()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSync_IsNewAccount
BOOL func_ov123_021d6ea4(GSyncWork *wk) {
    EventGameSync *param = wk->param;

    if (param->hasAccountInfo && param->accountInfo.status == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSync_HasAccountPokemon
BOOL func_ov123_021d6ebc(GSyncWork *wk) {
    EventGameSync *param = wk->param;

    if (param->hasAccountInfo) {
        if (param->accountInfo.status == 0) {
            return TRUE;
        }
        if (param->accountInfo.status == 4) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's GSync_SetState
void func_ov123_021d6edc(GSyncWork *wk, GSyncStateFunc state) {
    wk->state = state;
}

// Changes the state, from a line that the debug build reported
// pokebw2's GSync_ChangeState
void func_ov123_021d6ee0(GSyncWork *wk, GSyncStateFunc state, int line) {
    func_ov123_021d6edc(wk, state);
}

// pokebw2's GSync_StateWaitFadeOut
void func_ov123_021d6ee8(GSyncWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov123_021d6ee0(wk, NULL, 343);
    }
}

// pokebw2's GSync_StateFadeOut
void func_ov123_021d6f04(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
        func_ov123_021d6ee0(wk, func_ov123_021d6ee8, 363);
    }
}

// pokebw2's GSync_StateWaitCleanup
void func_ov123_021d6f44(GSyncWork *wk) {
    if (func_ov123_021d5c0c(wk->download)) {
        func_ov123_021d5b70(wk->download);
        wk->download = NULL;
        func_ov123_021d6ee0(wk, func_ov123_021d706c, 380);
    }
}

void func_ov123_021d6f70(GSyncWork *wk) {
    if (wk->receiving) {
        return;
    }
    if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        if (wk->saving) {
            func_02012e08(wk->gameData);
            wk->saving = FALSE;
        }
        func_ov123_021d8f58(wk);
        if (wk->download != NULL) {
            if (func_ov123_021d5d58(wk->download)) {
                func_ov123_021d6ee0(wk, func_ov123_021d6f44, 401);
            }
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 403);
        } else {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 406);
        }
    }
}


// pokebw2's GSync_StateError
void func_ov123_021d6ff0(GSyncWork *wk) {
    int msgId = wk->error + 29;
    if (wk->http != NULL) {
        func_ov114_021b9774(wk->http);
    }
    if (wk->error == GSYNC_ERROR_DOWNLOAD) {
        msgId = 39;
    } else if (wk->error == GSYNC_ERROR_SERVICE_UNAVAILABLE) {
        msgId = 40;
    } else if (wk->error == GSYNC_ERROR_BAD_GATEWAY) {
        msgId = 41;
    } else if (wk->error == GSYNC_ERROR_HTTP) {
        msgId = 38;
    } else if (wk->error <= 0 || wk->error >= 10) {
        msgId = 38;
    }
    func_ov123_021d584c(wk->msg);
    func_ov123_021d5950(wk->msg, msgId);
    func_ov123_021d5960(wk->msg, 1, 16);
    func_ov123_021d6ee0(wk, func_ov123_021d6f70, 441);
}

// pokebw2's GSync_StateEnd
void func_ov123_021d706c(GSyncWork *wk) {
    wk->param->gsyncResult = GSYNC_RESULT_DONE;
    func_ov123_021d6ee0(wk, func_ov123_021d6f04, 457);
}

void func_ov123_021d7088(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 473);
        }
    }
}


// Shows what was downloaded: the C-Gear skin, the musical and the Pokédex skin
// pokebw2's GSync_StateShowDownloads
void func_ov123_021d70bc(GSyncWork *wk) {
    u32 msgIds[] = { 0, 24, 25, 27, 26, 29, 28, 23 };
    if (wk->downloadFlags) {
        func_ov123_021d584c(wk->msg);
        func_ov123_021d5950(wk->msg, msgIds[wk->downloadFlags]);
        func_ov123_021d5960(wk->msg, 1, 8);
        func_ov123_021d6ee0(wk, func_ov123_021d7088, 496);
    } else {
        func_ov123_021d6ee0(wk, func_ov123_021d706c, 499);
    }
}

void func_ov123_021d7130(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        wk->wait--;
        if (wk->wait <= 0) {
            if (wk->wait == 0) {
                GFL_SndBGMPop();
                GFL_SndBGMSetPaused(FALSE);
                GFL_SndBGMFadeIn(6);
            }
            if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
                func_ov123_021d6ee0(wk, func_ov123_021d70bc, 523);
            }
        }
    }
}

void func_ov123_021d7180(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            if (wk->levels == 0) {
                func_ov123_021d6ee0(wk, func_ov123_021d70bc, 535);
                return;
            }
            func_ov123_021d5a60(wk->msg);
            func_ov123_021d57d0(wk->msg, 22, wk->levels, wk->pkm);
            func_ov123_021d56fc(wk->msg, FALSE);
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            GFL_SndBGMPlay(SEQ_ME_LVUP, SND_CHANNEL_MASK_ALL);
            wk->wait = 180;
            func_ov123_021d6ee0(wk, func_ov123_021d7130, 546);
        }
    }
}


// pokebw2's GSync_OnPokemonAnimEnd
void func_ov123_021d7204(uintptr_t param, fx32 frame) {
    GSyncWork *wk = (GSyncWork *)param;

    func_ov123_021d62bc(wk->disp, 0, 0);
    func_ov123_021d62b4(wk->disp, 12);
    func_ov123_021d63d4(wk->disp);
}

void func_ov123_021d7224(GSyncWork *wk) {
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
    if (wk->pkm != NULL) {
        GFL_HeapFree(wk->pkm);
        wk->pkm = NULL;
    }
    if (BoxSaveAccessor_GetNextFreeBoxSlot(wk->boxes, &wk->boxTray, &wk->boxPos)) {
        PartyPkm *pkm = GFL_HeapAllocate(wk->heapId, PokeParty_GetPkmRawSize(), TRUE, "gsync_state.c", 581);
        u32 size = PokeParty_GetPkmRawSize();
        MI_CpuCopy8(func_02009290(dreamWorld), pkm, size);
        if (wk->levels != 0) {
            int level = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
            if (level == 100) {
                wk->levels = 0;
            }
            if (level + wk->levels > 100) {
                wk->levels = 100 - level;
                level = 100;
            } else {
                level = level + wk->levels;
            }
            setLevel(pkm, level);
        }
        BoxSaveAccessor_InsertPkm(wk->boxes, func_0201d624(pkm));
        wk->pkm = pkm;
        func_020092f8(dreamWorld, FALSE);
        func_020093d4(dreamWorld, wk->resultId);
    }
}

void func_ov123_021d72d8(GSyncWork *wk) {
    getDreamWorldStuffAddress(wk->save);
    if (func_02012dd0(wk->gameData) == 2) {
        wk->saving = FALSE;
        wk->receiving = FALSE;
        wk->busy = FALSE;
        func_ov123_021d584c(wk->msg);
        func_ov123_021d57d0(wk->msg, 45, 0, wk->pkm);
        func_ov123_021d5960(wk->msg, 1, 4);
        func_ov123_021d62b4(wk->disp, 0);
        func_ov123_021d62bc(wk->disp, 6, 7);
        func_ov123_021d62bc(wk->disp, 0, 1);
        ClActorCallback callback;
        callback.type = CLACT_CALLBACK_LAST_FRAME;
        callback.param = (uintptr_t)wk;
        callback.func = func_ov123_021d7204;
        func_ov123_021d62d0(wk->disp, 0, &callback);
        func_ov123_021d6650(wk->disp, FALSE);
        func_ov123_021d6458(wk->disp, func_020185f0(wk->pkm), 0);
        func_ov123_021d652c(wk->disp);
        GFL_SndSEPlay(SEQ_SE_SYS_25);
        func_ov123_021d6ee0(wk, func_ov123_021d7180, 661);
    }
}

void func_ov123_021d7390(GSyncWork *wk) {
    int status = func_ov114_021b99dc(wk->http);
    if (func_ov123_021d6e08(wk, status) == FALSE && func_ov114_021b9790(wk->http) == 0) {
        func_ov114_021b97f4(wk->http);
        getDreamWorldStuffAddress(wk->save);
        wk->busy = TRUE;
        func_ov123_021d6ee0(wk, func_ov123_021d72d8, 686);
    }
}

void func_ov123_021d73d8(GSyncWork *wk) {
    if (wk->wait != 0) {
        wk->wait--;
        return;
    }
    if (func_0203cd98() && func_ov114_021b95d4(8, wk->http)) {
        wk->request[0] = 1;
        wk->request[1] = 0;
        func_ov114_021be79c(func_ov114_021b973c(wk->http), wk->request, 4);
        if (func_ov114_021b9748(wk->http) == 0) {
            func_ov123_021d6ee0(wk, func_ov123_021d7390, 717);
        }
    }
}


// pokebw2's GSync_StateWaitReceiveSaveStart
void func_ov123_021d7430(GSyncWork *wk) {
    if (wk->progress < 100) {
        wk->progress++;
    }
    func_ov123_021d66d0(wk->disp, wk->progress);
    if (func_02012dd0(wk->gameData) == 1) {
        wk->wait = GFL_RandomLC(190);
        func_ov123_021d6ee0(wk, func_ov123_021d73d8, 742);
    }
}

void func_ov123_021d746c(GSyncWork *wk) {
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
    wk->receiving = TRUE;
    func_ov123_021d7224(wk);
    if (!func_ov123_021d6ebc(wk)) {
        func_ov123_021d7cb4(dreamWorld, wk->result);
        func_ov123_021d7de8(wk, dreamWorld, wk->result);
        func_ov123_021d7c54(dreamWorld, wk->result);
        func_020092d0(dreamWorld, wk->result->unk5A);
    } else {
        int i;
        GSyncAccountInfo *info = &wk->param->accountInfo;
        func_ov123_021d7d18(wk, dreamWorld, info->species, info->sex, info->form, info->unk4, GFL_RandomLC(8));
        for (i = 0; i < 20; i++) {
            func_02009348(dreamWorld, i, info->items[i], info->itemCounts[i]);
        }
    }
    wk->saving = TRUE;
    func_02012dac(wk->gameData);
    func_ov123_021d6ee0(wk, func_ov123_021d7430, 785);
}


// pokebw2's GSync_StateCleanupDownload
void func_ov123_021d751c(GSyncWork *wk) {
    if (func_ov123_021d5d58(wk->download)) {
        func_ov123_021d6ee0(wk, func_ov123_021d7858, 793);
        return;
    }
    wk->error = GSYNC_ERROR_DOWNLOAD;
    func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 797);
}

// pokebw2's GSync_StateCGearSaved
void func_ov123_021d7560(GSyncWork *wk) {
    wk->getCGear = FALSE;
    GFL_HeapFree(wk->cgearSkin);
    wk->busy = FALSE;
    wk->cgearSkin = NULL;
    func_ov123_021d6ee0(wk, func_ov123_021d751c, 808);
}

void func_ov123_021d758c(GSyncWork *wk) {
    SaveControl *save = GameData_GetSaveControl(wk->gameData);
    if (func_02012e7c(wk->gameData, 4) == 2) {
        freeIntermediateSaveExtraBlksAfterLoad2(save, 4);
        func_020090f8(func_02009210(save), TRUE);
        func_ov123_021d6ee0(wk, func_ov123_021d7560, 819);
    }
}


// pokebw2's GSync_StateCGearDownloaded
void func_ov123_021d75d0(GSyncWork *wk) {
    SaveControl *save = GameData_GetSaveControl(wk->gameData);
    u8 *buffer = func_ov123_021d5ba0(wk->download);
    u16 crc = getCRC16(buffer, GSYNC_CGEAR_SKIN_SIZE);
    u8 *skin;
    if (crc != *(u16 *)(buffer + GSYNC_CGEAR_SKIN_SIZE)) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 837);
        return;
    }
    wk->busy = TRUE;
    skin = GFL_HeapAllocate(wk->heapId, 0x3000, FALSE, "gsync_state.c", 842);
    wk->cgearSkin = skin;
    func_020072cc(save, 4, wk->heapId, skin, 0x3000);
    MI_CpuCopy8(buffer, skin, GSYNC_CGEAR_SKIN_SIZE);
    func_02009108(func_02009210(save), crc);
    func_02012e4c(wk->gameData, 4);
    func_ov123_021d6ee0(wk, func_ov123_021d758c, 853);
}

// pokebw2's GSync_StateZukanSaved
void func_ov123_021d768c(GSyncWork *wk) {
    wk->getZukan = FALSE;
    GFL_HeapFree(wk->zukanSkin);
    wk->busy = FALSE;
    wk->zukanSkin = NULL;
    func_ov123_021d6ee0(wk, func_ov123_021d751c, 864);
}

// pokebw2's GSync_StateWaitZukanSave
void func_ov123_021d76b8(GSyncWork *wk) {
    SaveControl *save = GameData_GetSaveControl(wk->gameData);

    if (func_02012e7c(wk->gameData, 7) == 2) {
        freeIntermediateSaveExtraBlksAfterLoad2(save, 7);
        func_ov123_021d6ee0(wk, func_ov123_021d768c, 874);
    }
}

void func_ov123_021d76f0(GSyncWork *wk) {
    SaveControl *save = GameData_GetSaveControl(wk->gameData);
    u8 *buffer = func_ov123_021d5ba0(wk->download);
    u16 crc = getCRC16(buffer, GSYNC_ZUKAN_SKIN_SIZE);
    if (crc != *(u16 *)(buffer + GSYNC_ZUKAN_SKIN_SIZE)) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 888);
        return;
    }
    wk->busy = TRUE;
    wk->zukanSkin = GFL_HeapAllocate(wk->heapId, 0x6800, FALSE, "gsync_state.c", 893);
    func_020072cc(save, 7, wk->heapId, wk->zukanSkin, 0x6800);
    MI_CpuCopy8(buffer, wk->zukanSkin, GSYNC_ZUKAN_SKIN_SIZE);
    func_0200e8e0(wk->zukanSkin, TRUE);
    func_02012e4c(wk->gameData, 7);
    func_ov123_021d6ee0(wk, func_ov123_021d76b8, 899);
}


// pokebw2's GSync_StateWaitMusicalImport
void func_ov123_021d7798(GSyncWork *wk) {
    if (func_0200c5f8(wk->musical)) {
        wk->busy = FALSE;
        wk->musical = NULL;
        func_ov123_021d6ee0(wk, func_ov123_021d751c, 910);
    }
}

// Checks the musical's CRC, after it at the end of the file, and imports it
// pokebw2's GSync_MusicalDownloaded
void func_ov123_021d77c4(GSyncWork *wk, int size) {
    u16 *buffer;
    int crcIndex;
    u16 crc;
    wk->getMusical = FALSE;
    buffer = func_ov123_021d5ba0(wk->download);
    crcIndex = size / 2;
    if (size > GSYNC_MUSICAL_SIZE_MAX) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 925);
        return;
    }
    crc = getCRC16(buffer, size);
    if (crc != buffer[crcIndex]) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 931);
        return;
    }
    wk->busy = TRUE;
    wk->musical = func_0200c5c8(wk->gameData, buffer, size, wk->heapId);
    func_ov123_021d6ee0(wk, func_ov123_021d7798, 937);
}

// pokebw2's GSync_StateWaitDownloadCleanup
void func_ov123_021d7858(GSyncWork *wk) {
    if (func_ov123_021d5c0c(wk->download)) {
        if (func_ov123_021d5c2c(wk->download)) {
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 950);
            return;
        }
        func_ov123_021d5b70(wk->download);
        wk->download = NULL;
        if (wk->progress < 100) {
            wk->progress += 10;
        }
        func_ov123_021d66d0(wk->disp, wk->progress);
        func_ov123_021d6ee0(wk, func_ov123_021d7bdc, 963);
    }
}

// pokebw2's GSync_StateDownloaded
void func_ov123_021d78c0(GSyncWork *wk) {
    switch (wk->downloadKind) {
    case GSYNC_DOWNLOAD_CGEAR:
        func_ov123_021d6ee0(wk, func_ov123_021d75d0, 971);
        break;
    case GSYNC_DOWNLOAD_MUSICAL:
        func_ov123_021d77c4(wk, func_ov123_021d5d8c(wk->download) - 2);
        break;
    case GSYNC_DOWNLOAD_ZUKAN:
        func_ov123_021d6ee0(wk, func_ov123_021d76f0, 977);
        break;
    }
}

// pokebw2's GSync_StateWaitFile
void func_ov123_021d7910(GSyncWork *wk) {
    if (func_ov123_021d5c2c(wk->download)) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 989);
        return;
    }
    if (func_ov123_021d5c0c(wk->download)) {
        func_ov123_021d5c9c(wk->download);
        func_ov123_021d6ee0(wk, func_ov123_021d78c0, 1000);
    }
}

// pokebw2's GSync_StateGetFile
void func_ov123_021d7960(GSyncWork *wk) {
    if (func_ov123_021d5d24(wk->download)) {
        func_ov123_021d6ee0(wk, func_ov123_021d7910, 1009);
        return;
    }
    wk->error = GSYNC_ERROR_DOWNLOAD;
    func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1013);
}

// pokebw2's GSync_StateWaitFileList
void func_ov123_021d79a4(GSyncWork *wk) {
    if (func_ov123_021d5c2c(wk->download)) {
        wk->error = GSYNC_ERROR_DOWNLOAD;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1022);
        return;
    }
    if (func_ov123_021d5c0c(wk->download)) {
        if (func_ov123_021d5d8c(wk->download) == 0) {
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1031);
            return;
        }
        func_ov123_021d5c9c(wk->download);
        func_ov123_021d6ee0(wk, func_ov123_021d7960, 1037);
    }
}

// pokebw2's GSync_StateGetFileList
void func_ov123_021d7a18(GSyncWork *wk) {
    if (func_ov123_021d5cf8(wk->download)) {
        func_ov123_021d6ee0(wk, func_ov123_021d79a4, 1045);
        return;
    }
    wk->error = GSYNC_ERROR_DOWNLOAD;
    func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1049);
}

// pokebw2's GSync_StateSetAttr
void func_ov123_021d7a5c(GSyncWork *wk) {
    if (func_ov123_021d5c0c(wk->download)) {
        if (func_ov123_021d5c2c(wk->download)) {
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1060);
            return;
        }
        switch (wk->downloadKind) {
        case GSYNC_DOWNLOAD_CGEAR:
            if (func_ov123_021d5c64(wk->download, "CGEAR_E", wk->getCGear)) {
                func_ov123_021d6ee0(wk, func_ov123_021d7a18, 1067);
                return;
            }
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1071);
            return;
        case GSYNC_DOWNLOAD_MUSICAL:
            if (func_ov123_021d5c64(wk->download, "MUSICAL_E", wk->getMusical)) {
                func_ov123_021d6ee0(wk, func_ov123_021d7a18, 1076);
                return;
            }
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1080);
            return;
        case GSYNC_DOWNLOAD_ZUKAN:
            if (func_ov123_021d5c64(wk->download, "ZUKAN_E", wk->getZukan)) {
                func_ov123_021d6ee0(wk, func_ov123_021d7a18, 1085);
                return;
            }
            wk->error = GSYNC_ERROR_DOWNLOAD;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1089);
            return;
        }
    }
}

// pokebw2's GSync_StateStartDownload
void func_ov123_021d7b68(GSyncWork *wk) {
    switch (wk->downloadKind) {
    case GSYNC_DOWNLOAD_CGEAR:
        wk->download = func_ov123_021d5b10(wk->heapId, func_0200c6e4() + 4);
        break;
    case GSYNC_DOWNLOAD_MUSICAL:
        wk->download = func_ov123_021d5b10(wk->heapId, 0x20000);
        break;
    case GSYNC_DOWNLOAD_ZUKAN:
        wk->download = func_ov123_021d5b10(wk->heapId, func_0200e8b0() + 4);
        break;
    }
    if (func_ov123_021d5bd4(wk->download)) {
        func_ov123_021d6ee0(wk, func_ov123_021d7a5c, 1112);
        return;
    }
    wk->error = GSYNC_ERROR_DOWNLOAD;
    func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1116);
}

// Downloads the next of the files the Dream World sent, or takes the Pokémon back once they are done
// pokebw2's GSync_StateNextDownload
void func_ov123_021d7bdc(GSyncWork *wk) {
    if (wk->getCGear) {
        wk->downloadKind = GSYNC_DOWNLOAD_CGEAR;
        func_ov123_021d6ee0(wk, func_ov123_021d7b68, 1125);
    } else if (wk->getMusical) {
        wk->downloadKind = GSYNC_DOWNLOAD_MUSICAL;
        func_ov123_021d6ee0(wk, func_ov123_021d7b68, 1130);
    } else if (wk->getZukan) {
        wk->downloadKind = GSYNC_DOWNLOAD_ZUKAN;
        func_ov123_021d6ee0(wk, func_ov123_021d7b68, 1135);
    } else {
        func_ov123_021d6ee0(wk, func_ov123_021d746c, 1138);
    }
}

// Stores the result's entries, if they changed
// pokebw2's GSync_SetEntries
void func_ov123_021d7c54(DreamWorldSave *dreamWorld, GSyncResult *result) {
    BOOL changed = FALSE;
    int i;

    for (i = 0; i < 5; i++) {
        if (GFL_STD_MemCmp(func_02009390(dreamWorld, i), result->entries[i], sizeof(result->entries[i])) != 0) {
            changed = TRUE;
            break;
        }
    }
    if (changed) {
        for (i = 0; i < 5; i++) {
            func_020093a8(dreamWorld, i, result->entries[i]);
        }
        func_020093f0(dreamWorld, 127);
    }
}

// pokebw2's GSync_SetItems
BOOL func_ov123_021d7cb4(DreamWorldSave *dreamWorld, GSyncResult *result) {
    BOOL any = FALSE;
    int i;

    for (i = 0; i < 20; i++) {
        u16 item = result->items[i];

        if (item != ITEM_NONE && item <= GSYNC_ITEM_MAX) {
            func_02009348(dreamWorld, i, item, result->itemCounts[i]);
            any = TRUE;
        }
    }
    return any;
}

// pokebw2's GSync_HasItems
BOOL func_ov123_021d7cec(DreamWorldSave *dreamWorld, GSyncResult *result) {
    BOOL any = FALSE;
    int i;

    for (i = 0; i < 20; i++) {
        u16 item = result->items[i];

        if (item != ITEM_NONE && item <= GSYNC_ITEM_MAX) {
            any = TRUE;
        }
    }
    return any;
}

// Puts a Pokémon from the Dream World in the Entree Forest, if it has an overworld model
void func_ov123_021d7d18(GSyncWork *wk, DreamWorldSave *dreamWorld, int species, int sex, int form, int unk2,
                         int slot) {
    u8 forestSlot = slot;
    if (species != SPECIES_NONE && species <= SPECIES_ARCEUS) {
        TPokeData *data = LoadTPokeData(wk->heapId);
        int size = 3;
        u8 sanitized = PML_PkmSanitizeForme(species, form);
        u32 sexRatio = PML_PersonalGetParamSingle(species, sanitized, PERSONAL_SEX_RATIO);
        u8 modelForm = func_02019d58(species, sanitized);
        switch (sexRatio) {
        case 0:
            sex = 0;
            break;
        case 254:
            sex = 1;
            break;
        case 255:
            sex = 2;
            break;
        default:
            if (sex > 1) {
                sex = GFL_RandomLC(2);
            }
            break;
        }
        if (IsFieldPokemonSpriteHugeBillboard(wk->gameData, data, species, sex, modelForm)) {
            size = 2;
        }
        if (forestSlot > 8) {
            forestSlot = 0;
        }
        func_0200e1a8(getAreaNPCData(wk->save), species, unk2, sex, modelForm, forestSlot, size);
        FreeTPokeData(data);
    }
}

// pokebw2's GSync_AddForestPokemonList
void func_ov123_021d7de8(GSyncWork *wk, DreamWorldSave *dreamWorld, GSyncResult *result) {
    int i;

    for (i = 0; i < 10; i++) {
        if (result->pokemon[i].species == SPECIES_NONE) {
            break;
        }
        func_ov123_021d7d18(wk, dreamWorld, result->pokemon[i].species, result->pokemon[i].sex,
                               result->pokemon[i].form, result->pokemon[i].unk2, result->pokemon[i].slot);
    }
}

// pokebw2's GSync_SetFloatItems
void func_ov123_021d7e20(GSyncWork *wk, GSyncResult *result) {
    int i;

    for (i = 0; i < 20; i++) {
        u16 item = result->items[i];

        if (item != ITEM_NONE && result->itemCounts[i] != 0 && item <= GSYNC_ITEM_MAX) {
            func_ov123_021d6aa8(wk->disp, i, item);
        }
    }
}

void func_ov123_021d7e58(GSyncWork *wk, DreamWorldSave *dreamWorld, GSyncResult *result, GSyncResponse *response) {
    int i;
    if (response->status != 0) {
        wk->error = response->status;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1280);
        return;
    }
    if (!result->unk5B && result->id != func_020093d0(dreamWorld)) {
        wk->resultId = result->id;
        wk->getMusical = result->musical;
        wk->getCGear = result->cgear;
        wk->getZukan = result->zukan;
        wk->levels = result->levels;
        if (wk->getCGear) {
            wk->downloadFlags |= 1;
        }
        if (wk->getMusical) {
            wk->downloadFlags |= 2;
        }
        if (wk->getZukan) {
            wk->downloadFlags |= 4;
        }
        for (i = 0; i < 10; i++) {
            if (result->pokemon[i].species == SPECIES_NONE) {
                break;
            }
            func_ov123_021d6a70(wk->disp, i + GSYNC_DISP_FLOAT_ITEM_COUNT, result->pokemon[i].species,
                                result->pokemon[i].form, result->pokemon[i].sex);
        }
        wk->hasItems = func_ov123_021d7cec(dreamWorld, result);
        func_ov123_021d7e20(wk, result);
        func_ov123_021d6ab8(wk->disp);
    } else {
        wk->hasItems = FALSE;
    }
    func_ov123_021d7bdc(wk);
}


// pokebw2's GSync_AreAccountItemsUnique
BOOL func_ov123_021d7f5c(GSyncWork *wk) {
    int i;
    int j;
    GSyncAccountInfo *info;

    getDreamWorldStuffAddress(wk->save);
    info = &wk->param->accountInfo;
    for (i = 0; i < 20; i++) {
        for (j = i + 1; j < 20; j++) {
            if (info->items[i] != ITEM_NONE && info->items[j] == info->items[i]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void func_ov123_021d7f9c(GSyncWork *wk) {
    if (func_ov123_021d6e70(wk) && !func_ov123_021d6ebc(wk)) {
        if (wk->progress < 30) {
            wk->progress++;
        }
        func_ov123_021d66d0(wk->disp, wk->progress);
        if (func_ov123_021d6e08(wk, func_ov114_021b99dc(wk->http)) == FALSE && func_ov114_021b9790(wk->http) == 0) {
            GSyncResponse *response = func_ov114_021b97f4(wk->http);
            DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
            GSyncResult *result = &response->body.result;
            wk->result = GFL_HeapAllocate(wk->heapId, sizeof(GSyncResult), TRUE, "gsync_state.c", 1395);
            MI_CpuCopy8(result, wk->result, sizeof(GSyncResult));
            func_ov123_021d7e58(wk, dreamWorld, result, response);
        }
    } else {
        wk->frames++;
        if (wk->progress < 30) {
            wk->progress++;
        }
        if (wk->frames == 360) {
            func_ov123_021d5950(wk->msg, 43);
            func_ov123_021d59e4(wk->msg);
        } else if (wk->frames == 720) {
            func_ov123_021d5950(wk->msg, 44);
            func_ov123_021d59e4(wk->msg);
        } else if (wk->frames == 1080) {
            if (func_ov123_021d6ea4(wk)) {
                func_ov123_021d5a60(wk->msg);
            }
            if (func_ov123_021d7f5c(wk)) {
                func_ov123_021d6ee0(wk, func_ov123_021d746c, 1420);
            } else {
                wk->error = 9;
                func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1425);
            }
        }
        func_ov123_021d66d0(wk->disp, wk->progress);
    }
}


void func_ov123_021d80b8(GSyncWork *wk) {
    if (func_ov123_021d6e70(wk) && !func_ov123_021d6ebc(wk)) {
        if (func_ov114_021b95d4(2, wk->http) && func_ov114_021b9748(wk->http) == 0) {
            func_ov123_021d6ee0(wk, func_ov123_021d7f9c, 1437);
        }
    } else {
        if (func_ov123_021d6ea4(wk)) {
            wk->frames = 0;
            func_ov123_021d5950(wk->msg, 42);
            func_ov123_021d59e4(wk->msg);
        } else {
            wk->frames = 1070;
        }
        func_ov123_021d6ee0(wk, func_ov123_021d7f9c, 1450);
    }
}


// pokebw2's GSync_StateWaitWakeMessage
void func_ov123_021d8130(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d6ee0(wk, func_ov123_021d80b8, 1459);
    }
}

void func_ov123_021d8154(GSyncWork *wk) {
    func_ov123_021d57b8(wk->msg, 11);
    func_ov123_021d5a7c(wk->msg);
    func_ov123_021d62e0(wk->disp, 8);
    func_ov123_021d65f4(wk->disp);
    func_ov123_021d6650(wk->disp, TRUE);
    func_ov123_021d62bc(wk->disp, 6, 5);
    func_ov123_021d62b4(wk->disp, 0);
    func_ov123_021d62b4(wk->disp, 12);
    GFL_SndSEPlay(SEQ_SE_SYS_24);
    func_ov123_021d6ee0(wk, func_ov123_021d8130, 1475);
}


void func_ov123_021d81b4(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 1488);
        }
    }
}


// pokebw2's GSync_StateTooSoon
void func_ov123_021d81e4(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d57b8(wk->msg, 16);
        func_ov123_021d6ee0(wk, func_ov123_021d81b4, 1502);
    }
}

void func_ov123_021d8210(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 1513);
        }
    }
}


// pokebw2's GSync_StateNoRoom
void func_ov123_021d8244(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d57b8(wk->msg, 49);
        func_ov123_021d6ee0(wk, func_ov123_021d8210, 1526);
    }
}

// Whether to wake the Pokémon, with the sound of its sleep
// pokebw2's GSync_StateWakeAnswer
void func_ov123_021d8270(GSyncWork *wk) {
    if (wk->seTimer % 110 == 0) {
        GFL_SndSEPlay(SEQ_SE_SYS_26);
    }
    wk->seTimer++;
    if (func_020279e8(wk->yesNo)) {
        if (func_02027a04(wk->yesNo) == 0) {
            if (!BoxSaveAccessor_GetNextFreeBoxSlot(wk->boxes, &wk->boxTray, &wk->boxPos)) {
                func_ov123_021d6ee0(wk, func_ov123_021d8244, 1551);
            } else {
                func_ov123_021d6ee0(wk, func_ov123_021d8154, 1554);
            }
        } else {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 1558);
        }
        func_ov123_021d584c(wk->msg);
        func_02027858(wk->yesNo);
        reg_G2S_DB_BLDCNT = 0;
        wk->yesNo = NULL;
    }
}

// pokebw2's GSync_StateWakeAsk
void func_ov123_021d8304(GSyncWork *wk) {
    if (wk->seTimer % 110 == 0) {
        GFL_SndSEPlay(SEQ_SE_SYS_26);
    }
    wk->seTimer++;
    if (func_ov123_021d580c(wk->msg)) {
        wk->yesNo = func_ov123_021d5890(wk->msg, GSYNC_YESNO_POS_UPPER);
        func_ov123_021d6ee0(wk, func_ov123_021d8270, 1586);
    }
}

// pokebw2's GSync_StateWakeShow
void func_ov123_021d834c(GSyncWork *wk) {
    func_ov123_021d584c(wk->msg);
    func_ov123_021d62b4(wk->disp, 0);
    func_ov123_021d62b4(wk->disp, 12);
    func_ov123_021d62b4(wk->disp, 6);
    func_ov123_021d62b4(wk->disp, 8);
    if (func_02009370(getDreamWorldStuffAddress(wk->save)) != 0) {
        func_ov123_021d57b8(wk->msg, 21);
    } else {
        func_ov123_021d57b8(wk->msg, 10);
    }
    func_ov123_021d6ee0(wk, func_ov123_021d8304, 1612);
}

// pokebw2's GSync_StateWaitAccountMessage
void func_ov123_021d83a4(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d6ee0(wk, func_ov123_021d834c, 1677);
    }
}

void func_ov123_021d83c8(GSyncWork *wk) {
    if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d5a60(wk->msg);
        func_020092e0(getDreamWorldStuffAddress(wk->save), TRUE);
        wk->idShown = TRUE;
        func_ov123_021d6ee0(wk, func_ov123_021d8958, 1698);
    }
}

void func_ov123_021d840c(GSyncWork *wk) {
    if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d5950(wk->msg, 48);
        func_ov123_021d5960(wk->msg, 7, 14);
        func_ov123_021d6ee0(wk, func_ov123_021d83c8, 1707);
    }
}


// Shows the Game Sync ID
// pokebw2's GSync_StateShowId
void func_ov123_021d8448(GSyncWork *wk) {
    func_ov123_021d5ab0(wk->msg, func_0200853c(GetGameDataPlayerInfo(wk->param->gameData)));
    func_ov123_021d56fc(wk->msg, TRUE);
    func_ov123_021d5950(wk->msg, 47);
    func_ov123_021d5960(wk->msg, 7, 14);
    func_ov123_021d6ee0(wk, func_ov123_021d840c, 1719);
}

void func_ov123_021d8490(GSyncWork *wk) {
    GSyncResponse *response;
    if (func_0203cd98()) {
        if (func_ov123_021d6e08(wk, func_ov114_021b99dc(wk->http)) == FALSE && func_ov114_021b9790(wk->http) == 0) {
            response = func_ov114_021b97f4(wk->http);
            if (response->status == 2) {
                func_ov123_021d6ee0(wk, func_ov123_021d8958, 1748);
            } else if (response->status == 0) {
                func_ov123_021d584c(wk->msg);
                func_ov123_021d6ee0(wk, func_ov123_021d8448, 1752);
            } else {
                wk->error = response->status;
                func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1756);
            }
        }
    } else {
        func_ov123_021d6ee0(wk, func_ov123_021d8958, 1762);
    }
}

void func_ov123_021d8520(GSyncWork *wk) {
    if (func_0203cd98()) {
        if (func_ov114_021b95d4(9, wk->http)) {
            u32 size;
            void *data = func_020071d8(wk->save, &size);
            func_ov114_021be79c(func_ov114_021b973c(wk->http), data, 0x80000);
            if (func_ov114_021b9748(wk->http) == 0) {
                func_ov123_021d6ee0(wk, func_ov123_021d8490, 1786);
            }
        }
    } else if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d6ee0(wk, func_ov123_021d8490, 1792);
    }
}


void func_ov123_021d8590(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1831);
        }
    }
}


// pokebw2's GSync_StateNotDreaming
void func_ov123_021d85c4(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d57b8(wk->msg, 17);
        func_ov123_021d6ee0(wk, func_ov123_021d8590, 1849);
    }
}

void func_ov123_021d85f0(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            func_ov123_021d6ee0(wk, func_ov123_021d706c, 1862);
        }
    }
}


// pokebw2's GSync_StateDreamingNow
void func_ov123_021d8624(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d57b8(wk->msg, 20);
        func_ov123_021d6ee0(wk, func_ov123_021d85f0, 1880);
    }
}

// Goes on by the account's state and whether this proc sends or receives
// pokebw2's GSync_HandleAccount
void func_ov123_021d8650(GSyncWork *wk, u16 status, GSyncResponse *response) {
    switch (response->status) {
    case 0:
        switch (wk->param->gsyncResult) {
        case GSYNC_RESULT_NONE:
            if (status == 0) {
                func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1908);
            } else if (status == 3) {
                func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1911);
            } else if (status == 1) {
                func_ov123_021d6ee0(wk, func_ov123_021d85c4, 1915);
            } else if (status == 2) {
                func_ov123_021d6ee0(wk, func_ov123_021d8624, 1919);
            } else if (status == 4) {
                func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1922);
            } else {
                func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1927);
            }
            break;
        case GSYNC_RESULT_WIFI_SETTINGS:
            if (!wk->idShown) {
                func_ov123_021d6ee0(wk, func_ov123_021d8448, 1932);
            } else {
                func_ov123_021d6ee0(wk, func_ov123_021d8958, 1935);
            }
            break;
        }
        break;
    case 5:
        if (wk->param->gsyncResult == GSYNC_RESULT_WIFI_SETTINGS) {
            func_ov123_021d6ee0(wk, func_ov123_021d8958, 1943);
        } else {
            wk->error = response->status;
            func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1947);
        }
        break;
    case 8:
        switch (wk->param->gsyncResult) {
        case GSYNC_RESULT_NONE:
            func_ov123_021d6ee0(wk, func_ov123_021d83a4, 1954);
            break;
        case GSYNC_RESULT_WIFI_SETTINGS:
            func_ov123_021d6ee0(wk, func_ov123_021d8520, 1958);
            break;
        }
        break;
    default:
        wk->error = response->status;
        func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 1964);
        break;
    }
}

void func_ov123_021d8790(GSyncWork *wk) {
    if (func_0203cd98()) {
        if (func_ov123_021d6e08(wk, func_ov114_021b99dc(wk->http)) == FALSE && func_ov114_021b9790(wk->http) == 0) {
            GSyncResponse *response = func_ov114_021b97f4(wk->http);
            MI_CpuCopy8(&response->body.accountInfo, &wk->param->accountInfo, sizeof(GSyncAccountInfo));
            wk->param->hasAccountInfo = TRUE;
            func_ov123_021d8650(wk, response->body.accountInfo.status, response);
        }
    } else {
        func_ov123_021d6ee0(wk, func_ov123_021d83a4, 2004);
    }
}

void func_ov123_021d87f8(GSyncWork *wk) {
    func_ov123_021d57b8(wk->msg, 13);
    func_ov123_021d5a7c(wk->msg);
    if (func_0203cd98()) {
        if (func_ov114_021b95d4(0, wk->http)) {
            if (func_ov114_021b9748(wk->http) == 0) {
                func_ov123_021d6ee0(wk, func_ov123_021d8790, 2026);
            }
        }
    } else if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d6ee0(wk, func_ov123_021d8790, 2033);
    }
}

void func_ov123_021d885c(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
            wk->param->gsyncResult = GSYNC_RESULT_ACCOUNT;
            func_ov123_021d584c(wk->msg);
            func_ov123_021d6ee0(wk, func_ov123_021d6f04, 2056);
        }
    }
}


// pokebw2's GSync_StateNoPokemon
void func_ov123_021d88a0(GSyncWork *wk) {
    func_ov123_021d57b8(wk->msg, 19);
    func_ov123_021d6ee0(wk, func_ov123_021d885c, 2071);
}

void func_ov123_021d88c0(GSyncWork *wk) {
    if (func_0203cd98()) {
        if (func_ov123_021d6e08(wk, func_ov114_021b99dc(wk->http)) == FALSE && func_ov114_021b9790(wk->http) == 0) {
            GSyncResponse *response = func_ov114_021b97f4(wk->http);
            if (response->status == 0 || response->status == 8) {
                MI_CpuCopy8(&response->body.sendableSpecies, wk->param->sendableSpecies, sizeof(wk->param->sendableSpecies));
                wk->param->gsyncResult = GSYNC_RESULT_SELECT_POKEMON;
                func_ov123_021d6ee0(wk, func_ov123_021d6f04, 2103);
            } else {
                wk->error = response->status;
                func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 2114);
            }
        }
    } else {
        wk->param->gsyncResult = GSYNC_RESULT_SELECT_POKEMON;
        func_ov123_021d6ee0(wk, func_ov123_021d6f04, 2121);
    }
}

void func_ov123_021d8958(GSyncWork *wk) {
    RTCDate last;
    RTCDate today;
    u32 date;
    int lastKey;
    if (hasFullDayMinutesPassed(getSaveAdventureTimeBlock(wk->save)) && wk->idShownSaved) {
        func_ov123_021d6ee0(wk, func_ov123_021d81e4, 2140);
        return;
    }
    date = func_020093c8(getDreamWorldStuffAddress(wk->save));
    last.year = (date >> 24) & 0xff;
    last.month = (date >> 16) & 0xff;
    last.day = (date >> 8) & 0xff;
    last.week = date & 0xff;
    RTC_GetCachedDate(&today);
    lastKey = func_0203f540(&last);
    if (lastKey == func_0203f540(&today)) {
        func_ov123_021d6ee0(wk, func_ov123_021d81e4, 2153);
        return;
    }
    if (func_0203cd98()) {
        if (func_ov114_021b95d4(1, wk->http) && func_ov114_021b9748(wk->http) == 0) {
            func_ov123_021d6ee0(wk, func_ov123_021d88c0, 2161);
        }
    } else if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d6ee0(wk, func_ov123_021d88c0, 2168);
    }
}


void func_ov123_021d8a38(GSyncWork *wk) {
    if (wk->seTimer % 110 == 0) {
        GFL_SndSEPlay(SEQ_SE_SYS_26);
    }
    wk->seTimer++;
    if (GCTX_HIDGetPressedKeys() != 0 || func_02035c60()) {
        func_ov123_021d6ee0(wk, func_ov123_021d706c, 2190);
    }
}


// pokebw2's GSync_StateWaitSentText
void func_ov123_021d8a7c(GSyncWork *wk) {
    if (func_ov123_021d580c(wk->msg)) {
        func_ov123_021d6ee0(wk, func_ov123_021d8a38, 2208);
    }
}

void func_ov123_021d8a9c(GSyncWork *wk) {
    if (wk->unk44 != NULL) {
        GFL_HeapFree(wk->unk44);
        wk->unk44 = NULL;
    }
    wk->progress = 100;
    func_ov123_021d66d0(wk->disp, wk->progress);
    func_ov123_021d57d0(wk->msg, 14, 0, wk->pkm);
    func_ov123_021d56fc(wk->msg, FALSE);
    func_ov123_021d62b4(wk->disp, 8);
    func_ov123_021d6650(wk->disp, FALSE);
    func_ov123_021d62b4(wk->disp, 0);
    func_ov123_021d62bc(wk->disp, 4, 6);
    GFL_SndSEPlay(SEQ_SE_SYS_25);
    func_ov123_021d6ee0(wk, func_ov123_021d8a7c, 2237);
}


// pokebw2's GSync_StateWaitSendSave
void func_ov123_021d8b0c(GSyncWork *wk) {
    if (wk->progress < 100) {
        wk->progress++;
    }
    func_ov123_021d66d0(wk->disp, wk->progress);
    switch (func_02012dd0(wk->gameData)) {
    case 1:
        if (wk->dreamWorldCopy != NULL) {
            GFL_HeapFree(wk->dreamWorldCopy);
            wk->dreamWorldCopy = NULL;
        }
        break;
    case 3:
        func_0203c9ac(0, 0, 0, 1011);
        func_0203c938(0);
        break;
    case 2:
        wk->saving = FALSE;
        func_ov123_021d6ee0(wk, func_ov123_021d8a9c, 2278);
        break;
    }
}

void func_ov123_021d8b7c(GSyncWork *wk) {
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
    RTCDate date;
    wk->pkmTaken = FALSE;
    RTC_GetCachedDate(&date);
    func_020093cc(dreamWorld, (date.year << 24) | ((date.month & 0xff) << 16) | ((date.day & 0xff) << 8) | date.week);
    if (func_020093d8(dreamWorld) != 0x7f) {
        func_02009428(dreamWorld, 1);
        func_020093f0(dreamWorld, 0x7e);
    }
    wk->progress = 50;
    RecordAddOne(GameData_GetRecords(wk->gameData), 119);
    wk->saving = TRUE;
    func_02012dac(wk->gameData);
    func_ov123_021d6ee0(wk, func_ov123_021d8b0c, 2324);
}

void func_ov123_021d8c04(GSyncWork *wk) {
    getDreamWorldStuffAddress(wk->save);
    func_ov123_021d6934(wk->disp);
    if (func_ov123_021d6ea4(wk)) {
        if (wk->frames == 0) {
            func_ov123_021d5950(wk->msg, 42);
            func_ov123_021d59e4(wk->msg);
        } else if (wk->frames == 360) {
            func_ov123_021d5950(wk->msg, 43);
            func_ov123_021d59e4(wk->msg);
        } else if (wk->frames == 720) {
            func_ov123_021d5950(wk->msg, 44);
            func_ov123_021d59e4(wk->msg);
        }
        wk->frames++;
    }
    if (func_ov123_021d6e70(wk)) {
        if (wk->progress < 50) {
            wk->progress++;
        }
        func_ov123_021d66d0(wk->disp, wk->progress);
        if (func_ov123_021d6e08(wk, func_ov114_021b99dc(wk->http)) == FALSE && func_ov114_021b9790(wk->http) == 0) {
            GSyncResponse *response = func_ov114_021b97f4(wk->http);
            if (func_ov123_021d6ea4(wk)) {
                func_ov123_021d5a60(wk->msg);
            }
            if (response->status == 0) {
                func_ov123_021d6ee0(wk, func_ov123_021d8b7c, 2389);
            } else {
                wk->error = response->status;
                func_ov123_021d6ee0(wk, func_ov123_021d6ff0, 2393);
            }
        }
    } else {
        func_ov123_021d66d0(wk->disp, wk->idleProgress);
        if (++wk->idleProgress == 100) {
            if (func_ov123_021d6ea4(wk)) {
                func_ov123_021d5a60(wk->msg);
            }
            func_ov123_021d6ee0(wk, func_ov123_021d8b7c, 2407);
        }
    }
}


void func_ov123_021d8d08(GSyncWork *wk) {
    u32 size;
    void *data;
    getDreamWorldStuffAddress(wk->save);
    if (func_ov123_021d580c(wk->msg)) {
        wk->frames = 0;
        if (func_ov123_021d6e70(wk)) {
            if (func_ov114_021b95d4(3, wk->http)) {
                data = func_020071d8(wk->save, &size);
                func_ov114_021be79c(func_ov114_021b973c(wk->http), data, 0x80000);
                func_ov123_021d66d0(wk->disp, 0);
                wk->progress = 0;
                if (func_ov114_021b9748(wk->http) == 0) {
                    func_ov123_021d6ee0(wk, func_ov123_021d8c04, 2438);
                }
            }
        } else {
            func_ov123_021d66d0(wk->disp, 0);
            func_ov123_021d6ee0(wk, func_ov123_021d8c04, 2444);
        }
    }
}


void func_ov123_021d8d98(GSyncWork *wk) {
    if (--wk->timer == 0) {
        func_ov123_021d65f4(wk->disp);
        func_ov123_021d57b8(wk->msg, 8);
        func_ov123_021d5a7c(wk->msg);
        func_ov123_021d6650(wk->disp, TRUE);
        func_ov123_021d62bc(wk->disp, 4, 5);
        func_ov123_021d62bc(wk->disp, 0, 0);
        func_ov123_021d62b4(wk->disp, 12);
        GFL_SndSEPlay(SEQ_SE_SYS_24);
        func_ov123_021d6ee0(wk, func_ov123_021d8d08, 2472);
    }
}


// pokebw2's GSync_StateSendAnimLeave
void func_ov123_021d8dfc(GSyncWork *wk) {
    if (--wk->timer == 0) {
        func_ov123_021d62b4(wk->disp, 4);
        wk->timer = 60;
        func_ov123_021d6ee0(wk, func_ov123_021d8d98, 2489);
    }
}

// pokebw2's GSync_StateSendAnimJump
void func_ov123_021d8e28(GSyncWork *wk) {
    if (--wk->timer == 0) {
        func_ov123_021d62bc(wk->disp, 0, 1);
        func_ov123_021d62b4(wk->disp, 2);
        func_ov123_021d62b4(wk->disp, 3);
        wk->timer = 15;
        func_ov123_021d6ee0(wk, func_ov123_021d8dfc, 2511);
    }
}

// pokebw2's GSync_StateSendAnimIcon
void func_ov123_021d8e68(GSyncWork *wk) {
    if (--wk->timer == 0) {
        func_ov123_021d62a8(wk->disp);
        func_ov123_021d651c(wk->disp);
        wk->timer = 70;
        func_ov123_021d6ee0(wk, func_ov123_021d8e28, 2532);
    }
}

// pokebw2's GSync_StateSendShowIcon
void func_ov123_021d8e98(GSyncWork *wk) {
    func_ov123_021d6458(wk->disp, func_020185f0(wk->pkm), CLACT_SURFACE_SUB);
    wk->timer = 10;
    func_ov123_021d6ee0(wk, func_ov123_021d8e68, 2550);
}

// Takes the Pokémon chosen out of its box into the Dream World save
// pokebw2's GSync_StateSend
void func_ov123_021d8ec4(GSyncWork *wk) {
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
    BoxPkm *boxPkm;
    if (wk->pkm != NULL) {
        GFL_HeapFree(wk->pkm);
        wk->pkm = NULL;
    }
    boxPkm = BoxSaveAccessor_GetPkm(wk->boxes, wk->boxTray, wk->boxPos);
    if (boxPkm != NULL) {
        PartyPkm *pkm = boxPkmRegenToPartyPkm(boxPkm, wk->heapId);
        func_02009294(dreamWorld, pkm);
        wk->pkm = pkm;
        BoxSaveAccessor_ClearPkm(wk->boxes, wk->boxTray, wk->boxPos);
        func_020092f8(dreamWorld, TRUE);
        wk->pkmTaken = TRUE;
    }
    func_ov123_021d62b4(wk->disp, 0);
    func_ov123_021d62b4(wk->disp, 12);
    func_ov123_021d6ee0(wk, func_ov123_021d8e98, 2602);
}

// pokebw2's GSync_OnDisconnect
void func_ov123_021d8f3c(void *work, int a1, int code, int error) {
    GSyncWork *wk = work;

    if (wk->http != NULL) {
        func_ov114_021b9774(wk->http);
        func_ov114_021b9838(wk->http);
        wk->http = NULL;
    }
}

void func_ov123_021d8f58(GSyncWork *wk) {
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(wk->save);
    if (wk->dreamWorldCopy != NULL) {
        MI_CpuCopy8(wk->dreamWorldCopy, dreamWorld, func_02009930());
        GFL_HeapFree(wk->dreamWorldCopy);
        wk->dreamWorldCopy = NULL;
    }
    if (wk->pkmTaken) {
        PartyPkm *pkm = func_02009290(dreamWorld);
        BoxSaveAccessor_SetPkm(wk->boxes, wk->boxTray, wk->boxPos, func_0201d624(pkm));
        PokeParty_ClearPkm(pkm);
        func_020092f8(dreamWorld, FALSE);
        wk->pkmTaken = FALSE;
    }
}

BOOL func_ov123_021d8fbc(GameProc *proc, u32 *state, void *param, void *work) {
    EventGameSync *pParent = param;
    GSyncWork *wk;
    s32 profileID;

    GFL_OvlLoad(OVERLAY_ID(114));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GSYNC, 0x88000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(GSyncWork), HEAPID_GSYNC);
    MI_CpuFill8(wk, 0, sizeof(GSyncWork));
    wk->heapId = HEAPID_GSYNC;
    GFL_ASSERT(pParent);
    if (pParent != NULL) {
        wk->param = pParent;
        wk->gameData = pParent->gameData;
        wk->save = GameData_GetSaveControl(pParent->gameData);
        profileID = func_0200853c(GetGameDataPlayerInfo(pParent->gameData));
        wk->boxes = GameData_GetBoxSaveAccessor(pParent->gameData);
        wk->boxTray = pParent->boxTray;
        wk->boxPos = pParent->boxPosition;
        wk->idShownSaved = func_020092ec(getDreamWorldStuffAddress(wk->save));
        wk->idShown = wk->idShownSaved;
        getDreamWorldStuffAddress(wk->save);
        switch (pParent->gsyncResult) {
        case GSYNC_RESULT_NONE:
            GFL_ASSERT(profileID);
            func_ov123_021d6ee0(wk, func_ov123_021d87f8, 2697);
            break;
        case GSYNC_RESULT_ACCOUNT:
            GFL_ASSERT(profileID);
            func_ov123_021d6ee0(wk, func_ov123_021d8ec4, 2701);
            break;
        case GSYNC_RESULT_CONNECT:
            GFL_ASSERT(profileID);
            func_ov123_021d6ee0(wk, func_ov123_021d8ec4, 2705);
            break;
        case GSYNC_RESULT_WIFI_SETTINGS:
            GFL_ASSERT(profileID);
            func_ov123_021d6ee0(wk, func_ov123_021d87f8, 2709);
            break;
        case GSYNC_RESULT_NO_POKEMON:
            func_ov123_021d6ee0(wk, func_ov123_021d88a0, 2712);
            break;
        }
        if (pParent->gsyncResult != GSYNC_RESULT_NO_POKEMON) {
            wk->http = func_ov114_021b9800(HEAPID_GSYNC, profileID, pParent->loginBuffer);
            func_ov009_02156dc4(func_ov123_021d8f3c, wk);
        }
    }
    wk->disp = func_ov123_021d5d94(wk->heapId);
    wk->msg = func_ov123_021d54f4(wk->heapId, 39);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
    func_0203d34c(FALSE, wk->heapId);
    return TRUE;
}


// pokebw2's GSync_ProcMain
BOOL func_ov123_021d9158(GameProc *proc, u32 *state, void *param, void *work) {
    GSyncWork *wk = work;
    GSyncStateFunc func = wk->state;
    BOOL done = TRUE;

    if (GFL_WipeIsFinished() && !wk->busy && func_0200fb24()) {
        if (wk->download != NULL) {
            func_ov123_021d5d58(wk->download);
        }
        func_ov114_021b9774(wk->http);
        if (wk->saving) {
            func_02012e08(wk->gameData);
            wk->saving = FALSE;
        }
        func_ov123_021d8f58(wk);
        if (wk->receiving) {
            func_0200f7c0(41);
        }
        func_0202223c(0, WIPE_COLOR_BLACK);
        func_0202223c(1, WIPE_COLOR_BLACK);
        func_ov009_02157170(1, 0);
        wk->param->gsyncResult = GSYNC_RESULT_RETRY_LOGIN;
        return TRUE;
    }
    if (func != NULL) {
        func(wk);
        done = FALSE;
    }
    if (wk->yesNo != NULL) {
        func_02027974(wk->yesNo);
    }
    if (wk->download != NULL) {
        func_ov123_021d5d7c(wk->download);
    }
    func_ov123_021d5e60(wk->disp);
    func_ov123_021d562c(wk->msg);
    return done;
}

BOOL func_ov123_021d920c(GameProc *proc, u32 *state, void *param, void *work) {
    GSyncWork *wk = work;
    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (wk->yesNo != NULL) {
        func_02027858(wk->yesNo);
    }
    if (wk->result != NULL) {
        GFL_HeapFree(wk->result);
        wk->result = NULL;
    }
    func_ov123_021d5644(wk->msg);
    func_ov123_021d5e8c(wk->disp);
    func_ov123_021d5b70(wk->download);
    if (wk->musical != NULL) {
        GFL_HeapFree(wk->musical);
    }
    if (wk->zukanSkin != NULL) {
        GFL_HeapFree(wk->zukanSkin);
    }
    if (wk->cgearSkin != NULL) {
        GFL_HeapFree(wk->cgearSkin);
    }
    if (wk->pkm != NULL) {
        GFL_HeapFree(wk->pkm);
    }
    if (wk->unk44 != NULL) {
        GFL_HeapFree(wk->unk44);
    }
    if (func_0203cd98()) {
        func_ov009_02156de0(NULL, NULL);
    }
    if (wk->http != NULL) {
        func_ov114_021b9838(wk->http);
        func_ov009_02156dc4(NULL, NULL);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GSYNC);
    GFL_OvlUnload(OVERLAY_ID(114));
    return TRUE;
}


