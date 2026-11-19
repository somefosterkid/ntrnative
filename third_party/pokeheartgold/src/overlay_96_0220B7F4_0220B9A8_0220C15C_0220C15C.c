#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "sys_task_api.h"

s32 ov96_021EAB38(void *, s32);

s32 ov96_021EAB74(void *, s32);

typedef struct Ov96_0220C15C_Work {
    u8 filler0[4];
    void *objs[4];
    u8 filler14[0x2C - 0x14];
    ManagedSprite *spriteA;
    ManagedSprite *spriteB;
    int active;
    u32 a : 2;
    u32 state : 4;
    u32 cnt : 8;
    u32 flag : 1;
    u32 pad0 : 8;
    u32 b : 2;
    u32 pad1 : 2;
    u32 c : 2;
} Ov96_0220C15C_Work;

void ov96_0220C15C(SysTask *arg0, void *param);

void ov96_0220C15C(SysTask *arg0, void *param) {
    Ov96_0220C15C_Work *w = param;
    s16 sp0[4];
    ManagedSprite *s;
    s32 d;
    s16 v;
    s32 x;

    switch (w->state) {
    case 0:
        s = w->spriteA;
        ManagedSprite_ResetSpriteAnimCtrlState(s);
        ManagedSprite_SetDrawFlag(s, 1);
        w->state++;
        break;
    case 1:
        w->cnt++;
        if (w->cnt >= 2) {
            s = w->spriteB;
            ov96_021EAB38((u8 *)w->objs[w->b], 0);
            ManagedSprite_SetAnim(s, 0x1B);
            ManagedSprite_SetAnimateFlag(s, 0);
            ManagedSprite_SetDrawFlag(s, 1);
            w->cnt = 0;
            w->state++;
        }
        break;
    case 2:
        ManagedSprite_GetPositionXY(w->spriteB, &sp0[3], &sp0[2]);
        d = -0x40 - sp0[2];
        x = (s32)((d + ((u32)(d >> 1) >> 30)) << 14) >> 16;
        if (sp0[2] <= -0x40 || x == 0) {
            w->state++;
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(w->spriteB, sp0[3], sp0[2] + x, FX32_CONST(256));
        break;
    case 3:
        w->cnt++;
        if (w->cnt >= 4) {
            w->cnt = 0;
            w->state++;
        }
        break;
    case 4:
        ManagedSprite_GetPositionXY(w->spriteB, &sp0[1], &sp0[0]);
        d = -0x18 - sp0[0];
        if (d < 0) {
            d = -d;
        }
        v = d;
        if (v != 0) {
            v = v / 2;
        }
        sp0[0] += v + 2;
        if (sp0[0] >= 0x30) {
            sp0[1] = w->c * 64 + 0x48;
            sp0[0] = 0x30;
            ManagedSprite_SetPositionXY(w->spriteB, sp0[1], 0x30);
            ManagedSprite_SetAnimateFlag(w->spriteB, 1);
            ManagedSprite_ResetSpriteAnimCtrlState(w->spriteA);
            w->state++;
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(w->spriteB, sp0[1], sp0[0], FX32_CONST(256));
        break;
    case 5:
        w->cnt++;
        if (w->cnt >= 2) {
            ManagedSprite_SetDrawFlag(w->spriteB, 0);
            ov96_021EAB38((u8 *)w->objs[w->a], 1);
            ov96_021EAB74((u8 *)w->objs[w->a], 0);
            w->b = w->a;
            w->cnt = 0;
            w->state++;
        }
        break;
    case 6:
        w->active = 0;
        w->flag = 0;
        SysTask_Destroy(arg0);
        break;
    default:
        GF_AssertFail();
        break;
    }
}
