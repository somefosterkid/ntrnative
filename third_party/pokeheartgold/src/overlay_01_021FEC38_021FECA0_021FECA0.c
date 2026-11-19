#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "unk_0205FD20.h"
#include "overlay_01_simple_renderers.h"
#include "overworld_anim_manager.h"

extern const OverworldAnimManagerFuncs ov01_02209110;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FEC38.s (same label), with the context size from the type, not the DS literal.
int ov01_021FED14(OverworldAnimManager *param0, void *param1);
void ov01_021FED4C(OverworldAnimManager *param0, void *param1);
void ov01_021FED58(OverworldAnimManager *a0, UnkStruct_ov5_021F2C98 *arg1);
void ov01_021FED80(void *a0, UnkStruct_ov5_021F2C98 *arg1);
const OverworldAnimManagerFuncs ov01_02209110 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F2C98, ov01_021FED14, ov01_021FED4C, ov01_021FED58, ov01_021FED80);
#endif

void ov01_021FECA0(LocalMapObject *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov01_021FECA0(LocalMapObject *object, s32 x, s32 arg2, s32 arg3) {
    VecFx32 a2;
    UnkStruct_ov5_021F2C38 a4;
    FieldEffectManager *fieldEffMan;

    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1) {
        fieldEffMan = ov01_021F146C(object);
        a4.unk_00 = fieldEffMan;
        a4.unk_04 = ov01_021F1450(fieldEffMan, 6);
        sub_020611C8(x, arg3, &a2);
        a2.z += (arg2 * 8) + 0x1000;
        a2.y = (MapObject_GetPreviousYCoord(object) << 0xF) - 0x8000;
        ov01_021F1620(a4.unk_00, &ov01_02209110, &a2, 0, &a4, (s32)MapObject_GetPriorityPlusValue(object, 2U));
    }
}
