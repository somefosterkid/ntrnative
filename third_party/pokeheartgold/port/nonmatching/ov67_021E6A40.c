/* PORTOK hk39 */
#include "bg_window.h"
#include "sprite_system.h"

/* invented */
typedef struct UnkStruct_ov67_021E6A40 {
    u8 unk_00[0x10];
    BgConfig *bgConfig;
    u8 unk_14[0x484 - 0x14];
    ManagedSprite *sprites[4];  // 0x484
    u8 isBg : 1;                // 0x494
    u8 id : 7;                  // sprite index or bg layer
    u8 palette0 : 4;            // 0x495
    u8 palette1 : 4;
    u8 state;                   // 0x496
    u8 timer;                   // 0x497
    u8 x;                       // 0x498
    u8 y;
    u8 width;
    u8 height;
} UnkStruct_ov67_021E6A40;

s32 ov67_021E6A40(UnkStruct_ov67_021E6A40 *param0)
{
    switch (param0->state) {
    case 0:
        if (param0->isBg == 0) {
            ManagedSprite_SetPaletteOverride(param0->sprites[param0->id], param0->palette0);
        } else {
            BgTilemapRectChangePalette(param0->bgConfig, param0->id, param0->x, param0->y, param0->width, param0->height, param0->palette0);
            ScheduleBgTilemapBufferTransfer(param0->bgConfig, param0->id);
        }
        param0->state++;
        break;
    case 1:
        param0->timer++;
        if (param0->timer == 4) {
            if (param0->isBg == 0) {
                ManagedSprite_SetPaletteOverride(param0->sprites[param0->id], param0->palette1);
            } else {
                BgTilemapRectChangePalette(param0->bgConfig, param0->id, param0->x, param0->y, param0->width, param0->height, param0->palette1);
                ScheduleBgTilemapBufferTransfer(param0->bgConfig, param0->id);
            }
            param0->timer = 0;
            param0->state++;
        }
        break;
    case 2:
        param0->timer++;
        if (param0->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
