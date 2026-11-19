#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem.h"

u8 ov96_021E8448(PokeathlonCourseData *arg0, u32 arg1);

u8 ov96_021E8448(PokeathlonCourseData *data, u32 arg1) {
    long long dest;
    NARC *narc;
    if ((arg1 != 0) && (arg1 <= 0x64U)) {
        narc = NARC_New(NARC_a_2_5_8, PokeathlonCourse_GetHeapID(data));
        NARC_ReadWholeMember(narc, arg1 - 1, &dest);
        NARC_Delete(narc);
        return *((u8 *)(((u8 *)(&dest)) + 7));
    }
    GF_AssertFail();
    return 0U;
}
