#include "global.h"
#include "overlay_96_ov96_rd_obj.h"

u32 ov96_022158EC(VecFx32 *param_1, VecFx32 *param_2, s32 param_3);

s32 ov96_02218688(Ov96RdObj *obj);

extern s32 ov96_0221D708;

extern s32 ov96_0221D70A;

extern s32 ov96_0221D70C;

s32 ov96_0221862C(Ov96RdObj *obj);

s32 ov96_0221862C(Ov96RdObj *obj) {
    u8 *new_var;
    s32 temp_r2;
    VecFx32 sp0;

    if (ov96_02218688(obj) != 0) {
        new_var = (u8 *)(&sp0);
        *((s32 *)(new_var)) = 0;
        *((s32 *)(new_var + 4)) = 0;
        *((s32 *)(new_var + 8)) = 0;
        temp_r2 = (u8)obj->d * 6;
        sp0.x = (*((s16 *)(((u8 *)(&ov96_0221D708)) + temp_r2))) << 0xC;
        sp0.y = (*((s16 *)(((u8 *)(&ov96_0221D70A)) + temp_r2))) << 0xC;
        return ov96_022158EC(&obj->pos2C, &sp0, (*((s16 *)(((u8 *)(&ov96_0221D70C)) + temp_r2))) << 0xC);
    }
    return 0;
}
