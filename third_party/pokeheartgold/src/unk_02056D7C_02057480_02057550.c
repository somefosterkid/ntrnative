#include "global.h"

#include "comm_player_manager.h"

int sub_02057524(int param0);

s32 sub_02057BEC(s32 arg0);

s32 sub_02057C24(s32 arg0);

u8 sub_020374AC(s32);

u16 sub_020374C0(s32);

s32 sub_02057454(u32, u32);

s32 sub_020574C4(s32, s32, s32);

void sub_02057550(s32 unused, CommPlayerManager *manager) {
    u8 uVar1;
    s32 iVar2;
    s32 sVar3;
    s32 sVar4;
    s32 iVar5;
    CommPlayerPosition *pos;
    u32 uVar7;
    int arg0;

    arg0 = 0;
    do {
        if ((_021D41C4->active[arg0] != 0) && (_021D41C4->moveEnabled[arg0] != 0)) {
            pos = &_021D41C4->hostPositions[arg0];
            if (_021D41C4->hostMoveTimer[arg0] != 0) {
                _021D41C4->hostMoveTimer[arg0]--;
            }
            if (_021D41C4->inputLocked[arg0] != 0) {
                uVar7 = 0;
            } else {
                uVar7 = sub_020374C0(arg0);
            }
            uVar1 = sub_020374AC(arg0);
            pos->speed = uVar1;
            if (_021D41C4->hostMoveTimer[arg0] == 0) {
                if (pos->bumped == 1) {
                    _021D41C4->positionChanged[arg0] = 1;
                }
                pos->bumped = 0;
                if (_021D41C4->unk_C4[arg0] == 2) {
                    _021D41C4->unk_C4[arg0] = 1;
                } else {
                    iVar2 = sub_02057454(uVar7, uVar7);
                    sVar3 = sub_02057BEC(arg0);
                    sVar4 = sub_02057C24(arg0);
                    if (((sVar3 != 0xffff) && (sVar4 != 0xffff)) && (iVar2 != -1)) {
                        if (pos->facing != iVar2) {
                            pos->facing = (s8)iVar2;
                            _021D41C4->hostMoveTimer[arg0] = 4;
                            _021D41C4->positionChanged[arg0] = 1;
                            if (2 < _021D41C4->unk_C4[arg0]) {
                                _021D41C4->unk_C4[arg0]--;
                            }
                        } else {
                            if (_021D41C4->unk_C4[arg0] == 0) {
                                iVar5 = sub_020574C4(sVar3, sVar4, arg0);
                                if (iVar5 != 0) {
                                    pos->bumped = 1;
                                    _021D41C4->positionChanged[arg0] = 1;
                                    _021D41C4->hostMoveTimer[arg0] = 4;
                                } else {
                                    _021D41C4->positionChanged[arg0] = 1;
                                    pos->x = (s16)sVar3;
                                    pos->z = (s16)sVar4;
                                    pos->facing = (s8)iVar2;
                                    iVar2 = sub_02057524((u32)pos->speed);
                                    _021D41C4->hostMoveTimer[arg0] = (u8)iVar2;
                                }
                            }
                        }
                    }
                }
            }
        }
        arg0 = arg0 + 1;
    } while (arg0 < 8);
    return;
}
