#include "global.h"
struct FrontierGraphics;

#include "error_handling.h"
#include "frontier_map.h"
#include "gf_3d_vramman.h"
#include "overlay_42.h"
#include "frontier_graphics.h"

void *sub_02096868(void *);

void ov80_0223927C(struct FrontierGraphics *arg0);

void ov80_0223927C(FrontierGraphics *arg0) {
    UnkStruct_ov42_022293B8 *v1;
    UnkStruct_ov42_022293B8 *v2;
    s32 v3;
    void *v0;
    void *v4;

    v4 = sub_02096868(*((void **)&arg0->frontier));
    v3 = 0;
    do {
        v0 = *(void **)v4;
        if (v0 != NULL) {
            ov42_02228100(v0);
            if (*(s32 *)((u8 *)v4 + 0x38) != 0) {
                GF_AssertFail();
            }
        }
        v3 += 1;
        v4 += 0x3C;
    } while (v3 < 0x20);
    ov42_02228050(*((UnkStruct_ov42_022280A8 * *)&arg0->unk_14));
    ov42_02227F28(*((UnkStruct_ov42_02227F68 * *)&arg0->unk_18));
    ov42_02228F94(*((UnkStruct_ov42_02228EDC * *)&arg0->unk_20));
    v1 = *((UnkStruct_ov42_022293B8 * *)&arg0->unk_24);
    if (v1 != NULL) {
        ov42_0222940C(v1);
    }
    v2 = *((UnkStruct_ov42_022293B8 * *)&arg0->unk_28);
    if (v2 != NULL) {
        ov42_0222940C(v2);
    }
    ov42_02229A78(*((UnkStruct_ov42_02229A40 * *)&arg0->unk_2C));
    ov42_022299AC(*((u32 * *)&arg0->unk_30));
}

void ov80_022392F8(void);

void ov80_022392DC(enum HeapID heapID) {
    GF_3DVramMan_Create(heapID, 0, 1, 0, 1, ov80_022392F8);
}
