#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "player_avatar.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0205D3A8(PlayerAvatar *, LocalMapObject *, void *, u32, u16, u16);
#else
void sub_0205D3A8(PlayerAvatar *, LocalMapObject *, void *, u32, u32, u16);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0205D640(PlayerAvatar *, LocalMapObject *, void *, u32, u16, u16);
#else
void sub_0205D640(PlayerAvatar *, LocalMapObject *, void *, u32, u32, u16);
#endif

void sub_0205D340(PlayerAvatar *param_1, void *param_2, u32 param_3, u32 param_4, u16 param_5);

void sub_0205D340(PlayerAvatar *param_1, void *param_2, u32 param_3, u32 param_4, u16 param_5) {
    s32 sVar1;
    LocalMapObject *pLVar2;

    sVar1 = PlayerAvatar_GetState(param_1);
    pLVar2 = PlayerAvatar_GetMapObject(param_1);
    switch (sVar1) {
    case 0:
    case 2:
    case 3:
        sub_0205D3A8(param_1, pLVar2, param_2, param_3, param_4, param_5);
        return;
    case 1:
        sub_0205D640(param_1, pLVar2, param_2, param_3, param_4, param_5);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
