#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

typedef struct {
    u8 unk0[0x3F4];
    u32 max : 5;
    u32 unk3F4_5 : 9;
    u32 cur : 5;
    u32 unk3F4_19 : 8;
    u32 flag27 : 1;
    u32 flag28 : 1;
    u32 unk3F4_29 : 3;
    u8 unk3F8[4];
    GridInputHandler *gridInput;
    u8 unk400[8];
    ManagedSprite *sprite;
} UnkStruct_ov99_021E7AD4;

s32 ov99_021E7C58(UnkStruct_ov99_021E7AD4 *, s32);

void ov99_021E7AD4(UnkStruct_ov99_021E7AD4 *data, BOOL decrement, BOOL arg2);

void ov99_021E7AD4(UnkStruct_ov99_021E7AD4 *data, BOOL decrement, BOOL arg2) {
    u32 prev;
    u8 input;
    u32 cur;
    u32 max;

    prev = data->cur;
    if (decrement) {
        data->cur = prev + 1;
    } else if (prev != 0) {
        data->cur = prev - 1;
    }
    max = data->max;
    cur = data->cur;
    if (cur <= max) {
        max = cur;
    }
    data->cur = max;
    if (data->cur != prev) {
        PlaySE(1500);
        data->flag27 = decrement == FALSE ? 1 : 0;
        data->flag28 = 1;
        input = 0;
        cur = GridInputHandler_GetNextInput(data->gridInput);
        if (arg2) {
            if (cur == 30) {
                input = cur;
            }
        } else {
            u8 in = cur;
            if (decrement) {
                input = in - 5;
            } else {
                input = in + 5;
            }
        }
        GridInputHandler_SetNextInput(data->gridInput, input);
        ov99_021E7C58(data, input);
        ManagedSprite_SetDrawFlag(data->sprite, FALSE);
    }
}
