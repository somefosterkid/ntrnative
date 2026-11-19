#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "camera.h"
#include "error_handling.h"
#include "overlay_85.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E5AF0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E5AF0(UnkStruct_ov109_021D0F70 *param0) {
    param0->unk_2C.unk_08 = 0;
    param0->unk_2C.unk_10 = 0;
}

s32 ov85_021E7194(s32 *, s32);

s32 ov85_021E5AF8(s32 *arg0) {
    ov85_021E7194(arg0, 0);
    *arg0 = 1;
    return 0;
}

int ov85_021E5B0C(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B0C(UnkStruct_ov109_021D0F70 *param0) {
    if (IsPaletteFadeFinished()) {
        if (sub_0203769C() == 0) {
            param0->unk_00 = 2;
        } else {
            param0->unk_00 = 8;
        }

        return 1;
    }

    return 0;
}

BOOL ov85_021E8628(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B30(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B30(UnkStruct_ov109_021D0F70 *param0) {
    if (ov85_021E8628(param0)) {
        param0->unk_00 = 3;
        return 1;
    }

    return 0;
}

BOOL sub_02096D4C(UnkStruct_0209BDF8 *param0, u32 param1, const void *param2, u32 param3);

int ov85_021E5B48(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B48(UnkStruct_ov109_021D0F70 *param0) {
    u16 v0 = (1 << 0);

    if (sub_02096D4C(param0->unk_D0, 8, &v0, 2) == 1) {
        param0->unk_00 = 4;
        return 1;
    }

    return 0;
}

int ov85_021E8660(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B78(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B78(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = ov85_021E8660(param0) + 1;

    if (v0 != sub_02037454()) {
        return 0;
    }

    param0->unk_00 = 5;
    return 1;
}

int ov85_021E5B98(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5B98(UnkStruct_ov109_021D0F70 *param0) {
    int v0;

    param0->unk_2C.unk_04 = ov85_021E8660(param0) + 1;

    v0 = sub_02096D4C(param0->unk_D0, 13, &param0->unk_2C.unk_04, 4);

    if (v0 == 1) {
        param0->unk_14 = 0;
        param0->unk_00 = 6;
    }

    return 0;
}

int ov85_021E5BC8(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5BC8(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D1048 v1;

    if (param0->unk_14 == 0) {
        v1.unk_02 = 0;
        v1.unk_00 = 0;

        v0 = sub_02096D4C(param0->unk_D0, 12, &v1, sizeof(UnkStruct_ov109_021D1048));

        if (v0 == 1) {
            param0->unk_14++;
        }

        return 0;
    }

    {
        int v2 = 1, v3 = 1;
        u32 v4 = param0->unk_D0->unk_42;

        do {
            if (v4 & (1 << v2)) {
                if (v3 >= param0->unk_14) {
                    v1.unk_02 = v2;
                    v1.unk_00 = param0->unk_14;

                    v0 = sub_02096D4C(param0->unk_D0, 12, &v1, sizeof(UnkStruct_ov109_021D1048));

                    if (v0 == 1) {
                        param0->unk_14++;
                    }
                    break;
                }

                v3++;
            }

            v2++;
        } while (v2 < 5);
    }

    if (param0->unk_14 >= param0->unk_2C.unk_04) {
        param0->unk_00 = 7;
    }

    return 0;
}

int ov85_021E5C58(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5C58(UnkStruct_ov109_021D0F70 *param0) {
    u16 v0 = (1 << 3);

    if (sub_02096D4C(param0->unk_D0, 8, &v0, 2) == 1) {
        param0->unk_00 = 10;
    }

    return 0;
}

int ov85_021E5C80(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5C80(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = sub_02096D4C(param0->unk_D0, 9, NULL, 0);

    if (v0 == 1) {
        param0->unk_10 = 0;
        param0->unk_00 = 9;
        return 0;
    }

    return 0;
}

BOOL ov85_021E8614(UnkStruct_ov109_021D0F70 *param0, u16 param1);

int ov85_021E5CA4(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5CA4(UnkStruct_ov109_021D0F70 *param0) {
    if (ov85_021E8614(param0, 1 << 3) == 1) {
        GF_ASSERT(param0->unk_2C.unk_04 >= 2);
        GF_ASSERT(param0->unk_2C.unk_00 != 0);
        param0->unk_00 = 10;
    }

    return 0;
}

int ov85_021E5CD0(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5CD0(UnkStruct_ov109_021D0F70 *param0) {
    sub_02037AC0(202);
    param0->unk_00 = 11;
    return 0;
}

int ov85_021E5CE4(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5CE4(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02037B38(202)) {
        param0->unk_00 = 12;
    }

    return 0;
}

int ov85_021E5CFC(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5CFC(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02096D4C(param0->unk_D0, 14, &param0->unk_CC->unk_04, 4)) {
        param0->unk_00 = 13;
    }

    return 0;
}

BOOL sub_02036FD8(int cmd, const void *data, int size);

int ov85_021E5D20(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5D20(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    void *v1 = param0->unk_24;
    v0 = sub_02036FD8(131, (void *)v1, 236 * 6 + 4 * 2);

    if (v0) {
        param0->unk_00 = 14;
    }

    return 0;
}

int ov85_021E8720(UnkStruct_ov109_021D0F70 *param0);

BOOL ov85_021E8834(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5D3C(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5D3C(UnkStruct_ov109_021D0F70 *param0) {
    if (ov85_021E8720(param0) == param0->unk_2C.unk_04) {
        if (ov85_021E8834(param0) == 1) {
            sub_02096D4C(param0->unk_D0, 16, NULL, 0);
            param0->unk_00 = 46;
        } else {
            sub_02096D4C(param0->unk_D0, 17, NULL, 0);
            param0->unk_00 = 15;
        }
    }

    return 0;
}

s32 ov85_021E8878(void *);

s32 ov85_021E8898(UnkStruct_ov109_021D0F70 *param0);

s32 ov85_021E5D84(UnkStruct_ov109_021D0F70 *arg0) {
    if (ov85_021E8898(arg0) == 1) {
        arg0->unk_00 = 0x2E;
    } else if (arg0->unk_2C.unk_04 == ov85_021E8878(arg0)) {
        arg0->unk_00 = 0x10;
    }
    return 0;
}
