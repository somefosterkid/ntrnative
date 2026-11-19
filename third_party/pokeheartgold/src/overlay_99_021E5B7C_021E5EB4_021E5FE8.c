#include "global.h"

#include "overlay_99_pokeathlon_course_record.h"

#include "bg_window.h"

s32 ov99_021E5BB4(s32 arg0, s32 arg1);

void ov99_021E5EB4(void *param_1, s32 param_2, s32 param_3);

void ov99_021E5FE8(u32 *param_1, s32 param_2);

void ov99_021E5FE8(u32 *param_1, s32 param_2) {
    s32 iVar1;

    iVar1 = ov99_021E5BB4(*(u32 *)&((PokeathlonCourseRecord *)param_1)->args, param_2);
    if (iVar1 != 0) {
        ov99_021E5EB4((u8 *)param_1, param_2, *(u32 *)&((PokeathlonCourseRecord *)param_1)->selection.scrollForward);
        return;
    }
    BgClearTilemapBufferAndCommit(((PokeathlonCourseRecord *)param_1)->bgConfig, 2);
    BgClearTilemapBufferAndCommit(((PokeathlonCourseRecord *)param_1)->bgConfig, 5);
    return;
}
