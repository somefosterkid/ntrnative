#include "global.h"

void ov07_022202D8(int mode, s8 sign, VecFx32 *vec);

void ov07_022202D8(int mode, s8 sign, VecFx32 *vec) {
    fx32 *px = &vec->x;
    fx32 *py = &vec->y;
    fx32 *pz = &vec->z;

    switch (mode) {
    case 0:
        break;
    case 1:
        (*px) *= sign;
        break;
    case 2:
        (*py) *= sign;
        break;
    case 3:
        (*pz) *= sign;
        break;
    case 4:
        (*px) *= sign;
        (*py) *= sign;
        break;
    case 5:
        (*px) *= sign;
        (*pz) *= sign;
        break;
    case 6:
        (*py) *= sign;
        (*pz) *= sign;
        break;
    case 7:
        (*px) *= sign;
        (*py) *= sign;
        (*pz) *= sign;
        break;
    }
}
