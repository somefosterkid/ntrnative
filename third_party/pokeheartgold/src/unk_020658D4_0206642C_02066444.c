#include "global.h"

#include "error_handling.h"

typedef struct {
    s32 a[4];
} _020FE3F4_Array;

typedef struct {
    s32 a[4];
} _020FE434_Array;

typedef struct {
    s32 a[4];
} _020FE444_Array;

extern const _020FE3F4_Array _020FE3F4;

extern const _020FE434_Array _020FE434;

extern const _020FE444_Array _020FE444;

s32 sub_02066444(s32 arg0);

s32 sub_02066444(s32 arg0) {
    _020FE3F4_Array sp20 = _020FE3F4;
    _020FE434_Array sp10 = _020FE434;
    _020FE444_Array sp0 = _020FE444;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (arg0 == sp20.a[i]) {
            return 3;
        }
    }
    for (i = 0; i < 4; i++) {
        if (arg0 == sp10.a[i]) {
            return 2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (arg0 == sp0.a[i]) {
            return 1;
        }
    }
    GF_AssertFail();
    return 0;
}
