#include "global.h"

#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"

s32 sub_02031B10(void);

void *ov96_021E86FC(PokeathlonCourseData *arg0, u32 arg1);

void *ov96_021E86FC(PokeathlonCourseData *arg0, u32 arg1) {
    u32 new_var;
    if (arg1 >= 4U) {
        GF_AssertFail();
    }
    if (arg0->heapAllocPtr3 == NULL) {
        GF_AssertFail();
    }
    new_var = sub_02031B10();
    return (u8 *)arg0->heapAllocPtr3 + (new_var * arg1);
}
