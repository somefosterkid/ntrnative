#include "global.h"

#include "filesystem.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE2E0(MailBoxView *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, narc, 7, 1, 1, 0x37F3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, narc, 0xA, 0, 2, 1, 0x37F1);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, narc, 8, 1, 0x37F1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, narc, 9, 1, 0x37F1);
    NARC_Delete(narc);
}

void ov103_021EE374(MailBoxView *arg0) {
    ov103_021EE0F8(arg0, 2, 0xE0, 0xB0);
    ov103_021EE0AC(arg0, 2, 4);
}
