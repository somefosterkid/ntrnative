#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"
#include "map_object.h"
#include "gf_3d_loader.h"


struct GF_2DGfxRawResMan;

struct ResourceHeap;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

struct UnkStruct_ov5_021ED110;



typedef struct TextureResourceManager TextureResourceManager;






void ov01_021FA2B8(LocalMapObject *param0, int param1);

void ov01_021FA75C(UnkStruct_ov5_021ED0A4 *param0, LocalMapObject *param1, Billboard **param2, int param3);

void ov01_021FA804(UnkStruct_ov5_021ED0A4 *, UnkStruct_ov5_021EE294 *);
void ov01_021FA7C8(UnkStruct_ov5_021ED0A4 *arg0);

void ov01_021FA75C(UnkStruct_ov5_021ED0A4 *param0, LocalMapObject *param1, Billboard **param2, int param3) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EE294 *v2 = v1->unk_08;

    while (v0 < v1->unk_00) {
        if (v2->unk_04 == NULL) {
            v2->unk_04 = param1;
            v2->unk_08 = param2;
            v2->unk_00 = param3;

            *(v2->unk_08) = NULL;
            ov01_021FA2B8(param1, 1);
            return;
        }

        v2++;
        v0++;
    }

    GF_ASSERT(FALSE);
}

void ov01_021FA798(UnkStruct_ov5_021ED0A4 *param0, const LocalMapObject *param1);

void ov01_021FA798(UnkStruct_ov5_021ED0A4 *param0, const LocalMapObject *param1) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EE294 *v2 = v1->unk_08;

    while (v0 < v1->unk_00) {
        if (v2->unk_04 == param1) {
            v2->unk_04 = NULL;
            v2->unk_08 = NULL;

            return;
        }

        v2++;
        v0++;
    }
}

void ov01_021FA7C8(UnkStruct_ov5_021ED0A4 *arg0) {
    s32 v1;
    UnkStruct_ov5_021EDF3C *v0;
    UnkStruct_ov5_021EE294 *v2;

    v0 = arg0->unk_100;
    v1 = 0;
    v2 = v0->unk_08;
    if ((s32)v0->unk_00 > 0) {
        do {
            if (v2->unk_04 != NULL) {
                ov01_021FA804(arg0, v2);
            }
            v1 += 1;
            v2++;
        } while (v1 < (s32)v0->unk_00);
    }
}
