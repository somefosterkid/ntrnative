#include "global.h"

#include "message_format.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "overlay_98_ov98text_sys.h"

s32 ov99_021E5BB4(s32, s32);

u16 ov99_021E5BD8(u32 *, s32, s32);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov99_021E5C20(s32, s32, u8);
#else
s32 ov99_021E5C20(s32, s32, u8);
#endif

extern s32 ov99_021E958C;

void ov99_021E607C(void *arg0, s32 arg1);

void ov99_021E607C(void *arg0, s32 arg1) {
    MessageFormat *sp8;
    unsigned int var_r6;
    s32 *var_r4;
    sp8 = ov98_0221EEFC(((PokeathlonCourseRecord *)arg0)->textContext);
    var_r4 = &ov99_021E958C;
    var_r6 = 0;
    do {
        BufferPokeathlonEventName(sp8, 1U, (u8)ov99_021E5C20(*((s32 *)arg0), arg1, var_r6));
        ov98_0221ECD0(((PokeathlonCourseRecord *)arg0)->textContext, *((s32 *)var_r4), 0xA, var_r6 + 1, 1, 0);
        if (ov99_021E5BB4(*((s32 *)arg0), arg1) != 0) {
            ov98_0221ECD0(((PokeathlonCourseRecord *)arg0)->textContext, *((s32 *)(((u8 *)var_r4) + 4)), 8, ov99_021E5BD8(*((void **)arg0), arg1, (u16)var_r6), 3, 0);
        } else {
            ov98_0221EBD8(((PokeathlonCourseRecord *)arg0)->textContext, *((s32 *)(((u8 *)var_r4) + 4)), 9, 0);
        }
        var_r6 += 1;
        var_r4 += 2;
    } while (((s32)var_r6) < 3);
    if (ov99_021E5BB4(*((s32 *)arg0), arg1) != 0) {
        ov98_0221ECD0(((PokeathlonCourseRecord *)arg0)->textContext, 0xC, 8, ov99_021E5BD8(*((void **)arg0), arg1, 3U), 3, 0);
        return;
    }
    ov98_0221EBD8(((PokeathlonCourseRecord *)arg0)->textContext, 0xC, 9, 0);
}
