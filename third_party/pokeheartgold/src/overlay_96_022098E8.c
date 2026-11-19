#include "global.h"

#include "filesystem.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_021E9C0C(struct UnkStruct_ov96_021E9A78 *param_1);

void ov96_021EA894(struct Ov96SpriteSet *param_1);

void ov96_022098E8(Ov96_0220Data *param_1);

void ov96_022098E8(Ov96_0220Data *param_1) {
    NARC_Delete(param_1->narc);
    ov96_021EA894(param_1->spriteSet);
    ov96_021E9C0C(param_1->resSrc);
    SpriteSystem_FreeResourcesAndManager(param_1->spriteSystem, param_1->spriteManager);
    SpriteSystem_Free(param_1->spriteSystem);
    return;
}
