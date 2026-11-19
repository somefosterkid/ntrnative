#include "global.h"
struct UnkStruct_ov5_021ED0A4_t;

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "gf_3d_loader.h"
#include "constants/player_avatar.h"
#include "field_types_def.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"


const BillboardAnim *ov01_021FA2AC(s32);

void *ov01_021FC5A4(struct ResourceHeap *, u32);

void ov01_021FA370(struct UnkStruct_ov5_021ED0A4_t *arg0, s32 arg1, BillboardResources *arg2);

struct MapObject;
typedef struct MapObject MapObject;
MapObject *MapObject_GetManager(PlayerAvatar *playerAvatar);
void *sub_0205F1A0(const MapObject *mapObj);
void *ov01_021FA3DC(PlayerAvatar *playerAvatar);

void ov01_021FA370(UnkStruct_ov5_021ED0A4_t *arg0, s32 id, BillboardResources *arg2) {
    GF_3DGfxRawResObj *obj;

    arg2->modelRes = ov01_021FC5A4(arg0->unk_F8, ov01_021FA28C(id));
    BillboardGfxSequence_SetData(ov01_021FC5A4(arg0->unk_FC, ov01_021FA2A0(id)), &arg2->gfxSequence);
    obj = GF3dGfxRawResMan_GetObjById(ov01_021FA1F4(arg0), id);
    arg2->texture = GF3dGfxRawResObj_GetTex(obj);
    arg2->texKey = GF3dGfxRawResObj_GetTexKey(obj);
    arg2->tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(obj);
    arg2->plttKey = GF3dGfxRawResObj_GetPlttKey(obj);
    arg2->anims = ov01_021FA2AC(id);
}

void *ov01_021FA3DC(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = MapObject_GetManager(playerAvatar);
    return sub_0205F1A0(mapObj);
}
