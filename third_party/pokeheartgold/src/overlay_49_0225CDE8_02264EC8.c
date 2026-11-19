#include "global.h"

#include "overlay_45_wifi_plaza_system.h"
#include "overlay_49.h"
#include "overlay_49_unk_struct_ov49_02264cf8.h"

s32 ov45_0222B094();

s32 ov49_02259FE8(s32);

s32 ov49_0225A08C(s32, s32);

s32 ov49_0225A0BC(s32);

s32 ov49_0225A0CC(s32);

s32 ov49_0225A334(s32, u32, s32);

s32 ov49_02264C04(s32, u32, s32);

void ov49_02264EC8(UnkStruct_ov49_02264CF8 *arg0, s32 arg1) {
    ov49_02259FE8(arg1);
    if (ov45_0222B094() == 0x1C2) {
        ov49_0225A0CC(arg1);
        ov49_0225A334(arg1, arg0->unk_03, 0);
        ov49_0225A08C(arg1, ov49_02264C04(arg1, arg0->unk_03, 0x2AC));
        ov49_0225A0BC(arg1);
    }
}

void ov49_02264F10(UnkStruct_ov49_02264CF8 *arg0) {
    arg0->unk_0E = 0;
    arg0->unk_0D = 1;
}

void ov49_02264F1C(UnkStruct_ov49_02264CF8 *arg0) {
    arg0->unk_0E = 0;
    arg0->unk_0D = 0;
}

void ov49_02264F24(UnkStruct_ov49_02264CF8 *arg0, s32 arg1) {
    if ((arg0->unk_0D == 1) && (arg0->unk_0E == 0x1C2)) {
        ov49_0225A334(arg1, arg0->unk_03, 0);
        ov49_0225A08C(arg1, ov49_02264C04(arg1, arg0->unk_03, 0x2AB));
    }
}

void ov49_02264F60(UnkStruct_ov49_02264CF8 *arg0) {
    s16 v0;

    if (arg0->unk_0D == 1) {
        v0 = arg0->unk_0E;
        if ((s32)v0 <= 0x384) {
            arg0->unk_0E = (s16)(v0 + 1);
        }
    }
}

s32 ov45_0222A99C(const WiFiPlazaProfile *profile);

s32 ov45_0222A9A4(const WiFiPlazaProfile *profile);

void ov49_02264F78(UnkStruct_ov49_02264CF8 *arg0, const WiFiPlazaProfile *profile) {
    s16 v0;

    v0 = ov45_0222A9A4(profile);
    arg0->unk_44 = 1;
    arg0->unk_46 = v0;
    arg0->unk_48 = ov45_0222A99C(profile);
}
