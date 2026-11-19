#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "sprite.h"

void ov59_02239C70(Ov59ApricornApp *arg0);

void ov59_02239C70(Ov59ApricornApp *arg0) {
    Sprite *temp_r0;
    s32 var_r4;

    var_r4 = 0;
    do {
        temp_r0 = arg0->sprites[var_r4];
        if (temp_r0 != NULL) {
            Sprite_Delete(temp_r0);
        }
        var_r4 += 1;
    } while (var_r4 < 0xF);
}
