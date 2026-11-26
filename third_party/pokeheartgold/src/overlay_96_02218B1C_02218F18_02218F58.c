#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_obj.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(struct PokeathlonCourseData *data, unsigned int a, unsigned int b, s32 c, s32 d);
#else
void ov96_021E8228(struct PokeathlonCourseData *data, u8 a, u8 b, s32 c, s32 d);
#endif

void ov96_02218578(Ov96RdObj *obj, s32 state);

void ov96_02218F58(Ov96RdObj *obj);

void ov96_02218F58(Ov96RdObj *obj) {
    s32 temp_r0;
    s32 var_r1;

    temp_r0 = obj->state;
    var_r1 = 1;
    if ((temp_r0 != 1) && (temp_r0 != 4)) {
        var_r1 = 0;
    }
    if (var_r1 == 0) {
        GF_AssertFail();
    }
    if (obj->f28) {
        GF_AssertFail();
    }
    if (obj->f26) {
        GF_AssertFail();
    }
    obj->f29 = 0;
    obj->timer5A = 5;
    if (obj->info->stats->k2 != 0) {
        ov96_021E8228(obj->data, obj->d, obj->e, 2, 1);
    }
    ov96_02218578(obj, 8);
}
