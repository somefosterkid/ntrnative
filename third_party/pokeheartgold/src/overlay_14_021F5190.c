#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "pokedex_util.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F4FBC(UnkStruct_ov14_boxsys *, void *, s32, s32, s32, s32, s32, s32, s32);

void ov14_021F5190(UnkStruct_ov14_boxapp *arg0, UnkStruct_ov14_mon *arg1, s32 arg2) {
    s32 v2;
    u32 num;
    void *v0;
    void *v1;

    v2 = arg2 * 0x10;
    FillWindowPixelBuffer(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys) + 0x30 + v2, 0U);
    if (arg1->isEgg == 0) {
        num = Pokedex_ConvertToCurrentDexNo(SaveArray_IsNatDexEnabled(**(SaveData ***)arg0), (u32) arg1->species);
        if (num != 0) {
            v1 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
            sub_0200CDAC(((UnkStruct_ov14_boxsys *)v1)->unk_1C, 2U, v1 + 0x30 + v2, 0U, 5U);
            BufferIntegerAsString(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_24, 0U, (s32)num, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
            v0 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
            ov14_021F4FBC(v0, *((void **)&((UnkStruct_ov14_boxsys *)v0)->unk_20), arg2, 0x5B, 0x10, 0, 0, 0x10200, 0);
        }
    }
    ScheduleWindowCopyToVram(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys) + 0x30 + v2);
}
