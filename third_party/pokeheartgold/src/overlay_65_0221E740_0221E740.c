#include "global.h"
#include "pokemon_summary_args.h"

#include "constants/heap.h"

#include "bag.h"
#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "wild_encounter.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "sav_chatot.h"
#include "save_palpad.h"
#include "struct_defs/resdat_narc_structs.h"
#include "game_stats.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct JournalEntryTitle JournalEntryTitle;

typedef struct JournalEntryMon JournalEntryMon;

typedef struct JournalEntryTrainer JournalEntryTrainer;

typedef struct JournalEntry JournalEntry;

typedef struct WiFiHistory WiFiHistory;

typedef struct EncodingSeed EncodingSeed;

typedef struct UnkStruct_ov65_0221E740_0221E740_1B61 UnkStruct_ov65_0221E740_0221E740_1B61;

typedef struct TradeRoomMonDisplayData TradeRoomMonDisplayData;

typedef struct TradeRoomArgs TradeRoomArgs;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
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

struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
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

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_02095E80_sub1;

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
    SavePalPad *palPad;
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

struct UnkStruct_ov65_0221E740_0221E740_1B61 {
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
    int (*subStepCallback)(UnkStruct_ov65_0221E740_0221E740_1B61 *);
    Party *playerParty;
    Party *partnerParty;
    SOUND_CHATOT *unk_2278;
    SavePalPad *palPad;
    SavePalPad palPadNetworkObject;
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

s32 ov65_0221E330(UnkStruct_ov65_0221E740_0221E740_1B61 *, s32);

u8 ov65_0221FB90(Window *, s32, s32, MsgData *, MessageFormat *);

s32 sub_0202C2DC(WiFiList *, s32);

int ov65_0221E684(UnkStruct_ov65_0221E740_0221E740_1B61 *);

int ov65_0221E72C(UnkStruct_ov65_0221E740_0221E740_1B61 *);

int ov65_0221E740(UnkStruct_ov65_0221E740_0221E740_1B61 *tradeRoom);

int ov65_0221E740(UnkStruct_ov65_0221E740_0221E740_1B61 *tradeRoom) {
    UnkStruct_ov65_0221E740_0221E740_1B61 *var_r4;
    WiFiList *temp_r6;
    s32 var_r4_2;
    s32 var_r6;

    tradeRoom->registrationTargetNetId = -1;
    var_r6 = 0;
    if (sub_02037454() > 0) {
        var_r4 = tradeRoom;
loop_2:
        if (var_r4->pendingRegistrationFlags[0] == 2) {
            tradeRoom->registrationTargetNetId = var_r6;
            *(s32 *)((u8 *)(((u8 *)tradeRoom) + (var_r6 * 4)) + 0x35F8) = 0;
        } else {
            var_r4 = (void *)((u8 *)var_r4 + (4));
            var_r6 += 1;
            if (var_r6 < sub_02037454()) {
                goto loop_2;
            }
        }
    }
    if (tradeRoom->registrationTargetNetId == -1) {
        DestroyMsgData((MsgData *)tradeRoom->palPadMsgLoader);
        MessageFormat_Delete((MessageFormat *)tradeRoom->palPadStrTemplate);
        sub_02037AC0(0x13U);
        ov65_0221FB90(&tradeRoom->windows[0x17], 0x1C, 1, tradeRoom->msgLoader, tradeRoom->strTemplate);
        tradeRoom->subStepCallback = ov65_0221E72C;
        return 0;
    }
    if (Bag_HasItem(Save_Bag_Get(tradeRoom->saveData), 0x1B5U, 1U, HEAP_ID_26) == 1) {
        BufferPlayersName((MessageFormat *)tradeRoom->palPadStrTemplate, 0U, sub_02034818((u32)tradeRoom->registrationTargetNetId));
        ov65_0221E330(tradeRoom, 0x3E);
        tradeRoom->subStepCallback = ov65_0221E684;
        return 0;
    }
    temp_r6 = sub_0202C6F4(tradeRoom->saveData);
    var_r4_2 = 0;
loop_10:
    if (sub_0202C2DC(temp_r6, var_r4_2) == 0) {
        sub_0203A280(tradeRoom->saveData, tradeRoom->registrationTargetNetId, var_r4_2, HEAP_ID_26, 0U);
    } else {
        var_r4_2 += 1;
        if (var_r4_2 < 0x20) {
            goto loop_10;
        }
    }
    tradeRoom->subStepCallback = ov65_0221E740;
    return 0;
}
