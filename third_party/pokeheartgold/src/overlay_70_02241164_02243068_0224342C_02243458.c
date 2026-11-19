#include "global.h"

#include "heap.h"
#include "list_menu_items.h"
#include "pokedex.h"

u16 *ov70_0223F658(s32, s32, s32 *, s32);

s32 ov70_0224342C(s32, Pokedex *, s32, u16 *);

s32 ov70_02243458(ListMenuItem **arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, Pokedex *arg5);

s32 ov70_02243458(ListMenuItem **arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, Pokedex *pokedex) {
    s32 v1;
    u32 v0;
    u16 *ptr;
    MsgData *msgData;
    s32 v2;
    s32 v4;
    u16 *v5;
    u16 v3;

    msgData = arg1;
    ptr = ov70_0223F658(0x3D, arg4, &v1, arg3);
    v2 = ov70_0224342C(arg3, pokedex, v1, ptr);
    v0 = v2;
    *arg0 = ListMenuItems_New(v2 + 1, HEAP_ID_61);
    v4 = 0;
    if (v1 > 0) {
        v5 = ptr;
        do {
            if (Pokedex_CheckMonSeenFlag(pokedex, *v5) != 0) {
                v3 = *v5;
                ListMenuItems_AppendFromMsgData(*arg0, msgData, (s32)v3, (s32)v3);
            }
            v4 += 1;
            v5 += 1;
        } while (v4 < v1);
    }
    Heap_Free(ptr);
    return v0;
}
