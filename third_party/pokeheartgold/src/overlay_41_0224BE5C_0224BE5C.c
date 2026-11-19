#include "global.h"
struct UnkStruct_ov22_0225B85C;

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"

s32 ov41_0224BE80(void *);

void ov41_0224BE5C(struct UnkStruct_ov22_0225B85C *arg0);

void ov41_0224BE5C(UnkStruct_ov22_0225B85C *arg0) {
    FillWindowPixelBuffer(arg0->unk_200, 0U);
    ov41_0224BE80(arg0);
    CopyWindowToVram(arg0->unk_200);
}
