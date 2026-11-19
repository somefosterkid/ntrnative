#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "overlay_98.h"
#include "overlay_99_obj99.h"
#include "overlay_99_pokeathlon_course_summary.h"
#include "overlay_98_ov98text_sys.h"

s32 ov98_0221F0EC(void);

void ov99_021E7060(PokeathlonCourseSummary *summary);

FS_EXTERN_OVERLAY(OVY_98);

s32 ov99_021E6840(OverlayManager *man, s32 *state);

s32 ov99_021E6840(OverlayManager *man, s32 *state) {
    Obj99 *v0;

    v0 = OverlayManager_GetData(man);
    ov99_021E7060(v0->summary);
    ov98_0221E684(v0->spriteContext, v0->sprites, 0x43);
    ov98_0221EB84(v0->textSys, 0xF);
    ov99_021E6A70(v0);
    ov98_0221F0EC();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return 1;
}
