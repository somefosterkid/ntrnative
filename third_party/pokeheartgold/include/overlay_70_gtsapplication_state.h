#ifndef POKEHEARTGOLD_OVERLAY_70_GTSAPPLICATION_STATE_H
#define POKEHEARTGOLD_OVERLAY_70_GTSAPPLICATION_STATE_H
#include "sprite_resources_header.h"

#include <dwc.h>

#include "bg_window.h"
#include "g2d_renderer.h"
#include "pokemon_summary_args.h"
#include "pokemon_types_def.h"
#include "save_layout.h"
#include "unk_02009D48.h"

// GTSApplicationState: merged by tools/hg/promote.py from 32 m2c local copies (10 variants, base
// src/overlay_70_02238430_02238430.c) and 407 accesses; fields at every accessed offset, pads elsewhere.

struct OverlayManager;
struct BgConfig;
struct BoxPokemon;
struct ChatotCry;
struct EvolutionData;
struct GTSBoxPokemonCriteria;
struct GTSPlayerData;
struct ListMenu;
struct Menu;
struct MsgData;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteData;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct NNSiFndHeapHead;
struct Options;
struct Pokedex;

struct Sprite;
struct SpriteList;
struct SpriteResource;
struct GF_2DGfxResMan;
struct String;
struct StringList;
struct MessageFormat;
struct SysTask;
struct PlayerProfile;
struct Pokemon;
struct WiFiList;
struct YesNoPrompt;

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
    Pokemon pokemon;
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

typedef struct GTSCharpadScrollState {
    u16 unread_00;
    u16 unread_02;
    u16 cursorPos[10];
    u16 scrollPos[10];
} GTSCharpadScrollState;

