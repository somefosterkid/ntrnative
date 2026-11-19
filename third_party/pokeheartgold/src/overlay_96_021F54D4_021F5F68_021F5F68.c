#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

#include "error_handling.h"

s32 ov96_021E6104(void);

s32 ov96_021E872C(s32, s32, s32, s32, s32, s32 *);

s32 ov96_021EB0A4(void *, s32, s32, s32 *, s32 *);

u8 ov96_021F5F68(UnkStruct_ov96_021F5018 *arg0, s16 arg1, s16 arg2);

u8 ov96_021F5F68(UnkStruct_ov96_021F5018 *arg0, s16 arg1, s16 arg2) {
    s32 dist[3];
    u8 valid[3];
    s32 out;
    s32 x;
    s32 y;
    UnkStruct_ov96_021F5018_Ball *obj;
    u8 i;
    s32 d;
    u8 count;
    s32 minDist;
    u8 result;

    minDist = ov96_021E6104() << 12;
    count = 0;
    i = 0;
    do {
        obj = &arg0->unk_90[i];
        if (obj->unk_26 != 0) {
            valid[i] = 0;
            dist[i] = 0;
        } else {
            ov96_021EB0A4(obj->unk_00, obj->unk_1C / 0x1000, (0x60000 - (arg0->unk_FAC[i].unk_08 << 6)) / 0x1000, &x, &y);
            if (ov96_021E872C(x, y, arg1, arg2, ov96_021E6104(), &out) != 0) {
                dist[i] = out;
                valid[i] = 1;
                count++;
            } else {
                dist[i] = 0;
                valid[i] = 0;
            }
        }
        i++;
    } while (i < 3);
    if (count == 0) {
        return 3;
    }
    result = 3;
    i = 0;
    do {
        if (valid[i] != 0) {
            d = dist[i];
            if (d < minDist) {
                result = i;
                minDist = d;
            }
        }
        i++;
    } while (i < 3);
    if (result == 3) {
        GF_AssertFail();
        result = 3;
    }
    return result;
}
