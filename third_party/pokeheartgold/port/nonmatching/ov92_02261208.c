#include "global.h"
#include "sprite_system.h"
struct ManagedSprite;
struct NARC;
struct SysTask;
struct SysTaskManager;
typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;
typedef struct SysTask SysTask;
typedef struct SysTaskManager SysTaskManager;
typedef void (*SysTaskFunc)(SysTask *, void *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0)
{
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail()) ;
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
        } while (1 );
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1)
{
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

void ov92_02261208(UnkStruct_ov116_02265470 *param0);
typedef struct {
    s16 a[4];
} ov92_02263E3C_Array;
extern const ov92_02263E3C_Array ov92_02263E3C;
extern const ov92_02263E3C_Array ov92_02263E04;
void ov92_02261208(UnkStruct_ov116_02265470 *param0)
{
    int v0;
    s16 v1, v2;
    ov92_02263E3C_Array v3 = ov92_02263E04;
    BOOL v4[4] = { 1, 1, 1, 1 };
    ManagedSprite *v5;

    if (param0->unk_04 == 0) {
        return;
    }

    switch (param0->unk_00) {
    case 0:
        for (v0 = 0; v0 < 4; v0++) {
            v5 = param0->unk_08[v0];
            if (v5 == NULL) {
                continue;
            }

            ManagedSprite_GetPositionXY(v5, &v1, &v2);
            if (v2 > v3.a[v0]) {
                v2 -= 6;
                ManagedSprite_SetPositionXY(v5, v1, v2);
                v4[v0] = 0;
            }
        }
        if (v4[0] && v4[1] && v4[2] && v4[2]) {
            param0->unk_28[0] = 0;
            param0->unk_50[0] = 0;
            param0->unk_00++;
        }

        break;

    case 1:
        if (param0->unk_50[0] == 1) {
            if (++param0->unk_28[0] >= 15) {
                param0->unk_00++;
            }
        }

        break;

    case 2:
        for (v0 = 0; v0 < 4; v0++) {
            v5 = param0->unk_08[v0];
            if (v5 == NULL) {
                continue;
            }

            ManagedSprite_GetPositionXY(v5, &v1, &v2);
            if (v2 < (192 + 32)) {
                v2 += 6;
                ManagedSprite_SetPositionXY(v5, v1, v2);
                v4[v0] = 0;
            }
        }
        if (v4[0] && v4[1] && v4[2] && v4[2]) {
            param0->unk_00++;
        }

        break;

    default:
        param0->unk_50[0] = 0;
        param0->unk_04 = 0;
        param0->unk_00 = 0;

        break;
    }

    for (v0 = 0; v0 < 4; v0++) {
        v5 = param0->unk_08[v0];
        if (v5 == NULL) {
            continue;
        }

        ManagedSprite_TickTwoFrames(v5);
    }
}
