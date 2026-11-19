#include "global.h"

#include "unk_02031B0C.h"

extern u32 _020F68D0;

int ApricornBox_GetKurtBall(SaveApricornBox *apricornBox) {
    if ((7) <= ((u8)((u8 *)apricornBox)[8])) {
        return 4;
    }
    return (u32)(*((u16 *)(((u8 *)&_020F68D0) + (((u32)((u8)((u8 *)apricornBox)[8])) * 2))));
}
