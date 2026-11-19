#include "global.h"

#include "pokemon_anim.h"

static int sub_02017208(u32 *scriptPtr, u8 one);

// The script word at scriptPtr: sub_020171F4 returns it in r0 and these wrappers pass it on.
static int sub_02017208(u32 *scriptPtr, u8 one) {
    return sub_020171F4(scriptPtr, 0, one);
}

int sub_02017214(u32 *scriptPtr) {
    return sub_02017208(scriptPtr, 1);
}
