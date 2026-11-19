#include "global.h"

#include "unk_02031B0C.h"

void sub_02031B3C(SaveApricornBox *);

void InitApricornBox(SaveApricornBox *arg0);

void InitApricornBox(SaveApricornBox *dest) {
    MI_CpuFill8(dest, 0U, sizeof(SaveApricornBox));
    sub_02031B5C(&dest->juice);
    sub_02031B3C(dest);
}
