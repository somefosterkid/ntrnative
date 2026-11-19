#include "global.h"

#include "error_handling.h"
#include "overlay_40.h"
#include "pm_string.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0223A274(void) {
    GF_AssertFail();
    return 0;
}

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222F720(void *);

s32 ov40_0222F920(void *, void *);

s32 ov40_02238F00(void *);

s32 ov40_02238FF4(UnkStruct_0208C06C *param0);

s32 ov40_02239418(void *);

s32 ov40_02239514(UnkStruct_0208C06C *);

s32 ov40_02239574(void *);

s32 ov40_0223992C(void *);

s32 ov40_0223A280(UnkStruct_0208C06C *arg0) {
    String *string;
    s32 v0;
    void *v1;

    v1 = arg0->unk_860;
    v0 = arg0->unk_08;
    switch (v0) {
    case 0:
        ov40_02238FF4(arg0);
        ov40_0223992C(v1);
        ov40_02239514(arg0);
        ov40_0222F720(&arg0->unk_14.unk_48C);
        string = *(String **)((u8 *)v1 + 0x748);
        if (string != NULL) {
            String_Delete(string);
        }
        ov40_0222F920(&arg0->unk_14.unk_48C, arg0);
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        break;
    case 1:
        if (ov40_0222DA00(v1, v1 + 4, 1, 1) != 0) {
            arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        }
        break;
    case 2:
        if (ov40_0222DA00(v1, v1 + 4, 0, 1) != 0) {
            ov40_02239418(arg0);
            ov40_02239574(arg0);
            ov40_02238F00(arg0);
            arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        }
        break;
    default:
        ov40_0222BF80(arg0, 6);
        break;
    }
    return 0;
}
