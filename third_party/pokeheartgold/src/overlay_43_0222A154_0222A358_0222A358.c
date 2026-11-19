#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "naming_screen.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "unk_0202C034.h"
#include "unk_02037C94.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_palpad.h"

BOOL sub_0202C2DC(WiFiList *wiFiList, int param1);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

void ov43_0222A414(struct PalPadShared *arg0, void *arg1);
void ov43_0222A41C(struct PalPadShared *arg0, s8 arg1);

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
    u8 unk_04[32];
} UnkStruct_ov64_022301B4;

u32 ov43_0222A358(PalPadShared *param0, String *param1, String *param2);

u32 ov43_0222A358(PalPadShared *param0, String *param1, String *param2) {
    int v0;
    WiFiList *v1;
    BOOL v2;
    BOOL v3;
    u64 v4;
    u64 v5;
    DWCFriendData *v6 = NULL;
    DWCUserData *v7 = NULL;
    int v8;
    int v9;

    v1 = sub_0202C6F4(param0->saveData);
    v5 = DWC_CreateFriendKey(sub_0202C08C(v1));

    for (v0 = 0; v0 < 32; v0++) {
        v2 = sub_0202C2DC(v1, v0);

        if (v2 == 0) {
            v4 = String_atoi(param1, &v3);

            if ((v3 == 1) && (v4 != v5)) {
                v7 = sub_0202C08C(v1);

                if (!DWC_CheckFriendKey(v7, v4)) {
                    return 1;
                }

                v8 = sub_0203A128(param0->saveData, v4, &v9);

                if (v8 == 0) {
                    return 2;
                }

                v6 = sub_0202C23C(v1, v0);
                DWC_CreateFriendKeyToken(v6, v4);

                sub_0202C270(v1, v0, param2);
                sub_0202C190(v1, v0, 8, 2);
                return 0;
            } else {
                return 1;
            }
        }
    }

    GF_ASSERT(FALSE);

    return 0;
}

void ov43_0222A414(PalPadShared *arg0, void *arg1) {
    *((void **)&arg0->initialName) = arg1;
    *((s32 *)&arg0->useInitialName) = 1;
}

void ov43_0222A41C(PalPadShared *arg0, s8 arg1) {
    arg0->selectedFriend = arg1;
}
