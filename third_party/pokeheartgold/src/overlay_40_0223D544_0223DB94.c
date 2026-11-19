#include "global.h"

#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0223DB94(void *arg0) {
    void *v0;

    v0 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    if (*(u16 *)((u8 *)v0 + 0x4C0) != 0xFFFF) {
        return 1;
    }
    if (*(u8 *)((u8 *)v0 + 0x4C2) != 0xFF) {
        return 1;
    }
    if ((*(u8 *)((u8 *)v0 + 0x4C3) != 0xFF) || (*(u8 *)((u8 *)v0 + 0x4C4) != 0xFF)) {
        return 1;
    }
    return 0;
}
