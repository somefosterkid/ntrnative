#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "sprite_system.h"

s32 ov86_021E7094(FrontierRecordsApp *app);

s32 ov86_021E7094(FrontierRecordsApp *app) {
    switch (app->flash.state) {
    case 0:
        if (!app->flash.isBg) {
            ManagedSprite_SetPaletteOverride(app->sprites[app->flash.index], app->flash.palA);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, app->flash.index, app->flash.x, app->flash.y, app->flash.width, app->flash.height, app->flash.palA);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, app->flash.index);
        }
        app->flash.state++;
        break;
    case 1:
        app->flash.timer++;
        if (app->flash.timer == 4) {
            if (!app->flash.isBg) {
                ManagedSprite_SetPaletteOverride(app->sprites[app->flash.index], app->flash.palB);
            } else {
                BgTilemapRectChangePalette(app->bgConfig, app->flash.index, app->flash.x, app->flash.y, app->flash.width, app->flash.height, app->flash.palB);
                ScheduleBgTilemapBufferTransfer(app->bgConfig, app->flash.index);
            }
            app->flash.timer = 0;
            app->flash.state++;
        }
        break;
    case 2:
        app->flash.timer++;
        if (app->flash.timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
