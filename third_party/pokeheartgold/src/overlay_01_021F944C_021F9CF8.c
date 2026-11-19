#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "gf_3d_loader.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"

struct MapObject;

struct ResourceHeap;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

struct UnkStruct_ov5_021ED110;

typedef struct MapObject MapObject;


void ov01_021FC588(ResourceHeap *resHeap, u32 id);

typedef struct TextureResourceManager TextureResourceManager;







int ov01_021FA094(const MapObjectManager *param0, int param1, const MapObject *param2);





void ov01_021F9CF8(MapObjectManager *param0, UnkStruct_ov5_021ED0A4 *param1) {
    int new_var;
    int v0;
    GF_2DGfxRawResMan *v2 = ov01_021FA1E4(param1);
    int *v1 = ov01_021FA204(param1);
    unsigned char new_var2;
    new_var = ov01_021FA21C(param1);
    v1 = &v1[new_var];
    v0 = ov01_021FA224(param1);
    do {
        new_var2 = 0xffff;
        if ((*v1) != new_var2) {
            if (ov01_021FA094(param0, *v1, 0L) == 0) {
                ov01_021FC588(param1->unk_FC, *v1);
                *v1 = new_var2;
            }
        }
        v1++;
        v0--;
    } while (v0);
}

void ov01_021F9D48(UnkStruct_ov5_021ED0A4 *gfx) {
    ov01_021F9AAC(ov01_021FA1FC(gfx), 0xFFFF, 0x20);
}
