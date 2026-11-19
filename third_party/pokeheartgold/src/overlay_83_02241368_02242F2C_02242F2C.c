#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

extern const u8 ov83_02247E64[][4];

s32 ov83_02242F2C(BattleCastleSelfApp *arg0);

s32 ov83_02242F2C(BattleCastleSelfApp *arg0) {
    u8 temp_r0;
    const u8 *temp_r3;

    temp_r3 = ov83_02247E64[arg0->highlightIdx];
    temp_r0 = arg0->highlightState;
    switch (temp_r0) {
    case 0:
        BgTilemapRectChangePalette(arg0->bgConfig, 2U, temp_r3[0], temp_r3[1], temp_r3[2], temp_r3[3], 6U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
        arg0->highlightState = (u8)(arg0->highlightState + 1);
        break;
    case 1:
        arg0->highlightTimer = (u8)(arg0->highlightTimer + 1);
        if (arg0->highlightTimer == 4) {
            BgTilemapRectChangePalette(arg0->bgConfig, 2U, temp_r3[0], temp_r3[1], temp_r3[2], temp_r3[3], 5U);
            ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
            arg0->highlightTimer = 0U;
            arg0->highlightState = (u8)(arg0->highlightState + 1);
        }
        break;
    case 2:
        arg0->highlightTimer = (u8)(arg0->highlightTimer + 1);
        if (arg0->highlightTimer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
