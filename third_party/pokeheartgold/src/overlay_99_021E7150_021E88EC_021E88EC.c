#include "global.h"
struct PokeathlonEventRecord;

#include "error_handling.h"
#include "overlay_98.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_98_ov98text_sys.h"

void ov99_021E88EC(struct PokeathlonEventRecord *data, s32 value, u32 windowIdx, u32 unused, u32 y);

void ov99_021E88EC(PokeathlonEventRecord *data, s32 value, u32 windowIdx, u32 unused, u32 y) {
    s8 mode;

    if (value == 0xFFFF) {
        GF_AssertFail();
    }
    mode = data->cursor;
    if (mode == 0) {
        ov98_0221EDA4(data->textContext, (u8)(value / 30), 3, 0);
        ov98_0221EDA4(data->textContext, (u8)(((value % 30) * 10) / 30), 1, 1);
    } else if (mode == 6) {
        ov98_0221EDA4(data->textContext, (s32)((u32)value >> 10), 3, 0);
        ov98_0221EDA4(data->textContext, ((value % 1024) * 10) / 1024, 1, 1);
    } else {
        ov98_0221EDA4(data->textContext, value, 3, 0);
    }
    ov98_0221ED48(data->textContext, windowIdx, data->cursor + 0x56, 0, (u8)y);
}
