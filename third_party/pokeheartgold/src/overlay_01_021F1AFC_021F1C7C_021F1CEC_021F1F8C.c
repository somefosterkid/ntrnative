#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "script_manager.h"

extern s32 ov01_02206A14;

s32 ov01_021F1F8C(FieldSystem *arg0, LocalMapObject *arg1);

s32 ov01_021F1F8C(FieldSystem *arg0, LocalMapObject *arg1) {
    s32 *var_r4;
    u32 var_r6;

    var_r4 = &ov01_02206A14;
    var_r6 = 0;
loop_1:
    if ((*(u16 *)var_r4 == MapObject_GetMapID(arg1)) && (*(u16 *)((u8 *)var_r4 + 6) == MapObject_GetID(arg1)) && (*(u16 *)((u8 *)var_r4 + 2) == MapObject_GetXCoord(arg1)) && (*(u16 *)((u8 *)var_r4 + 4) == MapObject_GetZCoord(arg1))) {
        FieldSystem_FlagSet(arg0, (u16)MapObject_GetEventFlag(arg1));
        MapObject_Delete(arg1);
        FieldSystem_FlagClear(arg0, *(u16 *)((u8 *)(((u8 *)&ov01_02206A14) + (var_r6 * 0xA)) + 8));
        return 1;
    }
    var_r6 += 1;
    var_r4 = (void *)((u8 *)var_r4 + (0xA));
    if (var_r6 >= 4U) {
        return 0;
    }
    goto loop_1;
}
