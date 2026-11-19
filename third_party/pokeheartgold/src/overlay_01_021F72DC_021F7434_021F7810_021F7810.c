#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "sprite.h"
#if defined(SDK_PORT) || defined(NONMATCHING)

void ov01_021F8D80(LocalMapObject *object, Billboard *animObj, MapObjectBillboardWorkB *a2, u32 facingDir, BOOL flag);

s32 ov01_021F8C88(LocalMapObject *, Billboard *);

s32 ov01_02205564(LocalMapObject *);

void ov01_02205808(s32, LocalMapObject *, Billboard *);

s32 ov01_0220589C(s32, LocalMapObject *, void *);

void ov01_021F7810(LocalMapObject *arg0);

void ov01_021F7810(LocalMapObject *arg0) {
    s32 temp_r0;
    Billboard *temp_r6;
    MapObjectBillboardWorkB *temp_r4;
    MapObjectBillboardWorkB *new_var2;
    temp_r4 = (MapObjectBillboardWorkB *)sub_0205F40C(arg0);
    temp_r6 = temp_r4->billboard;
    if ((ov01_021FA2D4(arg0) != 1) && (temp_r6 != 0)) {
        if ((((u32)(temp_r4->flags << 0x1F)) >> 0x1F) == 0) {
            new_var2 = temp_r4;
            temp_r4->flags = (u8)(1 | (new_var2->flags & (~1)));
            if (ov01_02205564(arg0) != 0) {
                ov01_0220589C(1, arg0, temp_r4->billboard);
            } else if (ov01_0220553C(arg0) != 0) {
                ov01_02205808(1, arg0, temp_r6);
            }
        }
        temp_r0 = (s32)MapObject_GetFacingDirection(arg0);
        ov01_021F8D80(arg0, temp_r6, temp_r4, temp_r0, 1);
        temp_r4->unk_10 = (s8)temp_r0;
        ov01_021FA3E8(arg0, temp_r6);
        ov01_021F8C88(arg0, temp_r6);
    }
}
#else

void ov01_021F8D80(void *, s32, void *, s32, s32);

s32 ov01_021F8C88(LocalMapObject *, s32);

s32 ov01_02205564(LocalMapObject *);

s32 ov01_02205808(s32, LocalMapObject *, s32);

s32 ov01_0220589C(s32, LocalMapObject *, void *);

void ov01_021F7810(LocalMapObject *arg0);

void ov01_021F7810(LocalMapObject *arg0) {
    s32 temp_r0;
    s32 temp_r6;
    u8 *temp_r4;
    u8 temp_r1;
    int new_var;
    temp_r4 = sub_0205F40C(arg0);
    temp_r6 = *((s32 *)temp_r4);
    if ((ov01_021FA2D4(arg0) != 1) && (temp_r6 != 0)) {
        temp_r1 = *((u8 *)(((u8 *)temp_r4) + 0x17));
        if ((((u32)(temp_r1 << 0x1F)) >> 0x1F) == 0) {
            u8 *new_var2;
            new_var = 0x17;
            new_var2 = (u8 *)temp_r4;
            *((u8 *)(((u8 *)temp_r4) + new_var)) = (u8)(1 | ((*((u8 *)(new_var2 + new_var))) & (~1)));
            if (ov01_02205564(arg0) != 0) {
                ov01_0220589C(1, arg0, *((void **)temp_r4));
            } else if (ov01_0220553C(arg0) != 0) {
                ov01_02205808(1, arg0, temp_r6);
            }
        }
        temp_r0 = (s32)MapObject_GetFacingDirection(arg0);
        ov01_021F8D80((u8 *)arg0, temp_r6, temp_r4, temp_r0, 1);
        *((s8 *)(((u8 *)temp_r4) + 0x10)) = (s8)temp_r0;
        ov01_021FA3E8(arg0, (Sprite *)temp_r6);
        ov01_021F8C88(arg0, temp_r6);
    }
}
#endif
