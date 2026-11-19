#include "global.h"

#include "error_handling.h"
#include "unk_02031B0C.h"

u8 ApricornBox_TakeApricorn();

void ApricornBox_SetKurtApricorn(SaveApricornBox *apricornBox, u32 a0, u32 a1) {
    if ((u32) * (u8 *)((u8 *)apricornBox + a0) < a1) {
        GF_AssertFail();
        return;
    }
    *(s8 *)((u8 *)apricornBox + 8) = (s8)a0;
    *(s8 *)((u8 *)apricornBox + 7) = (s8)a1;
    ApricornBox_TakeApricorn();
}
