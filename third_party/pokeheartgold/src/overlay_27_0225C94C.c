#include "global.h"

#include "overlay_27.h"
#include "sprite.h"
#include "system.h"

// asm table of 7 pointers (the .word symbols) to the [item][direction] neighbour rows of each menu size 2..8
extern s8 (*ov27_0225D480[])[4];

void ov27_0225C618(Ov27TaskData *data);

s32 ov27_0225C94C(Ov27TaskData *data) {
    data->selection = 0;
    ov27_0225C540(data);
    Sprite_SetDrawFlag(data->cursor, 1);
    ov27_0225CCE0(data, data->selection);
    Sprite_SetAnimCtrlSeq(data->cursor, 1);
    data->state = 4;
    return 0;
}

s32 ov27_0225C988(Ov27TaskData *data) {
    ov27_0225CD94(data);
    return 0;
}

s32 ov27_0225C994(Ov27TaskData *data) {
    if (Sprite_IsAnimated(data->cursor) == 0) {
        Sprite_SetDrawFlag(data->cursor, 0);
        ov27_0225C5E4(data);
        *data->result = (s16)data->selection;
        data->state = 6;
    }
    return 0;
}

s32 ov27_0225C9CC(Ov27TaskData *data) {
    Sprite_SetDrawFlag(data->cursor, 0);
    data->state = 1;
    return 0;
}

s32 ov27_0225C9E4(Ov27TaskData *data) {
    s32 v0;

    v0 = data->counter + 1;
    data->counter = v0;
    if (v0 > 0x14) {
        return 1;
    }
    return 0;
}

s32 ov27_0225C9F8(Ov27TaskData *data) {
    if (gSystem.touchHeld != 0) {
        gSystem.simulatedInputs = 1;
    }
    return 0;
}

s32 ov27_0225CA14(Ov27TaskData *data) {
    data->state = 8;
    data->selection = 0;
    ov27_0225C618(data);
    Sprite_SetDrawFlag(data->cursor, 1);
    ov27_0225CD18(data, data->nMenuWindows, data->selection);
    if (data->callback != NULL) {
        data->callback(data->callbackMenu, data->selection);
    }
    *data->result = 0xeeee;
    return 0;
}

s32 ov27_0225CA68(s32 *arg0, s32 arg1, s32 arg2) {
    s32 v1;
    s8 v0;

    v1 = *arg0;
    s32 table = arg1 - 2;

    v0 = ov27_0225D480[table][v1][arg2];
    if ((v0 == -1) || (v1 == v0)) {
        return 0;
    }
    *arg0 = (s32)v0;
    return 1;
}
