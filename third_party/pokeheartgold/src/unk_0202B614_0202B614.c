#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "seal_case.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "unk_0202B614.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct Pokemon;
struct Pokepic;
struct PokepicDrawParam;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;
int ov41_02249710(UnkStruct_020298D8 *param0);
void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}
typedef struct PhotoPokemon {
    u32 personality;
    u32 otID;
    u16 species;
    u16 nickname[10 + 1];
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
    u8 trainerGender;
} PhotoPokemon;
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
u32 GetMonData(Pokemon *mon, int param, void *dest);

BOOL sub_0202B614(u32 value);
void sub_0202B630(UnkStruct_020298D8 *param0, u8 *xPos, u8 *yPos, s8 *priority);
void sub_0202B684(PhotoPokemon *photoMon, Pokemon *mon, u8 xPos, u8 yPos, s8 priority);

BOOL sub_0202B614(u32 value) {
    if (value == (0x1234) || value == (0x2345)) {
        return 1;
    }

    return 0;
}

void sub_0202B630(UnkStruct_020298D8 *param0, u8 *xPos, u8 *yPos, s8 *priority) {
    int spriteX, spriteY;
    int spritePriority;

    ov41_02249780(param0, &spriteX, &spriteY);
    spritePriority = ov41_02249710(param0);

    ((spriteX < 256) ? (void)0 : GF_AssertFail());
    ((spriteY < 256) ? (void)0 : GF_AssertFail());
    ((spritePriority > -128) ? (void)0 : GF_AssertFail());

    *xPos = spriteX;
    *yPos = spriteY;
    *priority = spritePriority;
}

void sub_0202B684(PhotoPokemon *photoMon, Pokemon *mon, u8 xPos, u8 yPos, s8 priority) {
    photoMon->species = GetMonData(mon, MON_DATA_SPECIES, 0L);
    GetMonData(mon, MON_DATA_NICKNAME, photoMon->nickname);

    photoMon->personality = GetMonData(mon, MON_DATA_PERSONALITY, 0L);
    photoMon->otID = GetMonData(mon, MON_DATA_OT_ID, 0L);
    photoMon->form = GetMonData(mon, MON_DATA_FORM, 0L);

    photoMon->xPos = xPos;
    photoMon->yPos = yPos;
    photoMon->priority = priority;
}
