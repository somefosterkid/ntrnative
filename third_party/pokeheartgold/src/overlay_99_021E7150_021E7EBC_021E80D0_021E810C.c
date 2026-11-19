#include "global.h"

#include "sprite_system.h"

ManagedSprite *ov98_0221E6E0(void *, ManagedSpriteTemplate *);

extern s32 ov99_021E9FB0[];

void ov99_021E810C(void *arg0);

void ov99_021E810C(void *arg0) {
    ManagedSpriteTemplate template = { 0 };
    void *data;
    s32 i;
    s32 *pos;
    s32 y;
    void *ptr;

    data = arg0;
    pos = ov99_021E9FB0;
    y = 0x30;
    i = 0;
    do {
        template.animation = i;
        template.resIdList[0] = 0x20;
        template.resIdList[1] = 3;
        template.resIdList[2] = 3;
        template.resIdList[3] = 3;
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.pal = i;
        template.x = *pos * 8 + 0x68;
        template.y = y;
        ptr = data + (i + 0xF) * 4;
        *(ManagedSprite **)((u8 *)ptr + 0x408) = ov98_0221E6E0(*(void **)((u8 *)data + 0x404), &template);
        ManagedSprite_SetAnimateFlag(*(ManagedSprite **)((u8 *)ptr + 0x408), 1);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)ptr + 0x408), 0);
        i++;
        pos++;
        y += 0x20;
    } while (i < 5);
}
