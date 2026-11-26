#include "global.h"

#include "overlay_112_pokewalker_app.h"

#include "sprite_system.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"

struct ManagedSprite;

struct SysTask;

struct SysTaskQueue;

struct System;

int ov112_021E9750(Ov112PokewalkerApp *app) {
    u32 result;
    u32 keys;

    if (app->unk_1F2E2 == 0) {
        result = ov112_021E9888(0);
        switch (result) {
        case 0:
            app->unk_1EC50 = 0;
            ov112_021EA51C(app, 0);
            ov112_021EA584(app);
            PlaySE(0x5DD);
            app->unk_1F2E2 = 1;
            break;
        case 1:
            app->unk_1EC50 = 2;
            ov112_021EA51C(app, 1);
            ov112_021EA584(app);
            PlaySE(0x5DD);
            app->unk_1F2E2 = 1;
            break;
        case -1:
            keys = gSystem.newKeys;
            if (keys & 0x0040) {
                if (app->unk_1EC50 != 0) {
                    PlaySE(0x5DC);
                }
                ov112_021EA51C(app, 0);
                app->unk_1EC50 = 0;
            } else if (keys & 0x0080) {
                if (app->unk_1EC50 != 1) {
                    PlaySE(0x5DC);
                }
                ov112_021EA51C(app, 1);
                app->unk_1EC50 = 1;
            } else if (keys & 0x0001) {
                app->unk_1F2E2 = 1;
                ov112_021EA584(app);
                PlaySE(0x5DD);
            } else if (keys & 0x0002) {
                ov112_021EA51C(app, 1);
                ov112_021EA584(app);
                app->unk_1EC50 = 1;
                app->unk_1F2E2 = 1;
                PlaySE(0x5DC);
            }
            break;
        }
    } else if (app->unk_1F2E2 == 1) {
        if (!ManagedSprite_IsAnimated(app->sprites[0])) {
            app->unk_1F2E2 = 2;
        }
    } else if (app->unk_1F2E2 == 2) {
        if (app->unk_1EC50 != 0) {
            return 1;
        }
        return 0;
    }
    return 2;
}
