#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2A74(UnkStruct_ov14_boxsys *arg0, s32 arg1, u32 priority);

void ov14_021F39D0(void *a0);

extern ManagedSpriteTemplate ov14_021F83E4;

extern s8 ov14_021F8070[];

extern s8 ov14_021F8078[];

void ov14_021F39D0(void *a0) {
    s16 pos[2];
    ManagedSpriteTemplate tpl;
    u32 drawPriority;
    s32 priority;
    u32 i;

    if (((UnkStruct_ov14_boxsys *)a0)->unk_88C8 == 0) {
        return;
    }
    ManagedSprite_GetPositionXY(((UnkStruct_ov14_boxsys *)a0)->sprites[11], &pos[1], &pos[0]);
    if (((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d] == NULL) {
        tpl = ov14_021F83E4;
        tpl.drawPriority = ManagedSprite_GetDrawPriority(((UnkStruct_ov14_boxsys *)a0)->sprites[11]) + 1;
        tpl.bgPriority = ManagedSprite_GetPriority(((UnkStruct_ov14_boxsys *)a0)->sprites[11]);
        tpl.resIdList[0] = 0xc11f;
        ((UnkStruct_ov14_boxsys *)a0)->unk_88D2 = 1;
        for (i = 0; i < 8; i++) {
            tpl.x = pos[1] + ov14_021F8070[i];
            tpl.y = pos[0] + ov14_021F8078[i];
            ((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i] = SpriteSystem_NewSprite(((UnkStruct_ov14_boxsys *)a0)->unk_2F4, ((UnkStruct_ov14_boxsys *)a0)->unk_2F8, &tpl);
            ManagedSprite_SetPaletteOverride(((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i], 8);
            ManagedSprite_SetDrawFlag(((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i], 0);
        }
        ((UnkStruct_ov14_boxsys *)a0)->unk_88D2 = 0;
        return;
    }
    drawPriority = ManagedSprite_GetDrawPriority(((UnkStruct_ov14_boxsys *)a0)->sprites[11]);
    priority = ManagedSprite_GetPriority(((UnkStruct_ov14_boxsys *)a0)->sprites[11]);
    for (i = 0; i < 8; i++) {
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i], pos[1] + ov14_021F8070[i], pos[0] + ov14_021F8078[i]);
        ov14_021F2A74(a0, 0x3d + i, drawPriority + 1);
        ManagedSprite_SetPriority(((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i], priority);
        ManagedSprite_SetDrawFlag(((UnkStruct_ov14_boxsys *)a0)->sprites[0x3d + i], 0);
    }
}
