#include "global.h"

#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021FB994(UnkStruct_ov96_021F94A8 *work);

void ov96_021FB994(UnkStruct_ov96_021F94A8 *work) {
    int time = (u16)work->raceTime;
    u8 sec = time / 30;
    u8 rem;

    time = (u8)((u8)(time % 30) * 100 / 30);
    Sprite_SetAnimCtrlSeq(work->sprites[0], (u8)(sec / 60) + 1);
    rem = sec % 60;
    Sprite_SetAnimCtrlSeq(work->sprites[2], (u8)(rem / 10) + 1);
    Sprite_SetAnimCtrlSeq(work->sprites[3], (u8)(rem % 10) + 1);
    Sprite_SetAnimCtrlSeq(work->sprites[5], (u8)(time / 10) + 1);
}
