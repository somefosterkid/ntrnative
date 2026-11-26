#include "global.h"
#include "pokemon_summary_args.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_bgm.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sys_task.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "unk_0208F658.h"
#include "wild_encounter.h"
#include "overlay_41_sprite_resource_collection.h"
#include "struct_defs/resdat_narc_structs.h"

typedef struct ListMenuCursor ListMenuCursor;

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenu2D ListMenu2D;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct ChatotCry ChatotCry;

typedef struct PalPad PalPad;

typedef struct JournalEntryTitle JournalEntryTitle;

typedef struct JournalEntryMon JournalEntryMon;

typedef struct JournalEntryTrainer JournalEntryTrainer;

typedef struct JournalEntry JournalEntry;

typedef struct EncodingSeed EncodingSeed;

typedef struct GameStats GameStats;

typedef struct TradeRoom TradeRoom;

typedef struct WiFiHistory WiFiHistory;

typedef struct TradeRoomMonDisplayData TradeRoomMonDisplayData;

typedef struct TradeRoomArgs TradeRoomArgs;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_02095E80_sub1;

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
};

struct PalPad {
    u16 trainerName[7 + 1];
    u32 trainerId;
    u8 language;
    u8 gameCode;
    u8 gender;
    u8 padding_17;

    u32 associatedTrainerIds[16];
    u8 associatedTrainerGameCodes[16];
    u8 associatedTrainerLanguages[16];
    u8 associatedTrainerGenders[16];
};

struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
};

struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
};

struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
};

struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
};

struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
};

struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
};

struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
};

struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
};

struct TradeRoomArgs {
    FieldSystem *fieldSystem;
    PlayerProfile *trainerInfo;
    Party *party;
    PalPad *palPad;
    SaveData *saveData;
    WiFiHistory *wiFiHistory;
    Options *options;
    JournalEntry *journalEntry;
    GameStats *records;
    Pokedex *pokedex;
    int tradeCompleted;
    int receivingPartySlot;
    int dexMode;
    int tradeCount;
    PlayerProfile *partnerTrainerInfoCopy;
    Pokemon *sendingMon;
    Pokemon *receivingMon;
};

struct TradeRoom {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    TradeRoomArgs *args;
    PokemonSummaryArgs summaryArgs;
    int summarySide;
    OverlayManager *appMan;
    int subAppActive;
    int phase;
    int connectStep;
    int partySendCount;
    int commMilestone;
    int partyReceiveCount;
    int didConfirmTrade;
    int partnerSyncStatus[2];
    int notificationMsgId;
    BOOL unk_6C;
    UnkStruct_02095E80_sub1 unk_70;
    int selectedSlot[2];
    u16 cursorGlowAngle;
    u16 unk_92[5][17];
    u16 unk_13C;
    u16 unk_13E;
    int subStepResult;
    u16 unk_144[3];
    u32 pendingDirection[2];
    u32 unk_154[2][3];
    int unk_16C;
    int unk_170;
    BgConfig *bgConfig;
    MessageFormat *strTemplate;
    MessageFormat *strTemplate2;
    MessageFormat *unread_180;
    MsgData *msgLoader;
    String *unk_188;
    String *playerNameStr;
    String *itemLabelStr;
    SpriteList *spriteList;
    G2dRenderer spriteRenderer;
    SpriteResourceCollection *spriteResourceCollections[4];
    SpriteResource *spriteResources[2][4];
    SpriteResourcesHeader mainSpriteResourcesHeader;
    SpriteResourcesHeader subSpriteResourcesHeader;
    Sprite *cursorSprites[14];
    Sprite *iconSprites[12];
    Sprite *itemIconSprites[12];
    Sprite *capsuleIconSprites[12];
    Sprite *detailCardSprites[6];
    Sprite *convergeIndicatorSprite;
    SysTask *unk_480[7];
    Window windows[34];
    ListMenu2D *yesNoMenu;
    ListMenuItem *actionMenuChoices;
    ListMenu2D *actionMenu;
    int yesNoMenuStep;
    int unk_6CC;
    int unk_6D0;
    int unk_6D4;
    VecFx32 unk_6D8[2];
    int unk_6F0;
    TradeRoomMonDisplayData monDisplayData[13];
    int unk_7B8;
    int unk_7BC;
    int unk_7C0;
    int unk_7C4[7];
    void *unk_7E0;
    NNSG2dCharacterData *unk_7E4;
    void *iconGraphicsBuffers[12];
    NNSG2dCharacterData *iconCharacterData[12];
    u8 previewSpritePixelBuf[2][3200];
    PokepicTemplate previewSpriteTemplate[2];
    int pendingPreviewLoadSide;
    u8 unk_216C[256];
    int (*subStepCallback)(TradeRoom *);
    Party *playerParty;
    Party *partnerParty;
    ChatotCry *unk_2278;
    PalPad *palPad;
    PalPad palPadNetworkObject;
    int unk_2308;
    int unk_230C;
    int bgScrollOffset;
    int slideAnimFrame;
    int staggerCountdown;
    VecFx32 playerSlidePath[2];
    VecFx32 partnerSlidePath[2];
    u8 partyTransferBuffer[2][1424];
    u8 chatotCryBuffer[2][1004];
    int pendingRegistrationFlags[32];
    int registrationTargetNetId;
    int friendToDeleteSlot;
    MessageFormat *palPadStrTemplate;
    MsgData *palPadMsgLoader;
    ListMenuItem *friendListChoices;
    ListMenu *friendListMenu;
    Window friendListWindow;
    WiFiList *wifiList;
    int tradeCompletedCount;
    int syncSaveState;
    int lastSyncedValue;
    int lastSyncedCmd;
    void *waitDialHandle;
    int connectionTimeoutArmed;
    int connectionTimeoutFrames;
};

