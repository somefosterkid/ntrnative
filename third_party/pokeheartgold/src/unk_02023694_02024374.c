#include "global.h"

#include "billboard.h"

void *sub_0202443C(const BillboardResources *resources, int value);

NNSG3dResTex *sub_02024374(const BillboardResources *resources);

NNSG3dResTex *sub_02024374(const BillboardResources *resources) {
    return sub_0202443C(resources, BILLBOARD_RESOURCE_TEXTURE);
}
