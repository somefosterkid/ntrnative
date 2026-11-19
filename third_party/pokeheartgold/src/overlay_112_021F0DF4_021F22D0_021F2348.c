#include "global.h"
#include "overlay_112_sprite_app.h"

void ov112_021F2348(Ov112SpriteApp *arg0);

void ov112_021F2348(Ov112SpriteApp *arg0) {
    arg0->bits148.active = 1;
}

void ov112_021F235C(Ov112SpriteApp *arg0);

void ov112_021F235C(Ov112SpriteApp *arg0) {
    arg0->bits148.active = 0;
    arg0->bits148.timer = 0;
    arg0->bits148.step = 0;
    arg0->bits148.loop = 0;
}
