#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE200.h"
#include "overlay_01_simple_renderers.h"
#include "unk_0205FD20.h"

Simple3DRenderObj *ov01_021FE2DC(UnkStruct_021F1CF8 *, u32, u32, u32);

Simple3DModel *ov01_021FE35C(UnkStruct_021F1CF8 *, u32, u32, u32);

extern const OverworldAnimManagerFuncs ov01_02208FE0;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FE200.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02208FE0 = OVERWORLD_ANIM_MANAGER_FUNCS(Ov01_021FE3F8_Work, ov01_021FE4FC, ov01_021FE524, ov01_021FE528, ov01_021FE558);
#endif

OverworldAnimManager *ov01_021FE3F8(LocalMapObject *arg0, u32 arg1) {
    VecFx32 sp2C;
    Ov01_021FE3F8_Args sp20;
    UnkStruct_021F1CF8 *sp1C;
    u32 sp18;
    u32 sp14;
    u32 sp10;
    u32 spC;
    FieldEffectManager *sp8;
    FieldEffectManager *temp_r0;
    s32 temp_r6;
    u32 temp_r7;

    spC = MapObject_GetPreviousXCoord(arg0);
    sp10 = MapObject_GetPreviousZCoord(arg0);
    temp_r6 = MapObject_GetPositionVectorYCoord(arg0);
    sp14 = MapObject_GetFacingDirection(arg0);
    sp18 = MapObject_GetPreviousFacingDirection(arg0);
    temp_r7 = MapObject_GetPriorityPlusValue(arg0, 2U);
    temp_r0 = ov01_021F146C(arg0);
    sp8 = temp_r0;
    sp1C = ov01_021F1450(temp_r0, 2);
    if (MapObject_TestFlagsBits(arg0, MAPOBJECTFLAG_VISIBLE) == 1) {
        return NULL;
    }
    sp20.kind = arg1;
    sp20.renderObj = ov01_021FE2DC(sp1C, arg1, sp14, sp18);
    sp20.model = ov01_021FE35C(sp1C, arg1, sp14, sp18);
    sub_020611C8((s32)spC, (s32)sp10, &sp2C);
    sp2C.y = temp_r6;
    {
        switch (arg1) {
        case 0:
        case 1:
        case 4:
        case 5:
            sp2C.y = temp_r6 - 0x8000;
            if ((u32)(arg1 - 2) <= 1U) {
                sp2C.z -= 0x8000U >> 2;
            } else {
                sp2C.z += 0x8000U >> 3;
            }
            break;
        case 2:
        case 3:
            sp2C.y = temp_r6 - 0xA000;
            if ((u32)(arg1 - 2) <= 1U) {
                sp2C.z -= 0x2000;
            } else {
                sp2C.z += 0x2000;
            }
            break;
        }
    }
    return ov01_021F1620(sp8, &ov01_02208FE0, &sp2C, 0, &sp20, (s32)temp_r7);
}