// 0x64 bytes, layout-identical to Unk020317F4 (unk_0203170C.h); not included here because save.h clashes with the
// local SaveData copies of the m2c-era files that still include this header.
typedef struct WorldExchangeTrainer {
    u8 gameCode;
    u8 language;
    u8 country;
    u8 unk_03;
    u32 trainerId;
    u16 trainerName[8];
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

typedef struct TradeAnimationTemplate {
    struct BoxPokemon *sendingPokemon;
    struct BoxPokemon *receivingPokemon;
    struct PlayerProfile *otherTrainer;
    u32 background;
    u32 tradeType;
    struct Options *options;
} TradeAnimationTemplate;

typedef struct GTSSpeciesMenuState {
    struct String **unused_00;
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

// 0x12D0: 0x32C bytes (Platinum's UnkStruct_ov61_0222C3B0 layout)
typedef struct GTSApplicationState_Unk12D0 {
    struct SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} GTSApplicationState_Unk12D0;

// stack template that ov70_0223B55C / ov70_0223E78C fill and ov70_02242014 copies (0x30 bytes on the DS)
typedef struct GTSSearchPanelTemplate {
    struct BgConfig *bgConfig;
    Window *menuButtonWindows;
    Window *unk_08;
    struct Sprite *unk_0C;
    struct Sprite *unk_10;
    struct Sprite *unk_14;
    struct Sprite *cursorSprite;
    struct MsgData *gtsMessageLoader;
    struct MsgData *speciesMessageLoader;
    struct MsgData *countryMessageLoader;
    struct Pokedex *pokedex;
    u8 *dexSeenFlags;
} GTSSearchPanelTemplate;

// one box/party slot icon of the select-Pokemon screen (0x20C bytes on the DS), 30 of them in selectPokemonIconHeapPtr
typedef struct PokemonIcon {
    int tileData;
    int paletteIndex;
    struct Sprite *sprite;
    u8 vramOffset[512];
} PokemonIcon;

typedef struct GTSApplicationState {
    struct GTSPlayerData *playerData;
    struct BgConfig *bgConfig;
    struct WiFiList *unused_08;
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
    struct NNSiFndHeapHead *dwcHeapHandle;
    DWCInetControl dwcInetControlObject;
    struct OverlayManager *appMan;
    PokemonSummaryArgs pokemonSummary;
    TradeAnimationTemplate tradeAnimationConfig;
    struct EvolutionData *evolutionTask;
    BOOL appManActive;
    u16 listMenuCursorIndex;
    u16 unused_11A;
    u16 mainMenuSelectedOption;
    u16 selectedInfoTab;
    u16 selectedBoxId;
    u16 partySlotIndex;
    struct BoxPokemon *selectedBoxMon;
    int searchResultCount;
    int selectedSearchResult;
    struct PlayerProfile *receivingPokemonTrainer;
    GTSTradedPokemonLocation tradedPokemonLocation;
    GTSPokemonListing receivedListing;
    GTSPokemonListing searchResults[7];
    GTSPokemonListing selectedListing;
    GTSPokemonCriteria offeredPokemonCriteria;
    GTSPokemonRequirements depositRequirements;
    GTSPokemonRequirements searchRequirements;
    GTSPokemonRequirements submittedRequirements;
    u8 pad_B96[0x2];
    int submittedCountryIndex;
    struct MessageFormat *stringTemplate;
    struct MsgData *gtsMessageLoader;
    struct MsgData *speciesMessageLoader;
    struct MsgData *connProgressMessageLoader;
    struct MsgData *disconnStatusMessageLoader;
    struct MsgData *countryMessageLoader;
    struct String *selectPokemonBoxName;
    struct String *cancelButtonLabel;
    struct String *genericMessageBuffer;
    struct String *title;
    struct String *trainerLocationStrings[10];
    struct String *shortErrorBuffer;
    int textPrinter;
    struct SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    struct GF_2DGfxResMan *spriteResourceCollection[4];
    struct SpriteResource *spriteResource[3][4];
    SpriteResourcesHeader cursorSpriteResourceHeader;
    SpriteResourcesHeader avatarSpriteResourceHeader;
    SpriteResourcesHeader unused_DA8;
    struct Sprite *cursorSprite;
    struct Sprite *unused_DD0;
    struct Sprite *unk_DD4;
    struct Sprite *boxSlotIconSprites[30];
    struct Sprite *boxSlotItemSprites[30];
    struct Sprite *selectPokemonPartySprites[6];
    struct Sprite *listingCursorSprite;
    struct Sprite *avatarSprites[8];
    struct Sprite *boxArrowSprites[2];
    struct Sprite *unk_F0C;
    struct Sprite *unk_F10;
    s32 unk_F14;
    Window bottomInstructionWindow;
    Window unused_F28;
    Window titleWindow;
    Window networkErrWindow;
    Window menuButtonWindows[3];
    Window infoWindows[29];
    Window confirmationWindow;
    Window unk_1168;
    Window locationCriteriaWindows[2];
    Window unk_1198;
    struct UnkStruct_ov70_02242E58 *unk_11A8;
    struct ListMenuItem *unk_11AC;
    void *unk_11B0;
    struct NNSG2dCharacterData *unk_11B4;
    void *unk_11B8;
    struct WaitingIcon *unk_11BC;
    s32 unk_11C0;
    GTSSpeciesMenuState *speciesMenuState;
    struct YesNoPrompt *unk_11C8;
    struct TouchscreenListMenuSpawner *unk_11CC;
    void *unk_11D0;
    u16 unk_11D4;
    u16 unk_11D6;
    struct SysTask *playerAvatarAnimationTask;
    u16 hasAvatarFinishedMoving;
    u16 searchResultsVisible;
    void *avatarCharDataHeapPtr;
    struct NNSG2dCharacterData *avatarCharData;
    void *avatarPaletteHeapPtr;
    struct NNSG2dPaletteData *avatarPaletteData;
    struct Pokemon *tradeTempPokemon;
    struct GTSBoxPokemonCriteria *boxCriteria;
    u16 deferredBoxPokemonCount;
    u16 deferredBoxId;
    BOOL fadeBothScreens;
    PokemonIcon *selectPokemonIconHeapPtr;
    void *updateBoxPalettesFunc;
    void *unk_1208;
    s16 avatarPositions[8][2];
    u8 unk_122C[8];
    GTSCharpadScrollState charpadScrollState;
    WorldExchangeTrainer worldExchangeTrainer;
    WorldExchangeTrainerError worldExchangeTrainerError;
    int selectedCountryIndex;
    GTSApplicationState_Unk12D0 unk_12D0;
    u32 unk_15FC;
    s16 wfcDisconnectMessageIndex;
    s16 wfcDisconnectMessageFrameDelay;
    s32 networkTimeoutCounter;
} GTSApplicationState;

#ifndef SDK_PORT
LAYOUT_CHECK(GTSApplicationState_size, sizeof(GTSApplicationState) == 0x1608);
#endif

#endif // POKEHEARTGOLD_OVERLAY_70_GTSAPPLICATION_STATE_H
