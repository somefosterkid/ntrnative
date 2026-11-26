#include "global.h"

#include "frontier/overlay_80.h"

#include "overlay_40.h"
#include "screen_fade.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DED0(void *, s32);

void ov40_0222DFB0(void *arg0);

s32 ov40_0222FCCC(void *);

s32 ov40_0222FDC4(void *);

s32 ov40_02233044(UnkStruct_0208C06C *);

s32 ov40_02244BBC(SaveData *, s32);

s32 sub_0202FC90(void *, s32, s32 *, void *, s32);

s32 ov40_02232A48(void *arg0) {
    s32 v0;
    s32 v1;

    v1 = *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08);
    switch (v1) {
    case 0:
        ov40_0222DED0(arg0, 0x12B);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_109);
        *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (s32)(*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (s32)(*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        }
        break;
    case 2:
        if (((UnkStruct_0208C06C *)arg0)->unk_874 == 0) {
            sub_0202FC90(*((void **)&((UnkStruct_0208C06C *)arg0)->saveData), 0x6D, &v0, 0, ((UnkStruct_0208C06C *)arg0)->unk_86C);
            ov40_02244BBC(*((void **)&((UnkStruct_0208C06C *)arg0)->saveData), ((UnkStruct_0208C06C *)arg0)->unk_86C);
            sub_0202FC24();
            ov40_0222FDC4(arg0);
            ov40_0222FCCC(arg0);
        }
        *((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (s32)(*((s32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    default:
        ov40_0222DFB0(arg0);
        ov40_02233044(arg0);
        ov40_0222BF80(arg0, 9);
        break;
    }
    return 0;
}
