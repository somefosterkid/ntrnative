#include "global.h"

struct BallSeal;

typedef struct BallSeal BallSeal;

int ov07_02234510(BallSeal *param0);

int CapsuleMenu_IsAlphabetSeal(u32 index);

u32 SealOnCapsuleGetID(const BallSeal *seal);

int ov07_02234530(BallSeal *param0);

int ov07_02234530(BallSeal *param0) {
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    const u8 v5[] = {
        0,
        8,
        14,
        20,
        20,
        16,
        16,
    };

    v3 = SealOnCapsuleGetID(param0);
    v0 = CapsuleMenu_IsAlphabetSeal((u8)v3);

    if (v0 == 0) {
        v2 = ov07_02234510(param0);

        if (v2 >= (60 - 4)) {
            v4 = 3;
        } else {
            v4 = ((v2 + 1) / 20);

            if (v4 >= (60 / 20)) {
                v4 = 3;
            }
        }

        v1 = v5[v4];
    } else {
        v1 = v5[1];
    }

    return v1;
}
