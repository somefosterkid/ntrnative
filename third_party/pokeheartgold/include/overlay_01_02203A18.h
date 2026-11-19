#ifndef POKEHEARTGOLD_OVERLAY_01_02203A18_H
#define POKEHEARTGOLD_OVERLAY_01_02203A18_H

#include "global.h"
#include "billboard_gfx_sequence.h"
#include "overlay_01_021F1348.h"
#include "field_types_def.h"
#include "sprite.h"

// The context of the field effect renderer 12 (sizeof == 8 at 32 bits): the manager first.
typedef struct UnkOv01_02203A18 {
    FieldEffectManager *fieldEffMan;
    int busy; // 1 from ov01_02203A48 until the effect's task ends
} UnkOv01_02203A18;

// The task environment of ov01_02203AB4 / ov01_02203AD8 (sizeof == 8 at 32 bits): the argument of the effect, then the partner mon's object.
typedef struct UnkOv01_02203AB4Env {
    s32 arg;
    LocalMapObject *partnerPokeObj;
} UnkOv01_02203AB4Env;

// The user data of the funcs table ov01_02209518 (ov01_02203A48); the init copies it to UnkOv01_02203A18_Work.unk30.
typedef struct UnkOv01_02203A18_Data {
    FieldEffectManager *fieldEffMan;
    UnkOv01_02203A18 *unk4;
    LocalMapObject *unk8;
} UnkOv01_02203A18_Data;

// The billboard animation state ov01_02203DF8 steps (sizeof == 0x28 on the DS).
typedef struct UnkOv01_02203A18_Anim {
    BillboardGfxSequence sequence;
    void *unk10;
    void *unk14;
    void *unk18;
    u32 unk1C;
    u32 unk20;
    u16 unk24;
    u16 unk26;
} UnkOv01_02203A18_Anim;

// The context of the funcs table ov01_02209518 (sizeof == 0x68 on the DS).
typedef struct UnkOv01_02203A18_Work {
    int unk0;
    int unk4;
    u32 unk8;
    u32 unkC;
    int unk10;
    u32 unk14;
    fx32 unk18;
    fx32 unk1C;
    fx32 unk20;
    u32 unk24;
    fx32 unk28;
    u32 unk2C;
    UnkOv01_02203A18_Data unk30;
    Sprite *unk3C;
    UnkOv01_02203A18_Anim anim;
} UnkOv01_02203A18_Work;

void *ov01_02203A18(FieldEffectManager *fieldEffMan);

void ov01_02203A38(void *a0);

void ov01_02203AB4(FieldSystem *fieldSystem, LocalMapObject *partnerPokeObj, s32 a2);

void ov01_02203B28(UnkOv01_02203A18 *ctx);
void ov01_02203B70(UnkOv01_02203A18 *ctx);

Sprite *ov01_02203B98(void *arg0, void *arg1);

#endif // POKEHEARTGOLD_OVERLAY_01_02203A18_H
