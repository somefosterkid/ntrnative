// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov201/gds_rap.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The game's side of the Global Data Server (GDS) client: it sends the Battle Recorder's requests through the GDS
// library, which runs them in a thread, and calls the request's callback once the answer has come and what follows it
// is done. A battle video's Pokémon are checked by the server of nhttp_rap.c before the video is uploaded, and the
// uploaded video is saved with the number that the server gave it. The name is the ROM's string, which
// GFL_HeapAllocate is given.

#include "types.h"
#include "app/gds_rap.h"
#include "app/battle_rec_ov.h"
#include "constants/arc.h"
#include "constants/language.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/nhttp_rap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/species_names.h"
#include "poke_net/poke_net_gds.h"
#include "save/battle_rec.h"
#include "save/player_info.h"
#include "system/game_data.h"
#include "system/wordset.h"

// The server, and the frames to wait before a request with data is sent
#define GDS_RAP_SERVER_URL "pkgdsprod.nintendo.co.jp"
#define GDS_RAP_SERVER_PORT 12401
#define GDS_RAP_SEND_WAIT 60
// The check that nhttp_rap.c sends for a battle video
#define GDS_RAP_CHECK_TYPE 3

// The steps of a battle video's upload
enum {
    GDS_RAP_UPLOAD_SEQ_CHECK_START,
    GDS_RAP_UPLOAD_SEQ_CHECK_WAIT,
    GDS_RAP_UPLOAD_SEQ_ENCRYPT,
    GDS_RAP_UPLOAD_SEQ_SEND,
};

// The battle mode of each search and the mask of its bits that the server compares
typedef struct {
    u16 mode;
    u16 mask;
} GdsRapBattleMode;

static BOOL func_ov125_021d6b40(GdsRapWork *gdsrap, BOOL *checkError);
static BOOL func_ov125_021d6fb0(GdsRapWork *gdsrap);
BOOL func_ov125_021d7060(GdsRapWork *gdsrap, GdsRapSubProc *sub);
static BOOL func_ov125_021d7064(GdsRapWork *gdsrap, GdsRapSubProc *sub);
BOOL func_ov125_021d7120(GdsRapWork *gdsrap, GdsRapSubProc *sub);
static BOOL func_ov125_021d7154(GdsRapWork *gdsrap, int index, s32 *client, s32 *pos);
static void func_ov125_021d71cc(const u16 *src, u16 *dest, int length);
BOOL func_ov125_021d71d8(GdsRapWork *gdsrap);

// pokebw2's BattleModeBitTbl
static const GdsRapBattleMode data_ov125_021d8c80[] = {
#include "asset/gds_rap_data_ov125_021d8c80.inc"
};

// pokebw2's GdsRap_SendMusicalShotUpload



BOOL func_ov125_021d6660(GdsRapWork *gdsrap, const GdsRapInit *init) {
    BOOL ret;

    GFL_OvlLoad(OVERLAY_ID(114));
    MI_CpuFill8(gdsrap, 0, sizeof(GdsRapWork));
    gdsrap->heapId = init->heapId;
    gdsrap->gameData = init->gameData;
    gdsrap->loginBuffer = init->loginBuffer;
    gdsrap->callbacks = init->callbacks;
    gdsrap->lastStatus = -1;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_LAST;
    gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;

    gdsrap->response = GFL_HeapAllocate(init->heapId, func_ov125_021d8600(POKE_NET_GDS_REQCODE_ADMIN), FALSE,
                                        "gds_rap.c", 132);
    MI_CpuFill8(gdsrap->response, 0, func_ov125_021d8600(POKE_NET_GDS_REQCODE_ADMIN));

    gdsrap->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, init->heapId);
    gdsrap->wordSet = GFL_WordSetSystemCreateDefault(init->heapId);
    gdsrap->strBuf = GFL_StrBufCreate(0x100, init->heapId);

    gdsrap->auth.PID = init->profileId;
    gdsrap->auth.ROMCode = GAME_VERSION;
    gdsrap->auth.LangCode = GAME_LANGUAGE;
    ret = func_ov125_021d8598(&gdsrap->auth, GDS_RAP_SERVER_URL, GDS_RAP_SERVER_PORT);
    GFL_ASSERT(ret == TRUE);
    gdsrap->isActive = TRUE;
    gdsrap->unk3B0 = TRUE;
    return TRUE;
}

