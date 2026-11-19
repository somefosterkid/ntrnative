#include "global.h"

#include "comm_player_manager.h"
#include "field_system.h"
#include "player_avatar.h"
#include "unk_02035900.h"
#include "unk_02056D7C.h"

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};

int sub_02057EEC(int dir);

u8 sub_02057F18(s32 arg0);
s32 sub_0205701C(s32);
void sub_02057F58(void);
s32 sub_02057358(u8);
void sub_02057F70(void);
void sub_02057F80(void);

int sub_02057EEC(int dir) {
    if (dir == FACE_UP) {
        return FACE_DOWN;
    } else if (dir == FACE_DOWN) {
        return FACE_UP;
    } else if (dir == FACE_LEFT) {
        return FACE_RIGHT;
    } else if (dir == FACE_RIGHT) {
        return FACE_LEFT;
    }

    ((0) ? (void)0 : GF_AssertFail());
    return FACE_LEFT;
}

u8 sub_02057F18(s32 arg0) {
    return _021D41C4->moveTimer[arg0];
}

void sub_02057F28(u16 val) {
    PlayerAvatar_SetFacingDirection(_021D41C4->fieldSystem->playerAvatar, (u32)val);
    _021D41C4->positions[sub_0203769C()].facing = (s8)val;
    sub_0205701C(0);
}

void sub_02057F58(void) {
    sub_0203E2F4();
    _021D41C4->localMoveEnabled = 0;
}

void sub_02057F70(void) {
    sub_0203E30C();
    sub_02057358(1);
}

void sub_02057F80(void) {
    if (_021D41C4->fieldResumed == 0) {
        sub_02057F70();
        _021D41C4->fieldResumed = 1U;
    }
}
