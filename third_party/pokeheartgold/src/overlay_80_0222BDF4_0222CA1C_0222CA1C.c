#include "global.h"

#include "frontier/overlay_80_0222BDF4.h"

#include "sprite_system.h"

ManagedSprite *ov80_02239700(s32, u32);

s32 ov80_02239708(s32, u32, s32);

s32 sub_0209680C(void *);

s32 FrtCmd_049(FrontierScriptContext *arg0);

s32 FrtCmd_049(FrontierScriptContext *ctx) {
    s32 v0;
    u32 v1;
    u32 seqno;

    v1 = FrontierScript_ReadVar(ctx);
    seqno = FrontierScript_ReadVar(ctx);
    v0 = sub_0209680C(ctx->frontierSystem->unk0);
    ManagedSprite_SetAnim(ov80_02239700(v0, v1), (s32)seqno);
    ov80_02239708(v0, v1, 1);
    return 0;
}

s32 FrtCmd_050(FrontierScriptContext *arg0);

s32 FrtCmd_050(FrontierScriptContext *ctx) {
    u32 v0;

    v0 = FrontierScript_ReadVar(ctx);
    ov80_02239708(sub_0209680C(ctx->frontierSystem->unk0), v0, 0);
    return 0;
}
