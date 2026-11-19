#include "global.h"

#include "save_local_field_data.h"

s32 ov112_021F328C(SaveData *, void *);

void ov112_021ED530(void *arg0);

void ov112_021ED530(void *arg0) {
    int i;
    u8 *base = arg0;
    u8 *p;
    if (LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(*(SaveData **)(base + 0x20)))->mapId != 0x1F3) {
        p = base + 9 * 0x224;
        for (i = 9; i >= 0; i--) {
            u16 v = *(u16 *)(p + 0xB3AC);
            if (v == 7 || v == 8) {
                ov112_021F328C(*(SaveData **)(base + 0x20), base + 0xB1B4 + (u32)(i + 1) * 0x224);
            }
            p -= 0x224;
        }
    }
}
