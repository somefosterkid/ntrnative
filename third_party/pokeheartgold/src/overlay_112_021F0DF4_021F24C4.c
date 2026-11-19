#include "global.h"
#include "overlay_112_sprite_app.h"

void ov112_021F24C4(Ov112SpriteApp *arg0);

void ov112_021F24C4(Ov112SpriteApp *arg0) {
    arg0->bits14C.active = 1;
}

void ov112_021F24D8(Ov112SpriteApp *arg0);

void ov112_021F24D8(Ov112SpriteApp *arg0) {
    arg0->bits14C.active = 0;
    arg0->bits14C.timer = 0;
    arg0->bits14C.state = 0;
}
