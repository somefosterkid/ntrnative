#include "global.h"

struct FashionCase;

typedef struct FashionCase FashionCase;

u32 sub_0202BA70(const FashionCase *fashionCase, u32 accessoryID);

BOOL sub_0202B8FC(u32 accessoryID);

BOOL sub_0202BA2C(const FashionCase *fashionCase, u32 accessoryID, u32 count);

BOOL sub_0202BA2C(const FashionCase *fashionCase, u32 accessoryID, u32 count) {
    u32 currentCount;
    BOOL canFit = 1;

    currentCount = sub_0202BA70(fashionCase, accessoryID);

    if (sub_0202B8FC(accessoryID)) {
        currentCount += count;

        if (currentCount > 9) {
            canFit = 0;
        }
    } else {
        currentCount += count;

        if (currentCount > 1) {
            canFit = 0;
        }
    }

    return canFit;
}
