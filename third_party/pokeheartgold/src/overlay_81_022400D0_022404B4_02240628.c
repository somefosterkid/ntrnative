#include "global.h"

#include "bg_window.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02240628(BattleFactoryApp *app, u32 bgLayer, u8 palette, u8 width, u8 height);

void ov81_02240628(BattleFactoryApp *app, u32 bgLayer, u8 palette, u8 width, u8 height) {
    BgTilemapRectChangePalette(app->bgConfig, (u8)bgLayer, 0, 0, width, height, palette);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, (u8)bgLayer);
}
