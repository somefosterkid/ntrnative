#include "global.h"

#include "overlay_29.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225E0E0(NumberEntryData *arg0) {
    s32 v1;
    s32 v0;
    u32 v2;
    unsigned int new_var;
    u32 *v3;
    v0 = arg0->numDigits;
    v1 = 0;
    v2 = 0;
    new_var = 0U;
    if (v0 > new_var) {
        v3 = (u32 *)arg0;
        do {
            v2 += 1;
            v1 = v1 * 0xA;
            v1 += *(s32 *)((u8 *)v3 + offsetof(NumberEntryData, digits));
            v3 += 1;
        } while (v2 < v0);
    }
    *((s32 *)&arg0->value) = v1;
}
