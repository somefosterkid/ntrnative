#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02013534_font_system.h"

struct FontOAM;
struct UnkStruct_02012744_t;

void sub_020135AC(UnkStruct_02012744 *param0);

void sub_020135AC(UnkStruct_02012744 *param0) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < 12; v0++) {
        Heap_Free(param0->unk_00[v0]);
    }

    Heap_Free(param0->unk_60);
    Heap_Free(param0);
}
