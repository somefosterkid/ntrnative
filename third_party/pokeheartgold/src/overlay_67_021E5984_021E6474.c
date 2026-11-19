#include "global.h"

#include "overlay_67_app.h"
#include "pm_string.h"

void ov67_021E6474(Ov67App *app) {
    s32 v0;

    v0 = 0;
    do {
        String_Delete(app->wifiTrainers[v0].name);
        v0 += 1;
    } while (v0 < 0x1E);
}
