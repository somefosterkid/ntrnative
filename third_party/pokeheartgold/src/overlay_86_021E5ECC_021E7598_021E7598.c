#include "global.h"
#include "overlay_86_frontier_records_app.h"
struct FrontierRecordsApp;

extern void *ov86_021E81DC;

void ov86_021E7598(FrontierRecordsApp *arg0) {
    u16 var_r4;
    u8 temp_r6;

    var_r4 = 0;
    do {
        temp_r6 = *(u8 *)(((u8 *)&ov86_021E81DC) + var_r4);
        ov86_021E6024(arg0, 4, var_r4 + 0x38, (((s32)temp_r6 % 7) << 5) + 0x18, ((s32)temp_r6 / 7) << 5, FONT_UNK_4, 0xF0100, 2);
        var_r4 += 1;
    } while ((u32)var_r4 < 0x1AU);
}
