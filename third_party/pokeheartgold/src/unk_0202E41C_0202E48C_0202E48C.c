#include "global.h"

#include "save.h"

void sub_0202E48C(void *arg0, s8 arg1);

void sub_0202E48C(void *arg0, s8 arg1) {
    *(s8 *)((u8 *)arg0 + 8) = arg1;
    SaveSubstruct_UpdateCRC(0x17);
}
