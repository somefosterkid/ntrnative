#include "global.h"

#include "comm_player_manager.h"
#include "field_system.h"
#include "field_types_def.h"
#include "unk_02035900.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_02057384(u8);
#else
s32 sub_02057384(s32);
#endif

s32 sub_02057D74(void);

void sub_020573F0(FieldSystem *fieldSystem, s32 a1);

int sub_02057454(u16 unused, u16 keys);

void sub_020573F0(FieldSystem *fieldSystem, s32 a1) {
    PlayerAvatar *v0;

    if (_021D41C4 != NULL) {
        sub_02057384(a1);
        _021D41C4->localWalkEnabled = (s8)a1;
        v0 = fieldSystem->playerAvatar;
        if (v0 != NULL) {
            _021D41C4->avatars[sub_0203769C()] = v0;
        }
        if ((sub_02037474() != 0) && (sub_0203769C() == 0)) {
            sub_02057550(0, _021D41C4);
        }
        if ((sub_02037474() != 0) && (sub_0203769C() == 0)) {
            sub_02057D74();
        }
    }
}

int sub_02057454(u16 unused, u16 keys) {
    if (keys & 0x0020) {
        return 2;
    }

    if (keys & 0x0010) {
        return 3;
    }

    if (keys & 0x0040) {
        return 0;
    }

    if (keys & 0x0080) {
        return 1;
    }

    return -1;
}
