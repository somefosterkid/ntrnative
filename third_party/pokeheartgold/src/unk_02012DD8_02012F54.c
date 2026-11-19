#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200FA24.h"
#include "unk_0201010C.h"
#include "unk_02012DD8.h"

typedef enum HeapID HeapID;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_02012DD8_effect;

typedef struct UnkStruct_02012DD8_effect {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} UnkStruct_02012DD8_effect;

extern void sub_02010F00(SysTask *task, void *data);

BOOL sub_02012F54(UnkStruct_02012DD8_effect *effect);

BOOL sub_02012F54(UnkStruct_02012DD8_effect *effect) {
    effect->unk2C++;
    if (effect->unk2C >= effect->unk28) {
        effect->unk2C = 0;
        if (effect->unk24 - 1 > 0) {
            effect->unk1C += effect->unk20;
            effect->unk24--;
            while (effect->unk1C >= 0x0000FFFF) {
                effect->unk1C -= 0x0000FFFF;
            }
            while (effect->unk1C < 0) {
                effect->unk1C += 0x0000FFFF;
            }
            if (effect->unk1C >= 0x00007FFF && effect->unk1C < effect->unk18 && effect->unk40 == 1) {
                sub_02010FEC(effect->unk44, effect->unk38, effect->unk3C, effect->unk34);
                effect->unk40 = 0;
            }
            sub_020131AC(effect);
            SysTask_CreateOnVWaitQueue(sub_02010F00, effect, 0x3FF);
        } else {
            sub_0200FFB4(effect->unk48, effect->unk8, (HeapID)effect->unk30);
            return 1;
        }
    }
    return 0;
}
