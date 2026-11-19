#include "global.h"

#include "overlay_40.h"
#include "touchscreen.h"
#include "unk_02026E30.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_02230944(void *);

s32 ov41_0224B554(void *);

extern TouchscreenHitbox ov40_022451C4;

extern TouchscreenHitbox ov40_022451D0;

extern TouchscreenHitbox ov40_022451D4;

extern TouchscreenHitbox ov40_022451D8;

s32 ov40_0223527C(UnkStruct_0208C06C *arg0) {
    s32 v1;
    void *v0;

    v0 = arg0->unk_860;
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C4) != 0) {
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 9);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D0) != 0) {
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 0xA);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D4) != 0) {
        v1 = *(s32 *)((u8 *)v0 + 0x234);
        if (v1 != 0) {
            *(s32 *)((u8 *)v0 + 0x234) = (s32)(v1 - 1);
        } else {
            v1 = *(s32 *)((u8 *)v0 + 0x2E4);
            *(s32 *)((u8 *)v0 + 0x234) = (s32)(v1 - 1);
        }
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 0xB);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D8) != 0) {
        *(s32 *)((u8 *)v0 + 0x234) = (s32)(*(s32 *)((u8 *)v0 + 0x234) + 1);
        *(s32 *)((u8 *)v0 + 0x234) = (s32)((s32) * (s32 *)((u8 *)v0 + 0x234) % (s32) * (s32 *)((u8 *)v0 + 0x2E4));
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 0xB);
    }
    if (*(s32 *)((u8 *)v0 + 0x228) != 0) {
        Thunk_G3X_Reset();
        ov41_0224B554(*(void **)((u8 *)v0 + 0x228));
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return 0;
}
