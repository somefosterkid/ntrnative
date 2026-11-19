#include "global.h"
struct HallOfFameShowcase;

#include "sprite_system.h"
#include "overlay_64_hall_of_fame_showcase.h"

s32 ov64_021E6010(void *, u32);

void ov64_021E64F8(struct HallOfFameShowcase *arg0);

void ov64_021E64F8(HallOfFameShowcase *arg0) {
    u32 v0;
    u32 character;

    v0 = 2;
    do {
        ov64_021E6010(arg0, v0);
        v0 += 1;
    } while (v0 <= 0xDU);
    character = 0xDCC2;
    do {
        SpriteManager_UnloadCharObjById(arg0->spriteManager, character);
        character += 1;
    } while (character <= 0xDCCDU);
}
