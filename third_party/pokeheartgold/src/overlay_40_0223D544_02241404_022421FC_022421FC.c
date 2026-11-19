#include "global.h"
struct UnkStruct_0208C06C;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222D6D0(void *);

void ov40_0222D7DC(void *);

void ov40_022421FC(struct UnkStruct_0208C06C *param_1);

void ov40_022421FC(UnkStruct_0208C06C *param_1) {
    u8 *iVar1;

    iVar1 = *((u8 * *)&param_1->unk_860);
    ov40_0222D6D0(iVar1 + 0x10);
    ov40_0222D6D0(iVar1 + 0x2c);
    Sprite_DeleteAndFreeResources(*(ManagedSprite **)(iVar1 + 0x14));
    Sprite_DeleteAndFreeResources(*(ManagedSprite **)(iVar1 + 0x30));
    ov40_0222D7DC(param_1);
    return;
}
