#include "global.h"

#include "pokeathlon/pokeathlon_save.h"

#include "error_handling.h"
#include "math_util.h"
#include "save.h"
#include "pokeathlon/pokeathlon.h"

s32 ov96_021E7D30(s32, s32, s32);

extern u8 _0221A7D8[];

extern u16 ov96_0221A894[];

s32 ov96_021E80C4(PokeathlonCourseData *arg0);

s32 ov96_021E80C4(PokeathlonCourseData *arg0) {
    u8 i;
    u16 speciesID;
    Pokeathlon_RecordsSolo *records = PokeathlonSave_GetRecordsSolo2(Save_Pokeathlon_Get(**((SaveData ** *)&arg0->args)));

    for (i = 0; i < 10; i++) {
        speciesID = *(u16 *)((u8 *)records + 0x2C * i);
        if (speciesID == 0xFFFF) {
            return 0;
        }
        if (ov96_021E7D30(ov96_0221A894[i], speciesID, _0221A7D8[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

extern u8 ov96_0221A934[];

extern u8 ov96_0221A95C[];

void ov96_021E811C(void *arg0, u32 arg1, s8 *arg2);

void ov96_021E811C(void *arg0, u32 arg1, s8 *arg2) {
    s32 var_r7;
    u8 temp_r0;
    u8 temp_r6;
    u8 var_r2;
    u8 var_r2_2;

    *arg2 = 0xA;
    temp_r6 = *(u8 *)((u8 *)arg0 + 0xC);
    var_r7 = 0;
    if (*(s32 *)((u8 *)arg0 + 4) == 1) {
        switch (temp_r6) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            var_r7 = 1;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        default:
            GF_AssertFail();
            break;
        }
    } else {
        switch (temp_r6) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            var_r7 = 1;
            break;
        default:
            GF_AssertFail();
            break;
        }
    }
    if (var_r7 != 0) {
        if (temp_r6 == 0xA) {
            temp_r0 = (u8)((s32)LCRandom() % 10);
            var_r2 = 0;
            while ((u32)var_r2 < arg1) {
                arg2[var_r2] = ov96_0221A95C[temp_r0 * 4 + var_r2];
                var_r2 += 1;
            }
        } else {
            var_r2_2 = 0;
            while ((u32)var_r2_2 < arg1) {
                arg2[var_r2_2] = ov96_0221A934[temp_r6 * 4 + var_r2_2];
                var_r2_2 += 1;
            }
        }
    }
}
