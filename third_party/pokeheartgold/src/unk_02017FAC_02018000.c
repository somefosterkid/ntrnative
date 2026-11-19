#include "global.h"

#include "map_section.h"

BOOL LocationIsDiamondPearlCompatible(u16 param0) {
    if (((param0 >= 1) && (param0 <= 111)) || ((param0 >= 2000) && (param0 <= 2010)) || ((param0 >= 3000) && (param0 <= 3076))) {
        return 1;
    }

    return 0;
}
