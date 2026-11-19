#include "global.h"

#include "error_handling.h"

typedef struct UnkStruct_ov96_02207818_Ent {
    s32 active;
    u8 pad4[8];
    s32 idx;
    u16 a;
    u16 b;
} UnkStruct_ov96_02207818_Ent;

typedef struct UnkStruct_ov96_02207818_Pair {
    u16 a;
    u16 b;
} UnkStruct_ov96_02207818_Pair;

typedef struct UnkStruct_ov96_02207818_Work {
    UnkStruct_ov96_02207818_Ent ent[5];
    UnkStruct_ov96_02207818_Pair pairs[20];
    u16 cur;
} UnkStruct_ov96_02207818_Work;

s32 ov96_02207800(UnkStruct_ov96_02207818_Work *);

void ov96_02207818(UnkStruct_ov96_02207818_Work *work);

void ov96_02207818(UnkStruct_ov96_02207818_Work *work) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (work->ent[i].active == 0) {
            work->ent[i].idx = work->cur;
            work->ent[i].a = work->pairs[work->cur].a;
            work->ent[i].b = work->pairs[work->cur].b;
            ov96_02207800(work);
            work->ent[i].active = 1;
            return;
        }
    }
    GF_AssertFail();
}
