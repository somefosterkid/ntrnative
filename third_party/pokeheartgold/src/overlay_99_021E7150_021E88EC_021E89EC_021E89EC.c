#include "global.h"

#include "message_format.h"
#include "overlay_98.h"
#include "player_data.h"
#include "pm_string.h"
#include "string_util.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_98_ov98text_sys.h"

s32 ov99_021E94FC(s32, u8, s32);

u16 ov99_021E82E4(s32 *, u32);

s32 ov99_021E84EC(void *, u32);

s32 ov99_021E8544(void *, s32);

s32 ov99_021E88EC(void *, s32, s32, s32, s32);

s32 ov99_021E9020(void *);

s32 ov99_021E945C(void **);

s32 ov99_021E94CC(void *, u16, u32, s32);

PlayerProfile *ov99_021E9508(void **);

String *ov99_021E9518(void *, u8, u8);

void ov99_021E89EC(void *arg0);

void ov99_021E89EC(void *arg0) {
    s32 var_r4;
    MessageFormat *msgFmt;
    s32 var_r6;
    s32 var_r7;
    s8 temp_r1;
    s32 temp_r0;
    void *var_r5;

    var_r5 = arg0;
    BufferPokeathlonEventName((MessageFormat *)ov98_0221EEFC(((PokeathlonEventRecord *)var_r5)->textContext), 0U, (u32) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor));
    ov98_0221EC08(((PokeathlonEventRecord *)var_r5)->textContext, 2, 0x3E, 1, 1);
    ov98_0221EBEC(((PokeathlonEventRecord *)var_r5)->textContext, 5, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor) + 0x41, 0, 1, 0);
    msgFmt = ov98_0221EEFC(((PokeathlonEventRecord *)var_r5)->textContext);
    BufferIntegerAsString(msgFmt, 0U, ov99_021E94FC(*(s32 *)var_r5, *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor), *((s32 *)&((PokeathlonEventRecord *)var_r5)->linkMode)), 7U, PRINTING_MODE_RIGHT_ALIGN, 1);
    ov98_0221EC08(((PokeathlonEventRecord *)var_r5)->textContext, 3, 0x36, 1, 0);
    var_r4 = 0;
    var_r7 = var_r4;
    do {
        if ((var_r4 == 0) && (*((s32 *)&((PokeathlonEventRecord *)var_r5)->linkMode) == 0)) {
            var_r6 = 1;
        } else {
            var_r6 = 0;
        }
        if (ov99_021E8544(var_r5, (u8)var_r4) == 0) {
            temp_r0 = ov99_021E94CC(*(void **)var_r5, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor), (u8)var_r4, *((s32 *)&((PokeathlonEventRecord *)var_r5)->linkMode));
            ov99_021E88EC(var_r5, temp_r0, var_r4 + 7, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor) + 0x56, var_r6);
        } else {
            ov98_0221EBEC(((PokeathlonEventRecord *)var_r5)->textContext, var_r4 + 7, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor) + 0x60, var_r7, var_r6, var_r7);
        }
        var_r4 += 1;
    } while (var_r4 < 5);
    if (*((s32 *)&((PokeathlonEventRecord *)var_r5)->linkMode) != 0) {
        ov98_0221EBD8(((PokeathlonEventRecord *)var_r5)->textContext, 6, 0x4B, 0);
        ov99_021E9020(var_r5);
        var_r4 = 0;
        var_r6 = 1;
        var_r7 = var_r4;
        do {
            if (ov99_021E8544(var_r5, (u8)var_r4) == 0) {
                ov98_0221EE9C(((PokeathlonEventRecord *)var_r5)->textContext, var_r4 + 0xC, ov99_021E9518(*(void **)var_r5, *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor), (u8)var_r4), var_r7, var_r6);
            }
            var_r4 += 1;
        } while (var_r4 < 5);
        return;
    }
    temp_r1 = (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor);
    if (*((s32 *)&((PokeathlonEventRecord *)var_r5)->noRecord[temp_r1]) != 0) {
        ov98_0221EE84(((PokeathlonEventRecord *)var_r5)->textContext, 0x11);
        ov98_0221EE84(((PokeathlonEventRecord *)var_r5)->textContext, 0x12);
    } else {
        ov98_0221ECD0(((PokeathlonEventRecord *)var_r5)->textContext, 0x11, 0x6B, ov99_021E82E4(*(void **)var_r5, (u8)temp_r1), 3U, 0U);
        ov98_0221EBD8(((PokeathlonEventRecord *)var_r5)->textContext, 0x12, 0x6A, 0);
    }
    if ((s32)(s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor) < 0) {
        ov98_0221EE84(((PokeathlonEventRecord *)var_r5)->textContext, 6);
    }
    if (ov99_021E945C(*(void **)var_r5) == 0) {
        if (*((s32 *)&((PokeathlonEventRecord *)var_r5)->noRecord[((s8)*((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor))]) != 0) {
            ov98_0221EBD8(((PokeathlonEventRecord *)var_r5)->textContext, 6, 0x6D, 0);
            return;
        }
        ov98_0221EDC4(((PokeathlonEventRecord *)var_r5)->textContext, 6, 0x6C, 0U, ov99_021E9508(*(void **)var_r5));
        return;
    }
    if (ov99_021E84EC(var_r5, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor)) != 0) {
        ov98_0221EDC4(((PokeathlonEventRecord *)var_r5)->textContext, 6, 0x6C, 0U, ov99_021E9508(*(void **)var_r5));
        return;
    }
    ov98_0221EBD8(((PokeathlonEventRecord *)var_r5)->textContext, 6, (s8) *((u8 *)&((PokeathlonEventRecord *)var_r5)->cursor) + 0x4C, 0);
}
