#include "global.h"

#include "font.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021F135C(s32 param_1, u32 param_2, u32 param_3, u8 param_4, enum Font param_5);
#else
void ov112_021F135C(s32 param_1, u32 param_2, u32 param_3, u32 param_4, enum Font param_5);
#endif

void ov112_021F13BC(s32 param_1, u32 param_2, u32 param_3, u32 param_4);

void ov112_021F13BC(s32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    ov112_021F135C(param_1, param_2, param_3, param_4, FONT_MESSAGE);
    return;
}
