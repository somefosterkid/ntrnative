#ifndef POKEHEARTGOLD_OVERLAY_65_TRADE_ROOM_H
#define POKEHEARTGOLD_OVERLAY_65_TRADE_ROOM_H
#include "sprite_resources_header.h"

#include "g2d_renderer.h"
#include "pokemon_summary_args.h"

// TradeRoom: merged by tools/hg/promote.py from 15 m2c local copies (3 variants, base
// src/overlay_65_0221CA64_0221CB5C.c) and 224 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct BgConfig;
struct ChatotCry;
struct FieldSystem;
struct ListMenu;
struct Menu;
struct MsgData;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct PalPad;
struct Party;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringList;
struct StringTemplate;
struct SysTask;
struct TradeRoomArgs;
struct WiFiList;

typedef struct TradeRoom {
    struct FieldSystem *fieldSystem;
    struct SaveData *saveData;
    struct TradeRoomArgs *args;
    PokemonSummaryArgs summaryArgs;
    int summarySide;
    struct ApplicationManager *appMan;
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
    struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; } unk_70;
    int selectedSlot[2];
    u16 cursorGlowAngle;
    u16 unk_92[17][5];
    u16 unk_13C;
    u16 unk_13E;
    int subStepResult;
    u16 unk_144[3];
    u8 pad_156[0x2];
    u32 pendingDirection[2];
    u32 unk_154[3][2];
    int unk_16C;
    int unk_170;
    struct BgConfig *bgConfig;
    struct StringTemplate *strTemplate;
    struct StringTemplate *strTemplate2;
    struct StringTemplate *unread_180;
    struct MsgData *msgLoader;
    struct String *unk_188;
    struct String *playerNameStr;
    struct String *itemLabelStr;
    struct SpriteList *spriteList;
    G2dRenderer spriteRenderer;
    struct SpriteResourceCollection *spriteResourceCollections[4];
    struct SpriteResource *spriteResources[4][2];
    SpriteResourcesHeader mainSpriteResourcesHeader;
    SpriteResourcesHeader subSpriteResourcesHeader;
    struct Sprite *cursorSprites[14];
    struct Sprite *iconSprites[12];
    struct Sprite *itemIconSprites[12];
    struct Sprite *capsuleIconSprites[12];
    struct Sprite *detailCardSprites[6];
    struct Sprite *convergeIndicatorSprite;
    struct SysTask *unk_480[7];
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } windows[34];
    struct Menu *yesNoMenu;
    struct StringList *actionMenuChoices;
    struct Menu *actionMenu;
    int yesNoMenuStep;
    int unk_6CC;
    int unk_6D0;
    int unk_6D4;
    struct { fx32 x; fx32 y; fx32 z; } unk_6D8[2];
    int unk_6F0;
    struct { u16 species; u16 heldItemType; u8 pokeBall; u8 isEgg; u8 form; u8 unk_8_pad[0x1]; u16 gender; u16 iconShouldFlip; int ballCapsuleId; } monDisplayData[13];
    int unk_7B8;
    int unk_7BC;
    int unk_7C0;
    int unk_7C4[7];
    void *unk_7E0;
    struct NNSG2dCharacterData *unk_7E4;
    void *iconGraphicsBuffers[12];
    struct NNSG2dCharacterData *iconCharacterData[12];
    u8 previewSpritePixelBuf[3200][2];
    struct { u16 narcID; u16 character; u16 palette; u16 spindaSpots; u8 dummy; u8 unk_C_pad[0x3]; u32 personality; } previewSpriteTemplate[2];
    int pendingPreviewLoadSide;
    u8 pad_2120[0xf4];
    void *unk_2214;
    struct Party *unk_2218;
    struct Party *unk_221C;
    void *subStepCallback;
    struct Party *playerParty;
    struct Party *partnerParty;
    struct ChatotCry *unk_2278;
    struct PalPad *palPad;
    struct { u16 trainerName[8]; u32 trainerId; u8 language; u8 gameCode; u8 gender; u8 padding_17; u32 associatedTrainerIds[16]; u8 associatedTrainerGameCodes[16]; u8 associatedTrainerLanguages[16]; u8 associatedTrainerGenders[16]; } palPadNetworkObject;
    int unk_2308;
    int unk_230C;
    int bgScrollOffset;
    int slideAnimFrame;
    int staggerCountdown;
    struct { fx32 x; fx32 y; fx32 z; } playerSlidePath[2];
    struct { fx32 x; fx32 y; fx32 z; } partnerSlidePath[2];
    u8 partyTransferBuffer[1424][2];
    u8 chatotCryBuffer[1004][2];
    int pendingRegistrationFlags[32];
    int registrationTargetNetId;
    int friendToDeleteSlot;
    struct StringTemplate *palPadStrTemplate;
    struct MsgData *palPadMsgLoader;
    struct StringList *friendListChoices;
    struct ListMenu *friendListMenu;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } friendListWindow;
    struct WiFiList *wifiList;
    int tradeCompletedCount;
    int syncSaveState;
    int lastSyncedValue;
    int lastSyncedCmd;
    void *waitDialHandle;
    int connectionTimeoutArmed;
    int connectionTimeoutFrames;
} TradeRoom;

// sizeof(TradeRoom) == 0x36c0

#endif // POKEHEARTGOLD_OVERLAY_65_TRADE_ROOM_H
