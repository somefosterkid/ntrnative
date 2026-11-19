#include "global.h"

#include "overlay_40.h"
#include "touchscreen.h"
#include "unk_02026E30.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_02230944(void *);

s32 ov41_0224B554(void *);

extern TouchscreenHitbox ov40_022451C8;

extern TouchscreenHitbox ov40_022451CC;

s32 ov40_02234470(UnkStruct_0208C06C *arg0) {
    void *v0;

    v0 = arg0->unk_860;
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C8) != 0) {
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 5);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451CC) != 0) {
        ov40_02230944(arg0);
        ov40_0222BF80(arg0, 4);
    }
    if (*(s32 *)((u8 *)v0 + 0x228) != 0) {
        Thunk_G3X_Reset();
        ov41_0224B554(*(void **)((u8 *)v0 + 0x228));
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return 0;
}
