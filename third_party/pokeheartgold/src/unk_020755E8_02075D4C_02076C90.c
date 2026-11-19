#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bag.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "game_stats.h"
#include "heap.h"
#include "mail.h"
#include "options.h"
#include "palette.h"
#include "party.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "msgdata.h"
#include "particle_system.h"
#include "overlay_manager.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "evolution_data.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct EncodingSeed EncodingSeed;

typedef struct GameRecords GameRecords;

typedef struct PokemonAnimManager PokemonAnimManager;

typedef struct Poketch Poketch;

typedef struct PaletteAnimator PaletteAnimator;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

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

struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
};

struct GameRecords {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
};

struct PokemonSummary;

typedef void (*TransformFunc)(TransformData *, PokemonAnim *);

struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
};

typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;

typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;

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

void sub_02076C90(EvolutionData *evolutionData);

void sub_02076C90(EvolutionData *evolutionData) {
    u16 uVar1;
    s32 iVar2;
    s32 iVar3;
    Pokemon *dest;
    Mail *ptr;
    s32 iStack_2c;
    u32 uStack_30;
    u8 auStack_28[24];

    switch (evolutionData->method) {
    case 0xd:
    case 0xe:
        uVar1 = Bag_GetQuantity(evolutionData->bag, 4, evolutionData->heapID);
        if (uVar1 != 0) {
            iVar2 = Party_GetCount(evolutionData->party);
            iVar3 = Party_GetMaxCount(evolutionData->party);
            if (iVar2 < iVar3) {
                dest = AllocMonZeroed(evolutionData->heapID);
                CopyPokemonToPokemon(evolutionData->mon, dest);
                uStack_30 = 0x124;
                SetMonData(dest, 5, &uStack_30);
                uStack_30 = 4;
                SetMonData(dest, 0x9b, &uStack_30);
                uStack_30 = 0;
                SetMonData(dest, 6, &uStack_30);
                SetMonData(dest, 0xb, &uStack_30);
                iStack_2c = 0x19;
                do {
                    SetMonData(dest, iStack_2c, &uStack_30);
                    iStack_2c = iStack_2c + 1;
                } while (iStack_2c < 0x36);
                iStack_2c = 0x4e;
                do {
                    SetMonData(dest, iStack_2c, &uStack_30);
                    iStack_2c = iStack_2c + 1;
                } while (iStack_2c < 0x6e);
                iStack_2c = 0x7b;
                do {
                    SetMonData(dest, iStack_2c, &uStack_30);
                    iStack_2c = iStack_2c + 1;
                } while (iStack_2c < 0x90);
                SetMonData(dest, 0xb5, &uStack_30);
                SetMonData(dest, 0xb6, &uStack_30);
                SetMonData(dest, 0xb7, &uStack_30);
                SetMonData(dest, 0xb8, &uStack_30);
                SetMonData(dest, 0xb9, &uStack_30);
                SetMonData(dest, 0xba, &uStack_30);
                SetMonData(dest, 0xbb, &uStack_30);
                SetMonData(dest, 0xb3, (void *)0x0);
                SetMonData(dest, 0x4d, &uStack_30);
                SetMonData(dest, 0xa0, &uStack_30);
                ptr = Mail_New(evolutionData->heapID);
                SetMonData(dest, 0xaa, ptr);
                Heap_Free(ptr);
                SetMonData(dest, 0xa2, &uStack_30);
                MIi_CpuClearFast(0, auStack_28, 0x18);
                SetMonData(dest, 0xab, auStack_28);
                UpdateMonAbility(dest);
                CalcMonLevelAndStats(dest);
                Party_AddMon(evolutionData->party, dest);
                Pokedex_SetMonCaughtFlag(evolutionData->pokedex, dest);
                GameStats_Inc(evolutionData->records, 0xd);
                GameStats_AddScore(evolutionData->records, 0x15);
                Heap_Free(dest);
                Bag_TakeItem(evolutionData->bag, 4, 1, evolutionData->heapID);
                return;
            }
        }
        break;
    case 6:
    case 0x12:
    case 0x13:
        iStack_2c = 0;
        SetMonData(evolutionData->mon, 6, &iStack_2c);
        break;
    }
}
