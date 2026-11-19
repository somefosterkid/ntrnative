#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"

struct BgConfig;

struct BoxPokemon;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct DWCstInetControl;

struct FrontierPokemon;

struct G2dRenderer;

struct Menu;

struct MessageLoader;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct SystemData;

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


enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};

typedef u16 charcode_t;

typedef struct String String;

typedef struct SaveData SaveData;

typedef struct Options Options;

typedef struct BoxPokemon BoxPokemon;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct StringTemplate StringTemplate;

typedef struct FrontierPokemon {
    struct {
        u16 species : 11;
        u16 form : 5;
    };
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 personality;
    union {
        struct {
            u32 hpIV : 5;
            u32 atkIV : 5;
            u32 defIV : 5;
            u32 speedIV : 5;
            u32 spAtkIV : 5;
            u32 spDefIV : 5;
            u32 setSpeciesAsNickname : 1;
            u32 : 1;
        };
        u32 combinedIVs;
    };
    union {
        struct {
            u8 hpEV;
            u8 atkEV;
            u8 defEV;
            u8 speedEV;
            u8 spAtkEV;
            u8 spDefEV;
        };
        u8 evList[STAT_MAX];
    };
    union {
        struct {
            u8 move0PPUps : 2;
            u8 move1PPUps : 2;
            u8 move2PPUps : 2;
            u8 move3PPUps : 2;
        };
        u8 combinedPPUps;
    };
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[10 + 1];
} FrontierPokemon;

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

typedef struct WifiBattleTowerMatchCandidate {
    u16 trainerName[8];
    u8 padding_10[2];
    u8 country;
    u8 region;
    u8 padding_14[4];
    u16 trainerKey[4];
    u8 isAnonymous : 1;
    u8 anonymousNameIdx : 1;
} WifiBattleTowerMatchCandidate;

typedef struct WifiBattleTowerDownloadBuffer {
    WifiPlayerProfile wifiPlayerProfile[7];
    WifiBattleTowerMatchCandidate matchCandidates[30];
} WifiBattleTowerDownloadBuffer;

typedef struct WifiBattleTowerDownloadData WifiBattleTowerDownloadData;

typedef struct WifiBattleTowerRecord WifiBattleTowerRecord;

typedef struct SystemData SystemData;

s32 Save_SysInfo_GetDwcProfileId(const SystemData *systemData);

void Save_SysInfo_SetDwcProfileId(SystemData *systemData, s32 profileId);

typedef struct {
    WifiBattleTowerRecord *record;
    WifiBattleTowerDownloadData *downloadData;
    SystemData *systemData;
    SaveData *saveData;
    Options *options;
    DWCUserData *userData;
    int mode;
    int profileId;
    int unk_20;
    int unk_24;
} UnkStruct_0206BC70;

typedef struct WorldExchangeTrainer {
    u8 gameCode;
    u8 language;
    u8 country;
    u8 unk_03;
    u32 trainerId;
    u16 trainerName[7 + 1];
    u32 unk_10;
    u8 macAddress[6];
    u8 unk_1A[2];
    char email[56];
    u32 emailInitialised;
    u16 rngValue;
    u16 unk_5A;
} WorldExchangeTrainer;

typedef struct WorldExchangeTrainerError {
    u32 validationError;
    u32 systemError;
} WorldExchangeTrainerError;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct MessageLoader MessageLoader;

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
    StringTemplate *stringTemplate;
    MessageLoader *msgLoader1;
    MessageLoader *msgLoader2;
    MessageLoader *msgLoader3;
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

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};

void ov72_022378C0(s32 profileId, u64 friendKey);

int ov72_02239370(WifiBattleTowerAppState *appState);

int ov72_02239370(WifiBattleTowerAppState *appState) {
    s32 profileId = Save_SysInfo_GetDwcProfileId(appState->args->systemData);

    if (profileId == 0) {
        Save_SysInfo_SetDwcProfileId(appState->args->systemData, appState->args->profileId);
    }

    profileId = Save_SysInfo_GetDwcProfileId(appState->args->systemData);
    ov72_022378C0(profileId, DWC_CreateFriendKey(appState->args->userData));

    appState->state = BT_STATE_REQUEST_INFO;

    return BT_LOOP_STATE_MAIN;
}
