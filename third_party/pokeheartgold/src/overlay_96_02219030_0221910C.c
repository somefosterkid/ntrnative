#include "global.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_obj.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(struct PokeathlonCourseData *data, unsigned int a, unsigned int b, s32 c, s32 d);
#else
void ov96_021E8228(struct PokeathlonCourseData *data, u8 a, u8 b, s32 c, s32 d);
#endif

void ov96_0221910C(Ov96RdObj *obj);

void ov96_0221910C(Ov96RdObj *obj) {
    if (obj->f26) {
        GF_AssertFail();
    }
    if (obj->f28) {
        GF_AssertFail();
    }
    obj->f26 = 1;
    obj->f24 = 1;
    obj->pos20 = obj->pos2C;
    ov96_021E8228(obj->data, obj->d, obj->e, 7, 1);
}
