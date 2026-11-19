#include "global.h"

#include "overlay_86_frontier_records_app.h"

extern u8 ov86_021E81DC[];

void ov86_021E78B8(FrontierRecordsApp *app, s32 arg1);

void ov86_021E78B8(FrontierRecordsApp *app, s32 arg1) {
    u32 temp_r1;

    app->flash.isBg = 1;
    app->flash.index = 2;
    app->flash.palA = 2;
    app->flash.palB = 0;
    temp_r1 = ov86_021E81DC[arg1];
    app->flash.x = (temp_r1 & 1) * 0xD + 0x26;
    app->flash.y = (temp_r1 >> 1) * 4 + 0x25;
    app->flash.width = 9;
    app->flash.height = 2;
    app->flash.state = 0;
    app->flash.timer = 0;
}
