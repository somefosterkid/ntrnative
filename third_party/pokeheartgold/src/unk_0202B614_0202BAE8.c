#include "global.h"

struct FashionCase;
typedef struct FashionCase FashionCase;
u32 sub_0202BAB0(const FashionCase *fashionCase, u32 backdropID);
enum Backdrop {
    BACKDROP_DRESS_UP = 0,
    BACKDROP_RANCH = 1,
    BACKDROP_CITY_AT_NIGHT = 2,
    BACKDROP_SNOWY_TOWN = 3,
    BACKDROP_FIERY = 4,
    BACKDROP_OUTER_SPACE = 5,
    BACKDROP_CUMULUS_CLOUD = 6,
    BACKDROP_DESERT = 7,
    BACKDROP_FLOWER_PATCH = 8,
    BACKDROP_FUTURE_ROOM = 9,
    BACKDROP_OPEN_SEA = 10,
    BACKDROP_TOTAL_DARKNESS = 11,
    BACKDROP_TATAMI_ROOM = 12,
    BACKDROP_GINGERBREAD_ROOM = 13,
    BACKDROP_SEAFLOOR = 14,
    BACKDROP_UNDERGROUND = 15,
    BACKDROP_SKY = 16,
    BACKDROP_THEATER = 17,
    BACKDROP_COUNT = 18,
};

u32 FashionCase_CountWallpapers(const FashionCase *fashionCase);

u32 FashionCase_CountWallpapers(const FashionCase *fashionCase) {
    int i;
    int count = 0;

    for (i = 0; i < BACKDROP_COUNT; i++) {
        if (sub_0202BAB0(fashionCase, i) != BACKDROP_COUNT) {
            count++;
        }
    }

    return count;
}
