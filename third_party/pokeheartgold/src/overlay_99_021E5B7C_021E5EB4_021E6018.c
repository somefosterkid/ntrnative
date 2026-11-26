#include "global.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "overlay_98_ov98text_sys.h"
#include "overlay_99.h"
struct PokeathlonCourseRecord;

void ov99_021E6018(struct PokeathlonCourseRecord *param_1);

void ov99_021E6018(PokeathlonCourseRecord *param_1) {
    ov98_0221EC08(param_1->textContext, 0, 2, 1, 0);
    ov98_0221EBEC(param_1->textContext, 2, 0, 1, 0, 4);
    ov98_0221EBD8(param_1->textContext, 0xb, 7, 0);
    return;
}


s32 ov99_021E607C(s32 *, s32);

s32 ov99_021E6144(s32 *, s32);

s32 ov99_021E6188(void *arg0, int arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov99_021E6218(s32 *);
#else
s32 ov99_021E6218(s32 *);
#endif

void ov99_021E6050(s32 *arg0, s32 arg1);

void ov99_021E6050(s32 *arg0, s32 arg1) {
    ov99_021E6188(arg0, arg1);
    ov99_021E607C(arg0, arg1);
    ov99_021E6144(arg0, ov99_021E5BB4(((PokeathlonCourseRecord *)arg0)->args, arg1));
    ov99_021E6218(arg0);
}
