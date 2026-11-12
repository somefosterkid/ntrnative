// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gds_rap.h: the lines in use so far.
#ifndef BWPORT_APP_GDS_RAP_H
#define BWPORT_APP_GDS_RAP_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/nhttp_rap.h"
#include "poke_net/poke_net_gds.h"
#include "save/battle_rec.h"
#include "struct_decls.h"

typedef struct GdsRapWork GdsRapWork;

// The result of the last request, valid when it failed
struct GdsRapResult {
    BOOL isValid;
    // GDS_RAP_RESULT_TYPE_*
    u32 type;
    // For a server error, the request, POKE_NET_GDS_REQCODE_*
    s32 code;
    // The library's error (POKE_NET_GDS_LASTERROR_*), the server's result, or GDS_RAP_HTTP_ERROR_*
    u32 detail;
};

enum {
    // The library could not finish the request
    GDS_RAP_RESULT_TYPE_LIB_ERROR,
    // The request was aborted
    GDS_RAP_RESULT_TYPE_ABORTED,
    // The server answered with an error, or the check of a video's Pokémon failed
    GDS_RAP_RESULT_TYPE_SERVER_ERROR,
};

// Black 2's, not checked against Black
// The details of a failed check of a video's Pokémon, by the HTTP status of its answer: 400, 401, 408 and any other
enum {
    GDS_RAP_HTTP_ERROR_BAD_REQUEST = 150,
    GDS_RAP_HTTP_ERROR_UNAUTHORIZED,
    GDS_RAP_HTTP_ERROR_TIMEOUT,
    GDS_RAP_HTTP_ERROR_OTHER,
};

// Black 2's, not checked against Black
// A function the client calls with the caller's work once a request has finished
typedef void (*GdsRapCallback)(void *work, const GdsRapResult *result);

// Black 2's; twinned code confirms the offsets of work and onUploadVideoEnd in Black
// The callbacks of the requests, and the work they are called with
struct GdsRapCallbacks {
    void *work;
    GdsRapCallback onUploadMusicalShotEnd;
    GdsRapCallback onDownloadMusicalShotEnd;
    GdsRapCallback onUploadVideoEnd;
    GdsRapCallback onDownloadVideoListEnd;
    GdsRapCallback onDownloadVideoEnd;
    GdsRapCallback onFavoriteVideoEnd;
};

// Black 2's, not checked against Black
// What GdsRap_Init is started with
struct GdsRapInit {
    // The player's GameSpy profile ID
    s32 profileId;
    HeapID heapId;
    GameData *gameData;
    // The buffer of the player's login to the Nintendo Wi-Fi Connection, which the check of a video's Pokémon takes
    void *loginBuffer;
    GdsRapCallbacks callbacks;
};

// What runs after a request's answer, until it returns TRUE: then the request's callback is called
typedef struct GdsRapSubProc GdsRapSubProc;

// Black 2's, not checked against Black
typedef BOOL (*GdsRapSubProcFunc)(GdsRapWork *gdsrap, GdsRapSubProc *sub);

// Black 2's; twinned code confirms the offsets of callback, seq, saveSeq and func, and its size in Black
struct GdsRapSubProc {
    GdsRapCallback callback;
    u16 seq;
    // The step of the save of the video
    u16 saveSeq;
    u32 unk8;
    GdsRapSubProcFunc func;
};

// Black 2's, not checked against Black
#define GDS_RAP_SIGNATURE_SIZE 0x80

