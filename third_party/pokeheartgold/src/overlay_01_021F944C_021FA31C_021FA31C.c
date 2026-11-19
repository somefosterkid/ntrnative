#include "global.h"

#include "billboard.h"

void ov01_021EA3B0(NNSG3dResMdl *model);

Billboard *sub_02023D44(const BillboardTemplate *template);

void sub_02023EA4(Billboard *billboard, u8 draw);

void sub_02023EE0(Billboard *billboard, int animNum);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

NNSG3dResMdl *sub_02023F90(const Billboard *billboard);

extern const VecFx32 ov01_02208B64;

Billboard *ov01_021FA31C(BillboardList *list, const BillboardResources *resources, const VecFx32 *pos);

Billboard *ov01_021FA31C(BillboardList *list, const BillboardResources *resources, const VecFx32 *pos) {
    BillboardTemplate template;
    Billboard *billboard;

    template.list = list;
    template.resources = resources;
    template.pos = *pos;
    template.scale = ov01_02208B64;
    billboard = sub_02023D44(&template);
    if (billboard != NULL) {
        sub_02023EA4(billboard, 0);
        sub_02023EE0(billboard, 0);
        sub_02023F40(billboard, 0);
        ov01_021EA3B0(sub_02023F90(billboard));
    }
    return billboard;
}
