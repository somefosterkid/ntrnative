#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "gf_gfx_planes.h"
#include "palette.h"

struct PaletteAnimator;
struct PaletteData;
typedef struct PaletteAnimator PaletteAnimator;
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
static u8 sub_02016548(PaletteAnimator *param0);

void sub_020164D0(SysTask *param0, void *param1);
static u8 sub_02016548(PaletteAnimator *param0);
void sub_020165A4(PaletteAnimator *param0, u16 *param1);
void sub_020165BC(PaletteAnimator *param0, u16 *param1);
void sub_020165D4(PaletteAnimator *param0, u16 *param1);
void sub_020165F0(PaletteAnimator *param0, u16 *param1);

void sub_020164D0(SysTask *param0, void *param1) {
    PaletteAnimator *v0 = (PaletteAnimator *)param1;

    switch (v0->unk_4E) {
    case 0:
        v0->unk_4F = 0;
        v0->unk_4E = 1;
        break;
    case 1:
        if (sub_02016548(v0) == 1) {
            v0->unk_04(v0, v0->unk_2C);
        }

        v0->unk_4F++;

        if (v0->unk_4F == 32) {
            v0->unk_4F = 0;
        }
        break;
    case 2:
        break;
    case 3:
        v0->unk_04(v0, v0->unk_0C);
        DestroySysTaskAndEnvironment(param0);
    }
}

static u8 sub_02016548(PaletteAnimator *param0) {
    u32 v0;

    if (param0->unk_4F == 0) {
        for (v0 = 0; v0 < 16; v0++) {
            if ((0x7800 & (1 << v0)) == 0) {
                continue;
            }

            param0->unk_2C[v0] = param0->unk_0C[v0];
        }

        return 1;
    } else if (param0->unk_4F == 24) {
        for (v0 = 0; v0 < 16; v0++) {
            if ((0x7800 & (1 << v0)) == 0) {
                continue;
            }

            param0->unk_2C[v0] = param0->unk_0C[15];
        }

        return 1;
    }

    return 0;
}

void sub_020165A4(PaletteAnimator *param0, u16 *param1) {
    BG_LoadPlttData(GF_BG_LYR_MAIN_0, param1, ((16) * sizeof(u16)), ((param0->unk_4D) * ((16) * sizeof(u16))));
}

void sub_020165BC(PaletteAnimator *param0, u16 *param1) {
    BG_LoadPlttData(GF_BG_LYR_SUB_0, param1, ((16) * sizeof(u16)), ((param0->unk_4D) * ((16) * sizeof(u16))));
}

void sub_020165D4(PaletteAnimator *param0, u16 *param1) {
    PaletteData_LoadPalette(param0->unk_08, param1, PLTTBUF_MAIN_BG, ((param0->unk_4D) * (16)), ((16) * sizeof(u16)));
}

void sub_020165F0(PaletteAnimator *param0, u16 *param1) {
    PaletteData_LoadPalette(param0->unk_08, param1, PLTTBUF_SUB_BG, ((param0->unk_4D) * (16)), ((16) * sizeof(u16)));
}