// Black 2's; twinned code confirms the offsets of gameData, heapId, status, lastStatus, callbacks, result, send,
// species, dataNumber, isActive, sendReq, recv_wait_req, searchType, sub, sendWait, uploadSeq, checkCount, p_nhttp,
// httpError, nicknames and nicknamesReplaced in Black
struct GdsRapWork {
    GameData *gameData;
    POKE_NET_REQUESTCOMMON_AUTH auth;
    u32 heapId;
    // The library's status, POKE_NET_GDS_STATUS_*, now and when it was last looked at
    int status;
    int lastStatus;
    GdsRapCallbacks callbacks;
    GdsRapResult result;
    // What the request sends
    union {
        POKE_NET_GDS_MUSICALSHOT_SEND musicalShot;
        BattleRecVideo *video;
        POKE_NET_GDS_BATTLEDATA_SEARCHCONDITION condition;
    } send;
    union {
        // The species whose musical photos are downloaded
        u16 species;
        // The number of the video that is downloaded or marked
        u64 dataNumber;
    };
    // The buffer for the library's answer
    void *response;
    BOOL unk3B0;
    BOOL isActive;
    u8 unk3B8[0x20];
    // The request to send, and the one whose answer is waited for; POKE_NET_GDS_REQCODE_LAST for none. The second
    // name and p_nhttp are the original ones, which the assertions' strings give
    int sendReq;
    int recv_wait_req;
    // The kind of the search, POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_*
    u32 searchType;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strBuf;
    GdsRapSubProc sub;
    u8 unk400;
    // Frames to wait before the request is sent
    u8 sendWait;
    // The step of a video's upload
    u8 uploadSeq;
    // How often the video's Pokémon were checked
    u8 checkCount;
    // The check of the video's Pokémon
    NHttpRap *p_nhttp;
    u8 unk408[2];
    u8 signature[GDS_RAP_SIGNATURE_SIZE];
    u32 checkResult;
    // The Pokémon in the video
    u8 pkmCount;
    u8 unk491;
    // Set once the Pokémon failed the check a second time
    u8 checkFailed;
    // GDS_RAP_HTTP_ERROR_*
    u8 httpError;
    void *loginBuffer;
    // The nicknames of the video's Pokémon, kept while the video is uploaded with the species' names in their place
    u16 nicknames[BATTLE_REC_HEADER_PKM_MAX][11];
    BOOL nicknamesReplaced;
};

