#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F395C(UnkStruct_ov14_boxsys *app, s16 x, s16 y);
void ov14_021F396C(void *app, u32 pos, u32 mode);
void ov14_021F39A0(void *app, u32 pos, u32 mode);

void ov14_021F2F88(u32 pos, s16 *x, s16 *y, u32 mode);

void ov14_021F396C(void *app, u32 pos, u32 mode) {
    s16 x;
    s16 y;

    ov14_021F2F88(pos, &x, &y, mode);
    ov14_021F395C(app, x + 8, y + 8);
}

void ov14_021F39A0(void *app, u32 pos, u32 mode) {
    s16 x;
    s16 y;

    ov14_021F2F88(pos, &x, &y, mode);
    ov14_021F395C(app, x, y + 4);
}
