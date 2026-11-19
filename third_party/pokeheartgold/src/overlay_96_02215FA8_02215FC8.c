#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02219FE4(void *arg0);

s32 ov96_022158D4(s32, s32);

void ov96_0221768C(Ov96RdSprites *sprites, PokeathlonCourseData *data);

void ov96_022180CC(Ov96RdPlayer *players, PokeathlonCourseData *data);

void ov96_02218330(Ov96RdCourse *course, PokeathlonCourseData *data);

void ov96_02218578(Ov96RdObj *obj, s32 a);

void ov96_02218A68(Ov96RdObj *obj);

void ov96_02219398(Ov96RdObj *obj);

void ov96_02219460(Ov96RdObj *obj, Ov96RdFlags *flags, s32 a);

typedef struct {
    u32 w[9];
} UnkStruct_ov96_02215FC8;

typedef struct {
    u32 pad0[8];
    u32 a : 3;
    u32 cnt : 16;
    u32 pad : 12;
    u32 f : 1;
} CtlW;

typedef struct {
    u32 x : 8;
    u32 y : 8;
    u32 on : 1;
    u32 kind : 2;
    u32 rest : 13;
} SrcW;


void ov96_02215FC8(PokeathlonCourseData *arg0);

void ov96_02215FC8(PokeathlonCourseData *arg0) {
    int i;
    Ov96RdWork *heap;
    CtlW *p8;
    u8 *data;
    Ov96RdPlayer *h;

    heap = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    data = PokeathlonCourse_GetDataCopyArea(arg0);
    p8 = (CtlW *)ov96_021E8A20(data + 0x28);

    if (ov96_021E5F24(arg0) == 0) {
        if (p8->f != 0) {
            ov96_02218330(&heap->course, arg0);
            return;
        }
        if (heap->unk_438 > 0) {
            heap->unk_438 = heap->unk_438 - 1;
            p8->cnt = heap->unk_438;
        }
        *(UnkStruct_ov96_02215FC8 *)ov96_021E8A20(data + 0x50) = *(UnkStruct_ov96_02215FC8 *)ov96_021E8A20(data);
        i = 0;
        data += 0x50;
        h = heap->course.players;
        for (; i < 4; i++) {
            SrcW *src = (SrcW *)ov96_021E8A20(data);
            Ov96RdFlags *flags = &h->flags;
            Ov96RdObj *sub = &h->obj;
            if (src->on != 0) {
                if (flags->b14 != 0 && flags->b15 != 0) {
                    flags->b14 = 0;
                } else if (flags->b14 == 0) {
                    if (flags->b15 == 0) {
                        flags->b14 = 1;
                        flags->b15 = 1;
                        h->tx = src->x;
                        h->ty = src->y;
                    }
                }
                h->pos.x = src->x;
                h->pos.y = src->y;
                flags->f0 = flags->f0 + 1;
            } else {
                if (flags->b15 != 0) {
                    flags->cnt16 = flags->f0;
                    flags->f0 = 0;
                    flags->b14 = 0;
                    flags->b15 = 0;
                }
            }
            ov96_02219460(sub, flags, ov96_022158D4(h->pos.x, h->pos.y));
            if (sub->f26 != 0) {
                if (sub->f24 == 1 && src->kind == 1) {
                    ov96_02219398(sub);
                    ov96_02218A68(sub);
                    sub->f24 = 2;
                } else if (sub->f24 == 2 && src->kind == 2) {
                    sub->f26 = 0;
                    sub->f24 = 0;
                    ov96_02218578(sub, 4);
                }
            }
            h++;
            data += 0x28;
        }
        ov96_02219FE4(heap->unk_184);
        ov96_022180CC(heap->course.players, arg0);
        if (p8->cnt == 0) {
            p8->f = 1;
        }
        ov96_02218330(&heap->course, arg0);
    }
    ov96_0221768C(&heap->sprites, arg0);
}
