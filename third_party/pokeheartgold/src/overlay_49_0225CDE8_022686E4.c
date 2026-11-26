#include "global.h"

#include "overlay_49.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_02268640(void *);
#else
s32 ov49_02268640(void *);
#endif

void ov49_022686E4(u8 *arg0) {
    ov49_02268640(arg0 + 4);
}
