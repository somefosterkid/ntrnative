#include "global.h"

#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223A430_v0 {
    u8 unk_0[0x114];
    u8 unk_114;
    u8 unk_115[0x3];
    ManagedSprite *unk_118;
    TextOBJ *unk_11C;
    u8 unk_120[0x10];
    u8 unk_130;
    u8 unk_131[0x3];
    ManagedSprite *unk_134;
    TextOBJ *unk_138;
} UnkStruct_ov40_0223A430_v0;

s32 ov40_0222D5AC(void *, void *, s32);

s32 ov40_0222D66C(void *, void *, s32);

s32 ov40_0222D6EC(void *, s32);

ManagedSprite *ov40_0222D800(void *, s32);

s32 ov40_0223B4BC(UnkStruct_0208C06C *, s32);

void ov40_0223A430(UnkStruct_0208C06C *arg0);

void ov40_0223A430(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_0223A430_v0 *v0;

    v0 = arg0->unk_860;
    ov40_0222D6EC(arg0, 2);
    v0->unk_118 = ov40_0222D800(arg0, 2);
    v0->unk_134 = ov40_0222D800(arg0, 2);
    ov40_0222D5AC((void *)&v0->unk_114, &arg0->unk_14, 2);
    ov40_0222D5AC((void *)&v0->unk_130, &arg0->unk_14, 2);
    ov40_0222D66C((void *)&v0->unk_114, &arg0->unk_14, 3);
    ov40_0222D66C((void *)&v0->unk_130, &arg0->unk_14, 0x5E);
    ManagedSprite_SetAnim(v0->unk_118, 0);
    ManagedSprite_SetAnim(v0->unk_134, 3);
    ManagedSprite_SetPositionXY(v0->unk_118, 0x20, 0xE8);
    ManagedSprite_SetPositionXY(v0->unk_134, 0x80, 0xE8);
    sub_020136B4(v0->unk_11C, 0x24, -8);
    sub_020136B4(v0->unk_138, 0x24, -8);
    TextOBJ_SetSpritesDrawFlag(v0->unk_11C, 1);
    TextOBJ_SetSpritesDrawFlag(v0->unk_138, 1);
    ov40_0223B4BC(arg0, 0);
}
