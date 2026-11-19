#include "global.h"

s32 sub_02062FAC(void *, s32, s16, u16);

s32 MapObjectMovementCmd076_Step0(void *arg0);

s32 MapObjectMovementCmd099_Step0(void *arg0);

s32 MapObjectMovementCmd076_Step0(void *arg0) {
    sub_02062FAC(arg0, 0, 6, 6);
    return 1;
}

s32 MapObjectMovementCmd077_Step0(void *arg0);

s32 MapObjectMovementCmd077_Step0(void *arg0) {
    sub_02062FAC(arg0, 1, 6, 6);
    return 1;
}

s32 MapObjectMovementCmd078_Step0(void *arg0);

s32 MapObjectMovementCmd078_Step0(void *arg0) {
    sub_02062FAC(arg0, 2, 6, 6);
    return 1;
}

s32 MapObjectMovementCmd079_Step0(void *arg0);

s32 MapObjectMovementCmd079_Step0(void *arg0) {
    sub_02062FAC(arg0, 3, 6, 6);
    return 1;
}

s32 sub_02062FEC(void *, s32 *);

extern s32 _020FDA50;

s32 MapObjectMovementCmd076_Step1(void *arg0);

s32 MapObjectMovementCmd076_Step1(void *arg0) {
    if (sub_02062FEC(arg0, &_020FDA50) == 1) {
        return 1;
    }
    return 0;
}

s32 MapObjectMovementCmd080_Step0(void *arg0);

s32 MapObjectMovementCmd080_Step0(void *arg0) {
    sub_02062FAC(arg0, 0, 3, 7);
    return 1;
}

s32 MapObjectMovementCmd081_Step0(void *arg0);

s32 MapObjectMovementCmd081_Step0(void *arg0) {
    sub_02062FAC(arg0, 1, 3, 7);
    return 1;
}

s32 MapObjectMovementCmd082_Step0(void *arg0);

s32 MapObjectMovementCmd082_Step0(void *arg0) {
    sub_02062FAC(arg0, 2, 3, 7);
    return 1;
}

s32 MapObjectMovementCmd083_Step0(void *arg0);

s32 MapObjectMovementCmd083_Step0(void *arg0) {
    sub_02062FAC(arg0, 3, 3, 7);
    return 1;
}

extern s32 _020FDA1C;

s32 MapObjectMovementCmd082_Step1(void *arg0);

s32 MapObjectMovementCmd082_Step1(void *arg0) {
    if (sub_02062FEC(arg0, &_020FDA1C) == 1) {
        return 1;
    }
    return 0;
}

s32 MapObjectMovementCmd096_Step0(void *arg0);

s32 MapObjectMovementCmd096_Step0(void *arg0) {
    sub_02062FAC(arg0, 0, 7, 8);
    return 1;
}

s32 MapObjectMovementCmd097_Step0(void *arg0);

s32 MapObjectMovementCmd097_Step0(void *arg0) {
    sub_02062FAC(arg0, 1, 7, 8);
    return 1;
}

s32 MapObjectMovementCmd098_Step0(void *arg0);

s32 MapObjectMovementCmd098_Step0(void *arg0) {
    sub_02062FAC(arg0, 2, 7, 8);
    return 1;
}

s32 MapObjectMovementCmd099_Step0(void *arg0) {
    sub_02062FAC(arg0, 3, 7, 8);
    return 1;
}
