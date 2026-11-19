#include "global.h"

#include "overlay_91.h"
#include "unk_02033AE0.h"

s32 GF_SndHandleSetTempoRatio(s32, s32);

void ov91_02261B10(void);

struct UnkStruct_ov115_0226095C_t;
typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;
void ov91_0225CA64(UnkStruct_ov115_0226095C *param0);
void ov91_0225CA6C(UnkStruct_ov115_0226095C *param0);

void ov91_02261B10(void) {
    GF_SndHandleSetTempoRatio(7, 0x100);
}

extern s32 ov91_02262754;

s32 *ov91_02261B20(void) {
    return &ov91_02262754;
}

s32 ov91_02261B28(void) {
    return 8;
}

void ov91_02261B2C(void *arg0) {
    void *v0;

    v0 = ov91_02261B20();
    sub_0203410C(v0, ov91_02261B28(), arg0);
}

s32 ov91_02261B48(void) {
    return 0;
}

void ov91_02261B4C(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov115_0226095C *v0 = param3;
    ov91_0225CA64(v0);
}

void ov91_02261B58(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov115_0226095C *v0 = param3;
    ov91_0225CA6C(v0);
}
