#include "global.h"
#include "constants/heap.h"
#include "error_handling.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222C5B4(void *arg0, s32 arg1);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2);
void ov45_0222C5E8(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
BOOL ov45_0222C61C(const UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
int ov45_0222C658(const UnkStruct_ov66_022302B0 *param0, u32 param1);
u32 ov45_0222A920(const WiFiPlazaProfile *profile);
BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);
BOOL ov45_0222B048(const WiFiPlazaSystem *param0, u32 param1);
u32 ov45_0222EC68(s32 param0);
BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
BOOL ov45_0222BD74(const UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BE08(WiFiPlazaSystem *param0, s32 param1, BOOL param2);
void ov45_0222BE48(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222C688(s32 param0, const void *param1, u32 param2, void *param3);
void ov45_0222BD4C(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BE54(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222C73C(s32 param0, const void *param1, u32 param2, void *param3);
void ov45_0222BD5C(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222C79C(s32 param0, const void *param1, u32 param2, void *param3);

s32 ov45_0222C5B4(void *arg0, s32 arg1) {
    s32 v1;
    s32 v2;
    void *v0;
    void *v3;

    v0 = arg0;
    v1 = 0;
loop_1:
    v2 = 0;
    v3 = v0;
loop_2:
    if (arg1 == *(s32 *)((u8 *)v3 + 0x50)) {
        return v2 + (v1 * 3);
    }
    v2 += 1;
    v3 += 4;
    if (v2 >= 3) {
        v1 += 1;
        v0 += 0xC;
        if (v1 >= 9) {
            return -1;
        }
        goto loop_1;
    }
    goto loop_2;
}

static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2) {
    *param1 = param0 / 3;
    *param2 = param0 % 3;
}

void ov45_0222C5E8(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);
    param0->unk_BC[v0][v1] = param2 + 1;
}

BOOL ov45_0222C61C(const UnkStruct_ov66_022302B0 *param0, u32 param1, int param2) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);

    if (param0->unk_BC[v0][v1] <= param2) {
        return 0;
    }

    return 1;
}

int ov45_0222C658(const UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);

    return param0->unk_BC[v0][v1];
}

void ov45_0222C688(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1;
    const UnkStruct_ov66_022305B0 *v2;
    u32 v3;
    BOOL v4;

    v2 = param1;
    v1 = ov45_0222EC68(param0);

    if (ov45_0222BADC(&v0->unk_108, v0->saveData) == 0) {
        v0->unk_52C = 1;
    }

    v3 = ov45_0222A920(&v0->unk_108.profile);
    v4 = ov45_0222AFF8(v0);

    if ((v3 != 1) || (v1 == 0xffffffff) || (v2->unk_02 != 1) || (v4 == 1) || (ov45_0222B048(v0, v1) == 0)) {
        ov45_0222BE08(v0, param0, 0);
        return;
    }

    ov45_0222BE08(v0, param0, 1);

    if (ov45_0222BD74(&v0->unk_1C0, v1) == 0) {
        v0->unk_1C0.unk_0B = 1;
    } else {
        v0->unk_1C0.unk_0B = 0;
    }

    v0->unk_1C0.unk_04 = v1;
    v0->unk_1C0.unk_06 = 2;
    v0->unk_1C0.unk_0A = 2;
    v0->unk_1C0.unk_07 = 0;

    ov45_0222BE48(&v0->unk_1C0);
}

void ov45_0222C73C(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1;
    const UnkStruct_ov66_022305B0 *v2 = param1;
    v1 = ov45_0222EC68(param0);

    if (v0->unk_1C0.unk_0A == 0) {
        return;
    }

    if (v0->unk_1C0.unk_04 == v1) {
        if (v2->unk_00 == 1) {
            if (ov45_0222BD74(&v0->unk_1C0, v1) == 0) {
                v0->unk_1C0.unk_0B = 1;
            } else {
                v0->unk_1C0.unk_0B = 0;
            }

            v0->unk_1C0.unk_06 = 2;
        } else {
            ov45_0222BD4C(&v0->unk_1C0);
        }

        ov45_0222BE54(&v0->unk_1C0);
    } else {
        (void)0;
    }
}

void ov45_0222C79C(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1;
    const UnkStruct_ov66_022305B0 *v2;

    if (v0->unk_1C0.unk_0A == 0) {
        return;
    }

    v2 = param1;
    v1 = ov45_0222EC68(param0);

    if (v0->unk_1C0.unk_04 == v1) {
        if ((v2->unk_02 == 2) || (v2->unk_02 == 3)) {
            v0->unk_1C0.unk_06 = v2->unk_02;

            v0->unk_1C0.unk_08 = v2->unk_00;

            ov45_0222BE54(&v0->unk_1C0);
        } else {
            ov45_0222BD5C(&v0->unk_1C0);
        }
    } else {
        if (v1 == 0xffffffff) {
            ov45_0222BD5C(&v0->unk_1C0);
        }
    }
}
