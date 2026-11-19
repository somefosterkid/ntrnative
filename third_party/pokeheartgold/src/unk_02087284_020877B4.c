#include "global.h"

#include "sprite_system.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sys_task.h"

void sub_020877B4(void *arg0);

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;
typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;
typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;
void sub_02087830(UnkStruct_0208B878 *param0);

void sub_020877B4(void *arg0) {
    ManagedSpriteTemplate unkTemplate;
    SpriteSystem *spriteSystem;
    ManagedSprite *managedSprite;
    SpriteManager *spriteManager;
    s32 v1;

    spriteSystem = ((UnkStruct_0208B878 *)arg0)->unk_00.unk_04;
    spriteManager = ((UnkStruct_0208B878 *)arg0)->unk_00.unk_08;
    unkTemplate.x = 0x80;
    unkTemplate.y = 0x60;
    v1 = 0;
    unkTemplate.z = 0;
    unkTemplate.animation = 0;
    unkTemplate.drawPriority = 0;
    unkTemplate.vram = *((enum NNS_G2D_VRAM_TYPE *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_08);
    unkTemplate.bgPriority = 0;
    unkTemplate.vramTransfer = 0;
    unkTemplate.pal = 0;
    unkTemplate.resIdList[0] = *((enum NNS_G2D_VRAM_TYPE *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_08) + 0x56CE;
    unkTemplate.resIdList[1] = *((enum NNS_G2D_VRAM_TYPE *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_08) + 0x56CE;
    unkTemplate.resIdList[2] = *((enum NNS_G2D_VRAM_TYPE *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_08) + 0x56CE;
    unkTemplate.resIdList[3] = *((enum NNS_G2D_VRAM_TYPE *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_08) + 0x56CE;
    unkTemplate.resIdList[4] = -1;
    unkTemplate.resIdList[5] = -1;
    if ((s32) *((s32 *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_00) > 0) {
        do {
            managedSprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &unkTemplate);
            ((UnkStruct_0208B878 *)arg0)->unk_24[v1].unk_00 = managedSprite;
            ManagedSprite_TickFrame(managedSprite);
            ManagedSprite_SetPositionXY(((UnkStruct_0208B878 *)arg0)->unk_24[v1].unk_00, 0x80, 0x60);
            v1 += 1;
        } while (v1 < (s32) *((s32 *)&((UnkStruct_0208B878 *)arg0)->unk_10.unk_00));
    }
}

void sub_02087830(UnkStruct_0208B878 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
        SpriteManager_UnloadCharObjById(param0->unk_00.unk_08, 22222 + param0->unk_10.unk_08);
        SpriteManager_UnloadCellObjById(param0->unk_00.unk_08, 22222 + param0->unk_10.unk_08);
        SpriteManager_UnloadAnimObjById(param0->unk_00.unk_08, 22222 + param0->unk_10.unk_08);
        Sprite_DeleteAndFreeResources(param0->unk_24[v0].unk_00);
    }
}
