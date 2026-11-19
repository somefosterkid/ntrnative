#include "global.h"

typedef struct Words2 {
    u32 w[2];
} Words2;

void ov45_02230378(void *arg0, void *arg1);

void ov45_02230378(void *arg0, void *arg1) {
    *(Words2 *)((u8 *)arg0 + 0x10) = *(Words2 *)((u8 *)arg1);
}
