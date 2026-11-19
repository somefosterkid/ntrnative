#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"

#include "overlay_83_battle_castle_self_app.h"

void ov83_02240664(BattleCastleSelfApp *app);

void ov83_022407FC(BattleCastleSelfApp *app);

void ov83_0224088C(BattleCastleSelfApp *app);

void ov83_02242894(u32 slot, s16 *x, s16 *y);

void ov83_0224755C(struct BattleCastleAppSprite *sprite, BOOL flag);

void ov83_02247568(struct BattleCastleAppSprite *sprite, u16 x, u16 y);

void ov83_022429E4(BattleCastleSelfApp *app);

void ov83_022429E4(BattleCastleSelfApp *app) {
    s32 page;
    s32 slot;
    void *ptr;
    u32 i;
    s16 x;
    s16 y;
    NNSG2dScreenData *screenData;
    ClearWindowTilemapAndScheduleTransfer(&app->windows[54]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[55]);
    ptr = GfGfxLoader_GetScrnDataFromOpenNarc(app->narc, 0x26, 1, &screenData, HEAP_ID_107);
    LoadRectToBgTilemapRect(app->bgConfig, 2, screenData->rawData, 0, 0, 0x20, 0x18);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, 2);
    Heap_Free(ptr);
    i = 0;
    page = app->page;
    do {
        ov83_02242894(i, &x, &y);
        ov83_02247568(app->listItemSprites[i], x, y);
        if ((page * 6) + (slot = i) < (u32)app->numEntries) {
            ov83_0224755C(app->listItemSprites[i], 1);
        }
        i++;
    } while (i < 6);
    ov83_0224755C(app->itemSelectCursorSprite, 1);
    ov83_02240664(app);
    ov83_022407FC(app);
    ov83_0224088C(app);
}
