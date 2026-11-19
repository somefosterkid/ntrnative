#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite_system.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ECA18(Ov96App_021ED1D0 *);

s32 ov96_021EDC38(u32, s32);

void ov96_021EC82C(PokeathlonCourseData *arg0);

void ov96_021EC82C(PokeathlonCourseData *data) {
    Ov96App_021ED1D0 *v0;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    s32 resId;
    u8 v2;
    u8 v3;
    Ov96App_021ED1D0 *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    spriteSystem = v1->spriteSystem;
    spriteManager = v1->spriteManager;
    v0 = v1;
    v3 = 0;
    do {
        resId = v3 + 0x64;
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 9, 1, 2, resId);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 8, 0, 1, 2, resId);
        v3 += 1;
    } while ((u32)v3 < 5U);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xA, 1, 0x64);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xB, 1, 0x64);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xD, 1, 2, 0x69);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xC, 0, 5, 2, 0x69);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xE, 1, 0x65);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xF, 1, 0x65);
    ov96_021ECA18(v0);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x10, 1, 1, 0x6A);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xC, 0, 1, 1, 0x6A);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x11, 1, 0x66);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x12, 1, 0x66);
    v2 = v0->courseId;
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, ov96_021EDC38(v2, 0) + 0x17, 0, 1, 1, 0x6B);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, ov96_021EDC38(v2, 1) + 0x17, 0, 1, 1, 0x6C);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, ov96_021EDC38(v2, 0) + 0x1C, 1, 1, 0x6B);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, ov96_021EDC38(v2, 1) + 0x1C, 1, 1, 0x6C);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x21, 1, 0x67);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x22, 1, 0x67);
}
