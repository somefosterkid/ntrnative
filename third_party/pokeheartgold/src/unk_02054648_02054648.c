#include "global.h"

fx32 sub_02054648(const fx32 lhs, const fx32 rhs);

fx32 sub_02054648(const fx32 lhs, const fx32 rhs) {
    return lhs >= rhs
        ? lhs - rhs
        : rhs - lhs;
}