void func_ov125_021d6770(GdsRapWork *gdsrap) {
    if (gdsrap->p_nhttp != NULL) {
        func_ov114_021b99b8(gdsrap->p_nhttp);
        func_ov114_021b9838(gdsrap->p_nhttp);
    }
    func_ov125_021d85b4();
    gdsrap->isActive = FALSE;
    GFL_StrBufFree(gdsrap->strBuf);
    GFL_WordSetSystemFree(gdsrap->wordSet);
    GFL_MsgDataFree(gdsrap->msgData);
    GFL_HeapFree(gdsrap->response);
    GFL_OvlUnload(OVERLAY_ID(114));
}

BOOL func_ov125_021d67c8(GdsRapWork *gdsrap, GdsProfile *profile, MusicalShot *shot) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->send.musicalShot.Profile = *profile;
    gdsrap->send.musicalShot.Data = *shot;
    gdsrap->sendWait = GDS_RAP_SEND_WAIT;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST;
    return TRUE;
}

// pokebw2's GdsRap_SendMusicalShotDownload
BOOL func_ov125_021d6814(GdsRapWork *gdsrap, u16 species) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->species = species;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_MUSICALSHOT_GET;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoUpload
BOOL func_ov125_021d683c(GdsRapWork *gdsrap, GdsProfile *profile) {
    BattleRecVideo *video = func_ov125_021d7700();
    BattleRecPkm *pkm;
    int i;
    s32 client;
    s32 pos;
    s32 clientCount;
    s32 pkmCount;

    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }

    // Keep the nicknames, in case the check of the Pokémon takes them out of the video
    func_0200b784(video->header.mode, &clientCount, &pkmCount);
    for (i = 0; i < BATTLE_REC_HEADER_PKM_MAX; i++) {
        if (!func_ov125_021d7154(gdsrap, i, &client, &pos)) {
            break;
        }
        pkm = &video->body.parties[client].pkm[pos];
        func_ov125_021d71cc(pkm->nickname, gdsrap->nicknames[i], 11);
    }
    gdsrap->nicknamesReplaced = FALSE;
    gdsrap->checkCount = 0;

    gdsrap->send.video = video;
    MI_CpuCopy8(profile, getVSPlayerAllocation(), sizeof(GdsProfile));
    gdsrap->sendWait = GDS_RAP_SEND_WAIT;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoSearch
BOOL func_ov125_021d68ec(GdsRapWork *gdsrap, u16 species, u32 search_mode_no, u8 a3, u8 a4) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    GFL_ASSERT(search_mode_no < NELEMS(data_ov125_021d8c80));

    MI_CpuFill8(&gdsrap->send.condition, 0, sizeof(POKE_NET_GDS_BATTLEDATA_SEARCHCONDITION));
    gdsrap->send.condition.MonsNo = species;
    gdsrap->send.condition.BattleMode = data_ov125_021d8c80[search_mode_no].mode;
    gdsrap->send.condition.BattleModeMask = data_ov125_021d8c80[search_mode_no].mask;
    gdsrap->send.condition.unk6 = a3;
    gdsrap->send.condition.unk7 = a4;
    gdsrap->send.condition.ServerVersion = BATTLE_REC_VERSION;
    gdsrap->searchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_CONDITION;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoList1
BOOL func_ov125_021d6968(GdsRapWork *gdsrap) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->searchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST1;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoList2
BOOL func_ov125_021d698c(GdsRapWork *gdsrap) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->searchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST2;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoList3
BOOL func_ov125_021d69b4(GdsRapWork *gdsrap) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->searchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST3;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoDownload
BOOL func_ov125_021d69dc(GdsRapWork *gdsrap, u64 dataNumber) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->dataNumber = dataNumber;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_GET;
    return TRUE;
}

// pokebw2's GdsRap_SendBattleVideoFavorite
BOOL func_ov125_021d6a08(GdsRapWork *gdsrap, u64 dataNumber) {
    if (func_ov125_021d730c(gdsrap) == FALSE) {
        return FALSE;
    }
    gdsrap->dataNumber = dataNumber;
    gdsrap->sendReq = POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE;
    return TRUE;
}

