#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_96_app_0221040c.h"

ManagedSprite *ov96_022104D8(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

Ov96App_0221040C *ov96_02210240(SpriteSystem *spriteSystem, SpriteManager *spriteManager, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 unk1C, enum HeapID heapId, PokeathlonCourseData *course) {
    Ov96App_0221040C *dest;
    ManagedSprite *sprite;
    void *scrnBuf;

    dest = Heap_Alloc(heapId, sizeof(Ov96App_0221040C));
    MI_CpuFill8(dest, 0, sizeof(Ov96App_0221040C));
    dest->spriteSystem = spriteSystem;
    dest->spriteManager = spriteManager;
    dest->unk_1C = unk1C;
    dest->heapId = heapId;
    dest->bgConfig = bgConfig;
    dest->msgData = msgData;
    dest->msgFormat = msgFormat;
    dest->course = course;
    ov96_02210538(dest);
    sprite = ov96_022104D8(dest->spriteSystem, dest->spriteManager, 0xd2, 0x4c, 8, 0);
    dest->sprites[0] = sprite;
    ov96_0221075C(dest);
    ov96_0221065C(dest);
    ov96_0221069C(dest);
    scrnBuf = GfGfxLoader_GetScrnData(NARC_a_2_3_5, 0xe, 1, &dest->scrnData, dest->heapId);
    dest->scrnBuf = scrnBuf;
    ov96_0221040C(dest, 0x3c);
    ov96_0221058C(dest, 0);
    return dest;
}
