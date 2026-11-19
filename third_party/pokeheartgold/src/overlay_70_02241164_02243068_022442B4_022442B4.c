#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon_summary_args.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "wild_encounter.h"
#include "particle_system.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "evolution_data.h"
#include "msgdata.h"
#include "pokepic.h"
#include "overlay_manager.h"
#include "sav_chatot.h"
#include "sav_system_info.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"
#include "game_stats.h"

typedef struct PaletteAnimator PaletteAnimator;

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

typedef struct EncodingSeed EncodingSeed;

typedef struct WiFiHistory WiFiHistory;

typedef struct PokemonAnimManager PokemonAnimManager;

typedef struct GlobalTrade GlobalTrade;

typedef struct PCBoxes PCBoxes;

typedef struct GameTime GameTime;

typedef struct GTSPlayerData GTSPlayerData;

typedef struct Poketch Poketch;

typedef struct GTSBoxPokemonCriteria GTSBoxPokemonCriteria;

typedef u32 TextColor;

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

typedef struct {
    u16 unk_00[8];
    u16 unk_10[8];
    u32 unk_20;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u16 year;
    u8 month;
    u8 day;
    u8 unk_2E;
    u8 unk_2F;
    u16 unk_30;
    u16 unk_32;
    u16 unk_34;
    u16 unk_36;
} UnkStruct_0202B370_sub1;

struct WiFiList {
    DWCUserData userData;
    DWCFriendData friendData[32];
    UnkStruct_0202B370_sub1 unk_1C0[32];
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

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

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

struct GTSBoxPokemonCriteria {
    GTSPokemonCriteria criteria[(5 * 6)];
};

void ov70_022444E4(SysTask *task, void *data);

void ov70_022442B4(GTSApplicationState *appState);

void ov70_022442B4(GTSApplicationState *appState) {
    NARC *narc;
    NNSG2dPaletteData *plttData;
    s32 i;
    s32 cnt;
    s32 frac;
    s32 done;
    u16 *dst;
    u16 *src;
    s32 j;
    BgConfig *bgConfig;
    void *pltt;

    bgConfig = appState->bgConfig;
    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xEU, (s32)Options_GetFrame(appState->playerData->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x1FU, 0xBU, 0U, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x600U, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xB, bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xC, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 0, HEAP_ID_61);
    BG_SetMaskColor(0U, 0U);
    BG_SetMaskColor(4U, 0U);
    MI_CpuFill8(&appState->unk_12D0, 0U, 0x330U);
    pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_61);
    MIi_CpuCopy16(plttData->pRawData, appState->unk_12D0.unk_08, 0x80U);
    MIi_CpuCopy16(plttData->pRawData, appState->unk_12D0.unk_88, 0x80U);
    Heap_Free(pltt);
    cnt = 0;
    i = 0;
    dst = appState->unk_12D0.unk_88[0];
    src = appState->unk_12D0.unk_08[0];
    for (; i < 3; i++) {
        frac = 0;
        done = 0;
        while (TRUE) {
            if (cnt >= 21) {
                GF_AssertFail();
            }
            for (j = 1; j < 16; j++) {
                BlendPalette(&src[j], &dst[j], 1, frac >> 8, appState->unk_12D0.unk_08[0][(i + 1) * 16 + j]);
            }
            dst += 16;
            cnt++;
            if (done == 1) {
                break;
            }
            frac += 0x300;
            if (frac >= 0x1000) {
                frac = 0x1000;
                done = 1;
            }
        }
        src += 16;
    }
    DC_FlushRange(appState->unk_12D0.unk_88, sizeof(appState->unk_12D0.unk_88));
    appState->unk_12D0.unk_04 = 1;
    appState->unk_15FC = 0;
    appState->unk_12D0.unk_00 = SysTask_CreateOnVBlankQueue(ov70_022444E4, &appState->unk_12D0, 0x14U);
    NARC_Delete(narc);
}
