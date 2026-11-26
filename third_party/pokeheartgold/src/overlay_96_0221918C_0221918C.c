#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"

#include "overlay_96_ov96_rd_obj.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0221935C.c (tools/pokehg_abi.py)
Ov96RdStats *ov96_0221935C(PokeathlonCourseData *data, unsigned int a, unsigned int b);
#else
Ov96RdStats *ov96_0221935C(PokeathlonCourseData *data, u8 a, u8 b);
#endif

#define OV96_FX(v) ((s32)((v) > 0 ? 0.5f + (f32)((v) << 12) : (f32)((v) << 12) - 0.5f))

void ov96_0221918C(PokeathlonCourseData *data);

void ov96_0221918C(PokeathlonCourseData *data) {
    s32 tbl[6][5] = { 0 };
    s32 i;
    s32 j;
    PokeathlonMonKinds *kinds;
    Ov96RdStats *stats;

    ReadWholeNarcMemberByIdPair(tbl, NARC_a_1_7_0, 9);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            kinds = ov96_021E60D8(data, i, j);
            stats = ov96_0221935C(data, i, j);
            GF_ASSERT(kinds->statIdx[0] < 5);
            GF_ASSERT(kinds->statIdx[3] < 5);
            GF_ASSERT(kinds->statIdx[1] < 5);
            GF_ASSERT(kinds->statIdx[2] < 5);
            stats->s0 = OV96_FX(tbl[0][kinds->statIdx[0]]) / 10;
            stats->s1 = OV96_FX(tbl[1][kinds->statIdx[0]]) / 10;
            stats->s2 = OV96_FX(tbl[2][kinds->statIdx[3]]);
            stats->s3 = OV96_FX(tbl[3][kinds->statIdx[1]]) / 10;
            stats->s4 = OV96_FX(tbl[5][kinds->statIdx[2]]) / 10;
            stats->s5 = OV96_FX(tbl[5][kinds->statIdx[2]]) / 10;
            stats->k2 = kinds->statIdx[2];
        }
    }
}
