#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_021_f7684.h"

s32 ov96_021F77EC(s32, UnkStruct_ov96_021F7684 *, u8);

s32 ov96_021F7740(s32 arg0, UnkStruct_ov96_021F7684 *data);

s32 ov96_021F7740(s32 arg0, UnkStruct_ov96_021F7684 *data) {
    s32 ret = 4;
    int i;
    int idx;

    if (data->unk_21 == 4) {
        return 0;
    }
    data->timer++;
    for (i = data->first; i < 4; i++) {
        *(u32 *)ov96_021E8A20(data->dataCopyArea + 0x50 + i * 0x28) = 0;
    }
    if (data->timer >= data->interval) {
        data->active = 1;
        data->timer = 0;
    }
    if (data->active != 0) {
        idx = data->first + data->cursor;
        if (idx < data->first) {
            GF_AssertFail();
        }
        if (idx < data->first) {
            return 4;
        }
        ov96_021F77EC(arg0, data, idx);
        ret = (u8)idx;
        data->cursor++;
        if (data->cursor >= data->count) {
            data->cursor = 0;
            data->active = 0;
        }
    }
    return ret;
}
