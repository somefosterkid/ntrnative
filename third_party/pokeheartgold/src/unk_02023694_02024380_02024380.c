#include "global.h"
#include "billboard.h"

void sub_020239D0(Billboard *billboard, const BillboardResources *resources);

void sub_02023A20(Billboard *billboard, const BillboardResources *resources);

void sub_02024380(Billboard *billboard, const BillboardResources *resources);

void sub_02024380(Billboard *billboard, const BillboardResources *resources) {
    if (resources->texKey == NNS_GFD_ALLOC_ERROR_TEXKEY) {
        sub_020239D0(billboard, resources);
    } else {
        sub_02023A20(billboard, resources);
    }
}
