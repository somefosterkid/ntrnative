#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overlay_99_obj99.h"

typedef struct Ov99BgTemplates {
    BgTemplate bg[4];
} Ov99BgTemplates;

extern u8 ov99_021E9D88[];

extern const GraphicsModes ov99_021E9D9C;

extern const Ov99BgTemplates ov99_021E9E60;

void ov99_021E695C(Obj99 *p);

void ov99_021E695C(Obj99 *p) {
    GraphicsModes a;
    Ov99BgTemplates b;
    u8 *ids;
    BgTemplate *t;
    s32 i;

    p->bgConfig = BgConfig_Alloc(p->heap);
    a = ov99_021E9D9C;
    SetBothScreensModesAndDisable(&a);
    b = ov99_021E9E60;
    ids = ov99_021E9D88;
    i = 0;
    t = b.bg;
    for (; i < 4; i++) {
        InitBgFromTemplate(p->bgConfig, *ids, t, 0);
        BgClearTilemapBufferAndCommit(p->bgConfig, *ids);
        BG_ClearCharDataRange(*ids, 0x20, 0, p->heap);
        t += 1;
        ids++;
    }
    p->narc = NARC_New(NARC_a_1_7_7, p->heap);
}
