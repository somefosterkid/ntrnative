#include "global.h"

#include "pokepic.h"

void ov41_022465CC(void *arg0);

void ov41_022465CC(void *arg0) {
    PokepicManager_HandleLoadImgAndOrPltt(*(PokepicManager **)((u8 *)arg0 + 0x20));
}
