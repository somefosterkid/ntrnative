#include "global.h"
#include "overlay_28.h"

void ov28_0225DB8C(DowsingMchnApp *app, NARC *narc) {
    DowsingMchnSpriteDef sp20 = { 0, 0, 0, 0 };
    SpriteResourcesHeader sp24;
    u32 var_r4;

    ov28_0225D92C(app->needleResObjs, app->resMans, narc, 4, 7, 5, 6, 9, 0x1F3, 0x1F3, 0x1F3, 0x1F3);
    ov28_0225D9BC(app->needleResObjs, app->resMans, &sp24, 2);
    var_r4 = 0;
    do {
        ov28_0225DA1C(app, var_r4 + 4, &sp24, &sp20);
        var_r4 += 1;
    } while (var_r4 < 4U);
}
