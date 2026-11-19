#include "global.h"

#include "unk_0205FD20.h"

extern s32 _020FD4DC;

u32 sub_020611F4(u32 direction) {
    return *(s32 *)(&_020FD4DC + direction);
}
