// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_light.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_LIGHT_H
#define BWPORT_APP_MUSICAL_STA_ACT_LIGHT_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of type, pos and actor in Black
struct StaActLight {
    // The light's kind, 0 for none
    u32 type;
    GXRgb color;
    u8 alpha;
    VecFx32 pos;
    u32 unk14;
    u32 unk18;
    ClActor *actor;
};

// Black 2's; twinned code confirms the offsets of heapId, stage, lights, followActive, followActor, clactUnit, palette,
// chars and cellAnims in Black
struct StaActLightSys {
    HeapID heapId;
    // NULL when the lights don't follow a stage, as in the photo
    StaActing *stage;
    StaActLight lights[4];
    // Whether the light that follows the Pokémon in the limelight is shown
    BOOL followActive;
    u8 unk8C[0x18];
    ClActor *followActor;
    ClActUnit *clactUnit;
    u32 palette;
    u32 chars;
    u32 cellAnims;
};

// pokebw2's StaActLight_TermSystem
void func_ov112_021f6c7c(StaActLightSys *sys);
// pokebw2's StaActLight_UpdateSystem
void func_ov112_021f6cb0(StaActLightSys *sys);
// pokebw2's StaActLight_AddLight
StaActLight *func_ov112_021f6e78(StaActLightSys *sys, u32 type);
// pokebw2's StaActLight_DelLight
void func_ov112_021f6ee4(StaActLightSys *sys, StaActLight *light);
// pokebw2's StaActLight_SetPosition
void func_ov112_021f6ef4(StaActLightSys *sys, StaActLight *light, VecFx32 *pos);
// pokebw2's StaActLight_GetPosition
void func_ov112_021f6f04(StaActLightSys *sys, StaActLight *light, VecFx32 *pos);
// pokebw2's StaActLight_SetColor
void func_ov112_021f6f14(StaActLightSys *sys, StaActLight *light, GXRgb color, u8 alpha);
void func_ov112_021f6f1c(StaActLightSys *sys, StaActLight *light, u32 a2, u32 a3);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActLight_InitSystem
StaActLightSys *func_ov112_021f6bd8(HeapID heapId, StaActing *stage);
// pokebw2's StaActLight_DrawSystem
void func_ov112_021f6e74(StaActLightSys *sys);

#endif // BWPORT_APP_MUSICAL_STA_ACT_LIGHT_H
