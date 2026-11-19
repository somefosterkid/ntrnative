#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov59_02239C90(SysTask *task, void *data);

void ov59_02239C90(SysTask *task, void *data) {
    Ov59SpriteMoveTask *moveTask = data;
    u8 step;
    Ov59ApricornApp *app;
    VecFx32 pos;

    app = moveTask->app;
    switch (moveTask->state) {
    case 0: {
        pos.x = moveTask->x;
        pos.y = moveTask->y;
        pos.z = 0;
        Sprite_SetMatrix(moveTask->sprite, &pos);
        moveTask->x = moveTask->x + moveTask->dx;
        moveTask->y = moveTask->y + moveTask->dy;
        step = moveTask->step;
        moveTask->step = step + 1;
        if (step >= moveTask->numSteps) {
            moveTask->state = moveTask->state + 1;
            return;
        }
    } break;
    case 1:
    default: {
        Sprite_SetPositionXY(moveTask->sprite, moveTask->targetX, moveTask->targetY);
        app->animCount = app->animCount + (-1);
        Heap_Free(moveTask);
        SysTask_Destroy(task);
    } break;
    }
    return;
}

u8 sub_02031C08(void *);

s32 sub_02032004(void *);

void ov59_02239D08(Ov59ApricornApp *arg0, s32 arg1);

void ov59_02239D08(Ov59ApricornApp *arg0, s32 arg1) {
    s32 var_r4;
    u8 var_r6;

    var_r4 = arg1;
    switch (var_r4) {
    case 6:
        var_r6 = sub_02031C08(arg0->apricornBox);
        BufferIntegerAsString(arg0->msgFormat, 0U, 5 - var_r6, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
        break;
    case 12:
        if ((u32) arg0->quantity > 1U) {
            BufferItemNamePlural(arg0->msgFormat, 0U, arg0->selected + 0x1E5);
        } else {
            BufferItemName(arg0->msgFormat, 0U, arg0->selected + 0x1E5);
        }
        BufferIntegerAsString(arg0->msgFormat, 1U, (s32) arg0->quantity, 2U, PRINTING_MODE_LEFT_ALIGN, 1);
        break;
    case 21:
        ReadMsgDataIntoString(arg0->msgData, sub_02032004(&arg0->juice) + 0x13, arg0->unk_64);
        BufferString(arg0->msgFormat, 0U, arg0->unk_64, 2, 0, 2);
        BufferIntegerAsString(arg0->msgFormat, 1U, (s32) arg0->unk_15, 3U, PRINTING_MODE_LEFT_ALIGN, 1);
        BufferIntegerAsString(arg0->msgFormat, 2U, (s32) arg0->unk_16, 3U, PRINTING_MODE_LEFT_ALIGN, 1);
        var_r6 = arg0->unk_18;
        BufferIntegerAsString(arg0->msgFormat, 3U, 5 - var_r6, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
        break;
    default:
        var_r4 += 0x1B;
        ReadMsgDataIntoString(arg0->msgData, var_r4, arg0->unk_68);
        return;
    }
    var_r4 += 0x1B;
    ReadMsgDataIntoString(arg0->msgData, var_r4, arg0->unk_64);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_68, arg0->unk_64);
}