BOOL func_ov125_021d6a34(GdsRapWork *gdsrap) {
    BOOL checkError;

    if (gdsrap->isActive == TRUE) {
        // Send the request once the library is free
        if (gdsrap->sendReq != POKE_NET_GDS_REQCODE_LAST) {
            if (gdsrap->sendWait != 0) {
                gdsrap->sendWait--;
            } else {
                switch (func_ov125_021d85c0()) {
                case POKE_NET_GDS_STATUS_INITIALIZED:
                case POKE_NET_GDS_STATUS_ABORTED:
                case POKE_NET_GDS_STATUS_FINISHED:
                case POKE_NET_GDS_STATUS_ERROR:
                    if (func_ov125_021d6b40(gdsrap, &checkError) == TRUE) {
                        gdsrap->sendReq = POKE_NET_GDS_REQCODE_LAST;
                        gdsrap->uploadSeq = 0;
                        if (checkError == TRUE) {
                            gdsrap->result.type = GDS_RAP_RESULT_TYPE_SERVER_ERROR;
                            gdsrap->result.code = POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST;
                            gdsrap->result.detail = gdsrap->httpError;
                            gdsrap->result.isValid = TRUE;
                            if (gdsrap->callbacks.onUploadVideoEnd != NULL) {
                                gdsrap->callbacks.onUploadVideoEnd(gdsrap->callbacks.work, &gdsrap->result);
                            }
                        }
                    }
                    break;
                }
            }
        }

        func_ov125_021d6fb0(gdsrap);

        // What follows the answer, then the request's callback
        if (gdsrap->sub.func != NULL) {
            if (gdsrap->sub.func(gdsrap, &gdsrap->sub) == TRUE) {
                gdsrap->sub.func = NULL;
                if (gdsrap->sub.callback != NULL) {
                    gdsrap->sub.callback(gdsrap->callbacks.work, &gdsrap->result);
                    gdsrap->sub.callback = NULL;
                }
                MI_CpuFill8(&gdsrap->sub, 0, sizeof(GdsRapSubProc));
                gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;
            }
        }
    }
    return TRUE;
}

