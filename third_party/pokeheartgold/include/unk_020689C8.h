#ifndef POKEHEARTGOLD_UNK_020689C8_H
#define POKEHEARTGOLD_UNK_020689C8_H

#include "field_types_def.h"
#include "overworld_anim_manager.h"
#include "simple3dmodel.h"

struct FieldEffectManager;
struct UnkOv01_021FFECC;

// The userData record of the ov01_021FFF5C anim managers (ov01_02209280's context embeds a copy of it); 0x14 bytes on the DS.
typedef struct {
    int unk0;
    struct FieldEffectManager *fieldEffMan;
    struct UnkOv01_021FFECC *unk8;
    LocalMapObject *unkC;
    NNSG3dRenderObj *unk10;
} UnkOv01_021FFFCC_common;

typedef struct {
    UnkOv01_021FFFCC_common unk0;
} UnkOv01_021FFFCC;

#endif // POKEHEARTGOLD_UNK_020689C8_H
