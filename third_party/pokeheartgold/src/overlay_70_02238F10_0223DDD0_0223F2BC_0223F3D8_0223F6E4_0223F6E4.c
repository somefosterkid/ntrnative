#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "list_menu.h"
#include "options.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_wifi_history.h"
#include "sprite.h"
#include "string_util.h"
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
#include "sav_system_info.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"
#include "game_stats.h"
#include "gts_player_data.h"

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

typedef struct EncodingSeed EncodingSeed;

typedef struct PokemonAnimManager PokemonAnimManager;

typedef struct GameTime GameTime;

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

s32 ov70_0223E490(s32 arg0);

void ov70_0223F6E4(GTSPokemonListing *listing, GTSApplicationState *appState);

void ov70_0223F6E4(GTSPokemonListing *listing, GTSApplicationState *appState) {
    u8 uVar1;
    u16 uVar2;
    s32 sVar3;
    u32 uVar4;
    u16 *src;

    sVar3 = ov70_0223E490((u32)(u16)appState->selectedBoxId);
    if (sVar3 != 0) {
        Mon_UpdateShayminForm((Pokemon *)appState->selectedBoxMon, 0);
        uVar4 = SizeOfStructPokemon();
        MIi_CpuCopyFast(appState->selectedBoxMon, listing, uVar4);
    } else {
        BoxMon_UpdateShayminForm(appState->selectedBoxMon, 0);
        CopyBoxPokemonToPokemon((BoxPokemon *)appState->selectedBoxMon, (Pokemon *)listing);
    }
    src = (unsigned short *)(PlayerProfile_GetNamePtr((PlayerProfile *)appState->playerData->trainerInfo));
    CopyU16StringArrayN(listing->trainerNames, src, 8);
    uVar2 = PlayerProfile_GetTrainerID_VisibleHalf((PlayerProfile *)appState->playerData->trainerInfo);
    listing->trainerId = uVar2;
    uVar1 = WifiHistory_GetPlayerCountry((SaveWiFiHistory *)appState->playerData->wiFiHistory);
    listing->trainerCountry = uVar1;
    uVar1 = WiFiHistory_GetPlayerRegion((SaveWiFiHistory *)appState->playerData->wiFiHistory);
    listing->trainerRegion = uVar1;
    uVar1 = PlayerProfile_GetAvatar((PlayerProfile *)appState->playerData->trainerInfo);
    listing->trainerAppearance = uVar1;
    uVar4 = PlayerProfile_GetTrainerGender((PlayerProfile *)appState->playerData->trainerInfo);
    listing->trainerGender = (u8)uVar4;
    listing->gameVersion = GAME_VERSION;
    listing->trainerLanguage = 2;
    return;
}
