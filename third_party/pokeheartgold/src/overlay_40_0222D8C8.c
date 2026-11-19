#include "global.h"
struct UnkStruct_0208C06C;

#include "overlay_40.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "message_format.h"

s32 ov40_0222C480(void *);

s32 ov40_0222CAD8(UnkStruct_0208C06C *param0);

s32 ov40_0222CBE0(void *);

s32 ov40_0222CCAC(void *);

s32 ov40_0222CE7C(void *);

s32 ov40_0222CED8(void *);

s32 ov40_0222CF10(void *);

s32 ov40_0222CFBC(void *, s32);

s32 sub_02088030(void *);

void ov40_0222D8C8(struct UnkStruct_0208C06C *arg0);

void ov40_0222D910(int *param0, int *param1, int param2, int param3, int param4, int param5, int param6);
void ov40_0222D9E8(int *param0, int *param1, int param2);
BOOL ov40_0222DA00(int *param0, int *param1, int param2, int param3);
BOOL ov40_0222DA84(int *param0, int param1);

void ov40_0222DAA8(s32 *arg0);

struct MessageFormat;
MessageFormat *ov40_0222DAB0(enum HeapID heapID);

void ov40_0222D8C8(UnkStruct_0208C06C *arg0) {
    ov40_0222CAD8(arg0);
    ov40_0222C480(arg0);
    sub_02088030(arg0);
    ov40_0222CCAC(arg0);
    ov40_0222CFBC(arg0, 1);
    ov40_0222CE7C(arg0);
    ov40_0222CED8(arg0);
    ov40_0222CBE0(arg0);
    ov40_0222CF10(arg0);
    arg0->unk_534.unk_1A4 = (s32)(arg0->unk_534.unk_1A4 + 1);
}

void ov40_0222D910(int *param0, int *param1, int param2, int param3, int param4, int param5, int param6) {
    *param0 = 0;
    *param1 = 31;

    if (param6 == 0) {
        G2_SetBlendAlpha(param2, param3, *param0, *param1);
        G2S_SetBlendAlpha(param4, param5, *param0, *param1);
    }

    if (param6 == 1) {
        G2_SetBlendAlpha(param2, param3, *param0, *param1);
    }

    if (param6 == 2) {
        G2S_SetBlendAlpha(param4, param5, *param0, *param1);
    }
}

void ov40_0222D980(int *param0, int *param1, int param2, int param3, int param4, int param5, int param6) {
    if (param6 == 0) {
        G2_SetBlendAlpha(param2, param3, *param0, *param1);
        G2S_SetBlendAlpha(param4, param5, *param0, *param1);
    }

    if (param6 == 1) {
        G2_SetBlendAlpha(param2, param3, *param0, *param1);
    }

    if (param6 == 2) {
        G2S_SetBlendAlpha(param4, param5, *param0, *param1);
    }
}

void ov40_0222D9E8(int *param0, int *param1, int param2) {
    ov40_0222D910(param0, param1, (GX_BLEND_PLANEMASK_BG3), (GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ), (GX_BLEND_PLANEMASK_BG3), (GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ), param2);
}

BOOL ov40_0222DA00(int *param0, int *param1, int param2, int param3) {
    BOOL v0 = 1;

    if (param2 == 0) {
        if (*param0 < 8) {
            *param0 += 1;
            v0 = 0;
        }
        if (*param1 > 7) {
            *param1 -= 3;
            v0 = 0;
        }
    } else {
        if (*param0 > 0) {
            *param0 -= 1;
            v0 = 0;
        }
        if (*param1 < 31) {
            *param1 += 3;
            v0 = 0;
        }
    }

    if (param3 == 0) {
        G2_ChangeBlendAlpha(*param0, *param1);
        G2S_ChangeBlendAlpha(*param0, *param1);
    }

    if (param3 == 1) {
        G2_ChangeBlendAlpha(*param0, *param1);
    }

    if (param3 == 2) {
        G2S_ChangeBlendAlpha(*param0, *param1);
    }

    return v0;
}

BOOL ov40_0222DA84(int *param0, int param1) {
    BOOL v0 = 1;

    if (param1 == 1) {
        if (*param0 < 16) {
            *param0 += 2;
            v0 = 0;
        }
    } else {
        if (*param0 > 0) {
            *param0 -= 2;
            v0 = 0;
        }
    }

    return v0;
}

void ov40_0222DAA8(s32 *arg0) {
    *arg0 = 0x10;
}

MessageFormat *ov40_0222DAB0(enum HeapID heapID) {
    return MessageFormat_New_Custom(4, 64, heapID);
}
