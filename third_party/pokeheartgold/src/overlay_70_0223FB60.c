#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "list_menu.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "wild_encounter.h"
#include "particle_system.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "evolution_data.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sav_chatot.h"
#include "pokepic.h"
#include "sav_system_info.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"
#include "game_stats.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct JournalEntryTitle JournalEntryTitle;

typedef struct JournalEntryMon JournalEntryMon;

typedef struct JournalEntryTrainer JournalEntryTrainer;

typedef struct JournalEntry JournalEntry;

typedef struct WiFiHistory WiFiHistory;

typedef struct EncodingSeed EncodingSeed;

typedef struct PokemonAnimManager PokemonAnimManager;

typedef struct GlobalTrade GlobalTrade;

typedef struct PCBoxes PCBoxes;

typedef struct GameTime GameTime;

typedef struct GTSPlayerData GTSPlayerData;

typedef struct Poketch Poketch;

typedef struct PaletteAnimator PaletteAnimator;

typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
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

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

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

struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
};

struct GlobalTrade {
    u8 storedPokemon[sizeof(Pokemon)];
    u16 pokemonListed;
    u16 neverSet_EE;
    u32 depositTradeDatestamp;
    u32 searchTradeDatestamp;
};

struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
};

struct GameTime {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp;
    u32 penaltyInMinutes;
};

struct GTSPlayerData {
    GlobalTrade *globalTrade;
    SysInfo *systemData;
    Party *party;
    PCBoxes *pcBoxes;
    Pokedex *pokedex;
    WiFiList *wiFiList;
    WiFiHistory *wiFiHistory;
    PlayerProfile *trainerInfo;
    SaveData *saveData;
    Options *options;
    GameStats *records;
    JournalEntry *journalEntry;
    Bag *bag;
    int dexMode;
    int dwcProfileId;
    int showContestData;
    BOOL connectToWiFi;
};

struct Poketch {
    u8 poketchEnabled : 1;
    u8 pedometerEnabled : 1;
    u8 dotArtModifiedByPlayer : 1;
    u8 screenColor : 3;

    s8 appCount;
    s8 appIndex;
    u8 appRegistry[32];

    u32 stepCount;

    u16 alarmSet : 1;
    u16 alarmHour : 5;
    u16 alarmMinute : 6;

    u8 dotArtData[120];

    u32 calendarMarkBitmap;
    u8 calendarMonth;

    struct {
        u8 x;
        u8 y;
    } markMapPositions[6];

    struct {
        u16 species;
        u16 icon;
        u32 form;
    } pokemonHistoryQueue[12];
};

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

struct GTSBoxPokemonCriteria {
    GTSPokemonCriteria criteria[(5 * 6)];
};

void ov70_02239C6C(void *arg0);

void ov70_02239CF8(void *arg0);

void ov70_0223FB60(GTSApplicationState *appState);

void ov70_0223FBF4(GTSApplicationState *arg0);

void ov70_0223FB60(GTSApplicationState *appState) {
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = appState->bgConfig;
    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    uVar1 = Options_GetFrame(appState->playerData->options);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 0xe, (u8)uVar1, HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 0xb, 0, HEAP_ID_61);
    if (appState->hasAvatarFinishedMoving == 0) {
        ToggleBgLayer(4, 0);
        ToggleBgLayer(5, 0);
        GfGfx_EngineBTogglePlanes(0x10, 0);
    }
    if (appState->previousScreen == 9) {
        ov70_02239C6C(appState);
        ov70_02239CF8(appState);
    }
    return;
}

void ov70_0223FBF4(GTSApplicationState *arg0) {
    AddWindowParameterized(arg0->bgConfig, &arg0->bottomInstructionWindow, 0U, 2U, 0x13U, 0x1BU, 4U, 0xDU, 0x28U);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0U);
}
