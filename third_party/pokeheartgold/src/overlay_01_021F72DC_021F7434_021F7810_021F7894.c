#include "global.h"

#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#if defined(SDK_PORT) || defined(NONMATCHING)

s32 ov01_021F8C88(LocalMapObject *, Billboard *);

void ov01_021F8D80(LocalMapObject *object, Billboard *animObj, MapObjectBillboardWorkB *a2, u32 facingDir, BOOL flag);

s32 ov01_02205564(LocalMapObject *);

void ov01_02205808(s32, LocalMapObject *, Billboard *);

s32 ov01_0220589C(s32, LocalMapObject *, void *);

void ov01_021F7894(LocalMapObject *arg0);

void ov01_021F7894(LocalMapObject *arg0) {
    Billboard *temp_r6;
    Billboard *new_var;
    MapObjectBillboardWorkB *new_var2;
    u32 temp_r0;
    MapObjectBillboardWorkB *temp_r4;
    temp_r4 = (MapObjectBillboardWorkB *)sub_0205F40C(arg0);
    temp_r6 = temp_r4->billboard;
    if ((ov01_021FA2D4(arg0) != 1) && (0L != temp_r6)) {
        new_var2 = temp_r4;
        if (0 == (((u32)(temp_r4->flags << 0x1F)) >> 0x1F)) {
            temp_r4->flags = (u8)(1 | (new_var2->flags & (~1)));
            if (ov01_02205564(arg0)) {
                ov01_0220589C(1, arg0, temp_r4->billboard);
            } else if (ov01_0220553C(arg0) != 0) {
                ov01_02205808(1, arg0, temp_r6);
            }
        }
        temp_r0 = MapObject_GetFacingDirection(arg0);
        new_var = temp_r6;
        ov01_021F8D80(arg0, new_var, temp_r4, temp_r0, 1);
        temp_r4->unk_10 = (s8)temp_r0;
        ov01_021FA3E8(arg0, new_var);
        ov01_021F8C88(arg0, new_var);
    }
}
#else

s32 ov01_021F8C88(LocalMapObject *, Sprite *);

s32 ov01_021F8D80(LocalMapObject *, Sprite *, u8 *, u32, s32);

s32 ov01_02205564(LocalMapObject *);

s32 ov01_02205808(s32, LocalMapObject *, Sprite *);

s32 ov01_0220589C(s32, LocalMapObject *, void *);

void ov01_021F7894(LocalMapObject *arg0);

void ov01_021F7894(LocalMapObject *arg0) {
    Sprite *temp_r6;
    Sprite *new_var;
    u8 *new_var2;
    u32 temp_r0;
    u8 *temp_r4;
    u8 temp_r1;
    temp_r4 = sub_0205F40C(arg0);
    temp_r6 = *((Sprite **)temp_r4);
    if ((ov01_021FA2D4(arg0) != 1) && (0L != temp_r6)) {
        new_var2 = (u8 *)temp_r4;
        temp_r1 = *((u8 *)(((u8 *)temp_r4) + 0x17));
        if (0 == (((u32)(temp_r1 << 0x1F)) >> 0x1F)) {
            *((u8 *)(((u8 *)temp_r4) + 0x17)) = (u8)(1 | ((*((u8 *)(new_var2 + 0x17))) & (~1)));
            if (ov01_02205564(arg0)) {
                ov01_0220589C(1, arg0, *((void **)temp_r4));
            } else if (ov01_0220553C(arg0) != 0) {
                ov01_02205808(1, arg0, temp_r6);
            }
        }
        temp_r0 = MapObject_GetFacingDirection(arg0);
        new_var = temp_r6;
        ov01_021F8D80(arg0, new_var, temp_r4, temp_r0, 1);
        *((s8 *)(((u8 *)temp_r4) + 0x10)) = (s8)temp_r0;
        ov01_021FA3E8(arg0, new_var);
        ov01_021F8C88(arg0, new_var);
    }
}
#endif
