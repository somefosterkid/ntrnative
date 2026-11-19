#include "global.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022001E4.h"
#include "unk_020689C8.h"
#include "map_object.h"
#include "overworld_anim_manager.h"

void sub_02023DA4(void *);
void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_02200508(OverworldAnimManager *param0, void *param1);

extern const OverworldAnimManagerFuncs ov01_022092DC;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_022001E4_02200228_0220035C_0220038C_022003C4_022092C8.s (same label), with the context size from the type, not the DS literal.
int ov01_0220059C(OverworldAnimManager *param0, void *param1);
void ov01_022004EC(OverworldAnimManager *animMan, NpcEmoteAnimCtx *ctx);
void ov01_02200614(void *param0, NpcEmoteAnimCtx *work);
void ov01_02200508(OverworldAnimManager *param0, void *param1);
const OverworldAnimManagerFuncs ov01_022092DC = OVERWORLD_ANIM_MANAGER_FUNCS(NpcEmoteAnimCtx, ov01_0220059C, ov01_022004EC, ov01_02200614, ov01_02200508);
#endif

void ov01_022004EC(OverworldAnimManager *animMan, NpcEmoteAnimCtx *ctx) {
    sub_02023DA4(ctx->billboard);
    ov01_0220024C(ctx->args.rendererCtx, ctx->args.emoteKind);
    ov01_022003B0(ctx->args.rendererCtx, ctx->args.emoteKind);
    return;
}

void ov01_02200508(OverworldAnimManager *param0, void *param1) {
    VecFx32 v0;
    NpcEmoteAnimCtx *v1 = param1;

    if (v1->unk_10 == 1) {
        return;
    }

    sub_02068DB8(param0, &v0);

    v0.x += v1->unk_18.x;
    v0.y += v1->unk_18.y;
    v0.z += v1->unk_18.z;

    sub_02023E50(v1->billboard, &v0);
}

OverworldAnimManager *ov01_02200540(LocalMapObject *obj, int a0, int a1) {
    NpcEmoteAnimArgs args;
    VecFx32 v0;
    VecFx32 v1;
    FieldEffectManager *fieldEffMan;
    int new_var;
    FieldEffectManager *v2;
    fieldEffMan = ov01_021F146C(obj);
    v2 = fieldEffMan;
    args.emoteKind = a0;
    args.fieldEffMan = v2;
    args.rendererCtx = ov01_021F1450(fieldEffMan, 9);
    args.mapObject = obj;
    MapObject_CopyPositionVector(obj, &v0);
    MapObject_CopyFacingVector(obj, &v1);
    VEC_Add(&v0, &v1, &v0);
    new_var = MapObject_GetPriority(obj) + 1;
    return ov01_021F1620(v2, &ov01_022092DC, &v0, a1, &args, new_var);
}
