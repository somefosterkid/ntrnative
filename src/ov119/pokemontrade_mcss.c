// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_mcss.c, the same code in
// Black 2/White 2's overlay 194; Black's sound numbers differ but have the same names. The names are Black's dsd
// names; pokebw2's guesses are in the header and the comments.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "constants/sound.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "system/mcss.h"

// The sounds of the trade's animation and the moves of the Pokémon's sprites across the screen (pokemontrade_mcss.c,
// named by the ROM's embedded string)

// A sound and the frame of the animation it plays at
typedef struct {
    u32 se;
    int frame;
} TradeSoundCue;

static void func_ov119_021dfad0(int frame, PokemonTradeWork *wk);
static fx32 func_ov119_021dfbf0(fx32 start, fx32 end, TradeMcssMove *move, BOOL bounce);
static void func_ov119_021dfc28(TradeMcssMove *move, PokemonTradeWork *wk);

void func_ov119_021dfa60(int frame) {
    TradeSoundCue cues[] = {
        { SEQ_SE_W554_PAKIN, 1 },   { SEQ_SE_BOWA2, 4 },       { SEQ_SE_KON, 29 },         { SEQ_SE_KON, 45 },
        { SEQ_SE_KON, 56 },         { SEQ_SE_KON, 63 },        { SEQ_SE_TDEMO_001, 17 },   { SEQ_SE_TDEMO_002, 107 },
        { SEQ_SE_W028_02, 152 },    { SEQ_SE_TDEMO_006, 266 }, { SEQ_SE_TDEMO_007, 350 },  { SEQ_SE_TDEMO_008, 636 },
        { SEQ_SE_TDEMO_009, 650 },  { SEQ_SE_TDEMO_010, 694 }, { SEQ_SE_TDEMO_011, 857 },  { SEQ_SE_TDEMO_001, 858 },
        { SEQ_SE_TDEMO_009, 896 },  { SEQ_SE_KON, 1048 },      { SEQ_SE_KON, 1068 },       { SEQ_SE_KON, 1082 },
        { SEQ_SE_KON, 1092 },       { SEQ_SE_KON, 1103 },      { SEQ_SE_TDEMO_004, 1123 }, { SEQ_SE_BOWA2, 1166 },
        { SEQ_SE_TDEMO_001, 1159 },
    };
    u32 i;
    for (i = 0; i < NELEMS(cues); i++) {
        if (frame == cues[i].frame) {
            GFL_SndSEPlay(cues[i].se);
        }
    }
}

void func_ov119_021dfa98(int frame) {
    TradeSoundCue cues[] = {
        { SEQ_SE_W554_PAKIN, 32 }, { SEQ_SE_BOWA2, 33 },      { SEQ_SE_KON, 38 },        { SEQ_SE_KON, 45 },
        { SEQ_SE_KON, 56 },        { SEQ_SE_KON, 64 },        { SEQ_SE_TDEMO_001, 30 },  { SEQ_SE_W028_02, 110 },
        { SEQ_SE_TDEMO_009, 130 }, { SEQ_SE_TDEMO_002, 241 }, { SEQ_SE_TDEMO_003, 320 }, { SEQ_SE_W054_01, 364 },
        { SEQ_SE_W179_02, 409 },   { SEQ_SE_W307_03, 484 },   { SEQ_SE_KON, 494 },       { SEQ_SE_KON, 514 },
        { SEQ_SE_KON, 528 },       { SEQ_SE_KON, 538 },       { SEQ_SE_KON, 547 },       { SEQ_SE_TDEMO_004, 569 },
        { SEQ_SE_TDEMO_005, 615 }, { SEQ_SE_TDEMO_001, 605 },
    };
    u32 i;
    for (i = 0; i < NELEMS(cues); i++) {
        if (frame == cues[i].frame) {
            GFL_SndSEPlay(cues[i].se);
        }
    }
}

