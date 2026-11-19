#include "global.h"
#include "gf_3d_loader.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "field_types_def.h"

struct MapObject;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

typedef struct MapObject MapObject;

typedef struct TextureResourceManager TextureResourceManager;

void GF3dGfxRawResMan_FreeObjById(GF_3DGfxRawResMan *texMgr, int id);



int ov01_021F9FCC(const MapObjectManager *param0, int param1, const MapObject *param2);




void ov01_021FA4F0(UnkStruct_ov5_021ED0A4 *param0, int param1);

void ov01_021FA6A4(UnkStruct_ov5_021ED0A4 *param0, int param1);


void ov01_021F9E30(MapObjectManager *param0, UnkStruct_ov5_021ED0A4 *param1) {
    int v0, v1;
    struct GF_3DGfxRawResMan *v3 = ov01_021FA1F4(param1);
    int *v2 = ov01_021FA1FC(param1);
    v2 = &v2[ov01_021FA22C(param1)];
    v0 = ov01_021FA234(param1);

    do {
        v1 = *v2;

        if (v1 != 0xffff) {
            if (ov01_021F9FCC(param0, v1, 0L) == 0) {
                GF3dGfxRawResMan_FreeObjById(v3, v1);
                ov01_021FA4F0(param1, v1);
                ov01_021FA6A4(param1, v1);

                *v2 = 0xffff;
            }
        }

        v2++;
        v0--;
    } while (v0);
}
