#include "global.h"
#include "sprite.h"
#include "systask_environment.h"
#include "unk_02005D10.h"
struct ApplicationManager;
struct ApplicationManagerTemplate;
struct AvatarAnimationData;
struct BgConfig;
struct BoxPokemon;
struct ChatotCry;
struct ColoredArrow;
struct DWCstInetControl;
struct EvolutionData;
struct G2dRenderer;
struct GTSApplicationState;
struct GTSBoxPokemonCriteria;
struct GTSCharpadScrollState;
struct GTSPlayerData;
struct GTSPokemonCriteria;
struct GTSPokemonListing;
struct GTSPokemonListingMon;
struct GTSPokemonRequirements;
struct GTSTradedPokemonLocation;
struct ListMenu;
struct ListMenuTemplate;
struct Menu;
struct MessageLoader;
struct Options;
struct Pokemon;
struct PokemonSummary;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourcesHeader;
struct String;
struct StringList;
struct StringTemplate;
struct SysTask;
struct SysTaskManager;
struct TradeAnimationTemplate;
struct TrainerInfo;
struct WiFiList;
struct Window;
struct WorldExchangeTrainer;
struct WorldExchangeTrainerError;
struct tagDWCWDSData;
typedef enum {
    DWC_WDS_STATE_IDLE         = 0,
    DWC_WDS_STATE_PROCESS      = 1,
    DWC_WDS_STATE_COMPLETED    = 2,
    DWC_WDS_STATE_FAILED       = 3,
    DWC_WDS_STATE_ERROR        = 4
} DWCWDSState;
typedef struct tagDWCWDSData {
    u8 ssid[32 ];
    u8 wep[32 ];
    int wepMode;
    char apnum[10];
} DWCWDSData;
typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;



} DWCInetControl;
typedef struct WiFiList WiFiList;
typedef struct Options Options;
typedef struct ChatotCry ChatotCry;
typedef u16 charcode_t;
typedef struct String String;
typedef struct BoxPokemon BoxPokemon;
typedef struct Pokemon Pokemon;
typedef struct SysTaskManager SysTaskManager;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct TrainerInfo TrainerInfo;
typedef struct GTSPlayerData GTSPlayerData;
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
typedef struct StringTemplate StringTemplate;
typedef struct PokemonSummary {
    void *monData;
    Options *options;
    const charcode_t *OTName;
    u32 OTID;
    u8 OTGender;
    u8 dataType;
    u8 mode;
    u8 monMax;
    u8 monIndex;
    u8 pageFlags;
    u8 selectedMoveSlot;
    u8 returnMode;
    u16 move;

    u8 padding_1A[2];

    u32 dexMode;
    void *specialRibbons;
    void *poffin;
    ChatotCry *chatotCry;
    BOOL showContest;
} PokemonSummary;
enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};
enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};
typedef struct TradeAnimationTemplate {
    const BoxPokemon *sendingPokemon;
    const BoxPokemon *receivingPokemon;
    const TrainerInfo *otherTrainer;
    enum TradeBackground background;
    enum TradeType tradeType;
    const Options *options;
} TradeAnimationTemplate;
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
typedef struct ApplicationManager ApplicationManager;
typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);
typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;
struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};
typedef struct EvolutionData EvolutionData;
typedef struct ColoredArrow ColoredArrow;
typedef struct StringList StringList;
typedef struct ListMenu ListMenu;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct ListMenuTemplate {
    const StringList *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;
struct ListMenu {
    ListMenuTemplate template;
    struct {
        u8 textColorFg : 4;
        u8 textColorBg : 4;
        u8 textColorShadow : 4;
        u8 letterSpacing : 6;
        u8 dummy : 6;
        u8 fontID : 7;
        u8 prefer : 1;
    } altFont;
    ColoredArrow *cursor;
    u16 listPos;
    u16 cursorPos;
    u8 dummy2C;
    u8 dummy2D;
    u8 dummy2E;
    u8 lastAction;
    u8 heapID;
};
typedef struct Menu Menu;
typedef struct GTSCharpadScrollState {
    u16 unread_00;
    u16 unread_02;
    u16 cursorPos[9];
    u16 scrollPos[9];
} GTSCharpadScrollState;
typedef struct GTSPokemonListingMon {
    u8 bytes[236];
} GTSPokemonListingMon;
typedef struct GTSPokemonCriteria {
    s16 species;
    s8 gender;
    s8 level;
} GTSPokemonCriteria;
typedef struct GTSPokemonRequirements {
    s16 species;
    s8 gender;
    s8 level;
    s8 level2;
    s8 unset_05;
} GTSPokemonRequirements;
typedef struct GTSPokemonListing {
    GTSPokemonListingMon pokemon;
    GTSPokemonCriteria criteria;
    GTSPokemonRequirements requirements;
    u8 trainerGender;
    u8 unused[17];
    s32 listingId;
    u16 trainerNames[8];
    u16 trainerId;
    u8 trainerCountry;
    u8 trainerRegion;
    u8 trainerAppearance;
    s8 exchangedFromRemote;
    u8 gameVersion;
    u8 trainerLanguage;
} GTSPokemonListing;
typedef struct GTSTradedPokemonLocation {
    int boxIndex;
    int index;
} GTSTradedPokemonLocation;
typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;
typedef struct {
    String **unused_00;
    u16 charpadCursorPos;
    u16 charpadScrollPos;
    u16 speciesListCursorPos;
    u16 speciesListScrollPos;
    int unused_0C;
    int unused_10;
    u8 *dexSeenFlags;
    u16 *alphabeticalSpecies;
    int alphabeticalSpeciesCount;
    int selectedSpeciesGenderRatio;
    int requirementsActivePanel;
    int leftPanelCursorPos;
    int rightPanelCursorPos;
} GTSSpeciesMenuState;
typedef struct GTSApplicationState {
    GTSPlayerData *playerData;
    BgConfig *bgConfig;
    WiFiList *unused_08;
    int unused_0C;
    int unused_10;
    int screenId;
    int nextScreen;
    int returnAfterNetworkScreen;
    int previousScreen;
    int screenArgument;
    int depositReturnError;
    int currentScreenInstruction;
    int nextScreenInstruction;
    u16 hasPlayerDescended;
    u16 isPokemonListed;
    u16 unused_38;
    u16 networkTimer;
    int commsErrorMessage;
    int dwcErr;
    int dwcErrCode;
    int dwcErrType;
    void *dwcHeapPointer;
    NNSFndHeapHandle dwcHeapHandle;
    DWCInetControl dwcInetControlObject;
    ApplicationManager *appMan;
    PokemonSummary pokemonSummary;
    TradeAnimationTemplate tradeAnimationConfig;
    EvolutionData *evolutionData;
    BOOL appManActive;
    u16 listMenuCursorIndex;
    u16 unused_10A;
    u16 mainMenuSelectedOption;
    u16 selectedInfoTab;
    u16 selectedBoxId;
    u16 partySlotIndex;
    BoxPokemon *selectedBoxMon;
    int searchResultCount;
    int selectedSearchResult;
    TrainerInfo *receivingPokemonTrainer;
    GTSTradedPokemonLocation tradedPokemonLocation;
    GTSPokemonListing receivedListing;
    GTSPokemonListing searchResults[7];
    GTSPokemonListing selectedListing;
    GTSPokemonCriteria offeredPokemonCriteria;
    GTSPokemonRequirements depositRequirements;
    GTSPokemonRequirements searchRequirements;
    GTSPokemonRequirements submittedRequirements;
    int submittedCountryIndex;
    StringTemplate *stringTemplate;
    MessageLoader *gtsMessageLoader;
    MessageLoader *speciesMessageLoader;
    MessageLoader *connProgressMessageLoader;
    MessageLoader *disconnStatusMessageLoader;
    MessageLoader *countryMessageLoader;
    String *selectPokemonBoxName;
    String *cancelButtonLabel;
    String *genericMessageBuffer;
    String *title;
    String *trainerLocationStrings[10];
    String *shortErrorBuffer;
    int textPrinter;
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    SpriteResourceCollection *spriteResourceCollection[4];
    SpriteResource *spriteResource[3][4];
    SpriteResourcesHeader cursorSpriteResourceHeader;
    SpriteResourcesHeader avatarSpriteResourceHeader;
    SpriteResourcesHeader unused_DFC;
    Sprite *cursorSprite;
    Sprite *unused_E24;
    Sprite *boxSlotIconSprites[30];
    Sprite *boxSlotItemSprites[30];
    Sprite *selectPokemonPartySprites[6];
    Sprite *listingCursorSprite;
    u8 pad_ED4[16];
    Sprite *avatarSprites[8];
    u8 hk9pad0_76[0x13C]; /* invented: asm offset */ Sprite *boxArrowSprites[2];
    Window bottomInstructionWindow;
    Window unused_F6C;
    Window titleWindow;
    Window networkErrWindow;
    Window menuButtonWindows[3];
    Window infoWindows[13];
    Window confirmationWindow;
    Window locationCriteriaWindows[2];
    StringList *menuStringList;
    Menu *yesNoMenu;
    Menu *popupMenu;
    ListMenu *activeListMenu;
    void *waitDial;
    int frameDelay;
    GTSSpeciesMenuState *speciesMenuState;
    u16 duringSaveInstruction;
    u16 successfulSaveInstruction;
    SysTask *playerAvatarAnimationTask;
    u16 hasAvatarFinishedMoving;
    u16 searchResultsVisible;
    void *avatarCharDataHeapPtr;
    NNSG2dCharacterData *avatarCharData;
    void *avatarPaletteHeapPtr;
    NNSG2dPaletteData *avatarPaletteData;
    Pokemon *tradeTempPokemon;
    GTSBoxPokemonCriteria *boxCriteria;
    u16 deferredBoxPokemonCount;
    u16 deferredBoxId;
    BOOL fadeBothScreens;
    void *selectPokemonIconHeapPtr;
    void (*updateBoxPalettesFunc)(void *param0);
    GTSCharpadScrollState charpadScrollState;
    WorldExchangeTrainer worldExchangeTrainer;
    WorldExchangeTrainerError worldExchangeTrainerError;
    int selectedCountryIndex;
    UnkStruct_ov61_0222C3B0 unk_11B4;
    s16 wfcDisconnectMessageIndex;
    s16 wfcDisconnectMessageFrameDelay;
    s32 networkTimeoutCounter;
} GTSApplicationState;
typedef struct AvatarAnimationData {
    int state;
    int y;
    int gender;
    GTSApplicationState *appState;
} AvatarAnimationData;
enum AvatarAnimState {
    AVATARANIM_INITIAL_SLIDE,
    AVATARANIM_WAIT_FOR_ANIM,
    AVATARANIM_SECONDARY_SLIDE,
    AVATARANIM_FINISH
};
void ov70_022410F0(Sprite *sprite, int x, int y);
void ov70_02240F3C(AvatarAnimationData *animData, int animIndex);
void ov70_022410F0(Sprite *sprite, int x, int y);

void ov70_02241048(SysTask *task, void *param);
void ov70_02241048(SysTask *task, void *param)
{
  AvatarAnimationData *animData = (AvatarAnimationData *) param;
  GTSApplicationState *appState = animData->appState;
  switch (animData->state)
  {
    case AVATARANIM_INITIAL_SLIDE:
      if (animData->y > 160)
    {
      animData->y = 160;
      animData->state = AVATARANIM_WAIT_FOR_ANIM;
      ov70_02240F3C(animData, 6);
    }
      animData->y += 2;
      ov70_022410F0(appState->avatarSprites[0], 128, animData->y);
      break;

    case AVATARANIM_WAIT_FOR_ANIM:
      if (!Sprite_IsAnimated(appState->avatarSprites[0]))
    {
      ov70_02240F3C(animData, 0);
      animData->state = AVATARANIM_SECONDARY_SLIDE;
      PlaySE(1583);
    }
      break;

    case AVATARANIM_SECONDARY_SLIDE:
      if (animData->y < (-20))
    {
      animData->state = AVATARANIM_FINISH;
      ov70_02240F3C(animData, 3);
    }
      animData->y -= 5;
      ov70_022410F0(appState->avatarSprites[0], 128, animData->y);
      break;

    case AVATARANIM_FINISH:
      animData->appState->hasAvatarFinishedMoving = 1;
      DestroySysTaskAndEnvironment(task);
      break;

  }

}


