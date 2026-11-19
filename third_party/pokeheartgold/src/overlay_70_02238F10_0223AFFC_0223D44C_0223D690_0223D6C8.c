#include "global.h"

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_0223D490(void *);

s32 ov70_0223D588(void *);

s32 ov70_0223D680(void *);

s32 ov70_0223D690(void *, s32);

void ov70_0223D808(GTSApplicationState *arg0);

s32 ov70_0223D8E8(Sprite *, s32);

s32 ov70_0223D924(s32, s32, s32);

s32 ov70_0223E264(void *, s32);

s32 ov70_0223D6C8(GTSApplicationState *arg0);

s32 ov70_0223D6C8(GTSApplicationState *arg0) {
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0;

    temp_r0 = ov70_0223D680(arg0);
    if (temp_r0 != -1) {
        switch (temp_r0) {
        case 31:
            Sprite_SetAnimActiveFlag(arg0->boxArrowSprites[0], 1);
            Sprite_SetAnimCtrlSeq(arg0->boxArrowSprites[0], 0x26);
            arg0->selectedBoxId = ov70_0223D924(arg0->selectedBoxId, 0x13, 1);
            ov70_0223E264(arg0, arg0->selectedBoxId);
            PlaySE(0x5DCU);
            break;
        case 32:
            Sprite_SetAnimActiveFlag(arg0->boxArrowSprites[1], 1);
            Sprite_SetAnimCtrlSeq(arg0->boxArrowSprites[1], 0x27);
            arg0->selectedBoxId = ov70_0223D924(arg0->selectedBoxId, 0x13, -1);
            ov70_0223E264(arg0, arg0->selectedBoxId);
            PlaySE(0x5DCU);
            break;
        case 30:
            ov70_0223D690(arg0, arg0->screenArgument);
            arg0->partySlotIndex = temp_r0;
            ov70_0223D8E8(arg0->cursorSprite, arg0->partySlotIndex);
            break;
        default:
            arg0->partySlotIndex = temp_r0;
            ov70_0223D8E8(arg0->cursorSprite, arg0->partySlotIndex);
            temp_r0_2 = arg0->screenArgument;
            if (temp_r0_2 == 5) {
                ov70_0223D490(arg0);
            } else if (temp_r0_2 == 6) {
                ov70_0223D588(arg0);
            }
            break;
        }
    } else {
        ov70_0223D808(arg0);
        temp_r0_3 = arg0->screenArgument;
        if (temp_r0_3 == 5) {
            if (2 & gSystem.newKeys) {
                ov70_0223D690(arg0, 5);
            } else if (1 & gSystem.newKeys) {
                ov70_0223D490(arg0);
            }
        } else if (temp_r0_3 == 6) {
            if (2 & gSystem.newKeys) {
                ov70_0223D690(arg0, 6);
            } else if (1 & gSystem.newKeys) {
                ov70_0223D588(arg0);
            }
        }
    }
    return 3;
}

extern const u8 ov70_02245804[][4];

void ov70_0223D808(GTSApplicationState *arg0) {
    s32 playSe;
    u8 dir;
    u16 pos;
    u8 next;
    u8 idx;
    Sprite **sprites;

    playSe = 0;
    dir = 0;
    if (gSystem.newKeys & 0x40) {
        dir = 1;
    } else if (gSystem.newKeys & 0x80) {
        dir = 2;
    } else if (gSystem.newKeys & 0x20) {
        dir = 3;
    } else if (gSystem.newKeys & 0x10) {
        dir = 4;
    }
    if (dir != 0) {
        pos = arg0->partySlotIndex;
        next = ov70_02245804[pos][dir - 1];
        if (next != pos) {
            if (next == 0x63 || next == 0x65) {
                idx = (next == 0x65) ? 0 : 1;
                sprites = (Sprite **)(&arg0->boxArrowSprites[0]);
                Sprite_SetAnimActiveFlag(sprites[idx], 1);
                Sprite_SetAnimCtrlSeq(sprites[idx], idx + 0x26);
                arg0->selectedBoxId = ov70_0223D924(arg0->selectedBoxId, 0x13, next - 0x64);
                ov70_0223E264(arg0, arg0->selectedBoxId);
                PlaySE(0x5DC);
            } else {
                playSe = 1;
                arg0->partySlotIndex = next;
            }
        }
    }
    if (playSe != 0) {
        PlaySE(0x5DC);
    }
    ov70_0223D8E8(arg0->cursorSprite, arg0->partySlotIndex);
}
