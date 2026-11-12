// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_audience.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_audience.h"
#include "app/musical/sta_act_light.h"
#include "app/musical/sta_act_poke.h"
#include "app/musical/sta_acting.h"
#include "field/musical_program.h"
#include "field/musical_stage_sys.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

// Overlay 209's sta_act_audience.c: the audience, a 12 by 5 grid of spectators in BGs 4 and 6 of the touch screen.
// Each is a fan of one Pokémon, shared out by the points of the program, and has four looks: right, a little right,
// a little left and left

enum {
    STA_ACT_AUDIENCE_LOOK_LEFT,
    STA_ACT_AUDIENCE_LOOK_LEFT_A_LITTLE,
    STA_ACT_AUDIENCE_LOOK_RIGHT_A_LITTLE,
    STA_ACT_AUDIENCE_LOOK_RIGHT,
};

static void func_ov132_021fa0d0(StaActAudience *sys);
static void func_ov132_021fa0d4(StaActAudience *sys);
static void func_ov132_021fa0d8(StaActAudience *sys, MusicalStageParam *param);
static void func_ov132_021fa278(StaActAudience *sys);
static void func_ov132_021fa27c(StaActAudience *sys);
static void func_ov132_021fa2a0(StaActAudience *sys, StaActAudienceMember *member);
static void func_ov132_021fa3a8(StaActAudience *sys, StaActAudienceMember *member, u8 look);
static void func_ov132_021fa4ac(StaActAudience *sys);
static void func_ov132_021fa528(StaActAudience *sys);

// Nothing reads it

// The first character of each type's spectator
const struct { u16 unused; u16 bases[5]; } data_ov132_021fdc14 = { 3, { 0x10, 0x90, 0x08, 0x88, 0 } };

// pokebw2's StaActAudience_InitSystem
StaActAudience *func_ov132_021f9fec(HeapID heapId, StaActing *stage, MusicalStageParam *param) {
    u8 i;
    StaActAudience *sys = GFL_HeapAllocate(heapId, sizeof(StaActAudience), FALSE, "sta_act_audience.c", 115);

    sys->heapId = heapId;
    func_ov132_021fa0d0(sys);
    func_ov132_021fa0d8(sys, param);
    sys->stage = stage;
    sys->refresh = FALSE;
    for (i = 0; i < 4; i++) {
        sys->cheer[i] = FALSE;
    }
    sys->lookTarget = STA_ACT_AUDIENCE_NONE;
    return sys;
}

// pokebw2's StaActAudience_TermSystem
void func_ov132_021fa04c(StaActAudience *sys) {
    func_ov132_021fa278(sys);
    func_ov132_021fa0d4(sys);
    GFL_HeapFree(sys);
}

// pokebw2's StaActAudience_UpdateSystem
void func_ov132_021fa064(StaActAudience *sys) {
    if (func_ov132_021f9dfc(sys->stage) == 0) {
        if (GFL_RandomLCAlt(10) == 0) {
            u8 i;

            for (i = 0; i < 3; i++) {
                u8 look = GFL_RandomLCAlt(4);
                u8 index = GFL_RandomLCAlt(STA_ACT_AUDIENCE_COUNT);

                func_ov132_021fa3a8(sys, &sys->members[index], look);
            }
        }
    } else {
        if (sys->refresh == TRUE) {
            func_ov132_021fa4ac(sys);
            sys->refresh = FALSE;
        }
        func_ov132_021fa27c(sys);
    }
}

// pokebw2's StaActAudience_InitGraphic
static void func_ov132_021fa0d0(StaActAudience *sys) {
}

// pokebw2's StaActAudience_TermGraphic
static void func_ov132_021fa0d4(StaActAudience *sys) {
}

