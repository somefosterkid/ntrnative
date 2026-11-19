#include "global.h"

typedef struct {
    void *unk_00;
    void *unk_04;
    void *unk_08;
    void *unk_0C;
    u32 unk_10;
    u32 unk_14;
    vu8 unk_18;
    vu8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
} UnkStruct_ov97_02232AC8;

extern u8 ov74_02231064(void);

extern u8 ov74_02231094(void);

UnkStruct_ov97_02232AC8 *ov74_02231184(void);

BOOL ov74_02230F40(void);

BOOL ov74_02230F40(void) {
    u32 v0 = 0;

    switch (ov74_02231094()) {
    case 1:
    case 2: {
        switch (ov74_02231064()) {
        case 11:
        case 10: {
            UnkStruct_ov97_02232AC8 *v1 = ov74_02231184();

            if (v1->unk_19 == 2) {
                return 1;
            }
        } break;
        }
    } break;
    }

    return 0;
}

BOOL ov74_02230F6C(void);

BOOL ov74_02230F6C(void) {
    u32 v0 = 0;

    switch (ov74_02231094()) {
    case 1:
    case 2: {
        switch (ov74_02231064()) {
        case 11:
        case 10: {
            UnkStruct_ov97_02232AC8 *v1 = ov74_02231184();

            if (v1->unk_19 == 2) {
                return 1;
            }
        } break;
        }
    } break;
    }

    return 0;
}
