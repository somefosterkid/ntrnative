#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"

#include "unk_0203A3B0.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "frontier_pokemon.h"
#include "wifi_battle_tower_match_candidate.h"
#include "save_frontier.h"
#include "sav_system_info.h"
#include "overlay_70_gtsapplication_state.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct DWCstInetControl;

struct FrontierPokemon;

struct G2dRenderer;

struct Menu;

struct MsgData;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct SysInfo;

struct WifiBattleTowerAppState;

struct WifiBattleTowerDownloadBuffer;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiBattleTowerRecord;

struct WifiPlayerProfile;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;



typedef u16 charcode_t;

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct WifiPlayerProfile {
    FrontierPokemon mons[3];
    charcode_t name[7 + 1];
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u8 trainerInfoId[4];
    s8 no1Sentence[8];
    union {
        struct {
            u8 sLotoId : 1;
            u8 gender : 1;
            u8 : 6;
        };
        u8 unused2;
    };
    u8 appearance;
    s8 battleSentences[24];
    u16 ratingScore;
} WifiPlayerProfile;

typedef struct WifiBattleTowerDownloadBuffer {
    WifiPlayerProfile wifiPlayerProfile[7];
    WifiBattleTowerMatchCandidate matchCandidates[30];
} WifiBattleTowerDownloadBuffer;

typedef struct {
    WifiBattleTowerRecord *record;
    WifiBattleTowerDownloadData *downloadData;
    SysInfo *systemData;
    SaveData *saveData;
    Options *options;
    DWCUserData *userData;
    int mode;
    int profileId;
    int unk_20;
    int unk_24;
} UnkStruct_0206BC70;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct Menu Menu;

enum WifiBattleTowerState {
    BT_STATE_INITIAL_PROMPT = 0,
    BT_STATE_INITIAL_YES_NO_MENU,
    BT_STATE_INIT_INET,
    BT_STATE_WAIT_INET_CONNECT,
    BT_STATE_INIT_NAS_LOGIN,
    BT_STATE_WAIT_NAS_LOGIN,
    BT_STATE_INIT_HTTP,
    BT_STATE_REQUEST_INFO,
    BT_STATE_WAIT_INFO,
    BT_STATE_SET_PROFILE,
    BT_STATE_WAIT_SET_PROFILE,
    BT_STATE_REQUEST_ROOM_COUNT,
    BT_STATE_WAIT_ROOM_COUNT,
    BT_STATE_SELECT_OPPONENT_PROMPT,
    BT_STATE_DRAW_OPPONENT_SELECT,
    BT_STATE_SELECT_OPPONENT,
    BT_STATE_DOWNLOAD_PROMPT,
    BT_STATE_REQUEST_DOWNLOAD,
    BT_STATE_WAIT_DOWNLOAD,
    BT_STATE_DOWNLOAD_COMPLETE_PROMPT,
    BT_STATE_ALREADY_CHALLENGED_PROMPT,
    BT_STATE_EXIT_PROMPT,
    BT_STATE_EXIT_YES_NO,
    BT_STATE_EXIT_MATCH_LIST_PROMPT,
    BT_STATE_EXIT_MATCH_LIST_YES_NO,
    BT_STATE_UPLOAD_PROMPT,
    BT_STATE_REQUEST_UPLOAD,
    BT_STATE_WAIT_UPLOAD,
    BT_STATE_UPLOAD_COMPLETE_PROMPT,
    BT_STATE_SELECT_RANK_PROMPT,
    BT_STATE_DRAW_RANK_SELECT,
    BT_STATE_SELECT_RANK,
    BT_STATE_MATCH_LIST_SELECT_PROMPT,
    BT_STATE_DRAW_MATCH_LIST_SELECT,
    BT_STATE_SELECT_MATCH_LIST,
    BT_STATE_DOWNLOAD_MATCH_LIST_PROMPT,
    BT_STATE_REQUEST_MATCH_LIST,
    BT_STATE_WAIT_MATCH_LIST,
    BT_STATE_MATCH_LIST_COMPLETE_PROMPT,
    BT_STATE_INIT_SAVE,
    BT_STATE_WAIT_SAVE,
    BT_STATE_GOODBYE_PROMPT,
    BT_STATE_CLEANUP_INET,
    BT_STATE_DISCONNECTING_PROMPT,
    BT_STATE_IDLE,
    BT_STATE_DISCONNECT_ENTRY,
    BT_STATE_END_SESSION,
    BT_STATE_WAIT_TEXT,
    BT_STATE_WAIT_TEXT_YES_NO,
    BT_STATE_WAIT_TEXT_TIMED,
    BT_STATE_DISCONNECT_YES_NO,
    BT_STATE_CONNECTED_PROMPT,
    BT_STATE_CONNECTED_YES_NO_MENU,
    BT_STATE_SHOW_ERROR,
    BT_STATE_ERROR_CLEANUP,
    BT_STATE_SHOW_DWC_ERROR,
    BT_STATE_DWC_ERROR_ACK,
};

