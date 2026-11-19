#include "global.h"
struct SubLcdHeroDemo;

#include "sprite.h"
#include "overlay_70_sub_lcd_hero_demo.h"

void ov70_02240F3C(struct SubLcdHeroDemo *arg0, s32 arg1);

void ov70_02240F3C(SubLcdHeroDemo *arg0, s32 arg1) {
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)(*((void * *)&arg0->wk)) + 0xEE4), arg1 + (7 * *((s32 *)&arg0->unk_8)));
}
