#include "global.h"

u8 ov74_02231064(void);

s32 ov74_02231118(void);

void ov74_02231124(s32 arg0);

void ov74_02231130(s32 arg0);

void *ov74_02231184(void);

void ov74_0223144C(u16 arg0);

s32 ov74_0222FEA0(u32);

s32 ov74_02230714(void *);

s32 ov74_02230818(void *);

void ov74_022308E0(WMPortRecvCallback *arg0);

void ov74_022308E0(WMPortRecvCallback *arg0) {
    u16 temp_r0;
    u8 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r0_4;

    ov74_0223144C(arg0->state);
    if (arg0->errcode == 0) {
        temp_r0 = arg0->state;
        switch (temp_r0) {
        case 21:
            temp_r0_2 = ov74_02231064();
            if (temp_r0_2 != 0xA) {
                if (temp_r0_2 == 0xB) {
                    goto block_6;
                }
            } else {
block_6:
                temp_r0_3 = *(u8 *)((u8 *)((void *)ov74_02231184()) + 0x18);
                switch (temp_r0_3) {
                case 1:
                    ov74_02230714(arg0);
                    return;
                case 2:
                    ov74_02230818(arg0);
                    return;
                }
            }
            break;
        case 7:
            ov74_02231124(1);
            ov74_02231130(0);
            return;
        case 9:
            if (ov74_02231118() != 0) {
                ov74_02231130(1);
            }
            temp_r0_4 = ov74_02231064();
            switch (temp_r0_4) {
            case 10:
                ov74_0222FEA0(arg0->aid);
                return;
            case 11:
                ov74_02231124(0);
                break;
            }
            break;
        }
    }
}
