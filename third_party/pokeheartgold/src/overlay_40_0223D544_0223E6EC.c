#include "global.h"

#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DF60(void *, s32);

s32 ov40_0223DD68(void *, int);

s32 ov40_022420B4(void *, s32);

s32 ov40_0223E6EC(UnkStruct_0208C06C *arg0) {
    void *v0;

    v0 = arg0->unk_860;
    if (arg0->unk_08 == 0) {
        ov40_0222DF60(arg0, 0x72);
        *(u16 *)((u8 *)v0 + 0x4C0) = 0xFFFFU;
        ov40_0223DD68(arg0, *(u16 *)((u8 *)v0 + 0x4C0));
        ov40_022420B4(arg0, 1);
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
    }
    ov40_0222BF80(arg0, 8);
    return 0;
}
