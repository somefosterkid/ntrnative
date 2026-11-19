#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

s32 ov96_021ED6E8(void);

s32 ov96_021ED6E8(void) {
    return (MTRandom() & 7) + 4;
}

u8 ov96_021ED6F8(PokeathlonCourseData *arg0, s32 arg1);

u8 ov96_021ED6F8(PokeathlonCourseData *data, s32 arg1) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return v1->u8s[arg1];
}

s32 ov96_021E95D8(void *, s32, s32);

void ov96_021ED728(PokeathlonCourseData *arg0, s32 arg1, u8 arg2);

void ov96_021ED728(PokeathlonCourseData *data, s32 arg1, u8 index) {
    u8 v0;

    v0 = (u8)PokeathlonCourse_GetField3D8_AtIndex(data, index);
    ov96_021E95D8(PokeathlonCourse_GetGraphicsSystem(data), v0, arg1);
}

int ov96_021ED748(void);

int ov96_021ED748(void) {
    return 10;
}

int ov96_021ED74C(void);

int ov96_021ED74C(void) {
    return sizeof(UnkStruct_ov117_022621D4);
}

int ov96_021ED750(void);

int ov96_021ED750(void) {
    return 10;
}

void ov96_021ED754(PokeathlonCourseData *arg0);

void ov96_021ED754(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    MATH_CountPopulation((u32)(v1->pairs[0].word1 << 5) >> 0x14);
}

void ov96_021ED78C(PokeathlonCourseData *arg0);

void ov96_021ED78C(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    MATH_CountPopulation((u32)(v1->pairs[1].word0 * 0x10) >> 0x14);
}

void ov96_021ED7C4(PokeathlonCourseData *arg0);

void ov96_021ED7C4(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    MATH_CountPopulation((u32)(v1->pairs[0].word1 << 0x11) >> 0x14);
}

void ov96_021ED7FC(PokeathlonCourseData *param_1, s32 param_2);

void ov96_021ED7FC(PokeathlonCourseData *param_1, s32 param_2) {
    u8 *puVar1;
    u8 *puVar2;
    s32 iVar3;

    puVar1 = PokeathlonCourse_GetDataCopyArea(param_1);
    puVar2 = ov96_021E8A20(puVar1 + 0xf0);
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (puVar1 == NULL) {
        GF_AssertFail();
    }
    if (puVar2 == NULL) {
        GF_AssertFail();
    }
    iVar3 = 0;
    do {
        *(u8 *)(param_2 + iVar3) = puVar2[iVar3 + 0xd];
        iVar3 = iVar3 + 1;
    } while (iVar3 < 4);
    return;
}

s32 ov96_021ED838(PokeathlonCourseData *arg0, s32 arg1);

s32 ov96_021ED838(PokeathlonCourseData *data, s32 arg1) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return v1->pairs[arg1].word0;
}

u32 ov96_021ED86C(PokeathlonCourseData *arg0, s32 arg1);

u32 ov96_021ED86C(PokeathlonCourseData *data, s32 arg1) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return (u32)(v1->pairs[arg1].word1 << 8) >> 8;
}

u32 ov96_021ED8A4(PokeathlonCourseData *arg0, s32 arg1);

u32 ov96_021ED8A4(PokeathlonCourseData *data, s32 arg1) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return (u32) v1->pairs[arg1].word1 >> 0x18;
}

s32 ov96_021EE0AC(u8 *, PokeathlonCourseData *);

s32 ov96_021EE144(u8 *, PokeathlonCourseData *, s32);

s32 ov96_021EE1D8(u8 *, PokeathlonCourseData *, u8);

void ov96_021ED8DC(PokeathlonCourseData *arg0);

void ov96_021ED8DC(PokeathlonCourseData *data) {
    s32 v2;
    u8 *v0;
    u32 v1;

    v0 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0x28);
    if (PokeathlonCourse_GetMode(data) == 0) {
        v1 = 0;
        do {
            if (v1 == ov96_021E5F24(data)) {
                ov96_021EE0AC(v0, data);
            } else {
                ov96_021EE1D8(v0, data, v1);
            }
            v1 += 1;
            v0 += 8;
        } while ((s32)v1 < 4);
        return;
    }
    v2 = 0;
    do {
        if ((s32)PokeathlonCourse_GetParticipantCount(data) > v2) {
            ov96_021EE144(v0, data, (u8)v2);
        } else {
            ov96_021EE1D8(v0, data, (u8)v2);
        }
        v2 += 1;
        v0 += 8;
    } while (v2 < 4);
}