// pokebw2's GdsRapResponse_MusicalShotUpload
// gds_rap_response.c: whether the server's answer to each request is a success
BOOL func_ov125_021d732c(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// pokebw2's GdsRapResponse_BattleVideoUpload
BOOL func_ov125_021d73b8(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// pokebw2's GdsRap_SendMusicalShotUpload
// Start a request, returning FALSE if another one is running. Upload a musical photo with the player's profile
BOOL func_ov125_021d67c8(GdsRapWork *gdsrap, GdsProfile *profile, MusicalShot *shot);
// pokebw2's GdsRap_SendMusicalShotDownload
// Download the newest musical photos of a species
BOOL func_ov125_021d6814(GdsRapWork *gdsrap, u16 species);
// pokebw2's GdsRap_SendBattleVideoUpload
// Upload the loaded battle video, with the player's profile
BOOL func_ov125_021d683c(GdsRapWork *gdsrap, GdsProfile *profile);
// pokebw2's GdsRap_SendBattleVideoList1
// Download one of the server's three lists of battle videos
BOOL func_ov125_021d6968(GdsRapWork *gdsrap);
// pokebw2's GdsRap_SendBattleVideoList2
BOOL func_ov125_021d698c(GdsRapWork *gdsrap);
// pokebw2's GdsRap_SendBattleVideoList3
BOOL func_ov125_021d69b4(GdsRapWork *gdsrap);
// pokebw2's GdsRap_SendBattleVideoDownload
// Download the battle video of a number, and mark it as a favorite
BOOL func_ov125_021d69dc(GdsRapWork *gdsrap, u64 dataNumber);
// pokebw2's GdsRap_SendBattleVideoFavorite
BOOL func_ov125_021d6a08(GdsRapWork *gdsrap, u64 dataNumber);
// pokebw2's GdsRap_Main
// Runs the requests, every frame
BOOL func_ov125_021d6a34(GdsRapWork *gdsrap);
// pokebw2's GdsRap_OnNetEvent
// Takes an event of the Wi-Fi connection: an error ends the check of a video's Pokémon
void func_ov125_021d7124(GdsRapWork *gdsrap, int a1, int event, int a3);
// pokebw2's GdsRap_GetResult
// Gives the result of the last request in result, and whether there is one
BOOL func_ov125_021d72ec(GdsRapWork *gdsrap, GdsRapResult **result);
// pokebw2's GdsRap_ClearResult
// Clears the result
void func_ov125_021d72f8(GdsRapWork *gdsrap);
// pokebw2's GdsRap_IsIdle
// Whether no request is running
BOOL func_ov125_021d730c(GdsRapWork *gdsrap);

// pokebw2's GdsRapResponse_MusicalShotDownload
BOOL func_ov125_021d7354(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// pokebw2's GdsRapResponse_GetMusicalShots
// What the last request received. Point to the downloaded musical photos, up to max of them, and give their number
int func_ov125_021d7374(GdsRapWork *gdsrap, POKE_NET_GDS_MUSICALSHOT_SEND **list, int max);
// pokebw2's GdsRapResponse_GetBattleVideoNumber
// The number that the server gave the uploaded video
u64 func_ov125_021d73e0(GdsRapWork *gdsrap);
// pokebw2's GdsRapResponse_BattleVideoSearch
BOOL func_ov125_021d73f0(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// pokebw2's GdsRapResponse_GetBattleVideo
// Points to the downloaded video, and gives the low word of its number
u32 func_ov125_021d74d8(GdsRapWork *gdsrap, BattleRecVideo **video);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GdsRap_Init
// gds_rap.c
// Starts the client, always returning TRUE
BOOL func_ov125_021d6660(GdsRapWork *gdsrap, const GdsRapInit *init);
// pokebw2's GdsRap_Exit
void func_ov125_021d6770(GdsRapWork *gdsrap);
// pokebw2's GdsRap_SendBattleVideoSearch
// Download a list of the battle videos with a species, of a battle mode (an index of the client's table of modes)
BOOL func_ov125_021d68ec(GdsRapWork *gdsrap, u16 species, u32 searchModeNo, u8 a3, u8 a4);
// pokebw2's GdsRapResponse_GetBattleVideoList
// Copy the videos of the downloaded list to list, up to max of them, and give their number
int func_ov125_021d7414(GdsRapWork *gdsrap, POKE_NET_GDS_BATTLEDATA_OUTLINE *list, int max);
// pokebw2's GdsRapResponse_BattleVideoDownload
BOOL func_ov125_021d74b4(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// pokebw2's GdsRapResponse_BattleVideoFavorite
BOOL func_ov125_021d7520(GdsRapWork *gdsrap, POKE_NET_RESPONSE *res);
// Black 2's, not checked against Black
// A function the client calls with the caller's work once a request has finished

// What runs after a request's answer, until it returns TRUE: then the request's callback is called

// Black 2's, not checked against Black

// Black 2's, not checked against Black

// pokebw2's GdsRap_Main
// Runs the requests, every frame
// pokebw2's GdsRapResponse_GetMusicalShots
// What the last request received. Point to the downloaded musical photos, up to max of them, and give their number
// pokebw2's GdsRapResponse_GetBattleVideo
// Points to the downloaded video, and gives the low word of its number
// pokebw2's GdsRap_GetResult
// Gives the result of the last request in result, and whether there is one
// pokebw2's GdsRap_ClearResult
// Clears the result
// pokebw2's GdsRap_IsIdle
// Whether no request is running
// pokebw2's GdsRap_SendMusicalShotUpload
// Start a request, returning FALSE if another one is running. Upload a musical photo with the player's profile
// pokebw2's GdsRap_SendMusicalShotDownload
// Download the newest musical photos of a species
// pokebw2's GdsRap_SendBattleVideoUpload
// Upload the loaded battle video, with the player's profile
// pokebw2's GdsRap_SendBattleVideoDownload
// Download the battle video of a number, and mark it as a favorite
// pokebw2's GdsRap_SendBattleVideoList1
// Download one of the server's three lists of battle videos
// pokebw2's GdsRap_SendBattleVideoList3
// pokebw2's GdsRap_SendBattleVideoList2
// pokebw2's GdsRap_SendBattleVideoFavorite
// pokebw2's GdsRap_OnNetEvent
// Takes an event of the Wi-Fi connection: an error ends the check of a video's Pokémon

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GdsRap_Exit
// pokebw2's GdsRapResponse_GetBattleVideoList
// Copy the videos of the downloaded list to list, up to max of them, and give their number

#endif // BWPORT_APP_GDS_RAP_H