static BOOL func_ov125_021d6b40(GdsRapWork *gdsrap, BOOL *checkError) {
    BOOL result = FALSE;
    BOOL searchResult = FALSE;

    *checkError = FALSE;
    switch (gdsrap->sendReq) {
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST:
        result = func_ov125_021d860c(&gdsrap->send.musicalShot, gdsrap->response);
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_GET:
        result = func_ov125_021d8654(gdsrap->species, gdsrap->response);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:
        switch (gdsrap->uploadSeq) {
        case GDS_RAP_UPLOAD_SEQ_CHECK_START: {
            BattleRecVideo *video = func_ov125_021d7700();
            s32 clientCount;
            s32 pkmCount;
            int i;
            BOOL ret;

            GFL_ASSERT(gdsrap->p_nhttp == NULL);
            gdsrap->p_nhttp = func_ov114_021b9800(gdsrap->heapId, func_0200853c(GetGameDataPlayerInfo(gdsrap->gameData)),
                                                  gdsrap->loginBuffer);
            func_ov114_021b98a0(gdsrap->p_nhttp, gdsrap->heapId, sizeof(BattleRecPkm) * BATTLE_REC_HEADER_PKM_MAX,
                                GDS_RAP_CHECK_TYPE);
            func_0200b784(video->header.mode, &clientCount, &pkmCount);
            gdsrap->pkmCount = 0;
            for (i = 0; i < clientCount; i++) {
                func_ov114_021b98d8(gdsrap->p_nhttp, video->body.parties[i].pkm,
                                    sizeof(BattleRecPkm) * video->body.parties[i].count);
                gdsrap->pkmCount += video->body.parties[i].count;
            }
            ret = func_ov114_021b98f8(gdsrap->p_nhttp);
            GFL_ASSERT(ret);
            ret = func_ov114_021b9748(gdsrap->p_nhttp) == 0;
            GFL_ASSERT(ret);
            gdsrap->uploadSeq++;
            break;
        }
        case GDS_RAP_UPLOAD_SEQ_CHECK_WAIT: {
            int status = func_ov114_021b99dc(gdsrap->p_nhttp);
            int error = func_ov114_021b9790(gdsrap->p_nhttp);

            if (status != 200 && error != 15) {
                switch (func_ov114_021b99dc(gdsrap->p_nhttp)) {
                case 400:
                    gdsrap->httpError = GDS_RAP_HTTP_ERROR_BAD_REQUEST;
                    break;
                case 401:
                    gdsrap->httpError = GDS_RAP_HTTP_ERROR_UNAUTHORIZED;
                    break;
                case 408:
                    gdsrap->httpError = GDS_RAP_HTTP_ERROR_TIMEOUT;
                    break;
                default:
                    gdsrap->httpError = GDS_RAP_HTTP_ERROR_OTHER;
                    break;
                }
                func_ov114_021b99b8(gdsrap->p_nhttp);
                func_ov114_021b9774(gdsrap->p_nhttp);
                func_ov114_021b9838(gdsrap->p_nhttp);
                gdsrap->p_nhttp = NULL;
                *checkError = TRUE;
                return TRUE;
            }
            if (error == 0) {
                void *answer = func_ov114_021b97f4(gdsrap->p_nhttp);
                int i;

                if (func_ov114_021b99f0(answer) == 0) {
                    // The Pokémon passed: the upload carries the check's signature
                    gdsrap->checkFailed = FALSE;
                    MI_CpuCopy8(func_ov114_021b9a14(answer, gdsrap->pkmCount), gdsrap->signature,
                               GDS_RAP_SIGNATURE_SIZE);
                    for (i = 0; i < gdsrap->pkmCount; i++) {
                        gdsrap->checkResult = func_ov114_021b99f4(answer, i);
                    }
                    gdsrap->uploadSeq++;
                } else {
                    // Give the Pokémon that failed their species' names and check once more
                    BattleRecVideo *video = func_ov125_021d7700();
                    s32 client;
                    s32 pos;
                    s32 clientCount;
                    s32 pkmCount;

                    if (gdsrap->checkCount == 1) {
                        gdsrap->checkFailed = TRUE;
                    }
                    func_0200b784(video->header.mode, &clientCount, &pkmCount);
                    for (i = 0; i < gdsrap->pkmCount; i++) {
                        gdsrap->checkResult = func_ov114_021b99f4(answer, i);
                        if (gdsrap->checkResult != 0 && gdsrap->checkCount == 0) {
                            BattleRecPkm *pkm;

                            if (!func_ov125_021d7154(gdsrap, i, &client, &pos)) {
                                break;
                            }
                            pkm = &video->body.parties[client].pkm[pos];
                            GFL_MsgDataLoadRawStr(g_PMLSpeciesNamesResident, pkm->species, pkm->nickname, 11);
                            gdsrap->nicknamesReplaced = TRUE;
                        }
                    }
                    if (gdsrap->checkCount == 0) {
                        func_0200ba44(func_0200b91c());
                        gdsrap->uploadSeq = GDS_RAP_UPLOAD_SEQ_CHECK_START;
                        gdsrap->checkCount++;
                    } else {
                        gdsrap->uploadSeq++;
                    }
                }
                func_ov114_021b99b8(gdsrap->p_nhttp);
                func_ov114_021b9838(gdsrap->p_nhttp);
                gdsrap->p_nhttp = NULL;
            } else {
                if (error != 15 && gdsrap->p_nhttp != NULL) {
                    func_ov114_021b99b8(gdsrap->p_nhttp);
                    func_ov114_021b9774(gdsrap->p_nhttp);
                    func_ov114_021b9838(gdsrap->p_nhttp);
                    gdsrap->p_nhttp = NULL;
                    return TRUE;
                }
                return FALSE;
            }
            break;
        }
        case GDS_RAP_UPLOAD_SEQ_ENCRYPT:
            func_ov125_021d75c0(GameData_GetSaveControl(gdsrap->gameData));
            gdsrap->uploadSeq++;
            break;
        case GDS_RAP_UPLOAD_SEQ_SEND:
            result = func_ov125_021d8694(gdsrap->send.video, gdsrap->signature, GDS_RAP_SIGNATURE_SIZE,
                                         gdsrap->response);
            break;
        }
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:
        switch (gdsrap->searchType) {
        case POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_CONDITION:
            searchResult = func_ov125_021d8754(&gdsrap->send.condition, gdsrap->response);
            break;
        case POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST1:
            searchResult = func_ov125_021d879c(BATTLE_REC_VERSION, gdsrap->response);
            break;
        case POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST2:
            searchResult = func_ov125_021d885c(BATTLE_REC_VERSION, gdsrap->response);
            break;
        case POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST3:
            searchResult = func_ov125_021d87fc(BATTLE_REC_VERSION, gdsrap->response);
            break;
        }
        result = searchResult;
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:
        result = func_ov125_021d88bc(gdsrap->dataNumber, BATTLE_REC_VERSION, gdsrap->response);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:
        result = func_ov125_021d88f0(gdsrap->dataNumber, gdsrap->response);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }

    if (result == TRUE) {
        gdsrap->recv_wait_req = gdsrap->sendReq;
    }
    return result;
}



