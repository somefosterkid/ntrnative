#include "global.h"
struct UnkStruct_ov66_0223177C_t;

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"

s32 ov45_0222DD44(const void *param0);

u8 ov45_0222DD5C(struct UnkStruct_ov66_0223177C_t *arg0);

u8 ov45_0222DD5C(UnkStruct_ov66_0223177C_t *arg0) {
    if (ov45_0222DD44(arg0) == 0) {
        GF_AssertFail();
    }
    return **((u8 * *)&arg0->unk_5C.unk_188.unk_2C);
}
