#include "global.h"
#include "field_system.h"
#include "overlay_28.h"
#include "touchscreen.h"

extern const TouchscreenHitbox _0225EA7C;

void ov28_0225E900(DowsingMchnApp *app) {
    if (sub_0203DF8C(app->fieldSystem) == 1) {
        ov28_0225E810(app);
    }
    if (ov28_0225E8B8(&_0225EA7C, app->sprites[0], 5, 6) != 0) {
        gSystem.simulatedInputs = 1;
    }
}
