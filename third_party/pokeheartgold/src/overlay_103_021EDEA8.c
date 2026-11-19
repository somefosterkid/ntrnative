#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

ManagedSprite *ov103_021EE048(MailBoxView *view, const ManagedSpriteTemplate *unkTemplate);

extern const ManagedSpriteTemplate ov103_021EED58[];

void ov103_021EDEA8(MailBoxWork *arg0);

void ov103_021EE028(MailBoxView *view);
void ov103_021EE094(MailBoxView *view);
void ov103_021EDF50(MailBoxWork *arg0);
void ov103_021EDEA8(MailBoxWork *arg0) {
    const ManagedSpriteTemplate *v2;
    u32 v3;
    MailBoxView *v0;
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    ov103_021EE13C(arg0->view);
    ov103_021EDF88(arg0->view);
    ov103_021EE2E0(arg0->view);
    ov103_021EE160(arg0->view);
    v3 = 0;
    v2 = ov103_021EED58;
    do {
        arg0->view->sprites[v3] = ov103_021EE048(arg0->view, v2);
        v2++;
        v3 += 1;
    } while (v3 < 7U);
    v0 = arg0->view;
    if ((u32) v0->mailCount <= 0xAU) {
        ov103_021EE0CC(v0, 0, 0);
        ov103_021EE0CC(arg0->view, 1, 0);
    } else if (arg0->page == 0) {
        ov103_021EE0CC(v0, 0, 0);
    } else {
        ov103_021EE0CC(v0, 1, 0);
    }
    ov103_021EE0CC(arg0->view, 4, 0);
    ov103_021EE0CC(arg0->view, 5, 0);
    ov103_021EE0CC(arg0->view, 6, 0);
}

void ov103_021EDF50(MailBoxWork *arg0) {
    ov103_021EE094(arg0->view);
    ov103_021EE028(arg0->view);
    ov103_021EE150(arg0->view);
}

void ov103_021EDF68(MailBoxView *view) {
    ManagedSprite *managedSprite;
    u32 v0;
    v0 = 0;
    do {
        managedSprite = view->sprites[v0];
        if (managedSprite != NULL) {
            ManagedSprite_TickFrame(managedSprite);
        }
        v0 += 1;
    } while (v0 < 7U);
}
