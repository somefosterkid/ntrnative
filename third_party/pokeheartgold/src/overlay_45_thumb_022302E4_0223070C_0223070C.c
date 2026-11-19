#include "global.h"
#include "billboard.h"
#include "filesystem.h"

struct Easy3DModel;

struct Easy3DObject;

struct NARC;

struct Resource;

struct ResourceCollection;

struct TextureResource;

struct TextureResourceManager;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234548_t;

struct UnkStruct_ov66_02234798_t;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;

s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);

UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DModel {
    void *data;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Easy3DModel;

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

void Field3dObject_SetPosEx(Easy3DObject *obj, fx32 x, fx32 y, fx32 z);

void ov49_02258800(const UnkStruct_ov63_0222CC3C *param0, VecFx32 *param1);

Billboard *sub_02023D44(const BillboardTemplate *template);

void sub_02023E04(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *param3, const BillboardGfxSequence *gfxSequence, NNSGfdTexKey texKey, NNSGfdTexKey tex4x4Key, NNSGfdPlttKey plttKey);

void sub_02023E2C(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *param3, const BillboardGfxSequence *gfxSequence);

void Billboard_SetCallback(Billboard *billboard, BillboardCallback callback, void *callbackParam);

typedef struct Resource Resource;

typedef struct ResourceCollection ResourceCollection;

typedef struct TextureResource TextureResource;

typedef struct TextureResourceManager TextureResourceManager;

Resource *GF2dGfxRawResMan_GetObjById(ResourceCollection *collection, int id);

void *GF2dGfxRawResObj_GetData(Resource *resource);

TextureResource *GF3dGfxRawResMan_GetObjById(const TextureResourceManager *texMgr, int id);

NNSG3dResTex *TextureResource_GetUnderlyingResource(const TextureResource *texResource);

NNSGfdTexKey TextureResource_GetTexKey(const TextureResource *texResource);

NNSGfdTexKey TextureResource_GetTex4x4Key(const TextureResource *texResource);

NNSGfdPlttKey GF3dGfxRawResObj_GetPlttKey(const TextureResource *texResource);

typedef struct {
    u16 unk_00;
    u16 unk_02_0 : 15;
    u16 unk_02_15 : 1;
} UnkStruct_ov66_02258B38;

typedef struct {
    Easy3DModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

struct UnkStruct_ov66_02234798_t {
    u8 unk_00_0 : 4;
    u8 unk_00_4 : 2;
    u8 unk_00_6 : 2;
    u8 unk_01_0 : 1;
    u8 unk_01_1 : 7;
    u8 unk_02;
    u8 unk_03;
    const UnkStruct_ov63_0222BEC0 *unk_04;
    Billboard *unk_08;
    Easy3DObject unk_0C;
    u8 unk_84;
    u8 unk_85;
    u16 unk_86;
    fx32 unk_88;
    int unk_8C;
};

struct UnkStruct_ov66_02234548_t {
    ResourceCollection *unk_00;
    ResourceCollection *unk_04;
    TextureResourceManager *unk_08;
    BillboardList *unk_0C;
    UnkStruct_ov66_02234798 *unk_10;
    u32 unk_14;
    UnkStruct_ov66_02234958 unk_18;
    int unk_2C;
};

const UnkStruct_ov66_02258B38 *ov45_0223099C(u32 param0);

void ov96_021E9A38(UnkStruct_ov66_02234958 *param0, Easy3DObject *param1);

UnkStruct_ov66_02234798 *ov45_02230A6C(UnkStruct_ov66_02234548 *param0, const UnkStruct_ov63_0222BEC0 *, s32);

void ov45_02230E64(Billboard *param0, void *param1);

extern const BillboardAnim ov45_02254C98[9];

UnkStruct_ov66_02234548 *ov45_02230498(u32 param0, u32 param1, enum HeapID heapID, u32 heapID2);

void ov45_022309E8(UnkStruct_ov66_02234958 *param0, NARC *param1, u32 param2, enum HeapID heapID);

void sub_02023FE4(Billboard *arg0, BillboardCallback arg1, void *arg2);

void ov45_02230A5C(UnkStruct_ov66_02234958 *arg0, Easy3DObject *arg1);

void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3);

UnkStruct_ov66_02234798 *ov45_0223070C(UnkStruct_ov66_02234548 *param0, const UnkStruct_ov63_0222BEC0 *param1, s32 arg2);

NNSG3dResTex *GF3dGfxRawResObj_GetTex(void *);

int GF3dGfxRawResObj_GetTexKey(void *);

int GF3dGfxRawResObj_GetTex4x4Key(void *);

UnkStruct_ov66_02234798 *ov45_0223070C(UnkStruct_ov66_02234548 *param0, const UnkStruct_ov63_0222BEC0 *param1, s32 arg2) {
    UnkStruct_ov66_02234798 *v0 = ov45_02230A6C(param0, param1, arg2);
    v0->unk_04 = param1;

    {
        const UnkStruct_ov66_02258B38 *v1;
        BillboardResources v2;
        BillboardTemplate v3;
        Resource *v4;
        TextureResource *v5;
        u32 v6;
        void *v7;
        const NNSG3dResTex *v8;
        BillboardGfxSequence v9;
        NNSGfdTexKey v10;
        NNSGfdTexKey v11;
        NNSGfdPlttKey v12;
        VecFx32 v13 = { 0, 0, 0 };
        VecFx32 v14 = { FX32_ONE, FX32_ONE, FX32_ONE };

        v6 = ov42_02228188(param1, 7);
        v1 = ov45_0223099C(v6);

        {
            v4 = GF2dGfxRawResMan_GetObjById(param0->unk_00, 127);
            v7 = GF2dGfxRawResObj_GetData(v4);
        }

        {
            v5 = GF3dGfxRawResMan_GetObjById(param0->unk_08, v1->unk_02_0);
            v8 = GF3dGfxRawResObj_GetTex(v5);
        }

        {
            u32 v15;
            const void *v16;

            if (v1->unk_02_15 == 1) {
                v15 = 0x119;
            } else {
                v15 = 0x118;
            }

            v4 = GF2dGfxRawResMan_GetObjById(param0->unk_04, v15);
            v16 = GF2dGfxRawResObj_GetData(v4);

            BillboardGfxSequence_SetData(v16, &v9);
        }

        if (v1->unk_02_15 == 1) {
            sub_02023E2C(&v2, v7, v8, ov45_02254C98, &v9);
        } else {
            v10 = GF3dGfxRawResObj_GetTexKey(v5);
            v11 = GF3dGfxRawResObj_GetTex4x4Key(v5);
            v12 = GF3dGfxRawResObj_GetPlttKey(v5);

            sub_02023E04(&v2, v7, v8, ov45_02254C98, &v9, v10, v11, v12);
        }

        {
            UnkStruct_ov63_0222CC3C v17;

            v17 = ov42_022282DC(param1);
            ov49_02258800(&v17, &v13);
            v13.z += (FX32_CONST(16));
        }

        v3.list = param0->unk_0C;
        v3.resources = &v2;
        v3.pos = v13;
        v3.scale = v14;
        v0->unk_08 = sub_02023D44(&v3);

        sub_02023FE4(v0->unk_08, ov45_02230E64, v0);
        ov45_02230A5C(&param0->unk_18, &v0->unk_0C);
        sub_020182A8(&v0->unk_0C, v13.x + 0, ((FX32_CONST(0)) + FX32_CONST(2)), v13.z + (FX32_CONST(-8)));
    }

    v0->unk_00_0 = 1;
    v0->unk_00_6 = 1;
    v0->unk_00_4 = 0;
    v0->unk_8C = param0->unk_2C;

    return v0;
}
