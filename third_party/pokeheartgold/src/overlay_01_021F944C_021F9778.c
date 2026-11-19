#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"
#include "gf_3d_loader.h"

struct MapObject;
struct GF_2DGfxRawResMan;
struct ResourceHeap;
struct TextureResourceManager;
struct UnkStruct_ov5_021ED0A4_t;
typedef struct MapObject MapObject;
typedef struct TextureResourceManager TextureResourceManager;

void ov01_021F9778(UnkStruct_ov5_021ED0A4 *param0, u32 param1);
void ov01_021F9798(UnkStruct_ov5_021ED0A4 *param0);

void ov01_021F9778(UnkStruct_ov5_021ED0A4 *param0, u32 param1) {
    int v0;
    UnkStruct_ov5_021ED110 *v1 = param0->unk_F4;
    v0 = param0->unk_04;

    do {
        if (v1->unk_00 == param1) {
            v1->unk_00 = 0xffff;

            return;
        }

        v1++;
        v0--;
    } while (v0);
}

void ov01_021F9798(UnkStruct_ov5_021ED0A4 *param0) {
    int v0;
    UnkStruct_ov5_021ED110 *v1 = param0->unk_F4;
    v0 = param0->unk_04;

    do {
        if (v1->unk_00 != 0xffff) {
            v1->unk_00 = 0xffff;
        }

        v1++;
        v0--;
    } while (v0);
}
