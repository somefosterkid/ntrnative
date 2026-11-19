#include "global.h"

#include "math_util.h"
#include "unk_02005D10.h"
#include "system.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

typedef struct UnkStruct_ov41_02249280_v1 {
    u8 unk_0[0x10];
    void *unk_10;
    s32 unk_14;
    s32 unk_18;
} UnkStruct_ov41_02249280_v1;

s32 ov41_022463FC(void);

s32 ov41_022480A4(void *, void *, void *);

s32 ov41_02248158(void *);

s32 ov41_022481D8(void *, s32, s32);

int ov41_022484E8(s32, s32, void *);

s32 ov41_0224883C(void *, s32, s32);

s32 ov41_02249418(void *);

s32 ov41_0224942C(void **, s32 *, s32 *, s32 *, s32 *);

s32 ov41_0224946C(void **, s32 *, s32 *, s32 *, s32 *);

void ov41_02249480(UnkStruct_ov22_02258EE4 *, s32, s32, s32, s32, s32);

s32 ov41_02249B44(void *, s32 *, s32 *);

s32 ov41_02249B94(void *, s32 *, s32 *);

s32 ov41_0224AC08(void *, s32, s32, s32);

void ov41_022490F0(void **arg0);

s32 ov41_02249AF4(void *, s32, s32);
void ov41_02249280(void **arg0);

void ov41_022490F0(void **arg0) {
    s32 v5;
    s32 v4;
    s32 v3;
    s32 v2;
    s32 v1;
    s32 v0;
    s32 v7;
    s32 v6;
    s32 v9;
    s32 v10;
    s32 v11;
    s32 v13;
    void *v8;

    v8 = *arg0;
    if (((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10) == NULL) {
        return;
    }
    ov41_0224946C(arg0, &v5, &v4, &v3, &v2);
    v9 = ov41_022481D8(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04), v3, v5);
    v10 = v9 + ov41_022481D8(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04), v2, v5);
    v11 = v10 + ov41_022481D8(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04), v3, v4);
    if ((s32)(v11 + ov41_022481D8(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04), v2, v4)) >= 4) {
        if (ov41_022480A4(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04), ((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10), *(void **)((u8 *)(*(void **)v8) + 0x38)) == 0) {
            v7 = (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_1C;
            v6 = (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_1E;
            PlaySE(0x682U);
            ov41_0224AC08(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_0C), 0x1B, 0xD7, 3);
            ov41_02249480(v8, 4, v7, v6, *(s32 *)((u8 *)(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10)) + 4), (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_21);
        } else {
            ov41_02248158(*((void **)&((UnkStruct_ov22_02258EE4 *)v8)->unk_04));
            ov41_022463FC();
            *((s32 *)&((UnkStruct_ov22_02258EE4 *)v8)->unk_30) = 0;
            PlaySE(0x5EAU);
        }
    } else {
        s32 *v12;
        v12 = *(s32 **)(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10));
        ov41_0224942C(arg0, &v5, &v4, &v3, &v2);
        v13 = ov41_0224883C(((UnkStruct_ov22_02258EE4 *)v8)->unk_08, v3, v5);
        if ((s32)(v13 + ov41_0224883C(((UnkStruct_ov22_02258EE4 *)v8)->unk_08, v2, v4)) < 2) {
            if (((UnkStruct_ov22_02258EE4 *)v8)->unk_20 == 1) {
                ov41_02249B94(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10), &v1, &v0);
                v7 = (MTRandom() % (u32)(0x6C - v1)) + 0xA;
                v6 = (MTRandom() % (u32)(0x7D - v0)) + 0x12;
            } else {
                v7 = (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_1C;
                v6 = (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_1E;
            }
            PlaySE(0x682U);
        } else {
            ov41_02249B44(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10), &v7, &v6);
            PlaySE(0x5EBU);
        }
        if (((UnkStruct_ov22_02258EE4 *)v8)->unk_20 == 1) {
            ((UnkStruct_ov22_02258EE4 *)v8)->unk_21 = ov41_022484E8(*(s32 *)((u8 *)(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10)) + 4), *v12, *(void **)((u8 *)(((UnkStruct_ov22_02258EE4 *)v8)->unk_08) + 4));
        }
        ov41_02249480(v8, 4, v7, v6, *(s32 *)((u8 *)(((void *)((UnkStruct_ov22_02258EE4 *)v8)->unk_10)) + 4), (s32) ((UnkStruct_ov22_02258EE4 *)v8)->unk_21);
    }
    ov41_02249418(v8);
}

void ov41_02249280(void **arg0) {
    void *v0;
    UnkStruct_ov41_02249280_v1 *v1;

    v1 = *arg0;
    v0 = v1->unk_10;
    if ((v0 != 0) && (gSystem.touchX != 0xFFFF) && (gSystem.touchX != 0xFFFF)) {
        ov41_02249AF4(v0, gSystem.touchX - v1->unk_14, gSystem.touchY - v1->unk_18);
    }
}