typedef struct WifiBattleTowerAppState {
    UnkStruct_0206BC70 *args;
    BgConfig *bgConfig;
    int unk_08;
    int unk_0C;
    int screenMode;
    int exitMode;
    int unused_18;
    enum WifiBattleTowerState state;
    enum WifiBattleTowerState nextState;
    void *dwcHeapBuffer;
    NNSFndHeapHandle dwcHeap;
    DWCInetControl inetControl;
    int playerRank;
    int selectedRank;
    int matchCount;
    int selectedOpponentIdx;
    WifiBattleTowerDownloadBuffer downloadBuffer;
    WifiPlayerProfile wifiPlayerProfile;
    u16 unk_BB8;
    u16 unk_BBA;
    u16 unk_BBC;
    u16 unk_BBE;
    BoxPokemon *unk_BC0;
    int unk_BC4;
    int unk_BC8;
    MessageFormat *stringTemplate;
    MsgData *msgLoader1;
    MsgData *msgLoader2;
    MsgData *msgLoader3;
    String *msgString;
    String *titleString;
    String *opponentIdxString;
    String *rankString;
    String *errorString;
    int textPrinterID;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *spriteResource[3][4];
    SpriteResourcesHeader arrowSpriteHeader;
    SpriteResourcesHeader unk_DE8;
    SpriteResourcesHeader unk_E0C;
    Sprite *selectionArrows[2];
    Window msgWindow;
    Window rankWindow;
    Window opponentWindow;
    Window titleWindow;
    Window popupWindow;
    Window unk_E88[3];
    Window unk_EB8[11];
    Menu *yesNoMenu;
    void *waitDial;
    u8 unk_F10[64];
    int dwcError;
    int dwcErrorDetail;
    int dwcErrorType;
    int asyncResult;
    int waitTimer;
    WorldExchangeTrainer weTrainer;
    WorldExchangeTrainerError weTrainerErr;
    s16 subStep;
    s16 subTimer;
    int timeoutCounter;
    UnkStruct_ov61_0222C3B0 unk_FF8;
} WifiBattleTowerAppState;

enum ErrorCleanupSubStep {
    ERROR_CLEANUP_SUBSTEP_DISPLAY_ERROR,
    ERROR_CLEANUP_SUBSTEP_DISCONNECT,
    ERROR_CLEANUP_SUBSTEP_DISPLAY_RETURN,
    ERROR_CLEANUP_SUBSTEP_WAIT_RETURN,
};

int WifiBattleTower_GetSignalStrength(WifiBattleTowerAppState *);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C0F8(UnkStruct_ov61_0222C3B0 *param0) {
    return;
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

int WifiBattleTower_State_InitialPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_InitialYesNoMenu(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ConnectedYesNoMenu(WifiBattleTowerAppState *appState);

int ov73_021E8D0C(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitInetConnect(WifiBattleTowerAppState *appState);

int ov73_021E8DB8(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitNasLogin(WifiBattleTowerAppState *appState);

int ov73_021E8E7C(WifiBattleTowerAppState *appState);

int ov73_021E8EBC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_Idle(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_EndSession(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DisconnectYesNo(WifiBattleTowerAppState *appState);

int ov73_021E9EAC(WifiBattleTowerAppState *appState);

int ov73_021E9F00(WifiBattleTowerAppState *appState);

int ov73_021E9ECC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ShowError(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ErrorCleanup(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitInfo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ConnectedPrompt(WifiBattleTowerAppState *appState);


int ov73_021E9FC8(WifiBattleTowerAppState *appState);

int ov73_021E8FB0(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitSetProfile(WifiBattleTowerAppState *appState);

int ov73_021E9120(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitRoomCount(WifiBattleTowerAppState *appState);

int ov73_021E91F4(WifiBattleTowerAppState *appState);

int ov73_021E9224(WifiBattleTowerAppState *appState);

int ov73_021E926C(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitYesNo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_RequestDownload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitDownload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadCompletePrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_UploadPrompt(WifiBattleTowerAppState *appState);

int ov73_021E95CC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitUpload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_UploadCompletePrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_SelectRankPrompt(WifiBattleTowerAppState *appState);

int ov73_021E9758(WifiBattleTowerAppState *appState);

int ov73_021E97A8(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitMatchListPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitMatchListYesNo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_AlreadyChallengedPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadMatchListPrompt(WifiBattleTowerAppState *appState);

int ov73_021E987C(WifiBattleTowerAppState *appState);

int ov73_021E98AC(WifiBattleTowerAppState *appState);

int ov73_021E98FC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_RequestMatchList(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitMatchList(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_MatchListCompletePrompt(WifiBattleTowerAppState *appState);

int ov73_021E9B94(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_InitSave(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitSave(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DisconnectingPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_CleanupInet(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_GoodbyePrompt(WifiBattleTowerAppState *appState);

extern int (*ov72_0223B660[57])(WifiBattleTowerAppState *);

int ov72_0223894C(WifiBattleTowerAppState *appState, int unused);

int ov72_022387C4(WifiBattleTowerAppState *, int);

int ov72_0223894C(WifiBattleTowerAppState *appState, int unused) {
    sub_0203A930(ov72_022387C4(appState, unused));

    int prevState = appState->state;
    int result = (*ov72_0223B660[appState->state])(appState);

    if (prevState != appState->state) {
        appState->subStep = ERROR_CLEANUP_SUBSTEP_DISPLAY_ERROR;
        appState->subTimer = 0;
    }

    inline_ov61_0222C0F8(&appState->unk_FF8);

    return result;
}
