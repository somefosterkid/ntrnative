#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_96_sprite_res_man.h"

SysTask *ov96_021EA8A8(Ov96SpriteSet *set, s32 count, Ov96SpriteResDesc *descs, Ov96SpriteResLoadParams *params, u32 priority, s32 baseIndex) {
    s32 i;
    s32 size;
    s32 prio;

    prio = priority;
    i = 0;
    if (set->count > 0) {
        do {
            if (descs[i].isLarge != 0) {
                size = 0x20;
            } else {
                size = 0x10;
            }
            set->elems[i].unk_14 = size;
            set->elems[i].unk_18 = size;
            switch (params->unk_14[i]) {
            case 1:
                set->elems[i].unk_10 = 3;
                break;
            case 2:
                set->elems[i].unk_10 = 4;
                break;
            case 3:
                set->elems[i].unk_10 = 5;
                break;
            default:
                GF_AssertFail();
                set->elems[i].unk_10 = 3;
                break;
            }
            if (ov96_021E9CF4(set->resSrc, 2) != 0) {
                set->elems[i].spriteA = ov96_021EAA88(set, params->unk_4, params->screen, 0x16);
                if (descs[i].isLarge != 0) {
                    Sprite_SetAnimCtrlSeq(set->elems[i].spriteA, 1);
                } else {
                    Sprite_SetAnimCtrlSeq(set->elems[i].spriteA, 0);
                }
                if ((u32)(u16)(descs[i].species + 0xFFCE) <= 1U) {
                    set->elems[i].hideSpriteA = 1;
                    Sprite_SetDrawFlag(set->elems[i].spriteA, 0);
                } else {
                    set->elems[i].hideSpriteA = 0;
                }
            }
            if (ov96_021E9CF4(set->resSrc, 4) != 0) {
                set->elems[i].spriteB = ov96_021EAA88(set, params->unk_4, params->screen, 0x15);
                if (descs[i].unk_8 >= 4) {
                    GF_AssertFail();
                }
                Sprite_SetAnimCtrlSeq(set->elems[i].spriteB, descs[i].unk_8);
                switch (params->unk_14[i]) {
                case 1:
                    set->elems[i].unk_0C = 3;
                    break;
                case 2:
                    set->elems[i].unk_0C = 4;
                    break;
                case 3:
                    set->elems[i].unk_0C = 5;
                    break;
                default:
                    GF_AssertFail();
                    set->elems[i].unk_0C = 3;
                    break;
                }
            }
            set->elems[i].priorityStep = 0xC;
            i++;
        } while (i < set->count);
    }
    if (prio <= 0) {
        prio = 1;
    }
    ov96_021E8B1C(set->man, count, descs, params, prio - 1, baseIndex);
    return SysTask_CreateOnMainQueue(ov96_021EAA24, set, prio);
}
