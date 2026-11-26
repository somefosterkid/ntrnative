#include "global.h"

#include "overlay_86_frontier_records_app.h"

void ov86_021E792C(FrontierRecordsApp *app, s8 x, s8 y) {
    app->flash.isBg = 1;
    app->flash.index = 2;
    app->flash.palA = 2;
    app->flash.palB = 0;
    app->flash.x = x;
    app->flash.y = y;
    app->flash.width = 4;
    app->flash.height = 4;
    app->flash.state = 0;
    app->flash.timer = 0;
}
