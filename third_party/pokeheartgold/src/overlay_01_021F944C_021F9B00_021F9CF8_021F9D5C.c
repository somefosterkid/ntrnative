#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

struct UnkStruct_ov5_021ED0A4_t;





void ov01_021F9D5C(UnkStruct_ov5_021ED0A4 *param0, int param1);

void ov01_021F9D5C(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0, v1;
    int *v2;

    v0 = ov01_021FA22C(param0);
    v2 = ov01_021FA1FC(param0);

    v1 = ov01_021F9AB4(v2, param1, 0xffff, v0);
    ((v1 != 0) ? (void)0 : GF_AssertFail());
}
