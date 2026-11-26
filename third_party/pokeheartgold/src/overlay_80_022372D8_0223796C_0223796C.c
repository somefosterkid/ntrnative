#include "global.h"

u16 ov80_022379C0(u8 typeRank);

extern s32 ov80_0223C5B8;

u8 ov80_0223796C(u8 typeRank);

u8 ov80_0223796C(u8 typeRank) {
    return *(u8 *)(&ov80_0223C5B8 + ov80_022379C0(typeRank));
}
