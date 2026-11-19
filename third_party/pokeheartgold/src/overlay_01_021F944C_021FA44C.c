#include "global.h"
#include "gf_3d_loader.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"

extern s32 ov01_02208B70;

s32 ov01_021FA44C(s32 arg0);

struct MapObject;
struct GF_2DGfxRawResMan;
struct ResourceHeap;
struct TextureResourceManager;
struct UnkStruct_ov5_021ED0A4_t;
struct UnkStruct_ov5_021ED110;
typedef struct MapObject MapObject;
BOOL sub_020238F8(BillboardList *list);
typedef struct TextureResourceManager TextureResourceManager;
void *FldObjSys_ReadMModelFromNarc(UnkStruct_ov5_021ED0A4 *param0, u32 param1, int param2);
void ov01_021FA61C(UnkStruct_ov5_021ED0A4 *param0, int param1, void *param2, int param3);
int ov01_021FA470(UnkStruct_ov5_021ED0A4 *param0, int param1, u32 param2, int param3);
void ov01_021FA4F0(UnkStruct_ov5_021ED0A4 *param0, int param1);
int ov01_021FA524(UnkStruct_ov5_021ED0A4 *param0, int param1);
void ov01_021FA564(SysTask *param0, void *param1);

s32 ov01_021FA44C(s32 arg0) {
    return *(s32 *)(&ov01_02208B70 + arg0);
}

extern s32 ov01_02208B80;

s32 ov01_021FA458(s32 arg0);

s32 ov01_021FA458(s32 arg0) {
    return *(s32 *)(&ov01_02208B80 + arg0);
}

extern s32 ov01_02208B90;

s32 ov01_021FA464(s32 arg0);

s32 ov01_021FA464(s32 arg0) {
    return *(s32 *)(&ov01_02208B90 + arg0);
}

int ov01_021FA470(UnkStruct_ov5_021ED0A4 *param0, int param1, u32 param2, int param3) {
    UnkStruct_ov5_021EDF3C *v0 = param0->unk_100;

    if ((v0->unk_04 >= v0->unk_02) || (sub_020238F8(param0->unk_E0) == 1)) {
        int v1 = 0;
        UnkStruct_ov5_021EDFBC *v2 = v0->unk_0C;

        while (v1 < v0->unk_00) {
            if (v2->unk_0C == NULL) {
                v2->unk_0C = param0->unk_F0;
                v2->unk_08 = param3;
                v2->unk_04 = param1;
                v2->unk_00 = param2;
                return 0;
            }

            v1++;
            v2++;
        }

        GF_ASSERT(FALSE);
        return 0;
    }

    {
        void *v3 = FldObjSys_ReadMModelFromNarc(param0, param2, 0);

        ov01_021FA61C(param0, param1, v3, param3);
        v0->unk_04++;
        return 1;
    }
}

void ov01_021FA4F0(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EDFBC *v2 = v1->unk_0C;

    while (v0 < v1->unk_00) {
        if ((v2->unk_04 == param1) && (v2->unk_0C != NULL)) {
            v2->unk_0C = NULL;
            return;
        }

        v0++;
        v2++;
    }
}

int ov01_021FA524(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0 = 0;
    UnkStruct_ov5_021EDF3C *v1 = param0->unk_100;
    UnkStruct_ov5_021EDFBC *v2 = v1->unk_0C;

    while (v0 < v1->unk_00) {
        if ((v2->unk_04 == param1) && (v2->unk_0C != NULL)) {
            if (v2->unk_08 == 0) {
                return 3;
            }

            return 4;
        }

        v0++;
        v2++;
    }

    return 0;
}

void ov01_021FA564(SysTask *param0, void *param1) {
    int v0 = 0;
    UnkStruct_ov5_021ED0A4 *v1 = param1;
    UnkStruct_ov5_021EDF3C *v2 = v1->unk_100;
    UnkStruct_ov5_021EDFBC *v3 = v2->unk_0C;

    {
        if (sub_020238F8(v1->unk_E0) == 1) {
            return;
        }
    }

    while ((v2->unk_04 < v2->unk_02) && v0 < v2->unk_00) {
        if (v3->unk_0C != NULL) {
            void *v4 = FldObjSys_ReadMModelFromNarc(v1, v3->unk_00, 0);

            ov01_021FA61C(v1, v3->unk_04, v4, v3->unk_08);
            v3->unk_0C = NULL;
            v2->unk_04++;
        }

        v3++;
        v0++;
    }

    {
        int v5;

        v3 = v2->unk_0C;

        for (v0 = 0; v0 < (v2->unk_00 - 1); v0++) {
            if (v3[v0].unk_0C == NULL) {
                for (v5 = v0 + 1; v5 < v2->unk_00; v5++) {
                    if (v3[v5].unk_0C != NULL) {
                        v3[v0] = v3[v5];
                        v3[v5].unk_0C = NULL;
                        break;
                    }
                }
            }
        }
    }

    v2->unk_04 = 0;
}
