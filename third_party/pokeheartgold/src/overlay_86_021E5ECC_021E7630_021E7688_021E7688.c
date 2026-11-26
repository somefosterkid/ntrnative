#include "global.h"

#include "overlay_86_frontier_records_app.h"

extern u8 ov86_021E81DC[];

void ov86_021E7688(FrontierRecordsApp *p, u32 idx) {

    p->flash.isBg = 1;
    p->flash.index = 2;
    p->flash.palA = 2;
    p->flash.palB = 0;
    idx = ov86_021E81DC[idx];
    p->flash.x = ((idx % 7) * 32 + 24) / 8;
    p->flash.y = ((idx / 7) * 32 + 40) / 8 + 32;
    p->flash.width = 2;
    p->flash.height = 2;
    p->flash.state = 0;
    p->flash.timer = 0;
}
