#include "global.h"
struct BattleFactoryApp;

#include "unk_0201956C.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

BOOL ov80_02237254(u8 challengeType);

BOOL ov81_02242710(struct BattleFactoryApp *arg0);

BOOL ov81_02242710(BattleFactoryApp *arg0) {
    return ov80_02237254(arg0->challengeType);
}

void ov81_0224271C(struct BattleFactoryApp *arg0);

void ov81_0224271C(BattleFactoryApp *arg0) {
    *((UnkStruct_0201956C * *)&arg0->unk_474) = sub_0201956C(arg0->bgConfig, 2, 1, HEAP_ID_100);
    sub_020195F4(*((UnkStruct_0201956C * *)&arg0->unk_474), 0, 4, 0x12, 4);
    sub_020196B8(*((UnkStruct_0201956C * *)&arg0->unk_474), 0, arg0->narc, 0x90, 1);
}

void ov81_0224275C(BattleFactoryApp *arg0) {
    sub_020195C0(arg0->unk_474);
}