static void func_ov119_021dfad0(int frame, PokemonTradeWork *wk) {
    TradeSoundCue cues[] = {
        { SEQ_SE_DANSA, 1 },
        { SEQ_SE_SYS_83, 13 },
    };
    TradeSoundCue bounceCues[] = {
        { SEQ_SE_DANSA, 1 },   { SEQ_SE_SYS_83, 13 }, { SEQ_SE_DANSA, 15 },
        { SEQ_SE_SYS_83, 25 }, { SEQ_SE_DANSA, 27 },  { SEQ_SE_SYS_83, 39 },
    };
    u32 i;
    if (wk->unk11EF == 0) {
        if (wk->unk11ED == 0) {
            for (i = 0; i < NELEMS(cues); i++) {
                if (frame == cues[i].frame) {
                    GFL_SndSEPlay(cues[i].se);
                }
            }
        } else {
            for (i = 0; i < NELEMS(bounceCues); i++) {
                if (frame == bounceCues[i].frame) {
                    GFL_SndSEPlay(bounceCues[i].se);
                }
            }
        }
    }
}

void func_ov119_021dfb4c(TradeMcssMove *move, int duration, const VecFx32 *end) {
    VecFx32 start;
    func_02015c30(move->mcss, &start);
    move->frame = 0;
    move->duration = duration;
    MI_CpuCopy8(end, &move->end, sizeof(VecFx32));
    MI_CpuCopy8(&start, &move->start, sizeof(VecFx32));
}

TradeMcssMove *func_ov119_021dfb84(MCSS *mcss, int duration, const VecFx32 *end, HeapID heapId) {
    TradeMcssMove *move = GFL_HeapAllocate(heapId, sizeof(TradeMcssMove), TRUE, "pokemontrade_mcss.c", 210);
    move->mcss = mcss;
    func_ov119_021dfb4c(move, duration, end);
    move->unkC = 0;
    return move;
}

TradeMcssMove *func_ov119_021dfbb4(MCSS *mcss, int duration, const VecFx32 *end, const VecFx32 *path,
                                   HeapID heapId) {
    TradeMcssMove *move = GFL_HeapAllocate(heapId, sizeof(TradeMcssMove), TRUE, "pokemontrade_mcss.c", 231);
    move->mcss = mcss;
    func_ov119_021dfb4c(move, duration, end);
    move->unkC = 0;
    move->path = path;
    return move;
}

// TradeMcssMove_Interpolate
static fx32 func_ov119_021dfbf0(fx32 start, fx32 end, TradeMcssMove *move, BOOL bounce) {
    fx32 step = (end - start) / move->duration;
    fx32 pos = start + step * move->frame;
    if (move->bounce != 0 && bounce) {
        pos = end + FX_SinIdx(move->angle) * 8;
    }
    return pos;
}

// TradeMcssMove_FollowPath
static void func_ov119_021dfc28(TradeMcssMove *move, PokemonTradeWork *wk) {
    VecFx32 pos;
    const VecFx32 *offset = &move->path[move->frame];
    pos.x = offset->x + move->start.x;
    pos.y = offset->y + move->start.y;
    pos.z = offset->z + move->start.z;
    func_02015c48(move->mcss, &pos);
    func_ov119_021dfad0(move->frame, wk);
}

void func_ov119_021dfc68(TradeMcssMove *move, PokemonTradeWork *wk) {
    VecFx32 pos;
    VecFx32 current;
    if (move != NULL) {
        move->frame++;
        if (move->duration >= move->frame) {
            if (move->duration != move->frame) {
                if (move->path != NULL) {
                    func_ov119_021dfc28(move, wk);
                    return;
                }
                func_02015c30(move->mcss, &current);
                pos.x = func_ov119_021dfbf0(move->start.x, move->end.x, move, FALSE);
                if (move->bounce != 0) {
                    move->angle -= (u16)(int)((f32)(current.x - pos.x) / FX32_ONE * 374.0f);
                }
                pos.y = func_ov119_021dfbf0(move->start.y, move->end.y, move, TRUE);
                pos.z = func_ov119_021dfbf0(move->start.z, move->end.z, move, FALSE);
                func_02015c48(move->mcss, &pos);
            } else {
                func_02015c48(move->mcss, &move->end);
            }
        }
    }
}

void func_ov119_021dfd0c(PokemonTradeWork *wk) {
    func_02015e04(wk->mcss[0]);
    wk->unk854 = 1;
}

void func_ov119_021dfd24(TradeMcssMove *move) {
    if (move != NULL) {
        GFL_HeapFree(move);
    }
}
