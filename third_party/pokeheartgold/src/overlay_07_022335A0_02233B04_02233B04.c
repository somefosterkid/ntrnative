#include "global.h"
struct BallRotation;

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_ball_rotation.h"
#include "particle_system.h"

typedef struct UnkStruct_ov12_02236648_t UnkStruct_ov12_02236648;

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

typedef struct {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    enum HeapID heapID;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02236690;

struct UnkStruct_ov12_02236648_t {
    UnkStruct_ov12_02236690 unk_00;
    SPLEmitter *unk_14;
    ParticleSystem *unk_18;
    int unk_1C;
    int unk_20;
};

UnkStruct_ov12_02236648 *ov07_02232D20(UnkStruct_ov12_02236690 *param0);

void ov07_02232D80(void *arg0);

s32 ov07_02232DF4(void *arg0);

void ov07_02232E10(void *);

void ov07_022344C0(void *, u32);

u32 ov07_02233B04(struct BallRotation *param_1);

u32 ov07_02233B04(BallRotation *param_1) {
    UnkStruct_ov12_02236648 *pUVar1;
    s32 sVar2;
    UnkStruct_ov12_02236690 UStack_20;

    switch (*((u32 *)&param_1->unk_08)) {
    case 0:
        ov07_022344C0(param_1, 0);
        ManagedSprite_SetAnim(param_1->unk_30, 1);
        *((s32 *)&param_1->unk_B8.unk_0C) = *((s32 *)&param_1->unk_B8.unk_0C) + 1;
        if (*((s32 *)&param_1->unk_B8.unk_0C) >= 2) {
            ManagedSprite_SetAnimationFrame(param_1->unk_30, 0);
            *((u32 *)&param_1->unk_B8.unk_0C) = 0;
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    case 1:
        *((s32 *)&param_1->unk_B8.unk_0C) = *((s32 *)&param_1->unk_B8.unk_0C) + 1;
        if (*((s32 *)&param_1->unk_B8.unk_0C) >= 2) {
            ManagedSprite_SetAnimationFrame(param_1->unk_30, 2);
            *((u32 *)&param_1->unk_B8.unk_0C) = 0;
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    case 2:
        UStack_20.unk_04 = *((s32 *)&param_1->unk_90.ball);
        UStack_20.heapID = *((enum HeapID *)&param_1->unk_90.heapID);
        UStack_20.unk_0C = 0xff;
        UStack_20.unk_10 = 1;
        ManagedSprite_GetPositionXY(param_1->unk_30, &UStack_20.unk_00, &UStack_20.unk_02);
        pUVar1 = ov07_02232D20(&UStack_20);
        *((UnkStruct_ov12_02236648 * *)&param_1->unk_D0) = pUVar1;
        *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        break;
    case 3:
        ov07_02232D80(*((void * *)&param_1->unk_D0));
        *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        break;
    case 4:
        *((s32 *)&param_1->unk_B8.unk_0C) = *((s32 *)&param_1->unk_B8.unk_0C) + 1;
        if (*((s32 *)&param_1->unk_B8.unk_0C) >= 6) {
            ManagedSprite_SetAnimationFrame(param_1->unk_30, 2);
            *((u32 *)&param_1->unk_B8.unk_0C) = 0;
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    case 5:
        *((s32 *)&param_1->unk_B8.unk_0C) = *((s32 *)&param_1->unk_B8.unk_0C) + 1;
        if (*((s32 *)&param_1->unk_B8.unk_0C) >= 6) {
            ManagedSprite_SetAnimationFrame(param_1->unk_30, 1);
            *((u32 *)&param_1->unk_B8.unk_0C) = 0;
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    case 6:
        *((s32 *)&param_1->unk_B8.unk_0C) = *((s32 *)&param_1->unk_B8.unk_0C) + 1;
        if (*((s32 *)&param_1->unk_B8.unk_0C) >= 6) {
            ManagedSprite_SetAnimationFrame(param_1->unk_30, 0);
            *((u32 *)&param_1->unk_B8.unk_0C) = 0;
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    case 7:
        sVar2 = ov07_02232DF4(*((void * *)&param_1->unk_D0));
        if (sVar2 == 0) {
            ov07_02232E10(*((void * *)&param_1->unk_D0));
            *((s32 *)&param_1->unk_08) = *((s32 *)&param_1->unk_08) + 1;
        }
        break;
    default:
        return 0;
    }
    return 1;
}
