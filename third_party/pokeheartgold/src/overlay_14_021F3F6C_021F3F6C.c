#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2A74(struct UnkStruct_ov14_boxsys *arg0, s32 arg1, u32 priority);

void ov14_021F3F6C(UnkStruct_ov14_boxapp *a0);

extern ManagedSpriteTemplate ov14_021F810C;

extern s8 ov14_021F8070[];

extern s8 ov14_021F8078[];

void ov14_021F3F6C(UnkStruct_ov14_boxapp *a0) {
    s16 pos[2];
    ManagedSpriteTemplate tpl;
    UnkStruct_ov14_boxsys *r4;
    u32 idx;
    void *proxy;
    u32 drawPriority;
    s32 priority;
    u16 i;

    if (a0->unk21 == 0xff) {
        return;
    }
    r4 = a0->sys;
    idx = r4->unk_4094[a0->unk21];
    ManagedSprite_GetPositionXY(r4->sprites[idx], &pos[1], &pos[0]);
    if (r4->sprites[0x3d] == NULL) {
        tpl = ov14_021F810C;
        tpl.drawPriority = ManagedSprite_GetDrawPriority(r4->sprites[idx]) + 1;
        tpl.bgPriority = ManagedSprite_GetPriority(r4->sprites[idx]);
        tpl.resIdList[0] = idx + 0xc0e0;
        r4->unk_88D2 = 1;
        for (i = 0; i < 8; i = i + 1) {
            tpl.x = pos[1] + ov14_021F8070[i];
            tpl.y = pos[0] + ov14_021F8078[i];
            r4->sprites[0x3d + i] = SpriteSystem_NewSprite(r4->unk_2F4, r4->unk_2F8, &tpl);
            ManagedSprite_SetPaletteOverride(r4->sprites[0x3d + i], 8);
        }
        r4->unk_88D2 = 0;
        return;
    }
    proxy = Sprite_GetImageProxy(r4->sprites[idx]->sprite);
    drawPriority = ManagedSprite_GetDrawPriority(r4->sprites[idx]);
    priority = ManagedSprite_GetPriority(r4->sprites[idx]);
    for (i = 0; i < 8; i = i + 1) {
        Sprite_SetImageProxy(r4->sprites[0x3d + i]->sprite, proxy);
        ManagedSprite_SetPositionXY(r4->sprites[0x3d + i], pos[1] + ov14_021F8070[i], pos[0] + ov14_021F8078[i]);
        ov14_021F2A74(a0->sys, i + 0x3d, drawPriority + 1);
        ManagedSprite_SetPriority(a0->sys->sprites[0x3d + i], priority);
        ManagedSprite_SetDrawFlag(a0->sys->sprites[0x3d + i], 1);
    }
}
