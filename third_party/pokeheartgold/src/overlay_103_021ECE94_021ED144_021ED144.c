#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

s32 ov103_021ED144(MailBoxWork *arg0);

s32 ov103_021ED144(MailBoxWork *arg0) {
    MailBoxPaletteFx *t;
    MailBoxView *base;

    base = arg0->view;
    t = &base->paletteFx;
    switch (t->phase) {
    case 0:
        if (t->onScreen == 0) {
            ManagedSprite_SetPaletteOverride(base->sprites[t->target], t->firstPalette);
        } else {
            BgTilemapRectChangePalette(base->bgConfig, t->target, t->x, t->y, t->width, t->height, t->firstPalette);
            ScheduleBgTilemapBufferTransfer(arg0->view->bgConfig, t->target);
        }
        t->phase++;
        break;
    case 1:
        t->timer++;
        if (t->timer == 4) {
            if (t->onScreen == 0) {
                ManagedSprite_SetPaletteOverride(arg0->view->sprites[t->target], t->secondPalette);
            } else {
                BgTilemapRectChangePalette(arg0->view->bgConfig, t->target, t->x, t->y, t->width, t->height, t->secondPalette);
                ScheduleBgTilemapBufferTransfer(arg0->view->bgConfig, t->target);
            }
            t->timer = 0;
            t->phase++;
        }
        break;
    case 2:
        t->timer++;
        if (t->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
