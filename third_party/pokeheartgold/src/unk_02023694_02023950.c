#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "gf_3d_render.h"

void sub_02023FC0(Billboard *billboard);
void sub_02023FEC(Billboard *billboard);
void sub_020243FC(Billboard *billboard);
void sub_0202441C(Billboard *billboard);

void sub_02023950(BillboardList *list);

void sub_02023950(BillboardList *list) {
    MtxFx33 identityMatrix;
    const MtxFx33 *rotMatrix;

    ((list) ? (void)0 : GF_AssertFail());
    MTX_Identity33(&identityMatrix);

    Billboard *billboard = list->sentinelData.next;

    while (billboard != &list->sentinelData) {
        if (billboard->draw == 1) {
            if (billboard->callback != 0L) {
                billboard->callback(billboard, billboard->callbackParam);
            }

            sub_020243FC(billboard);

            if (billboard->state == BILLBOARD_STATE_ACTIVE) {
                sub_02023FEC(billboard);
            } else if (billboard->state == BILLBOARD_STATE_VRAM_TRANSFER) {
                sub_02023FC0(billboard);
            }

            rotMatrix = billboard->rotMatrix;

            if (rotMatrix == 0L) {
                rotMatrix = &identityMatrix;
            }

            GF3dRender_DrawModel(&billboard->renderObj, &billboard->pos, rotMatrix, &billboard->scale);
            sub_0202441C(billboard);
        }

        billboard = billboard->next;
    }
}
