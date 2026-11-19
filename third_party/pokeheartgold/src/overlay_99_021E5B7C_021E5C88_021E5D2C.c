#include "global.h"
struct PokeathlonCourseRecord;

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_99_pokeathlon_course_record.h"

extern u8 _021E954C;

void ov99_021E5D2C(struct PokeathlonCourseRecord *param_1);

void ov99_021E5D2C(PokeathlonCourseRecord *param_1) {
    u32 uVar1;
    NARC_Delete(param_1->narc);
    uVar1 = 0;
    uVar1 = uVar1;
    do {
        FreeBgTilemapBuffer(param_1->bgConfig, (&_021E954C)[uVar1]);
        uVar1 = (uVar1 + 1) & 0xff;
    } while (uVar1 < 8);
    Heap_Free(*((void * *)&param_1->bgConfig));
    return;
}
