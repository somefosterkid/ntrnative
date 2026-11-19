#include "global.h"

#include "unk_0202C034.h"

void sub_0202C554(WiFiList *wiFiList, s32 param1, s32 param2);

void sub_0202C554(WiFiList *wiFiList, s32 param1, s32 param2) {
    s32 temp_r3;
    u16 var_r5;
    void *var_r4;

    if ((param1 >= 0) && (param1 < 0x20)) {
        var_r4 = (u8 *)wiFiList + 0x1F2;
        temp_r3 = 0x38 * param1;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        *(u16 *)(var_r4 + temp_r3) = var_r5 + param2;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        if ((u32)var_r5 > 0x270FU) {
            *(u16 *)(var_r4 + temp_r3) = 0x270F;
        }
        sub_0202C4B0(wiFiList, param1);
    }
}

void sub_0202C584(WiFiList *wiFiList, s32 param1, s32 param2);

void sub_0202C584(WiFiList *wiFiList, s32 param1, s32 param2) {
    s32 temp_r3;
    u16 var_r5;
    void *var_r4;

    if ((param1 >= 0) && (param1 < 0x20)) {
        var_r4 = (u8 *)wiFiList + 0x1F4;
        temp_r3 = 0x38 * param1;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        *(u16 *)(var_r4 + temp_r3) = var_r5 + param2;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        if ((u32)var_r5 > 0x270FU) {
            *(u16 *)(var_r4 + temp_r3) = 0x270F;
        }
        sub_0202C4B0(wiFiList, param1);
    }
}

void sub_0202C5B4(WiFiList *wiFiList, s32 param1, s32 param2);

void sub_0202C5B4(WiFiList *wiFiList, s32 param1, s32 param2) {
    s32 temp_r3;
    u16 var_r5;
    void *var_r4;

    if ((param1 >= 0) && (param1 < 0x20)) {
        var_r4 = (u8 *)wiFiList + 0x1F6;
        temp_r3 = 0x38 * param1;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        *(u16 *)(var_r4 + temp_r3) = var_r5 + param2;
        var_r5 = *(u16 *)(var_r4 + temp_r3);
        if ((u32)var_r5 > 0x270FU) {
            *(u16 *)(var_r4 + temp_r3) = 0x270F;
        }
        sub_0202C4B0(wiFiList, param1);
    }
}
