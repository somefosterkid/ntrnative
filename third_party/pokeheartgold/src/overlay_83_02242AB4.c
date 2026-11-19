#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
struct BattleCastleSelfApp;

s32 ov83_02240DB0();

void ov83_02242AB4(struct BattleCastleSelfApp *arg0, s8 arg1, s8 arg2);

void ov83_02247568(void *);
void ov83_022475D4(void *, u32);
void ov83_02242AC0(struct BattleCastleSelfApp *param_1, u32 param_2, u32 param_3, u32 param_4);

s32 ov83_022408E0(BattleCastleSelfApp *, u32);
void ov83_02242AE0(BattleCastleSelfApp *arg0, u32 arg1);

void ov83_02242AB4(BattleCastleSelfApp *arg0, s8 arg1, s8 arg2) {
    *((s8 *)&arg0->selectedMonSlot) = arg1;
    *((s8 *)&arg0->previousSlot) = arg2;
    ov83_02240DB0();
}

void ov83_02242AC0(BattleCastleSelfApp *param_1, u32 param_2, u32 param_3, u32 param_4) {
    ov83_02247568(*((void * *)&param_1->itemSelectCursorSprite));
    ov83_022475D4(*((void * *)&param_1->itemSelectCursorSprite), param_4);
    return;
}

void ov83_02242AE0(BattleCastleSelfApp *arg0, u32 arg1) {
    ov83_022408E0(arg0, (u16)arg1);
}
