#include "global.h"
struct UnkStruct_0208C06C;

#include "fashion_case.h"
#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF94(void *arg0);

s32 ov40_0222C018(void *arg0);

s32 ov40_0222C018(void *arg0) {
    *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_818) = (s32) * (s32 *)((u8 *)(arg0 + (((UnkStruct_0208C06C *)arg0)->unk_534.unk_1A4 * 4)) + 0x81C);
    ov40_0222BF94(arg0);
    return 1;
}

s32 ov40_0222BF64(void *, s32, s32, s32 *);

s32 ov40_0222BF80(void *, s32);

s32 sub_02087A84(u32 *, s32, s32);

void ov40_0222C03C(struct UnkStruct_0208C06C *arg0);

void ov40_0222C03C(UnkStruct_0208C06C *arg0) {
    u32 v0;

    v0 = *(u32 *)((u8 *)(*((u8 **)&arg0->unk_818) + (0x24 * arg0->unk_534.unk_1B0)) + 0x10);
    switch (v0) {
    case 2:
        sub_02087A84(*((void **)&arg0->unk_868), 0, 0);
        ov40_0222BF80(arg0, 2);
        return;
    case 4:
        ov40_0222BF64(arg0, 2, 1, *((void **)&arg0->unk_10));
        return;
    case 5:
        ov40_0222BF64(arg0, 3, 1, *((void **)&arg0->unk_10));
        return;
    case 8:
        ov40_0222BF64(arg0, 4, 1, *((void **)&arg0->unk_10));
        return;
    case 9:
        ov40_0222BF64(arg0, 5, 1, *((void **)&arg0->unk_10));
        return;
    case 10:
        ov40_0222BF64(arg0, 6, 1, *((void **)&arg0->unk_10));
        return;
    case 11:
        ov40_0222BF64(arg0, 7, 1, *((void **)&arg0->unk_10));
        return;
    case 12:
        ov40_0222BF64(arg0, 8, 1, *((void **)&arg0->unk_10));
        return;
    case 13:
        ov40_0222BF64(arg0, 9, 1, *((void **)&arg0->unk_10));
        return;
    case 14:
        ov40_0222BF64(arg0, 0xA, 1, *((void **)&arg0->unk_10));
        return;
    case 15:
        ov40_0222BF64(arg0, 0xB, 1, *((void **)&arg0->unk_10));
        return;
    case 16:
        ov40_0222BF64(arg0, 0xC, 1, *((void **)&arg0->unk_10));
        return;
    case 17:
        ov40_0222BF64(arg0, 0xD, 1, *((void **)&arg0->unk_10));
        return;
    case 18:
        ov40_0222BF64(arg0, 0xE, 1, *((void **)&arg0->unk_10));
        return;
    case 19:
        ov40_0222BF64(arg0, 0xF, 1, *((void **)&arg0->unk_10));
        return;
    case 7:
        ov40_0222BF64(arg0, 0x11, 1, *((void **)&arg0->unk_10));
    default:
        return;
    }
}

s32 ov40_0222DD94();

s32 ov40_0222DD9C(void *, s32);

s32 sub_0202BC10();

s32 ov40_0222C15C(void *arg0);

s32 ov40_0222C15C(void *arg0) {
    s32 a1;
    s32 v1;
    u32 v0;

    a1 = *(s32 *)((u8 *)(*((u8 **)&((UnkStruct_0208C06C *)arg0)->unk_818) + (0x24 * ((UnkStruct_0208C06C *)arg0)->unk_534.unk_1B0)) + 0x18);
    v0 = *(u32 *)arg0;
    v1 = 1;
    switch (v0) {
    case 3:
    case 4:
        break;
    default:
    case 0:
    case 1:
        if (((u32)a1 <= 3U) && (*(s32 *)((u8 *)(arg0 + (a1 * 4)) + 0x87C) == 0)) {
            ov40_0222DD9C(arg0, 0x10D);
            v1 = 0;
        }
        break;
    case 5:
        if (a1 == 0) {
            sub_0202B9B8(Save_FashionData_Get(((SaveData *)((UnkStruct_0208C06C *)arg0)->saveData)), a1);
            v1 = sub_0202BC10();
            if (v1 == 0) {
                ov40_0222DD9C(arg0, 0x122);
            }
        }
        break;
    case 6:
        if ((a1 == 0x64) && (ov40_0222DD94() != 0)) {
            ov40_0222DD9C(arg0, 0x111);
            v1 = 0;
        }
        break;
    case 2:
        if ((a1 == 0x270F) && (*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_87C[0]) == 0)) {
            ov40_0222DD9C(arg0, 0x81);
            v1 = 0;
        }
        break;
    }
    if (v1 == 0) {
        PlaySE(0x57CU);
    } else {
        PlaySE(0x57BU);
    }
    return v1;
}
