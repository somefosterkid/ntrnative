#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "list_menu.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_palpad.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

typedef struct {
    String *unk_00;
    String *unk_04;
} UnkStruct_ov64_0222E060_sub1;

typedef struct {
    const u16 *unk_00;
    BOOL unk_04;
} UnkStruct_ov64_0222E060_sub2;

typedef struct {
    u32 unk_00;
    u8 pad_4[4];
    u8 unk_04[32];
} UnkStruct_ov64_022301B4;

BOOL ov43_0222DCC4(PalPadShared *param0, int param1);

BOOL ov43_0222DCC4(PalPadShared *param0, int param1) {
    int v0;
    u32 v1;
    s32 v2;
    WiFiList *v3 = sub_0202C6F4(param0->saveData);
    v2 = param0->selectedFriend;

    if (param1 == 0) {
        for (v0 = 0; v0 < param0->friends.count - 1; v0++) {
            v2--;

            if (v2 < 0) {
                v2 += param0->friends.count;
            }

            v1 = sub_0202C090(v3, param0->friends.slots[v2], 8);

            if (v1 != 2) {
                param0->selectedFriend = v2;
                return 1;
            }
        }
    } else {
        for (v0 = 0; v0 < param0->friends.count - 1; v0++) {
            v2 = (v2 + 1) % param0->friends.count;

            v1 = sub_0202C090(v3, param0->friends.slots[v2], 8);

            if (v1 != 2) {
                param0->selectedFriend = v2;
                return 1;
            }
        }
    }

    return 0;
}
