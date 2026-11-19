#include "global.h"
struct UnkStruct_ov61_0222C664;

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "text.h"
#include "render_window.h"
#include "overlay_39_unk_struct_ov61_0222_c664.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct UnkStruct_ov39_02228A8C {
    u8 unk_0[0x44];
    u8 unk_44;
    u8 unk_45[0x43];
    WaitingIcon *unk_88;
} UnkStruct_ov39_02228A8C;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

void ov39_02228A8C(UnkStruct_ov39_02228A8C *arg0);
void ov39_02228AA8(struct UnkStruct_ov61_0222C664 *arg0);

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

int ov39_02228A70(int param0);

int ov39_02228A70(int param0) {
    if ((param0 == 0xff) || (TextPrinterCheckActive(param0) == 0)) {
        return 0;
    }

    return 1;
}

void ov39_02228A8C(UnkStruct_ov39_02228A8C *arg0) {
    if (arg0->unk_88 == NULL) {
        arg0->unk_88 = WaitingIcon_New((void *)&arg0->unk_44, 1U);
    }
}

void ov39_02228AA8(UnkStruct_ov61_0222C664 *arg0) {
    WaitingIcon *waitingIcon;

    waitingIcon = *((WaitingIcon * *)&arg0->unk_88);
    if (waitingIcon != NULL) {
        sub_0200F450(waitingIcon);
        *((WaitingIcon * *)&arg0->unk_88) = NULL;
    }
}
