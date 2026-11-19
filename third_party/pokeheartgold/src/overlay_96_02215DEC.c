#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021EB06C(void *sprite, s32 x, s32 y, s32 *outX, s32 *outY);

void ov96_02215DEC(PokeathlonCourseData *data, s32 idx, VecFx32 *out);

s32 ov96_02218688(Ov96RdObj *obj);
void ov96_02215ECC(PokeathlonCourseData *arg0, s32 arg1);

void ov96_02215DEC(PokeathlonCourseData *data, s32 idx, VecFx32 *out) {
    s32 x;
    s32 y;
    Ov96RdObj *obj;

    obj = &((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj;
    ov96_021EB06C(obj->info->sprite, obj->pos2C.x >> 12, obj->pos2C.y >> 12, &x, &y);
    out->x = x << 12;
    out->y = y << 12;
}

u32 ov96_02215E2C(PokeathlonCourseData *arg0, s32 arg1);

u32 ov96_02215E2C(PokeathlonCourseData *data, s32 idx) {
    return ((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj.f16;
}

u32 ov96_02215E48(PokeathlonCourseData *param_1, s32 param_2);

u32 ov96_02215E48(PokeathlonCourseData *data, s32 idx) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    return work->course.players[idx].obj.info->value >> 12 & 0xff;
}

u32 ov96_02215E68(PokeathlonCourseData *param_1, s32 param_2);

u32 ov96_02215E68(PokeathlonCourseData *data, s32 idx) {
    Ov96RdWork *work;
    fx32 ratio;
    Ov96RdEntry *info;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    info = work->course.players[idx].obj.info;
    ratio = FX_Div(info->value, info->stats->s2);
    return ratio * 100 >> 12 & 0xff;
}

u32 ov96_02215E94(PokeathlonCourseData *arg0, s32 arg1);

u32 ov96_02215E94(PokeathlonCourseData *data, s32 idx) {
    return ((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj.f26;
}

u32 ov96_02215EB0(PokeathlonCourseData *arg0, s32 arg1);

u32 ov96_02215EB0(PokeathlonCourseData *data, s32 idx) {
    return ((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj.f27;
}

void ov96_02215ECC(PokeathlonCourseData *data, s32 idx) {
    ov96_02218688(&((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj);
}
