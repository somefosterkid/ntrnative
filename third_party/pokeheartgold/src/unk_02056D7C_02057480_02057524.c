#include "global.h"

#include "error_handling.h"

int sub_02057524(int param0);

int sub_02057524(int param0) {
    int v0[5] = { 2, 4, 8, 16, 2 };

    GF_ASSERT(param0 < 5);
    return v0[param0];
}
