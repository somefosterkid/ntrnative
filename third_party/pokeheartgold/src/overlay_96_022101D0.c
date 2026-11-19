#include "global.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov96_022101D0(UnkStruct_ov96_0220E3F0_Hud *arg0);

void ov96_022101D0(UnkStruct_ov96_0220E3F0_Hud *arg0) {
    SysTask *task;
    s32 v0;
    s32 v1;
    s32 v5;
    ManagedSprite **v3;
    UnkStruct_ov96_0220E3F0_HudGroup *v4;

    v0 = 0;
    do {
        if (arg0->slots[v0].sprite == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(arg0->slots[v0].sprite);
        task = arg0->slots[v0].anim.task;
        if (task != NULL) {
            SysTask_Destroy(task);
        }
        v0 += 1;
    } while (v0 < 0xC);
    v5 = 0;
    v4 = arg0->groups;
    do {
        v1 = 0;
        v3 = v4->unk_0;
loop_8:
        Sprite_DeleteAndFreeResources(v3[0]);
        Sprite_DeleteAndFreeResources(v3[2]);
        v1 += 1;
        v3++;
        if (v1 < 2) {
            goto loop_8;
        }
        Sprite_DeleteAndFreeResources(v4->unk_10);
        Sprite_DeleteAndFreeResources(v4->unk_14);
        v5 += 1;
        v4++;
    } while (v5 < 2);
}
