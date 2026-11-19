#include "global.h"

#include "unk_02095EAC.h"
#include "error_handling.h"
#include "sprite.h"
#include "unk_02032844.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0205A44C.h"

struct BgConfig;

struct Options;

struct SpriteResource;

struct String;

enum UnionRoomStatus {
    UR_STATUS_NONE = 0,
    UR_STATUS_DRAWING,
    UR_STATUS_MIXING_RECORDS,
    UR_STATUS_UNK_03,
    UR_STATUS_BUSY,
    UR_STATUS_SHOWING_TRAINER_CASE,
    UR_STATUS_BATTLING,
    UR_STATUS_TRADING,
    UR_STATUS_UNK_08,
    UR_STATUS_UNK_09,
    UR_STATUS_UNK_10,
    UR_STATUS_UNK_11,
    UR_STATUS_UNK_12,
    UR_STATUS_SPIN_TRADING,
};

int ov58_021D2A30(UnkStruct_02095EAC *);

void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1);

void ov37_021E78A4(UnkStruct_02095EAC *param0);

u32 ov37_021E75C4(void);

int ov37_021E76F0(UnkStruct_02095EAC *param0);

int ov37_021E76F0(UnkStruct_02095EAC *param0) {
    int v0 = ov37_021E75C4();

    switch (v0) {
    case 1:
        if (param0->unk_368 <= 14) {
            if (sub_02033250() != 1) {
                param0->unk_9464 = 1;
            }

            param0->unk_36C = 19;
            sub_02038C1C(1);

            if (param0->unk_945C) {
                ov37_021E78A4(param0);
                Sprite_SetAnimCtrlSeq(param0->unk_2AC[8], 21);
            }

            return 2;
        }
        break;
    case 2:
    case 3:
    case 4:
        sub_0205A904(UR_STATUS_DRAWING);

        if (v0 < param0->unk_378) {
            if (param0->unk_9458 == 2) {
                ov37_021E76D0(param0, 0);
            } else {
                ov37_021E76D0(param0, 1);
            }
        }
        break;
    case 5:
        sub_0205A904(UR_STATUS_UNK_08);
        break;
    }

    if (v0 < param0->unk_378) {
        param0->unk_37C = sub_02037454();
        param0->unk_380 = sub_02033250();

        if (param0->unk_9468) {
            if (!(param0->unk_380 & param0->unk_946C)) {
                ov37_021E76D0(param0, 1);

                param0->unk_9458 = 1;
                param0->unk_9468 = 0;
                param0->unk_946C = 0;
            }
        }
    }

    param0->unk_378 = ov37_021E75C4();

    if (param0->unk_37C < sub_02037454()) {
        param0->unk_9458 = 2;
        param0->unk_9468 = 1;
        param0->unk_946C = param0->unk_380 ^ sub_02033250();
        GF_ASSERT((param0->unk_946C == 2) || (param0->unk_946C == 4) || (param0->unk_946C == 8) || (param0->unk_946C == 16));
    }

    return 1;
}
