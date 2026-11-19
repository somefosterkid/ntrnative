#include "global.h"
#include "list_menu_items.h"
#include "overlay_83_battle_castle_self_app.h"

u16 ov83_02240F48(BattleCastleSelfApp *app, s32 itemSlot, s32 menuEntry);

void ov83_02242894(u32 slot, s16 *x, s16 *y);

void ov83_02247264(struct BattleCastleAppSpriteManager *spriteMan, s32 id, u16 itemId);

void ov83_022472A0(struct BattleCastleAppSpriteManager *spriteMan, s32 id, u16 itemId);

void ov83_0224755C(struct BattleCastleAppSprite *sprite, BOOL flag);

void ov83_02247568(struct BattleCastleAppSprite *sprite, u16 x, u16 y);

void ov83_02240748(BattleCastleSelfApp *app);

void ov83_02240748(BattleCastleSelfApp *app) {
    s16 pos[2];
    u16 itemId;
    u32 i;
    u32 entry;
    u32 start;

    i = 0;
    entry = (u16)(6 * app->page);
    start = entry;
    do {
        if (entry < app->numEntries) {
            itemId = ov83_02240F48(app, (u16)app->listItems[start + i].value, app->selectedMenuEntry);
            ov83_02247264(&app->spriteMan, i + 4, itemId);
            ov83_022472A0(&app->spriteMan, i + 4, itemId);
            ov83_02242894(i, &pos[1], &pos[0]);
            ov83_02247568(app->listItemSprites[i], pos[1], pos[0]);
            ov83_0224755C(app->listItemSprites[i], 1);
        } else {
            ov83_0224755C(app->listItemSprites[i], 0);
        }
        entry++;
        i++;
    } while (i < 6);
}