// Watches the library's status while an answer is waited for: the answer, an abort or an error ends the request
// pokebw2's GdsRap_MainRecv
static BOOL func_ov125_021d6fb0(GdsRapWork *gdsrap) {
    if (gdsrap->recv_wait_req == POKE_NET_GDS_REQCODE_LAST) {
        return TRUE;
    }

    gdsrap->status = func_ov125_021d85c0();
    if (gdsrap->status != gdsrap->lastStatus) {
        switch (gdsrap->status) {
        case POKE_NET_GDS_STATUS_INACTIVE:
        case POKE_NET_GDS_STATUS_INITIALIZED:
        case POKE_NET_GDS_STATUS_REQUEST:
        case POKE_NET_GDS_STATUS_NETSETTING:
        case POKE_NET_GDS_STATUS_CONNECTING:
        case POKE_NET_GDS_STATUS_SENDING:
        case POKE_NET_GDS_STATUS_RECEIVING:
            break;
        case POKE_NET_GDS_STATUS_ABORTED:
            gdsrap->result.type = GDS_RAP_RESULT_TYPE_ABORTED;
            gdsrap->result.code = 0;
            gdsrap->result.detail = POKE_NET_GDS_STATUS_ABORTED;
            gdsrap->result.isValid = TRUE;
            gdsrap->sub.func = func_ov125_021d7120;
            break;
        case POKE_NET_GDS_STATUS_FINISHED:
            gdsrap->sub.func = func_ov125_021d7060;
            func_ov125_021d71d8(gdsrap);
            break;
        case POKE_NET_GDS_STATUS_ERROR:
            gdsrap->result.type = GDS_RAP_RESULT_TYPE_LIB_ERROR;
            gdsrap->result.code = 0;
            gdsrap->result.detail = func_ov125_021d85dc();
            gdsrap->result.isValid = TRUE;
            gdsrap->sub.func = func_ov125_021d7120;
            break;
        }
        gdsrap->lastStatus = gdsrap->status;
    }
    return TRUE;
}

// pokebw2's GdsRap_SubProcNone
BOOL func_ov125_021d7060(GdsRapWork *gdsrap, GdsRapSubProc *sub) {
    return TRUE;
}