void ov65_0221CE98(void);

void ov65_0221CEB8(BgConfig *bgConfig);

void ov65_0221D3B8(void);

void ov65_0221D930(TradeRoom *tradeRoom);

void ov65_0221CE1C(void *);

s32 ov65_0221D0EC(TradeRoom *, OverlayManager *);

s32 ov65_0221D280(TradeRoom *, NARC *);

s32 ov65_0221D3E8(TradeRoom *, NARC *);

s32 ov65_0221F8D0(BgConfig *, Window *, Options *);

u8 ov65_0221FB90(Window *, s32, s32, MsgData *, MessageFormat *);

s32 WirelessTradeSelectMon_Init(OverlayManager *man, s32 *state);

s32 WirelessTradeSelectMon_Init(OverlayManager *man, s32 *state) {
    FieldSystem *temp_r0_2;
    NARC *temp_r5;
    TradeRoom *temp_r0;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    Heap_Create(HEAP_ID_3, HEAP_ID_26, 0x70FA0U);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_26);
    temp_r5 = NARC_New(NARC_a_0_5_2, HEAP_ID_26);
    temp_r0 = OverlayManager_CreateAndGetData(man, 0x36CCU, HEAP_ID_26);
    MIi_CpuClearFast(0U, temp_r0, 0x36CCU);
    temp_r0->bgConfig = BgConfig_Alloc(HEAP_ID_26);
    temp_r0->strTemplate = (struct MessageFormat *)(MessageFormat_New_Custom(0xCU, 0x16U, HEAP_ID_26));
    temp_r0->strTemplate2 = (struct MessageFormat *)(MessageFormat_New(HEAP_ID_26));
    temp_r0->unread_180 = (struct MessageFormat *)(MessageFormat_New(HEAP_ID_26));
    temp_r0->msgLoader = (struct MsgData *)(NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xB7, HEAP_ID_26));
    temp_r0->appMan = NULL;
    ov65_0221D0EC(temp_r0, man);
    SetKeyRepeatTimers(4, 8);
    ov65_0221CE98();
    ov65_0221CEB8(temp_r0->bgConfig);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 0x10, 1, HEAP_ID_26);
    ov65_0221D280(temp_r0, temp_r5);
    Main_SetVBlankIntrCB(ov65_0221CE1C, temp_r0);
    ov65_0221D3B8();
    ov65_0221D3E8(temp_r0, temp_r5);
    ov65_0221D930(temp_r0);
    ov65_0221F8D0(temp_r0->bgConfig, &temp_r0->windows[0], temp_r0->args->options);
    temp_r0_2 = temp_r0->args->fieldSystem;
    temp_r0->fieldSystem = temp_r0_2;
    sub_0208F7E0(temp_r0_2, temp_r0);
    ov65_0221FB90(&temp_r0->windows[0x17], 0x14, 1, temp_r0->msgLoader, temp_r0->strTemplate);
    sub_0208F658(temp_r0->fieldSystem);
    ToggleBgLayer(0U, 1U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    sub_020399FC(HEAP_ID_26, temp_r0->bgConfig);
    sub_0203A880();
    IsNighttime();
    sub_02055198(NULL, 0x427U);
    sub_02005448(0x427);
    NARC_Delete(temp_r5);
    return 1;
}
