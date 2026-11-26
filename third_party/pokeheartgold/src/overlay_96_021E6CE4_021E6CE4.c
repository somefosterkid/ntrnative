#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern const PokeathlonStateFunc *const _0221DA00[];

extern const PokeathlonEventHandlers *const ov96_0221DA28[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021F010C(PokeathlonCourseData *, int);
BOOL ov96_021F08E4(PokeathlonCourseData *, int);
BOOL ov96_021F08F4(PokeathlonCourseData *, int);
BOOL ov96_021F095C(PokeathlonCourseData *, int);
BOOL ov96_021F5018(PokeathlonCourseData *, int);
BOOL ov96_021F54C0(PokeathlonCourseData *, int);
BOOL ov96_021F54D4(PokeathlonCourseData *, int);
BOOL ov96_021F553C(PokeathlonCourseData *, int);
BOOL ov96_021F94A8(PokeathlonCourseData *, int);
BOOL ov96_021F9CA4(PokeathlonCourseData *, int);
BOOL ov96_021F9CC4(PokeathlonCourseData *, int);
BOOL ov96_021F9D58(PokeathlonCourseData *, int);
BOOL ov96_021FC768(PokeathlonCourseData *, int);
BOOL ov96_021FCD74(PokeathlonCourseData *, int);
BOOL ov96_021FCD84(PokeathlonCourseData *, int);
BOOL ov96_021FCE10(PokeathlonCourseData *, int);
BOOL ov96_02201558(PokeathlonCourseData *, int);
BOOL ov96_02201B18(PokeathlonCourseData *, int);
BOOL ov96_02201B34(PokeathlonCourseData *, int);
BOOL ov96_02201B98(PokeathlonCourseData *, int);
BOOL ov96_022047EC(PokeathlonCourseData *, int);
BOOL ov96_02204DE8(PokeathlonCourseData *, int);
BOOL ov96_02204DF8(PokeathlonCourseData *, int);
BOOL ov96_02204E58(PokeathlonCourseData *, int);
BOOL ov96_02209448(PokeathlonCourseData *, int);
BOOL ov96_02209738(PokeathlonCourseData *, int);
BOOL ov96_02209748(PokeathlonCourseData *, int);
BOOL ov96_022097AC(PokeathlonCourseData *, int);
BOOL ov96_0220E3F0(PokeathlonCourseData *, int);
BOOL ov96_0220E5F4(PokeathlonCourseData *, int);
BOOL ov96_0220E60C(PokeathlonCourseData *, int);
BOOL ov96_0220E670(PokeathlonCourseData *, int);
BOOL ov96_02210C98(PokeathlonCourseData *, int);
BOOL ov96_02211650(PokeathlonCourseData *, int);
BOOL ov96_02211660(PokeathlonCourseData *, int);
BOOL ov96_022116C8(PokeathlonCourseData *, int);
BOOL ov96_02215AC4(PokeathlonCourseData *, int);
BOOL ov96_02215CD0(PokeathlonCourseData *, int);
BOOL ov96_02215CE8(PokeathlonCourseData *, int);
BOOL ov96_02215D4C(PokeathlonCourseData *, int);

static const PokeathlonEventHandlers ov96_0221A90C = {
    .funcs = { ov96_021F94A8, ov96_021F9CA4, ov96_021F9CC4, ov96_021F9D58 },
    .flag = 1,
};

static const PokeathlonEventHandlers ov96_0221A8D0 = {
    .funcs = { ov96_022047EC, ov96_02204DE8, ov96_02204DF8, ov96_02204E58 },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A7F4 = {
    .funcs = { ov96_021F010C, ov96_021F08E4, ov96_021F08F4, ov96_021F095C },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A830 = {
    .funcs = { ov96_02209448, ov96_02209738, ov96_02209748, ov96_022097AC },
    .flag = 1,
};

static const PokeathlonEventHandlers ov96_0221A8A8 = {
    .funcs = { ov96_0220E3F0, ov96_0220E5F4, ov96_0220E60C, ov96_0220E670 },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A920 = {
    .funcs = { ov96_021F5018, ov96_021F54C0, ov96_021F54D4, ov96_021F553C },
    .flag = 1,
};

static const PokeathlonEventHandlers ov96_0221A8F8 = {
    .funcs = { ov96_021FC768, ov96_021FCD74, ov96_021FCD84, ov96_021FCE10 },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A880 = {
    .funcs = { ov96_02215AC4, ov96_02215CD0, ov96_02215CE8, ov96_02215D4C },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A858 = {
    .funcs = { ov96_02201558, ov96_02201B18, ov96_02201B34, ov96_02201B98 },
    .flag = 0,
};

static const PokeathlonEventHandlers ov96_0221A81C = {
    .funcs = { ov96_02210C98, ov96_02211650, ov96_02211660, ov96_022116C8 },
    .flag = 0,
};

BOOL ov96_021F0A7C(PokeathlonCourseData *, u8 *);
BOOL ov96_021F0AE0(PokeathlonCourseData *, u8 *);
BOOL ov96_021F0B58(PokeathlonCourseData *, u8 *);
BOOL ov96_021F5650(PokeathlonCourseData *, u8 *);
BOOL ov96_021F5730(PokeathlonCourseData *, u8 *);
BOOL ov96_021F9234(PokeathlonCourseData *, u8 *);
BOOL ov96_021F9240(PokeathlonCourseData *, u8 *);
BOOL ov96_021F9380(PokeathlonCourseData *, u8 *);
BOOL ov96_021FD2E0(PokeathlonCourseData *, u8 *);
BOOL ov96_021FD3EC(PokeathlonCourseData *, u8 *);
BOOL ov96_021FD46C(PokeathlonCourseData *, u8 *);
BOOL ov96_02202154(PokeathlonCourseData *, u8 *);
BOOL ov96_022021D4(PokeathlonCourseData *, u8 *);
BOOL ov96_022052B0(PokeathlonCourseData *, u8 *);
BOOL ov96_0220549C(PokeathlonCourseData *, u8 *);
BOOL ov96_02205520(PokeathlonCourseData *, u8 *);
BOOL ov96_02209130(PokeathlonCourseData *, u8 *);
BOOL ov96_0220935C(PokeathlonCourseData *, u8 *);
BOOL ov96_022093D0(PokeathlonCourseData *, u8 *);
BOOL ov96_0220E22C(PokeathlonCourseData *, u8 *);
BOOL ov96_0220E2DC(PokeathlonCourseData *, u8 *);
BOOL ov96_0220E394(PokeathlonCourseData *, u8 *);

static const PokeathlonStateFunc ov96_0221DC30[] = { ov96_021F9234, ov96_021F9240, ov96_021F9380 };
static const PokeathlonStateFunc ov96_0221DC50[] = { ov96_022052B0, ov96_0220549C, ov96_02205520 };
static const PokeathlonStateFunc ov96_0221DB50[] = { ov96_021F0A7C, ov96_021F0AE0, ov96_021F0B58 };
static const PokeathlonStateFunc ov96_0221DC5C[] = { ov96_02209130, ov96_0220935C, ov96_022093D0 };
static const PokeathlonStateFunc ov96_0221DC74[] = { ov96_0220E22C, ov96_0220E2DC, ov96_0220E394 };
static const PokeathlonStateFunc ov96_0221DC1C[] = { ov96_021F5650, ov96_021F5730 };
static const PokeathlonStateFunc ov96_0221DC3C[] = { ov96_021FD2E0, ov96_021FD3EC, ov96_021FD46C };
static const PokeathlonStateFunc ov96_0221DC48[] = { ov96_02202154, ov96_022021D4 };
extern PokeathlonStateFunc ov96_0221DC90[];
extern PokeathlonStateFunc ov96_0221DC88[];

const PokeathlonStateFunc *const _0221DA00[] = { ov96_0221DC30, ov96_0221DC50, ov96_0221DB50, ov96_0221DC5C, ov96_0221DC74, ov96_0221DC1C, ov96_0221DC3C, ov96_0221DC90, ov96_0221DC48, ov96_0221DC88 };
const PokeathlonEventHandlers *const ov96_0221DA28[] = { &ov96_0221A90C, &ov96_0221A8D0, &ov96_0221A7F4, &ov96_0221A830, &ov96_0221A8A8, &ov96_0221A920, &ov96_0221A8F8, &ov96_0221A880, &ov96_0221A858, &ov96_0221A81C };
#endif

BOOL ov96_021E6CE4(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6CE4(PokeathlonCourseData *arg0, u8 *unused) {
    u8 idx = arg0->field_3D8[arg0->currentParticipantIndex];
    arg0->unk_728 = 0;
    arg0->filler_D2A = 0xFFFF;
    arg0->filler_72B[0] = 0;
    arg0->unk_D6C = 0;
    PokeathlonCourse_ResetDataCopyArea(arg0);
    arg0->state = ov96_0221DA28[idx];
    arg0->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(_0221DA00[idx], &arg0->subStateInfo);
    PokeathlonCourse_SetStateField07(arg0, 0x10);
    return 0;
}
