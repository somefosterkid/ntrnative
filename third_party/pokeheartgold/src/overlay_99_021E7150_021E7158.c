#include "global.h"

typedef struct UnkStruct_ov99_021E7158 {
    u32 a : 5;
    u32 b : 9;
    u32 c : 5;
    u32 rest : 13;
} UnkStruct_ov99_021E7158;

u8 ov99_021E7158(void *arg0);

u8 ov99_021E7158(void *arg0) {
    UnkStruct_ov99_021E7158 *bits = (UnkStruct_ov99_021E7158 *)((u8 *)arg0 + 0x3F4);
    u8 rem;
    if (bits->c == bits->a) {
        rem = bits->b % 30U;
        if (rem != 0) {
            return rem;
        }
    }
    return 0x1E;
}
