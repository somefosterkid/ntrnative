#include "global.h"
#include "overlay_28.h"
#include "sprite.h"

extern void (*const ov28_0225EAAC[])(DowsingMchnNeedle *needle);

void ov28_0225E43C(DowsingMchnApp *app) {
    u32 i;
    VecFx32 vec;
    u32 j;

    if (app->needleTimer != 0) {
        app->needleTimer--;
        if (app->needleTimer == 0) {
            for (i = 0; i < 4; i++) {
                app->needles[i].angle = app->needles[i].targetAngle;
                ov28_0225DE04(&vec, app->needles[i].angle);
                Sprite_SetMatrix(app->sprites[4 + i], &vec);
                ov28_0225E424(app->sprites[4 + i], app->needles[i].angle);
            }
            return;
        }
        for (j = 0; j < 4; j++) {
            app->needles[j].angle = app->needles[j].nextAngle;
            ov28_0225DE04(&vec, app->needles[j].angle);
            Sprite_SetMatrix(app->sprites[4 + j], &vec);
            ov28_0225E424(app->sprites[4 + j], app->needles[j].angle);
            ov28_0225EAAC[app->proximity](&app->needles[j]);
        }
    }
}
