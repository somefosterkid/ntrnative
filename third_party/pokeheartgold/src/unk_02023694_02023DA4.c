#include "global.h"

#include "error_handling.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void sub_02023B70(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);

void sub_02023C04(BillboardList *unused, Billboard *billboard);

BOOL sub_020242AC(BillboardList *list, Billboard *billboard);

void sub_02024308(Billboard *billboard);

BOOL sub_02023DA4(Billboard *billboard);

void sub_02023E04(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *anims, const BillboardGfxSequence *gfxSequence, NNSGfdTexKey texKey, NNSGfdTexKey tex4x4Key, NNSGfdPlttKey plttKey);
void sub_02023E2C(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *anims, const BillboardGfxSequence *gfxSequence);
void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

BOOL sub_02023DA4(Billboard *billboard) {
    BillboardList *list;

    if (billboard == NULL) {
        GF_AssertFail();
    }
    if (billboard->state == BILLBOARD_STATE_INITIALIZED) {
        GF_AssertFail();
    }
    list = billboard->list;
    if (billboard->state == BILLBOARD_STATE_INACTIVE) {
        return 0;
    }
    sub_02024308(billboard);
    if (billboard->state == BILLBOARD_STATE_VRAM_TRANSFER) {
        sub_02023B70(&billboard->texKey, &billboard->tex4x4Key, &billboard->plttKey);
    }
    sub_02023C04(list, billboard);
    sub_020242AC(list, billboard);
    list->redraw = BILLBOARD_REDRAW_NEEDED;
    return 1;
}

void sub_02023E04(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *anims, const BillboardGfxSequence *gfxSequence, NNSGfdTexKey texKey, NNSGfdTexKey tex4x4Key, NNSGfdPlttKey plttKey) {
    resources->modelRes = modelRes;
    resources->texture = texture;
    resources->anims = anims;
    resources->gfxSequence = *gfxSequence;
    resources->texKey = texKey;
    resources->tex4x4Key = tex4x4Key;
    resources->plttKey = plttKey;
}

void sub_02023E2C(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *anims, const BillboardGfxSequence *gfxSequence) {
    resources->modelRes = modelRes;
    resources->texture = texture;
    resources->anims = anims;
    resources->gfxSequence = *gfxSequence;
    resources->texKey = (u32)0x0;
    resources->tex4x4Key = (u32)0x0;
    resources->plttKey = (u32)0x0;
}

void sub_02023E50(Billboard *billboard, const VecFx32 *pos) {
    ((billboard) ? (void)0 : GF_AssertFail());
    billboard->pos = *pos;
}
