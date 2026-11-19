#include "global.h"

#include "constants/heap.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_103_mail_box_view.h"

extern const OamManagerParam ov103_021EED38;

extern const OamCharTransferParam ov103_021EED0C;

extern const SpriteResourceCountsListUnion ov103_021EED20;

void ov103_021EDF88(MailBoxView *view) {
    view->spriteSystem = SpriteSystem_Alloc(HEAP_ID_157);
    view->spriteManager = SpriteManager_New(view->spriteSystem);

    OamManagerParam renderOamTemplate = ov103_021EED38;

    OamCharTransferParam charTransferTemplate = ov103_021EED0C;

    SpriteSystem_Init(view->spriteSystem, &renderOamTemplate, &charTransferTemplate, 32);
    SpriteResourceCountsListUnion capacities = ov103_021EED20;
    SpriteSystem_InitSprites(view->spriteSystem, view->spriteManager, 7);
    SpriteSystem_InitManagerWithCapacities(view->spriteSystem, view->spriteManager, &capacities);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(view->spriteSystem), 0, 0x200000);
}