// After a battle video's upload: gives the video its nicknames back, and saves it with the number the server gave it
// pokebw2's GdsRap_SubProcSaveDataNumber
static BOOL func_ov125_021d7064(GdsRapWork *gdsrap, GdsRapSubProc *sub) {
    POKE_NET_RESPONSE *res;
    POKE_NET_GDS_RESPONSE_REGIST *param;
    u32 result;

    if (sub->seq == 0 && gdsrap->nicknamesReplaced) {
        BattleRecVideo *video = func_ov125_021d7700();
        BattleRecPkm *pkm;
        int i;
        s32 client;
        s32 pos;
        s32 clientCount;
        s32 pkmCount;

        func_0200b490();
        func_0200b784(video->header.mode, &clientCount, &pkmCount);
        for (i = 0; i < BATTLE_REC_HEADER_PKM_MAX; i++) {
            if (!func_ov125_021d7154(gdsrap, i, &client, &pos)) {
                break;
            }
            pkm = &video->body.parties[client].pkm[pos];
            func_ov125_021d71cc(gdsrap->nicknames[i], pkm->nickname, 11);
        }
        func_0200ba44(func_0200b91c());
        func_0200b4b8();
    }

    res = func_ov125_021d85e8();
    param = (POKE_NET_GDS_RESPONSE_REGIST *)res->Param;
    result = func_ov125_021d75e8(gdsrap->gameData, param->Code, &sub->seq, &sub->saveSeq, gdsrap->heapId);
    if (result == 2 || result == 3) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GdsRap_SubProcError
BOOL func_ov125_021d7120(GdsRapWork *gdsrap, GdsRapSubProc *sub) {
    return TRUE;
}

// pokebw2's GdsRap_OnNetEvent
void func_ov125_021d7124(GdsRapWork *gdsrap, int a1, int event, int a3) {
    if (event == 3 || event == 6) {
        if (gdsrap->p_nhttp != NULL) {
            func_ov114_021b99b8(gdsrap->p_nhttp);
            func_ov114_021b9774(gdsrap->p_nhttp);
            func_ov114_021b9838(gdsrap->p_nhttp);
            gdsrap->p_nhttp = NULL;
        }
    }
}

// Gives the client and the place in its party of the loaded video's Pokémon of an index, counting through the
// parties; FALSE if there are fewer
// pokebw2's GdsRap_GetVideoPokemonPos
static BOOL func_ov125_021d7154(GdsRapWork *gdsrap, int index, s32 *client, s32 *pos) {
    int count = 0;
    BattleRecVideo *video = func_ov125_021d7700();
    s32 clientCount;
    s32 pkmCount;

    func_0200b784(video->header.mode, &clientCount, &pkmCount);
    for (*client = 0; *client < clientCount; (*client)++) {
        for (*pos = 0; *pos < video->body.parties[*client].count; (*pos)++) {
            if (count == index) {
                return TRUE;
            }
            count++;
        }
    }
    return FALSE;
}

// pokebw2's GdsRap_CopyNickname
static void func_ov125_021d71cc(const u16 *src, u16 *dest, int length) {
    MI_CpuCopy8(src, dest, length * sizeof(u16));
}

BOOL func_ov125_021d71d8(GdsRapWork *gdsrap) {
    BOOL ret = TRUE;
    POKE_NET_RESPONSE *res = func_ov125_021d85e8();

    func_ov125_021d85f4();
    GFL_ASSERT(gdsrap->recv_wait_req == res->ReqCode);

    gdsrap->sub.callback = NULL;
    switch (res->ReqCode) {
    case POKE_NET_GDS_REQCODE_DEBUG_MESSAGE:
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST:
        ret = func_ov125_021d732c(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onUploadMusicalShotEnd;
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_GET:
        ret = func_ov125_021d7354(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onDownloadMusicalShotEnd;
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:
        ret = func_ov125_021d73b8(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onUploadVideoEnd;
        if (ret == TRUE) {
            gdsrap->sub.func = func_ov125_021d7064;
        }
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:
        ret = func_ov125_021d73f0(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onDownloadVideoListEnd;
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:
        ret = func_ov125_021d74b4(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onDownloadVideoEnd;
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:
        ret = func_ov125_021d7520(gdsrap, res);
        gdsrap->sub.callback = gdsrap->callbacks.onFavoriteVideoEnd;
        break;
    }

    if (ret == FALSE) {
        gdsrap->result.type = GDS_RAP_RESULT_TYPE_SERVER_ERROR;
        gdsrap->result.code = res->ReqCode;
        gdsrap->result.detail = res->Result;
        gdsrap->result.isValid = TRUE;
    } else {
        gdsrap->result.isValid = FALSE;
    }
    return ret;
}


// pokebw2's GdsRap_GetResult
BOOL func_ov125_021d72ec(GdsRapWork *gdsrap, GdsRapResult **result) {
    *result = &gdsrap->result;
    return (*result)->isValid;
}

// pokebw2's GdsRap_ClearResult
void func_ov125_021d72f8(GdsRapWork *gdsrap) {
    MI_CpuFill8(&gdsrap->result, 0, sizeof(GdsRapResult));
}

// pokebw2's GdsRap_IsIdle
BOOL func_ov125_021d730c(GdsRapWork *gdsrap) {
    if (gdsrap->sendReq == POKE_NET_GDS_REQCODE_LAST && gdsrap->recv_wait_req == POKE_NET_GDS_REQCODE_LAST) {
        return TRUE;
    }
    return FALSE;
}
