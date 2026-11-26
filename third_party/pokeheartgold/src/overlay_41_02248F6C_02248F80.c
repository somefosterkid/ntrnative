#include "global.h"

#include "unk_02005D10.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

typedef struct UnkStruct_ov41_02249390 {
    void *unk_0;
    u8 unk_4[0x4];
    s32 *unk_8;
    s32 *unk_C;
} UnkStruct_ov41_02249390;

s32 ov41_0224642C(void);

void *ov41_02248030(void *);

s32 ov41_022480C8(void *, void *);

s32 ov41_02248158(void *);

s32 ov41_022481BC(void *);

void *ov41_022481F4(void *, s32 *, s32 *, void *);

void ov41_022486F0(void *arg0);

s32 ov41_02248724(void *);

s32 ov41_02248820(void *);

void *ov41_02248858(void *, s32 *, s32 *, void *);

s32 ov41_02249390(void *);

s32 ov41_022493BC(void *, void *, s32, s32, s32, s32);

s32 ov41_02249574(void *);

s32 ov41_02249710(void *);

s32 ov41_02249B44(void *, void *, void *);

extern s32 ov41_022490AC;

extern s32 ov41_022490B0;

extern s32 ov41_022490F0;

extern s32 ov41_02249280;

extern s32 ov41_022492B0;

extern s32 ov41_022492E0;

void ov41_02248F80(UnkStruct_ov41_02249390 *arg0);

void ov41_02248F80(UnkStruct_ov41_02249390 *arg0) {
    s32 v1;
    s32 v0;
    s32 v4;
    s32 v7;
    u32 sndseq;
    void *v2;
    void *v3;
    UnkStruct_ov22_02258EE4 *v5;
    void *v6;

    v5 = arg0->unk_0;
    if (ov41_022481BC(*((void **)&v5->unk_04)) != 0) {
        v6 = ov41_022481F4(*((void **)&v5->unk_04), &v1, &v0, *(void **)((u8 *)(*(void **)v5) + 0x38));
        if ((v6 != NULL) && (*(s32 *)((u8 *)v6 + 4) == 3) && (*((s32 *)&v5->unk_2C) == 0)) {
            v6 = NULL;
        }
        if (v6 != NULL) {
            if (*(s32 *)((u8 *)v6 + 4) == 0) {
                ov41_022480C8(*((void **)&v5->unk_04), v6);
                v7 = 0;
            } else {
                ov41_02248030(v6);
                v7 = ov41_02249710(*(void **)v6);
                ov41_02249B44(v6, &v5->unk_24, &v5->unk_28);
            }
            ov41_02248158(*((void **)&v5->unk_04));
            ov41_022493BC(v5, v6, 1, v1, v0, v7);
            ov41_0224642C();
        }
    } else if (ov41_02248820(*((void **)&v5->unk_08)) != 0) {
        v2 = ov41_02248858(*((void **)&v5->unk_08), &v1, &v0, *(void **)((u8 *)(*(void **)v5) + 0x38));
        if (v2 != NULL) {
            ov41_022486F0(v2);
            ov41_02248724(*((void **)&v5->unk_08));
            ov41_022493BC(v5, v2, 0, v1, v0, 0);
            ov41_0224642C();
        }
    }
    v3 = ((void *)v5->unk_10);
    if (v3 != NULL) {
        v4 = *(s32 *)((u8 *)v3 + 4);
        sndseq = 0x5EB;
        switch (v4) {
        case 0:
            arg0->unk_8 = &ov41_022490F0;
            arg0->unk_C = &ov41_02249280;
            break;
        case 1:
            arg0->unk_8 = &ov41_022490B0;
            arg0->unk_C = &ov41_022490AC;
            ov41_02249390(arg0);
            sndseq = 0x67D;
            break;
        case 3:
            arg0->unk_8 = &ov41_022492B0;
            arg0->unk_C = &ov41_022492E0;
            break;
        }
        PlaySE(sndseq);
        ov41_02249574(v5);
    }
}
