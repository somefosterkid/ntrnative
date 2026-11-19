#include "global.h"

#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021E9C0C(void *param_1);

void ov96_02216390(Ov96RdWork *work);

void ov96_02216390(Ov96RdWork *work) {
    ov96_021E9C0C(work->resSrc);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
    return;
}
