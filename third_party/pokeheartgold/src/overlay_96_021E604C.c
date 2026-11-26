#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "pokeathlon/pokeathlon.h"
#include "follow_mon.h"
#include "overlay_96_sprite_res_man.h"

u32 ov96_021E679C(s32, s32);

void ov96_021E604C(PokeathlonCourseData *arg0);

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

void ov96_021E6168(PokeathlonCourseData *data, s32 index, s32 arg2, Ov96SpriteResDesc *out);

void ov96_021E604C(PokeathlonCourseData *arg0) {
    NARC *narc;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 i;
    s32 j;

    narc = NARC_New(NARC_poketool_personal_performance, arg0->heapId);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            NARC_ReadWholeMember(narc, ov96_021E679C(arg0->participants[i].mons[j].species, arg0->participants[i].mons[j].unk_2), &arg0->monPerf[i][j]);
        }
    }
#else
    s32 v2;
    void *v1;
    void *v0;
    s32 v3;
    s32 v5;
    void *v4;
    void *dest;

    v0 = arg0;
    narc = NARC_New(NARC_poketool_personal_performance, arg0->heapId);
    v2 = 0;
    v1 = arg0->monPerf;
    do {
        v4 = v0;
        dest = v1;
        v5 = 0;
loop_2:
        NARC_ReadWholeMember(narc, ov96_021E679C(*(u16 *)((u8 *)v4 + 0x3F0), *(u16 *)((u8 *)v4 + 0x3F2)), dest);
        v5 += 1;
        v4 += 0x28;
        dest += 0x14;
        if (v5 < 3) {
            goto loop_2;
        }
        v0 += 0x7C;
        v1 += 0x3C;
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 4);
#endif
    NARC_Delete(narc);
}

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *arg0, s32 arg1, s32 arg2) {
    return &arg0->monPerf[arg1][arg2];
}

PokeathlonMonKinds *ov96_021E60D8(PokeathlonCourseData *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 4) {
        GF_AssertFail();
    }
    if (arg2 >= 3) {
        GF_AssertFail();
    }
    return &arg0->participants[arg1].mons[arg2].kinds;
}

s32 ov96_021E6104(void);

s32 ov96_021E6104(void) {
    return 0x50;
}

s32 ov96_021E6108(void *arg0);

s32 ov96_021E6108(void *arg0) {
    u8 v0;

    if (arg0 == NULL) {
        return 0;
    }
    v0 = *(u8 *)((u8 *)arg0 + 8);
    switch (v0) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov96_021E6138(PokeathlonMonPerf *arg0);

s32 ov96_021E6138(PokeathlonMonPerf *arg0) {
    u8 v0;

    if (arg0 == NULL) {
        return 0;
    }
    v0 = arg0->unk_6;
    switch (v0) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

void ov96_021E6168(PokeathlonCourseData *data, s32 index, s32 arg2, Ov96SpriteResDesc *out) {
    s32 dest;
    PokeathlonMonEntry *mon;

    if (arg2 >= 3) {
        GF_AssertFail();
    }
    mon = (PokeathlonMonEntry *)PokeathlonCourse_GetParticipantUnk04(data, index) + arg2;
    out->unk_6 = mon->unk_10;
    out->species = mon->species;
    out->unk_2 = mon->unk_2;
    out->unk_7 = mon->unk_11;
    out->unk_8 = index;
    out->unk_C = mon->unk_4;
    if (out->species == 0) {
        GF_AssertFail();
    }
    if (out->species == 0) {
        out->species = 1U;
    }
    ReadWholeNarcMemberByIdPair(&dest, NARC_fielddata_tsurepoke_tp_param, SpeciesToOverworldModelIndexOffset(out->species));
    if (*(u8 *)((u8 *)(&dest) + 1) != 0) {
        out->isLarge = 1;
        return;
    }
    out->isLarge = 0;
}
