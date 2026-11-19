#include "global.h"
struct PokeathlonEventRecord;

#include "gf_gfx_loader.h"
#include "overlay_99_pokeathlon_event_record.h"

s32 ov99_021E86D4(void *, s32, u8, s32, s32, s32);

s32 ov99_021E89EC(void *);

void ov99_021E8788(void *arg0);

void ov99_021E8788(void *arg0) {
    s32 v0;

    if (*((s32 *)&((PokeathlonEventRecord *)arg0)->linkMode) != 0) {
        v0 = 1;
    } else if (*((s32 *)&((PokeathlonEventRecord *)arg0)->noRecord[(((PokeathlonEventRecord *)arg0)->cursor)]) == 0) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    ov99_021E89EC(arg0);
    ov99_021E86D4(arg0, (u8) ((PokeathlonEventRecord *)arg0)->cursor, 0x1A, 0, v0, 1);
}

void ov99_021E87D0(struct PokeathlonEventRecord *arg0);

void ov99_021E87D0(PokeathlonEventRecord *arg0) {
    s32 v0;
    u8 srcOffset;

    if (*((s32 *)&arg0->linkMode) != 0) {
        v0 = 2;
    } else {
        v0 = 1;
    }
    srcOffset = v0 << 5;
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(arg0->narc, 0x33, GF_PAL_LOCATION_MAIN_BG, (u32)srcOffset, GF_PAL_SLOT_1_OFFSET, 0x20U, *((enum HeapID *)&arg0->heapId));
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(arg0->narc, 0x33, GF_PAL_LOCATION_SUB_BG, (u32)srcOffset, GF_PAL_SLOT_1_OFFSET, 0x20U, *((enum HeapID *)&arg0->heapId));
}

s32 ov99_021E89A4();

s32 ov99_021E9038(void *);

void ov99_021E8818(void *arg0);

void ov99_021E8818(void *arg0) {
    ov99_021E89A4();
    ov99_021E9038(arg0);
}
