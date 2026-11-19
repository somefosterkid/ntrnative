#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "sprite.h"
#include "sprite_system.h"

extern UnmanagedSpriteTemplate ov109_021E7A58[];

void ov109_021E6EE4(PhotoAlbumApp *app) {
    UnmanagedSpriteTemplate *var_r4;
    s32 var_r7;

    var_r4 = ov109_021E7A58;
    var_r7 = 0;
    do {
        app->sprites[var_r7] = SpriteSystem_CreateSpriteFromResourceHeader(app->spriteSystem, app->spriteManager, var_r4);
        Sprite_SetDrawFlag(app->sprites[var_r7], 1);
        Sprite_SetAnimSpeed(app->sprites[var_r7], FX32_ONE);
        var_r4++;
        var_r7 += 1;
    } while (var_r7 < 5);
    Sprite_SetAnimActiveFlag(app->sprites[0], 0);
    Sprite_SetDrawFlag(app->sprites[1], 0);
    Sprite_SetPriority(app->sprites[4], 2U);
    Sprite_SetAnimActiveFlag(app->sprites[4], 1);
}
