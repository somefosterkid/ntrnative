#ifndef POKEHEARTGOLD_OVERLAY_01_021FDE64_H
#define POKEHEARTGOLD_OVERLAY_01_021FDE64_H

#include "global.h"
#include "billboard.h"
#include "overworld_anim_manager.h"

// The includer provides FieldSystem and FieldEffectManager (field_types_def.h / overlay_01_021F1348.h); the old split
// sources still dump their own copies of those declarations, which clash with the headers.

typedef struct UnkStruct_ov5_021F17E4 {
    FieldEffectManager *unk_00;
} UnkStruct_ov5_021F17E4;

// The user data ov01_021FDE64 hands to ov01_021F1620 for the ov01_02208F9C billboard anim manager (0x38 bytes at 32 bits).
typedef struct UnkStruct_ov5_021F1BEC {
    FieldSystem *fieldSystem;
    FieldEffectManager *unk_04;
    UnkStruct_ov5_021F17E4 *unk_08;
    BillboardResources unk_0C;
    const Billboard *unk_34;
} UnkStruct_ov5_021F1BEC;

// The context of the ov01_02208F9C billboard anim manager (0x5c bytes at 32 bits, so the DS table's 0x38 is only its
// memset size; the host table takes sizeof(UnkStruct_ov5_021F1AD8)). ov01_021FFD64's table shares it.
typedef struct UnkStruct_ov5_021F1AD8 {
    int unk_00;
    UnkStruct_ov5_021F1BEC unk_04;
    Billboard *unk_3C;
    VecFx32 unk_40;
    fx32 unk_4C;
    VecFx32 unk_50;
} UnkStruct_ov5_021F1AD8;

void ov01_021FDE08(UnkStruct_ov5_021F1AD8 *param0, VecFx32 *param1);
void ov01_021FDD94(OverworldAnimManager *param0, UnkStruct_ov5_021F1AD8 *param1);
void ov01_021FDF14(OverworldAnimManager *param0, UnkStruct_ov5_021F1AD8 *param1);
void ov01_021FDF20(OverworldAnimManager *param0, UnkStruct_ov5_021F1AD8 *param1);
void ov01_021FDF64(OverworldAnimManager *manager, UnkStruct_ov5_021F1AD8 *work);

#endif // POKEHEARTGOLD_OVERLAY_01_021FDE64_H
