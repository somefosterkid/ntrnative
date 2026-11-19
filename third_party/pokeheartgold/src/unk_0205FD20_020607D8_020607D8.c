#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"

u32 sub_02060FA8(void *mapObj, u32 which);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov01_022056C4(void *mapObj, u32 which);
#else
u32 ov01_022056C4(void *mapObj, u32 which);
#endif

void ov01_021FDF88(void *mapObj, u32 kind);

void ov01_021FDA74(void *mapObj, u32 kind);

void sub_020607D8(void *mapObj, u8 behavior, u32 unused, u16 *renderFlags);

void sub_020607D8(void *mapObj, u8 behavior, u32 unused, u16 *renderFlags) {
    u8 chosen;
    u8 t1;
    u8 t3;
    u8 t2;
    u8 t4;
    u32 found;
    u8 r4;
    u8 t5;
    u32 kind;

    if ((((u32)*renderFlags << 19) >> 30) != 0) {
        if (MapObject_CheckFlag24(mapObj) != 1) {
            r4 = GetMetatileBehavior_None();

            if (MapObject_GetID(mapObj) == 0xfd) {
                if (ov01_022055DC(mapObj) != 0) {
                    if (MapObject_CheckFlag24(mapObj) != 0) {
                        return;
                    }
                    if (MetatileBehavior_HasReflectiveSurface(behavior) == 1) {
                        r4 = behavior;
                    } else {
                        t1 = (u8)sub_02060FA8(mapObj, 1);
                        t3 = (u8)sub_02060FA8(mapObj, 3);
                        t2 = (u8)sub_02060FA8(mapObj, 2);
                        t4 = (u8)ov01_022056C4(mapObj, 4);
                        t5 = (u8)ov01_022056C4(mapObj, 5);

                        found = 0;
                        if (MetatileBehavior_HasReflectiveSurface(t1) == 1) {
                            chosen = t1;
                            found = 1;
                        } else if (MetatileBehavior_HasReflectiveSurface(t3) == 1) {
                            chosen = t3;
                            found = 1;
                        } else if (MetatileBehavior_HasReflectiveSurface(t2) == 1) {
                            chosen = t2;
                            found = 1;
                        } else if (MetatileBehavior_HasReflectiveSurface(t4) == 1) {
                            chosen = t4;
                            found = 1;
                        } else if (MetatileBehavior_HasReflectiveSurface(t5) == 1) {
                            chosen = t5;
                            found = 1;
                        }
                        if (found == 1) {
                            r4 = chosen;
                        }
                    }

                    if (r4 == GetMetatileBehavior_None()) {
                        return;
                    }
                    MapObject_SetFlag24(mapObj, 1);
                    if (MetatileBehavior_IsReflective(r4) == 1) {
                        kind = 5;
                    } else if (MetatileBehavior_IsPuddle(r4) == 1) {
                        kind = 3;
                    } else {
                        kind = 4;
                    }
                    ov01_021FDF88(mapObj, kind);
                } else {
                    if (MapObject_CheckFlag24(mapObj) != 0) {
                        return;
                    }
                    if (MetatileBehavior_HasReflectiveSurface(behavior) == 1) {
                        r4 = behavior;
                    } else {
                        t1 = (u8)sub_02060FA8(mapObj, 1);
                        if (MetatileBehavior_HasReflectiveSurface(t1) == 1) {
                            r4 = t1;
                        }
                    }

                    if (r4 == GetMetatileBehavior_None()) {
                        return;
                    }
                    MapObject_SetFlag24(mapObj, 1);
                    if (MetatileBehavior_IsReflective(r4) == 1) {
                        kind = 2;
                    } else if (MetatileBehavior_IsPuddle(r4) == 1) {
                        kind = 0;
                    } else {
                        kind = 1;
                    }
                    ov01_021FDF88(mapObj, kind);
                }
            } else {
                if (MapObject_CheckFlag24(mapObj) != 0) {
                    return;
                }
                if (MetatileBehavior_HasReflectiveSurface(behavior) == 1) {
                    r4 = behavior;
                } else {
                    t1 = (u8)sub_02060FA8(mapObj, 1);
                    if (MetatileBehavior_HasReflectiveSurface(t1) == 1) {
                        r4 = t1;
                    }
                }

                if (r4 == GetMetatileBehavior_None()) {
                    return;
                }
                MapObject_SetFlag24(mapObj, 1);
                if (MetatileBehavior_IsReflective(r4) == 1) {
                    kind = 2;
                } else if (MetatileBehavior_IsPuddle(r4) == 1) {
                    kind = 0;
                } else {
                    kind = 1;
                }
                ov01_021FDA74(mapObj, kind);
            }
            return;
        }
    }
    return;
}

void sub_020609D4(LocalMapObject *arg0, s32 arg1, s32 arg2, u16 *arg3);

void sub_020609D4(LocalMapObject *arg0, s32 arg1, s32 arg2, u16 *arg3) {
    u8 a;
    u8 b;
    u8 c;
    s32 var_r4;
    u8 temp_r7;
    u8 temp_r6;
    if (((((u32)((*arg3) << 0x13)) >> 0x1E) != 0) && (MapObject_CheckFlag24(arg0) != 0)) {
        if ((MapObject_GetID(arg0) == 0xFD) && (ov01_022055DC(arg0) != 0)) {
            a = (u8)sub_02060FA8(arg0, 1);

            b = (u8)sub_02060FA8(arg0, 3);
            c = (u8)sub_02060FA8(arg0, 2);
            temp_r7 = ov01_022056C4(arg0, 4);
            temp_r6 = ov01_022056C4(arg0, 5);
            var_r4 = 0;
            if (MetatileBehavior_HasReflectiveSurface(a) == 1) {
                var_r4 = 1;
            } else if (MetatileBehavior_HasReflectiveSurface(b) == 1) {
                var_r4 = 1;
            } else if (MetatileBehavior_HasReflectiveSurface(c) == 1) {
                var_r4 = 1;
            } else if (MetatileBehavior_HasReflectiveSurface(temp_r7) == 1) {
                var_r4 = 1;
            } else if (MetatileBehavior_HasReflectiveSurface(temp_r6) == 1) {
                var_r4 = 1;
            }
            if (var_r4 == 0) {
                MapObject_SetFlag24(arg0, 0);
            }
        } else if (MetatileBehavior_HasReflectiveSurface((u8)sub_02060FA8(arg0, 1)) == 0) {
            MapObject_SetFlag24(arg0, 0);
        }
    }
}