// pokebw2's StaActAudience_InitMembers
static void func_ov132_021fa0d8(StaActAudience *sys, MusicalStageParam *param) {
    u8 i;
    u8 j;
    u8 n;
    u8 count;
    u8 sum;
    u16 total = 0;
    u8 row;
    u8 points[4];
    u8 shares[4] = {0};
    u8 types[STA_ACT_AUDIENCE_COUNT];

    sum = 0;
    for (i = 0; i < 4; i++) {
        points[i] = func_ov010_0215725c(param->program, i);
        total += points[i];
    }
    for (i = 0; i < 4; i++) {
        shares[i] = points[i] * STA_ACT_AUDIENCE_COUNT / total;
        sum += shares[i];
    }
    for (; sum < STA_ACT_AUDIENCE_COUNT; sum++) {
        shares[GFL_RandomLCAlt(4)]++;
    }
    n = 0;
    for (i = 0; i < 4; i++) {
        j = 0;
        count = shares[i];
        for (; j < count; j++) {
            types[n] = i;
            n++;
        }
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < STA_ACT_AUDIENCE_COUNT; i++) {
            u8 k = GFL_RandomLCAlt(STA_ACT_AUDIENCE_COUNT);
            u8 type = types[i];
            types[i] = types[k];
            types[k] = type;
        }
    }
    for (row = 0; row < 5; row++) {
        for (i = 0; i < 12; i++) {
            u8 index = i + row * 12;
            sys->members[index].x = i + 2;
            sys->members[index].y = row;
            sys->members[index].type = types[index];
            sys->members[index].timer = GFL_RandomLCAlt(60) + 1;
            sys->members[index].target = STA_ACT_AUDIENCE_NONE;
            if (i < 2) {
                func_ov132_021fa3a8(sys, &sys->members[index], STA_ACT_AUDIENCE_LOOK_RIGHT);
            } else if (i < 4) {
                func_ov132_021fa3a8(sys, &sys->members[index], STA_ACT_AUDIENCE_LOOK_RIGHT_A_LITTLE);
            } else if (i < 6) {
                func_ov132_021fa3a8(sys, &sys->members[index], STA_ACT_AUDIENCE_LOOK_LEFT_A_LITTLE);
            } else {
                func_ov132_021fa3a8(sys, &sys->members[index], STA_ACT_AUDIENCE_LOOK_LEFT);
            }
        }
    }
}

// pokebw2's StaActAudience_TermMembers
static void func_ov132_021fa278(StaActAudience *sys) {
}

// pokebw2's StaActAudience_UpdateMembers
static void func_ov132_021fa27c(StaActAudience *sys) {
    u8 i;

    for (i = 0; i < STA_ACT_AUDIENCE_COUNT; i++) {
        func_ov132_021fa2a0(sys, &sys->members[i]);
    }
}

// pokebw2's StaActAudience_UpdateMember
static void func_ov132_021fa2a0(StaActAudience *sys, StaActAudienceMember *member) {
    if (member->timer != 0) {
        member->timer--;
    }
    if (member->timer == 0) {
        if (member->target == STA_ACT_AUDIENCE_NONE && sys->lookTarget == STA_ACT_AUDIENCE_NONE) {
            member->unk4 = 0xffff;
            if (member->x < 2) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_RIGHT);
            } else if (member->x < 4) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_RIGHT_A_LITTLE);
            } else if (member->x < 6) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_LEFT_A_LITTLE);
            } else {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_LEFT);
            }
        } else {
            u16 memberX;
            s16 dx;
            VecFx32 pos;

            func_ov132_021f9e04(sys->stage);
            memberX = (member->x * 32 - 64) * 4 / 3;
            if (member->target != STA_ACT_AUDIENCE_NONE) {
                StaActPokeSys *pokeSys = func_ov132_021f9da0(sys->stage);

                func_ov112_021f8044(pokeSys, func_ov132_021f9da4(sys->stage, member->target), &pos);
            } else {
                StaActLightSys *lightSys = func_ov132_021f9dd4(sys->stage);

                func_ov112_021f6f04(lightSys, func_ov132_021f9ddc(sys->stage, sys->lookTarget), &pos);
            }
            dx = FX_FX32_TO_F32(pos.x) - memberX;
            if (dx < -64) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_LEFT);
            } else if (dx > 64) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_RIGHT);
            } else if (dx < 0) {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_LEFT_A_LITTLE);
            } else {
                func_ov132_021fa3a8(sys, member, STA_ACT_AUDIENCE_LOOK_RIGHT_A_LITTLE);
            }
        }
        member->timer = GFL_RandomLCAlt(60) + 1;
    }
}

