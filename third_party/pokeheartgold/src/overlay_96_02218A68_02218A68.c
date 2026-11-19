#include "global.h"

#include "overlay_96_ov96_rd_obj.h"

void ov96_021EB0A4(void *sprite, s32 x, s32 y, s32 *outX, s32 *outY);

extern Ov96RdPos16 ov96_0221D6E4[];

extern s16 ov96_0221D6E6; // = ov96_0221D6E4[0].y (the DS code reads y through this label)

void ov96_02218A68(Ov96RdObj *obj);

void ov96_02218A68(Ov96RdObj *obj) {
    s32 x;
    s32 y;

    ov96_021EB0A4(obj->info->sprite, ov96_0221D6E4[obj->d].x, *(s16 *)((u8 *)&ov96_0221D6E6 + obj->d * sizeof(Ov96RdPos16)), &x, &y);
    obj->pos2C.x = x << 0xC;
    obj->pos2C.y = y << 0xC;
    obj->pos20.x = x << 0xC;
    obj->pos20.y = y << 0xC;
    {
        VecFx32 zero = { 0, 0, 0 };
        obj->vel = zero;
    }
    obj->f16 = 2;
    obj->f27 = 0;
    obj->f29 = 0;
    obj->f30 = 0;
}
