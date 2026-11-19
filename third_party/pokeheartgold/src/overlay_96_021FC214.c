#include "global.h"

#include "constants/heap.h"

#include "overlay_96_ov96_icon_work.h"
#include "unk_02009D48.h"

void ov96_021FC214(Ov96IconWork *work, Ov96IconSpec *specs) {
    ov96_021FC320(work);
    ov96_021FC404(work);
    ov96_021FC450(work, specs);
    G2dRenderer_SetSubSurfaceCoords(&work->renderer, 0, 0);
    G2dRenderer_SetMainSurfaceCoords(&work->renderer, 0, 0xc0000);
}