// pokebw2's StaActAudience_DrawMember
static void func_ov132_021fa3a8(StaActAudience *sys, StaActAudienceMember *member, u8 look) {
    u16 base;
    BOOL flip = FALSE;
    u8 x;
    u8 y;
    u8 col;

    switch (look) {
    case STA_ACT_AUDIENCE_LOOK_RIGHT:
        flip = TRUE;
        base = data_ov132_021fdc14.bases[member->type] + (flip << 10);
        break;
    case STA_ACT_AUDIENCE_LOOK_RIGHT_A_LITTLE:
        flip = TRUE;
        base = data_ov132_021fdc14.bases[member->type] + 0x404;
        break;
    case STA_ACT_AUDIENCE_LOOK_LEFT_A_LITTLE:
        base = data_ov132_021fdc14.bases[member->type] + 4;
        break;
    case STA_ACT_AUDIENCE_LOOK_LEFT:
        base = data_ov132_021fdc14.bases[member->type];
        break;
    }
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            if (flip == TRUE) {
                col = 3 - x + member->x * 4;
            } else {
                col = x + member->x * 4;
            }
            GFL_BGSysFillScrArea(4, base + x + y * 32, col, y + member->y * 4 + 2, 1, 1, 0);
            GFL_BGSysFillScrArea(6, base + x + y * 32, col, y + member->y * 4 + 5, 1, 1, 0);
        }
    }
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysQueueScrLoad(6);
}

// pokebw2's StaActAudience_SetCheerTargets
static void func_ov132_021fa4ac(StaActAudience *sys) {
    u8 count = 0;
    u8 i;
    u8 targets[4] = { 0, 0, 0, 0 };

    for (i = 0; i < 4; i++) {
        if (sys->cheer[i] == TRUE) {
            targets[count] = i;
            count++;
        }
    }
    if (count == 0) {
        func_ov132_021fa528(sys);
        return;
    }
    for (i = 0; i < STA_ACT_AUDIENCE_COUNT; i++) {
        sys->members[i].timer = GFL_RandomLCAlt(60) + 1;
        sys->members[i].target = targets[GFL_RandomLCAlt(count)];
    }
}

// pokebw2's StaActAudience_ClearTargets
static void func_ov132_021fa528(StaActAudience *sys) {
    u8 i;

    for (i = 0; i < STA_ACT_AUDIENCE_COUNT; i++) {
        sys->members[i].timer = GFL_RandomLCAlt(60) + 1;
        sys->members[i].target = STA_ACT_AUDIENCE_NONE;
    }
}

// pokebw2's StaActAudience_SetCheerPoke
void func_ov132_021fa558(StaActAudience *sys, u8 pos, BOOL cheer) {
    sys->cheer[pos] = cheer;
    sys->refresh = TRUE;
}

// pokebw2's StaActAudience_SetLookLight
void func_ov132_021fa56c(StaActAudience *sys, u8 light) {
    sys->lookTarget = light;
    sys->refresh = TRUE;
}

// pokebw2's StaActAudience_SetScrollOffset
void func_ov132_021fa57c(StaActAudience *sys, int scroll) {
    u16 bgX = scroll * 128 / 256 + 64;

    GFL_BGSysMoveBG(4, BG_MOVE_SET_X, bgX);
    GFL_BGSysMoveBG(6, BG_MOVE_SET_X, bgX);
    GFL_BGSysMoveBG(5, BG_MOVE_SET_X, bgX);
}
