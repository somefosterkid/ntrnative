#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_02238BB0_v1 {
    u8 unk_0[0x1A4];
    u8 unk_1A4;
    u8 unk_1A5[0x3];
    u8 unk_1A8;
    u8 unk_1A9[0x2C63];
    u8 unk_2E0C;
    u8 unk_2E0D[0x9F];
    u8 unk_2EAC;
    u8 unk_2EAD[0x2B];
    u8 unk_2ED8;
    u8 unk_2ED9[0x8B];
    s32 unk_2F64;
} UnkStruct_ov40_02238BB0_v1;

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222E7B8(void *, void *);

s32 ov40_0222EED0(void *, void *, void *, void *);

s32 ov40_0222EFD8(void *, void *, void *);

s32 ov40_0222F5EC(void *, s32);

s32 ov40_0222F740(void *, void *, s32);

s32 ov40_0222F858(void *, s16, s16);

s32 ov40_0222F9E0(void *, void *, s32);

s32 ov40_0222FA5C(void *, void *);

s32 ov40_0222FA88(void *);

s32 ov40_02230964(void *, s32);

s32 ov40_022361B0(UnkStruct_0208C06C *);

s32 ov40_02237B7C(void *, s32);

s32 ov40_02237C9C(void *);

s32 sub_020878EC(void *, s16, s16);

s32 sub_020879E0(void *, s32);

s32 ov40_02238BB0(UnkStruct_0208C06C *arg0) {
    s32 v0;
    UnkStruct_ov40_02238BB0_v1 *v1;

    v1 = arg0->unk_860;
    v0 = arg0->unk_08;
    switch (v0) {
    case 0:
        v1->unk_2F64 = 0;
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        break;
    case 1:
        ov40_02230964(arg0, 1);
        ov40_0222E7B8((void *)&v1->unk_2ED8, arg0);
        ov40_022361B0(arg0);
        ov40_02230964(arg0, 0);
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
    case 2:
        if (!(ov40_0222DA00((void *)&v1->unk_1A4, (void *)&v1->unk_1A8, 1, 1) == 0)) {
            BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
            GfGfx_EngineATogglePlanes(4U, 0U);
            GfGfxLoader_LoadCharDataFromOpenNarc(arg0->unk_14.unk_00, 0x3E, arg0->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
            GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->unk_14.unk_00, 3, arg0->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
            arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        }
        break;
    default:
        if (ov40_0222DA00((void *)&v1->unk_1A4, (void *)&v1->unk_1A8, 0, 1) != 0) {
            ov40_02230964(arg0, 1);
            ov40_02237B7C(arg0, v1->unk_2F64 + 0x79);
            ov40_02237C9C(arg0);
            ov40_0222F9E0(&arg0->unk_14.unk_46C, arg0, 2);
            ov40_0222EED0(&arg0->unk_14.unk_48C, arg0, (void *)&v1->unk_2EAC, (void *)&v1->unk_2E0C);
            ov40_0222FA5C(&arg0->unk_14.unk_46C, &arg0->unk_14.unk_48C);
            ov40_0222F740(&arg0->unk_14.unk_48C, arg0, 1);
            ov40_0222F858(&arg0->unk_14.unk_48C, 0x38, 0xB0);
            if (v1->unk_2F64 == 0) {
                ov40_0222FA88(&arg0->unk_14.unk_46C);
                ov40_0222F5EC(&arg0->unk_14.unk_48C, arg0->unk_14.unk_46C.unk_10);
                ov40_0222EFD8(&arg0->unk_14.unk_48C, arg0, (void *)&v1->unk_2E0C);
                sub_020878EC(arg0->unk_6F0, 0x10, (s16)((0x18 * arg0->unk_14.unk_48C.unk_38.unk_04) + 0x44));
            }
            sub_020879E0(arg0->unk_6F0, 1);
            GfGfx_EngineATogglePlanes(4U, 1U);
            ov40_02230964(arg0, 0);
            ov40_0222BF80(arg0, 5);
        }
        break;
    }
    return 0;
}
