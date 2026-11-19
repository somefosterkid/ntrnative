#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_022001E4.h"
#include "sys_task.h"
#include "error_handling.h"

void *ov01_022001E4(FieldEffectManager *fieldEffMan) {
    UnkOv01_022001E4 *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkOv01_022001E4), 0, 0);
    ctx->fieldEffMan = fieldEffMan;
    return ctx;
}

void ov01_022001F8(void *a0) {
    ov01_022002FC(a0);
    ov01_021F1448(a0);
}

void ov01_02200208(UnkOv01_022001E4 *ctx) {
    ctx->unk_00++;
}

void ov01_02200210(UnkOv01_022001E4 *ctx) {
    ctx->unk_00--;

    ((ctx->unk_00 >= 0) ? (void)0 : GF_AssertFail());
}

void ov01_02200220(UnkOv01_022001E4 *ctx) {
    ctx->unk_04++;
}

void ov01_02200228(UnkOv01_022001E4 *ctx) {
    s32 v0;

    v0 = ctx->unk_04 - 1;
    ctx->unk_04 = v0;
    if (v0 < 0) {
        GF_AssertFail();
    }
}
