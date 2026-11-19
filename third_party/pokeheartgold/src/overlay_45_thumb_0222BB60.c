#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "overlay_45_wifi_plaza_system.h"
#include "ppw_lobby.h"

void ov45_0222C3A8(UnkStruct_ov66_022302B0 *param0);

void ov45_0222CA8C(UnkStruct_ov66_022309A4 *param0);

void ov45_0222BB60(UnkStruct_ov66_0222FB64 *param0, UnkStruct_ov66_0222FA80 *param1, UnkStruct_ov66_022302B0 *param2, UnkStruct_ov66_022309A4 *param3);

void ov45_0222BC3C(UnkStruct_ov66_0222FB64 *param0);
void ov45_0222BC84(const UnkStruct_ov66_0222FB64 *param0);
void ov45_0222BCA0(UnkStruct_ov66_0222FBC8 *param0);

void ov45_0222BB60(UnkStruct_ov66_0222FB64 *param0, UnkStruct_ov66_0222FA80 *param1, UnkStruct_ov66_022302B0 *param2, UnkStruct_ov66_022309A4 *param3) {
    if ((param0->unk_04 - 1) >= 0) {
        param0->unk_04--;

        if (param0->unk_04 == (28 * 30)) {
            GF_SndStartFadeOutBGM(0, 127);
            param0->unk_0E = 1;
        }
    } else if (param0->unk_04 == 0) {
        param0->unk_00_1 = 1;
        param1->unk_08 |= 1 << 7;
        param0->unk_04--;
    }

    if ((param0->unk_06 - 1) >= 0) {
        param0->unk_06--;
    } else if (param0->unk_06 == 0) {
        param1->unk_08 |= 1 << 2;
        param1->unk_08 |= 1 << 3;
        param1->unk_08 |= 1 << 1;
        param0->unk_06--;
    }

    if ((param0->unk_08 - 1) >= 0) {
        param0->unk_08--;
    } else if (param0->unk_08 == 0) {
        param0->unk_00_2 = 1;
        param1->unk_08 |= 1 << 5;
        param0->unk_08--;
        ov45_0222CA8C(param3);
    }

    if ((param0->unk_0A - 1) >= 0) {
        param0->unk_0A--;
    } else if (param0->unk_0A == 0) {
        ov45_0222C3A8(param2);

        if (param0->unk_0C == 0) {
            PlayBGM(1153);
        }

        param0->unk_00_4 = 1;
        param1->unk_08 |= 1 << 6;
        param0->unk_0A--;
    }
}

void ov45_0222BC3C(UnkStruct_ov66_0222FB64 *param0) {
    param0->unk_00_0 = 0;
    param0->unk_00_1 = 0;
    param0->unk_00_2 = 0;
    param0->unk_00_4 = 0;
    param0->unk_00_5 = 0;
    param0->unk_00_7 = 0;
    param0->unk_01 = PPW_LOBBY_TIME_EVENT_NEON_A0;
    param0->unk_02 = PPW_LOBBY_TIME_EVENT_NEON_B0;
    param0->unk_03 = PPW_LOBBY_TIME_EVENT_NEON_C0;
    param0->unk_04 = -1;
    param0->unk_06 = -1;
    param0->unk_08 = -1;
    param0->unk_0A = -1;
}

void ov45_0222BC84(const UnkStruct_ov66_0222FB64 *param0) {
    if (param0->unk_0D == 1) {
        GF_SndHandleSetPlayerVolume(7, (127 / 3));
    } else {
        GF_SndHandleSetPlayerVolume(7, 127);
    }
}

void ov45_0222BCA0(UnkStruct_ov66_0222FBC8 *param0) {
    param0->unk_00 = (30 * 30);
}
