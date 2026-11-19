#include "global.h"
#include "constants/heap.h"
#include "unk_02095EAC.h"

struct Window;

void CopyWindowToVram(Window *window);

extern void ov37_021E713C(Window *param0, const u8 *param1, int param2, int param3, int *param4, int *param5, int param6, int param7);

extern void ov37_021E72B4(UnkStruct_ov58_021D2820 *param0, UnkStruct_ov58_021D2754 *param1);

extern const u8 ov37_021E7AC8[3][8][24];

void ov37_021E72E8(Window *param0, UnkStruct_ov58_021D2820 *param1, UnkStruct_ov58_021D2754 *param2, int param3);

void ov37_021E72E8(Window *param0, UnkStruct_ov58_021D2820 *param1, UnkStruct_ov58_021D2754 *param2, int param3) {
    int v0, v1, v2, v3, v4 = 0, v5, v6;

    for (v2 = 0; v2 < 5; v2++) {
        if (param1[v2].unk_08_3 != 0) {
            if (param2[v2].unk_02) {
                v5 = param2[v2].unk_00 - 9;
                v6 = param2[v2].unk_01 - 17;
            }

            for (v3 = 0; v3 < param1[v2].unk_08_3; v3++) {
                v0 = param1[v2].unk_00[v3] - 9;
                v1 = param1[v2].unk_04[v3] - 17;

                ov37_021E713C(param0, ov37_021E7AC8[param1[v2].unk_08_6][param1[v2].unk_08_0], v0, v1, &v5, &v6, v3, param2[v2].unk_02);

                v4 = 1;
            }
        }
    }

    if (v4 && param3) {
        CopyWindowToVram(param0);
    }

    ov37_021E72B4(param1, param2);

    for (v2 = 0; v2 < 5; v2++) {
        param1[v2].unk_08_3 = 0;
    }
}
