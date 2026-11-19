#ifndef POKEHEARTGOLD_OVERLAY_73_WIFI_BATTLE_TOWER_APP_STATE_H
#define POKEHEARTGOLD_OVERLAY_73_WIFI_BATTLE_TOWER_APP_STATE_H

#include <dwc.h>

#include "bg_window.h"
#include "g2d_renderer.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_72.h"
#include "overlay_72_wfc_state.h"
#include "overlay_80_frontier_opponent.h"
#include "save_frontier.h"
#include "sprite_resources_header.h"
#include "unk_0202D230.h"

// WifiBattleTowerAppState: the Nintendo WFC / Wi-Fi battle tower connect app's data (ov73_021E7E4C allocates 0x12cc).

struct BoxPokemon;
struct Menu;
struct MessageFormat;
struct MsgData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct GF_2DGfxResMan;
struct String;
struct SysTask;
struct YesNoPrompt;


typedef struct WifiPlayerProfile {
    FrontierPokemon mons[3];
    u16 name[7 + 1];
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

enum WifiBattleTowerMode {
    WIFI_BT_MODE_BROWSE,
    WIFI_BT_MODE_UPLOAD,
    WIFI_BT_MODE_SELECT_RANK,
};

typedef struct UnkStruct_ov61_0222C3B0 {
    struct SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct WifiBattleTowerAppState {
    NintendoWifiConnectArgs *args;
    struct BgConfig *bgConfig;
    int unk_08;
    int unk_0C;
    int screenMode;
    int exitMode;
    int unused_18;
    enum NintendoWfcState state;
    enum NintendoWfcState nextState;
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
    struct BoxPokemon *unk_BC0;
    int unk_BC4;
    int unk_BC8;
    struct MessageFormat *stringTemplate;
    struct MsgData *msgLoader1;
    struct MsgData *msgLoader2;
    struct MsgData *msgLoader3;
    struct String *msgString;
    struct String *titleString;
    struct String *opponentIdxString;
    struct String *rankString;
    struct String *errorString;
    int textPrinterID;
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resourceCollection[4];
    struct SpriteResource *spriteResource[4][3];
    SpriteResourcesHeader arrowSpriteHeader;
    SpriteResourcesHeader unk_DE8;
    SpriteResourcesHeader unk_E0C;
    struct Sprite *selectionArrows[2];
    Window msgWindow;
    Window rankWindow;
    Window opponentWindow;
    Window titleWindow;
    Window popupWindow;
    Window unk_E88[3];
    Window unk_EB8[11];
    struct Menu *yesNoMenu;
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
    u32 unk_12C4;
    struct YesNoPrompt *yesNoPrompt;
} WifiBattleTowerAppState;

// sizeof(WifiBattleTowerAppState) == 0x12cc (ov73_021E7E4C allocates 0x12cc)

#endif // POKEHEARTGOLD_OVERLAY_73_WIFI_BATTLE_TOWER_APP_STATE_H
