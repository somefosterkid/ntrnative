#include "global.h"
#include "billboard.h"

void sub_02023C04(BillboardList *unused, Billboard *billboard);
void sub_02023C20(Billboard *billboard, const BillboardResources *resources);
void sub_02023C9C(const BillboardList *list, Billboard *billboard, const BillboardResources *resources);
void sub_02023B70(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
void sub_02023CF0(Billboard *billboard, const BillboardResources *resources);
void sub_02023D24(Billboard *billboard, const BillboardResources *resources);
void sub_020242E4(Billboard *sentinel, Billboard *billboard);

void sub_020239D0(Billboard *billboard, const BillboardResources *resources);
void sub_02023A20(Billboard *billboard, const BillboardResources *resources);

void sub_020239D0(Billboard *billboard, const BillboardResources *resources) {
    BillboardList *list = billboard->list;

    sub_02023C04(list, billboard);
    sub_02023C20(billboard, resources);
    sub_02023C9C(list, billboard, resources);
    if (billboard->state == BILLBOARD_STATE_INITIALIZED) {
        sub_020242E4(&list->sentinelData, billboard);
    }
    billboard->state = BILLBOARD_STATE_VRAM_TRANSFER;
    billboard->anims = resources->anims;
    billboard->animNum = 0;
    billboard->frameNum = 0;
}

void sub_02023A20(Billboard *billboard, const BillboardResources *resources) {
    BillboardList *list = billboard->list;

    sub_02023C04(list, billboard);
    if (billboard->state == BILLBOARD_STATE_VRAM_TRANSFER) {
        sub_02023B70(&billboard->texKey, &billboard->tex4x4Key, &billboard->plttKey);
    }
    billboard->texKey = resources->texKey;
    billboard->tex4x4Key = resources->tex4x4Key;
    billboard->plttKey = resources->plttKey;
    sub_02023CF0(billboard, resources);
    sub_02023D24(billboard, resources);
    if (billboard->state == BILLBOARD_STATE_INITIALIZED) {
        sub_020242E4(&list->sentinelData, billboard);
    }
    billboard->state = BILLBOARD_STATE_ACTIVE;
    billboard->anims = resources->anims;
    billboard->animNum = 0;
    billboard->frameNum = 0;
}
