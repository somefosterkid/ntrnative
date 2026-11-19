#include "global.h"
#include "pokemon_summary_args.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "list_menu.h"
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
#include "wild_encounter.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_65_trade_room.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sav_chatot.h"
#include "save_palpad.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"
#include "game_stats.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct JournalEntryTitle JournalEntryTitle;

typedef struct JournalEntryMon JournalEntryMon;

typedef struct JournalEntryTrainer JournalEntryTrainer;

typedef struct JournalEntry JournalEntry;

typedef struct WiFiHistory WiFiHistory;

typedef struct EncodingSeed EncodingSeed;

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

void ov65_0221DD34(int slot, Sprite *sprite, int cursorIdx);

extern u8 ov65_0221FEA4;

extern void *ov65_0221FF4C;

void ov65_0221D930(TradeRoom *tradeRoom);

void ov65_0221D930(TradeRoom *tradeRoom) {
    SpriteResourcesHeader *pSVar1;
    Sprite *pSVar2;
    TradeRoom *pTVar3;
    s32 *piVar4;
    s32 iVar5;
    SpriteTemplate SStack_44;
    CreateSpriteResourcesHeader(&tradeRoom->mainSpriteResourcesHeader, 0, 0, 0, 0, -1, -1, 0, 2, (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[0], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[1], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[2], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    CreateSpriteResourcesHeader(&tradeRoom->subSpriteResourcesHeader, 1, 1, 1, 1, -1, -1, 0, 0, (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[0], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[1], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[2], (GF_2DGfxResMan *)tradeRoom->spriteResourceCollections[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_44.spriteList = tradeRoom->spriteList;
    SStack_44.header = &tradeRoom->mainSpriteResourcesHeader;
    SStack_44.position.x = 0x20000;
    SStack_44.position.y = 0x60000;
    iVar5 = 0;
    SStack_44.position.z = 0;
    SStack_44.scale.x = 0x1000;
    SStack_44.scale.y = 0x1000;
    SStack_44.scale.z = 0x1000;
    SStack_44.rotation = 0;
    SStack_44.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    piVar4 = (s32 *)(&ov65_0221FF4C);
    SStack_44.drawPriority = 0;
    SStack_44.heapID = HEAP_ID_26;
    pTVar3 = tradeRoom;
    do {
        SStack_44.position.x = (*piVar4) << 0xc;
        SStack_44.position.y = piVar4[1] << 0xc;
        pSVar2 = Sprite_CreateAffine(&SStack_44);
        pTVar3->cursorSprites[0] = pSVar2;
        Sprite_SetAnimActiveFlag(pTVar3->cursorSprites[0], 1);
        ov65_0221DD34(pTVar3->selectedSlot[0], pTVar3->cursorSprites[0], iVar5);
        Sprite_SetDrawPriority(pTVar3->cursorSprites[0], 100);
        Sprite_SetDrawFlag(pTVar3->cursorSprites[0], 0);
        Sprite_SetOamMode(pTVar3->cursorSprites[0], GX_OAM_MODE_XLU);
        piVar4 = piVar4 + 2;
        pTVar3 = (TradeRoom *)(&pTVar3->saveData);
        iVar5 = iVar5 + 1;
    } while (iVar5 < 2);
    piVar4 = (s32 *)(&ov65_0221FF4C);
    iVar5 = 0;
    pTVar3 = tradeRoom;
    do {
        SStack_44.position.x = ((*piVar4) + 0x10) * 0x1000;
        SStack_44.position.y = (piVar4[1] + (-6)) * 0x1000;
        pSVar2 = Sprite_CreateAffine(&SStack_44);
        pTVar3->iconSprites[0] = pSVar2;
        Sprite_SetAnimActiveFlag(pTVar3->iconSprites[0], 1);
        Sprite_SetAnimCtrlSeq(pTVar3->iconSprites[0], iVar5 + 5);
        Sprite_SetDrawPriority(pTVar3->iconSprites[0], 5);
        Sprite_SetDrawFlag(pTVar3->iconSprites[0], 0);
        Sprite_SetOamMode(pTVar3->iconSprites[0], GX_OAM_MODE_XLU);
        iVar5 = iVar5 + 1;
        piVar4 = piVar4 + 2;
        pTVar3 = (TradeRoom *)(&pTVar3->saveData);
    } while (iVar5 < 0xc);
    piVar4 = (s32 *)(&ov65_0221FF4C);
    iVar5 = 0;
    pTVar3 = tradeRoom;
    do {
        SStack_44.position.x = ((*piVar4) + 0x24) * 0x1000;
        SStack_44.position.y = (piVar4[1] + 0x10) * 0x1000;
        SStack_44.drawPriority = 0;
        pSVar2 = Sprite_CreateAffine(&SStack_44);
        pTVar3->itemIconSprites[0] = pSVar2;
        Sprite_SetDrawPriority(pTVar3->itemIconSprites[0], 3);
        Sprite_SetDrawFlag(pTVar3->itemIconSprites[0], 0);
        Sprite_SetOamMode(pTVar3->itemIconSprites[0], GX_OAM_MODE_XLU);
        iVar5 = iVar5 + 1;
        piVar4 = piVar4 + 2;
        pTVar3 = (TradeRoom *)(&pTVar3->saveData);
    } while (iVar5 < 0xc);
    piVar4 = (s32 *)(&ov65_0221FF4C);
    iVar5 = 0;
    pTVar3 = tradeRoom;
    do {
        SStack_44.position.x = ((*piVar4) + 0x2d) * 0x1000;
        SStack_44.position.y = (piVar4[1] + 0x10) * 0x1000;
        SStack_44.drawPriority = 0;
        pSVar2 = Sprite_CreateAffine(&SStack_44);
        pTVar3->capsuleIconSprites[0] = pSVar2;
        Sprite_SetDrawPriority(pTVar3->capsuleIconSprites[0], 3);
        Sprite_SetDrawFlag(pTVar3->capsuleIconSprites[0], 0);
        Sprite_SetOamMode(pTVar3->capsuleIconSprites[0], GX_OAM_MODE_XLU);
        iVar5 = iVar5 + 1;
        piVar4 = piVar4 + 2;
        pTVar3 = (TradeRoom *)(&pTVar3->saveData);
    } while (iVar5 < 0xc);
    SStack_44.position.x = 0x60000;
    SStack_44.position.y = 0x42000;
    pSVar2 = Sprite_CreateAffine(&SStack_44);
    tradeRoom->convergeIndicatorSprite = pSVar2;
    Sprite_SetAnimActiveFlag(tradeRoom->convergeIndicatorSprite, 1);
    Sprite_SetAnimCtrlSeq(tradeRoom->convergeIndicatorSprite, 0x14);
    Sprite_SetDrawFlag(tradeRoom->convergeIndicatorSprite, 0);
    piVar4 = (s32 *)(&ov65_0221FEA4);
    iVar5 = 0;
    pSVar1 = &tradeRoom->subSpriteResourcesHeader;
    do {
        SStack_44.position.x = (*piVar4) << 0xc;
        SStack_44.position.y = (piVar4[1] * 0x1000) + 0xc0000;
        SStack_44.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        SStack_44.header = pSVar1;
        pSVar2 = Sprite_CreateAffine(&SStack_44);
        tradeRoom->detailCardSprites[0] = pSVar2;
        Sprite_SetDrawFlag(tradeRoom->detailCardSprites[0], 0);
        Sprite_SetAnimActiveFlag(tradeRoom->detailCardSprites[0], 0);
        Sprite_SetAnimCtrlSeq(tradeRoom->detailCardSprites[0], piVar4[2]);
        iVar5 = iVar5 + 1;
        piVar4 = piVar4 + 3;
        tradeRoom = (TradeRoom *)(&tradeRoom->saveData);
    } while (iVar5 < 6);
    return;
}
