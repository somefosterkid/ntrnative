#include "global.h"

#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE160(MailBoxView *arg0) {
    s32 resId;

    resId = 0x37F0;
    do {
        SpriteSystem_LoadCharResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, arg0->iconNarc, (s32)GetMonIconNaixEx(0U, 0, 0U), 0, 2, resId);
        resId += 1;
    } while ((u32)resId <= 0x37F2U);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, arg0->iconNarc, (s32)sub_02074490(), 0, 3, 2, 0x37F0);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, arg0->iconNarc, (s32)sub_02074494(), 0, 0x37F0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, arg0->iconNarc, (s32)sub_02074498(), 0, 0x37F0);
}
