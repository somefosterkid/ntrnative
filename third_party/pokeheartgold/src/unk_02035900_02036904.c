#include "global.h"

#include "sys_task.h"

void sub_0203690C(s32 param_1, u8 *param_2);

void sub_02036904(u16 aid, u16 *data, u16 length);

void sub_02036904(u16 aid, u16 *data, u16 length) {
    sub_0203690C(aid, (u8 *)data);
}
