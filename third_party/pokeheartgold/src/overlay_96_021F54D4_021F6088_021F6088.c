#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void sub_0200606C(u32, s32);

extern u8 ov96_0221DC18[];

void ov96_021F6088(UnkStruct_ov96_021F5018 *work, int idx, s32 *touch);

void ov96_021F6088(UnkStruct_ov96_021F5018 *work, int idx, s32 *touch) {
    s32 power;
    s32 speed;
    UnkStruct_ov96_021F5018_Row *ball;
    s32 *vel;
    int ofs;

    power = touch[1];
    if (power > 0) {
        ball = &work->unk_FAC[idx];
        if (ball->unk_14 == 0) {
            speed = (s32)((double)(power / 64) / 2.5);
            power = -touch[0] / 4;
            if (power >= 0) {
                if (power > 0xA000) {
                    power = 0xA000;
                }
            } else if (power < -0xA000) {
                power = -0xA000;
            }
#if defined(SDK_PORT) || defined(NONMATCHING)
            UnkStruct_ov96_021F5018_Ball *rec = &work->unk_90[idx];
            rec->unk_18 = power;
            rec->unk_18 = rec->unk_18 * rec->unk_34 / 12;
            ball->unk_00 = -speed;
            ball->unk_18 = -speed;
            ball->unk_14 = 1;
#else
            ofs = idx * 0x38;
            vel = (s32 *)((u8 *)work + 0xA8);
            *(s32 *)((u8 *)vel + ofs) = power;
            *(s32 *)((u8 *)vel + ofs) = *(s32 *)((u8 *)vel + ofs) * *(s32 *)((u8 *)work + ofs + 0xC4) / 12;
            ((s32 *)ball)[0] = -speed;
            ((s32 *)ball)[6] = -speed;
            ((s32 *)ball)[5] = 1;
#endif
            sub_0200606C(0x8C2, ov96_0221DC18[idx]);
        }
    }
}
