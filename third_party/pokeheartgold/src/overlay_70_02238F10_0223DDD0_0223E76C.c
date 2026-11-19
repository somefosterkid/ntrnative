#include "global.h"

#include "item.h"
#include "pokemon.h"

s32 ov70_0223E76C(Pokemon *arg0);

s32 ov70_0223E76C(Pokemon *mon) {
    if (ItemIdIsMail((u16)GetMonData(mon, 6, NULL)) != 0) {
        return 1;
    }
    return 0;
}
