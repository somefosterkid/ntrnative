#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

extern ManagedSpriteTemplate ov14_021F8140;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F4278(UnkStruct_ov14_boxsys *param_1);

void ov14_021F4278(UnkStruct_ov14_boxsys *param_1) {
    u32 i;
    int x;
    ManagedSpriteTemplate t;
    ManagedSpriteTemplate base;
    ManagedSpriteTemplate *pb;
    i = 0;
    x = 0;
    base = ov14_021F8140;
    pb = &base;
    do {
        t = *pb;
        t.x = t.x + x;
        t.resIdList[0] = i + 0xc123;
        param_1->sprites[15 + i] = SpriteSystem_NewSprite(param_1->unk_2F4, param_1->unk_2F8, &t);
        x += 0x22;
        i++;
    } while (i < 6);
}
#else
void ov14_021F4278(u8 *param_1);

void ov14_021F4278(u8 *param_1) {
    u32 i;
    int x;
    u8 *p;
    ManagedSpriteTemplate t;
    ManagedSpriteTemplate base;
    ManagedSpriteTemplate *pb;
    u8 *new_var;
    i = 0;
    x = 0;
    p = param_1;
    base = ov14_021F8140;
    new_var = param_1 + 0x2f4;
    pb = &base;
    do {
        t = *pb;
        t.x = t.x + x;
        t.resIdList[0] = i + 0xc123;
        *((ManagedSprite **)(p + 0x338)) = SpriteSystem_NewSprite(*((SpriteSystem **)new_var), ((SpriteManager *)((UnkStruct_ov14_boxsys *)param_1)->unk_2F8), &t);
        x += 0x22;
        p += 4;
        i++;
    } while (i < 6);
}
#endif
