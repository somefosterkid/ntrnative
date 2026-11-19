#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "gf_3d_loader.h"
#include "sys_task.h"
#include "error_handling.h"
#include "overlay_01_021FC4C4.h"

struct MapObject;

struct ResourceHeap;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

struct UnkStruct_ov5_021ED110;

typedef struct MapObject MapObject;


void ov01_021FC588(ResourceHeap *resHeap, u32 id);

typedef struct TextureResourceManager TextureResourceManager;







int ov01_021FA01C(const MapObjectManager *param0, int param1, const MapObject *param2);





void ov01_021F9BD4(MapObjectManager *param0, UnkStruct_ov5_021ED0A4 *param1) {
    MapObjectManager *new_var;
    int v0;
    unsigned char new_var2;
    GF_2DGfxRawResMan *v2 = ov01_021FA1DC(param1);
    int *v1 = ov01_021FA200(param1);
    v1 = &v1[ov01_021FA20C(param1)];
    v0 = ov01_021FA214(param1);
    new_var2 = 0xffff;
    do {
        if ((*v1) != new_var2) {
            if (ov01_021FA01C(new_var = param0, *v1, 0L) == 0) {
                ov01_021FC588(param1->unk_F8, *v1);
                *v1 = new_var2;
            }
        }
        v1++;
        v0--;
    } while (v0);
}

void ov01_021F9C24(UnkStruct_ov5_021ED0A4 *gfx) {
    ov01_021F9AAC(ov01_021FA204(gfx), 0xFF, 8);
}

void ov01_021F9C34(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;

    v0 = ov01_021FA21C(gfx);
    if (ov01_021F9AB4(ov01_021FA204(gfx), id, 0xFF, v0) == 0) {
        GF_AssertFail();
    }
}

int ov01_021F9C5C(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;

    v0 = ov01_021FA21C(gfx);
    return ov01_021F9AD0(ov01_021FA204(gfx), id, v0);
}

void ov01_021F9C78(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA204(gfx);
    v0 = ov01_021FA21C(gfx) * 4;
    if (ov01_021F9AB4((int *)(v1 + v0), id, 0xFF, ov01_021FA224(gfx)) == 0) {
        GF_AssertFail();
    }
}

int ov01_021F9CA8(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA204(gfx);
    v0 = ov01_021FA21C(gfx) * 4;
    return ov01_021F9AD0((int *)(v1 + v0), id, ov01_021FA224(gfx));
}

void ov01_021F9CD0(UnkStruct_ov5_021ED0A4 *gfx, int id) {
    int v0;
    u8 *v1;

    v1 = (u8 *)ov01_021FA204(gfx);
    v0 = ov01_021FA21C(gfx) * 4;
    ov01_021F9AE4((int *)(v1 + v0), id, 0xFF, ov01_021FA224(gfx));
}
