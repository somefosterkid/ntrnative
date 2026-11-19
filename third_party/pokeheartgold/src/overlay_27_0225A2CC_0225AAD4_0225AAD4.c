#include "global.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "overlay_27.h"
#include "sprite.h"
#include "start_menu.h"
#include "overlay_27_ov27task.h"


s32 ov27_0225C1AC(Ov27Task *, s32);

// One start-menu icon's graphics: its sprite (unk_390 index), the sprite's "enabled" animation and its label window (unk_3F0 index).
typedef struct Ov27IconGfx {
    u16 sprite;
    u16 anim;
    u8 window;
} Ov27IconGfx;

extern const Ov27IconGfx ov27_0225CF10[7];

void ov27_0225AAD4(Ov27Task *task);

void ov27_0225AAD4(Ov27Task *task) {
    s32 icon;
    const Ov27IconGfx *gfx;
    s32 after;
    s32 before;

    before = ov27_0225AA60(task->unk_514, 7);
    after = before;
    gfx = ov27_0225CF10;
    for (icon = 0; icon < 7; gfx++, icon++) {
        if (FieldSystem_ShouldDrawStartMenuIcon(task->fieldSystem, (enum StartMenuIcon)icon) == 1 && task->unk_514[icon] == 0) {
            Sprite_SetDrawFlag(task->unk_390[gfx->sprite], 1);
            Sprite_SetAnimCtrlSeq(task->unk_390[gfx->sprite], gfx->anim);
            CopyWindowToVram(&task->unk_3F0[gfx->window]);
            task->unk_514[icon] = 1;
            task->unk_470[icon].unk_0 = 1;
            task->fieldSystem->unkD2_7 = TRUE;
            after++;
        }
    }
    if (after != 0 && before == 0) {
        Sprite_SetDrawFlag(task->unk_390[14], 1);
        CopyWindowToVram(&task->unk_3E0);
    }
    if (before != after) {
        task->fieldSystem->unkD3 = 0;
        task->unk_14 = ov27_0225C1AC(task, task->fieldSystem->unkD3);
        ov27_0225C1EC(task);
    }
    if (ov27_0225BDC8(task) == 1 && task->unk_50C == 0) {
        Sprite_SetDrawFlag(task->unk_390[11], 1);
        Sprite_SetDrawFlag(task->unk_390[13], 1);
        Sprite_SetAnimCtrlSeq(task->unk_390[11], 10);
        task->unk_50C = 1;
    }
}
