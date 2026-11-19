#include "global.h"

#include "pokeathlon/pokeathlon.h"

typedef struct Ov96MonStats {
    f32 speed;  // 0x0
    f32 power;  // 0x4
    f32 skill;  // 0x8
    u32 jump;   // 0xC
    u8 stamina; // 0x10
} Ov96MonStats;

// 0x14

typedef struct Ov96StatsOut {
    u8 filler_00[0xC];
    Ov96MonStats mons[3];
} Ov96StatsOut;

typedef struct Ov96StatTable {
    s32 rows[5][5];
} Ov96StatTable;

void ov96_02206E88(PokeathlonCourseData *data, Ov96StatTable *table, int participant, u8 mon, Ov96StatsOut *out);

void ov96_02206E88(PokeathlonCourseData *data, Ov96StatTable *table, int participant, u8 mon, Ov96StatsOut *out) {
    PokeathlonMonKinds *e = ov96_021E60D8(data, participant, mon);
    out->mons[mon].skill = (f32)table->rows[0][e->statIdx[0]] / 10.0;
    out->mons[mon].stamina = table->rows[4][e->statIdx[1]];
    out->mons[mon].speed = (f32)table->rows[1][e->statIdx[4]] / 10.0;
    out->mons[mon].power = (f32)table->rows[2][e->statIdx[1]] / 10.0;
    out->mons[mon].jump = table->rows[3][e->statIdx[3]];
}
