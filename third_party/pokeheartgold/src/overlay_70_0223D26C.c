#include "global.h"

#include "overlay_70_gtsapplication_state.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "constants/heap.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "bag_types_def.h"
#include "camera.h"
#include "filesystem.h"
#include "list_menu.h"
#include "message_format.h"
#include "options.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02005D10.h"
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
#include "overlay_manager.h"
#include "sav_chatot.h"
#include "pokepic.h"
#include "sav_system_info.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"
#include "game_stats.h"
#include "gts_player_data.h"

typedef struct JournalEntry JournalEntry;
typedef struct WiFiHistory WiFiHistory;
typedef struct GlobalTrade GlobalTrade;
typedef struct PCBoxes PCBoxes;

void ov70_02239D44(void *, u32);

void ov70_02245084(void *, void *, u32, u32, u32, u32);

void ov70_0223D26C(GTSApplicationState *param_1);

void ov70_0223D3BC(struct GTSApplicationState *arg0);
void ov70_0223D414(struct GTSApplicationState *arg0);

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
void ov70_0223E01C(GTSApplicationState *appState, int entryID, int renderDelay, int unused1, u16 unused2, int isConfirmWindow);
void ov70_02238D84(GTSApplicationState *, u32, u32);
u32 ov70_0223D44C(GTSApplicationState *param_1);
s32 ov70_02238E50(GTSApplicationState *, s32, s32);
BoxPokemon *ov70_0223E49C(Party *, PCBoxes *, u32, s32);
s32 ov70_0223E4DC(Party *, PCBoxes *, int, int);
s32 ov70_0223E5C8(Party *, PCBoxes *, int, int);
void ov70_0223D490(GTSApplicationState *arg0);

void ov70_0223D26C(GTSApplicationState *param_1) {
    AddWindowParameterized(param_1->bgConfig, (Window *)(&param_1->networkErrWindow), 3, 4, 0, 0xd, 3, 0xd, 0x60);
    FillWindowPixelBuffer((Window *)(&param_1->networkErrWindow), 0);
    CopyWindowToVram((Window *)(&param_1->networkErrWindow));
    AddWindowParameterized(param_1->bgConfig, (Window *)(&param_1->bottomInstructionWindow), 0, 2, 0x15, 0x1b, 2, 0xd, 0x109);
    FillWindowPixelBuffer((Window *)(&param_1->bottomInstructionWindow), 0);
    AddWindowParameterized(param_1->bgConfig, (Window *)(&param_1->confirmationWindow), 0, 2, 0x13, 0x1b, 4, 0xd, 0x13f);
    FillWindowPixelBuffer((Window *)(&param_1->confirmationWindow), 0);
    AddWindowParameterized(param_1->bgConfig, (Window *)(&param_1->menuButtonWindows[1]), 1, 0x18, 0x10, 6, 2, 0, 0x165);
    FillWindowPixelBuffer((Window *)(&param_1->menuButtonWindows[1]), 6);
    ov70_02245084(&param_1->menuButtonWindows[1], param_1->cancelButtonLabel, 0, 1, 1, 0x10306);
    if (param_1->screenArgument == 5) {
        ov70_02239D44(param_1, 3);
        return;
    }
    ov70_02239D44(param_1, 1);
    return;
}

void ov70_0223D378(GTSApplicationState *param_1);

void ov70_0223D378(GTSApplicationState *param_1) {
    RemoveWindow(&param_1->unk_1198);
    RemoveWindow(&param_1->confirmationWindow);
    RemoveWindow(&param_1->menuButtonWindows[1]);
    RemoveWindow(&param_1->bottomInstructionWindow);
    RemoveWindow(&param_1->networkErrWindow);
    return;
}

void ov70_0223D3BC(GTSApplicationState *arg0) {
    arg0->selectPokemonBoxName = String_New(0x12U, HEAP_ID_61);
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    arg0->cancelButtonLabel = NewString_ReadMsgData(*((MsgData * *)&arg0->gtsMessageLoader), 0x6D);
    if (arg0->partySlotIndex == 0x1E) {
        arg0->partySlotIndex = 0U;
    }
    *((void * *)&arg0->boxCriteria) = Heap_Alloc(HEAP_ID_61, 0x78U);
}

void ov70_0223D414(GTSApplicationState *arg0) {
    Heap_Free(*((void * *)&arg0->boxCriteria));
    String_Delete(arg0->selectPokemonBoxName);
    String_Delete(arg0->genericMessageBuffer);
    String_Delete(arg0->cancelButtonLabel);
}

u32 ov70_0223D44C(GTSApplicationState *param_1) {
    BOOL BVar1;
    s32 unaff_r4;

    BVar1 = IsPaletteFadeFinished();
    if (BVar1 != 0) {
        if (param_1->screenArgument == 5) {
            unaff_r4 = 0x15;
        } else if (param_1->screenArgument == 6) {
            unaff_r4 = 0x11;
        }
        ov70_0223E01C(param_1, unaff_r4, 1, 0, 0xf0f, 0);
        ov70_02238D84(param_1, 3, 1);
    }
    return 3;
}

void ov70_0223D490(GTSApplicationState *arg0) {
    GTSPlayerData *gTSPlayerData;
    GTSPlayerData *v2;
    GTSPlayerData *v3;
    s32 v0;
    u16 v1;

    v1 = arg0->partySlotIndex;
    if (v1 == 0x1E) {
        ov70_02238E50(arg0, 1, 0);
        arg0->currentScreenInstruction = 2;
        PlaySE(0x5DCU);
        return;
    }
    if (v1 != 0x1F) {
        PlaySE(0x5DCU);
        gTSPlayerData = arg0->playerData;
        v0 = ov70_0223E5C8(gTSPlayerData->party, gTSPlayerData->pcBoxes, arg0->selectedBoxId, arg0->partySlotIndex);
        switch (v0) {
        case 1:
            v2 = arg0->playerData;
            if (ov70_0223E4DC(v2->party, v2->pcBoxes, arg0->selectedBoxId, arg0->partySlotIndex) != 0) {
                v3 = arg0->playerData;
                BufferBoxMonNickname(arg0->stringTemplate, 0U, ov70_0223E49C(v3->party, v3->pcBoxes, arg0->selectedBoxId, arg0->partySlotIndex));
                ov70_0223E01C(arg0, 0x16, 1, 0, 0xF0FU, 0);
                ov70_02238D84(arg0, 3, 7);
                return;
            }
            ov70_0223E01C(arg0, 0x1A, 1, 0, 0xF0FU, 1);
            ov70_02238D84(arg0, 4, 1);
            return;
        case 2:
            ov70_0223E01C(arg0, 0x1B, 1, 0, 0xF0FU, 1);
            ov70_02238D84(arg0, 4, 1);
            break;
        }
    }
}
