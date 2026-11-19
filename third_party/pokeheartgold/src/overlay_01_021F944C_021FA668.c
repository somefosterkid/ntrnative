#include "global.h"
#include "gf_3d_loader.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"

struct MapObject;

struct GF_2DGfxRawResMan;

struct ResourceHeap;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

struct UnkStruct_ov5_021ED110;

typedef struct MapObject MapObject;


typedef struct TextureResourceManager TextureResourceManager;






void ov01_021FA668(UnkStruct_ov5_021ED0A4 *param0, int param1);

void ov01_021FA6A4(UnkStruct_ov5_021ED0A4 *param0, int param1);

void GF3dGfxRawResMan_LoadObjTexById(GF_3DGfxRawResMan *texMgr, int id);
void GF3dGfxRawResMan_FreeObjVramAndSecondaryHeaderById(GF_3DGfxRawResMan *texMgr, int id);
void ov01_021FA6E0(SysTask *param0, void *param1);
void ov01_021FA71C(SysTask *param0, void *param1);

void ov01_021FA668(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EE134 *v2 = v1->unk_10;

    v1->unk_06 = 1;

    while (v0 < v1->unk_00) {
        if (v2->unk_08 == NULL) {
            v2->unk_08 = param0->unk_F0;
            v2->unk_04 = param1;
            v2->unk_00 = 0;

            v1->unk_06 = 0;
            return;
        }

        v2++;
        v0++;
    }

    GF_ASSERT(FALSE);
}

void ov01_021FA6A4(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EE134 *v2 = v1->unk_10;

    v1->unk_06 = 1;

    while (v0 < v1->unk_00) {
        if ((v2->unk_08 != NULL) && (v2->unk_04 == param1)) {
            v2->unk_08 = NULL;
            v2->unk_00 = 0;
            break;
        }

        v2++;
        v0++;
    }

    v1->unk_06 = 0;
}

void ov01_021FA6E0(SysTask *param0, void *param1) {
    UnkStruct_ov5_021ED0A4 *v0 = param1;
    UnkStruct_ov5_021EDF3C *v1 = v0->unk_100;

    if (v1->unk_06 == 1) {
        return;
    }

    {
        int v2 = 0;
        UnkStruct_ov5_021EE134 *v3 = v1->unk_10;

        while (v2 < v1->unk_00) {
            if ((v3->unk_00 == 0) && (v3->unk_08 != 0L)) {
                GF3dGfxRawResMan_LoadObjTexById(v3->unk_08, v3->unk_04);
                v3->unk_00 = 1;
            }

            v3++;
            v2++;
        }
    }
}

void ov01_021FA71C(SysTask *param0, void *param1) {
    int v0 = 0;
    UnkStruct_ov5_021ED0A4 *v1 = param1;
    UnkStruct_ov5_021EDF3C *v2 = v1->unk_100;
    UnkStruct_ov5_021EE134 *v3 = v2->unk_10;

    v2->unk_06 = 1;

    while (v0 < v2->unk_00) {
        if ((v3->unk_00 == 1) && (v3->unk_08 != 0L)) {
            GF3dGfxRawResMan_FreeObjVramAndSecondaryHeaderById(v3->unk_08, v3->unk_04);
            v3->unk_00 = 0;
            v3->unk_08 = 0L;
        }

        v3++;
        v0++;
    }

    v2->unk_06 = 0;
}
