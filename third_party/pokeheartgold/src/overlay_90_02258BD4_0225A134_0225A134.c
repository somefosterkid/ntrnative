#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"

struct BufferManager;

void *sub_02014A4C(const BufferManager *bufferManager);

void sub_02014A78(BufferManager *bufferManager, u8 mode);

typedef struct {
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08[4];
} UnkStruct_ov114_02260204;

extern const UnkStruct_ov114_02260204 ov90_0225C31C[4];

void ov90_0225A134(UnkStruct_ov114_0225DFFC *param0, const UnkStruct_ov114_0225DAC0 *param1);

void ov90_0225A134(UnkStruct_ov114_0225DFFC *param0, const UnkStruct_ov114_0225DAC0 *param1) {
    s16 *v0;
    int v1;
    int v2;

    GF_ASSERT(param0->bufferManager != NULL);

    v0 = sub_02014A4C(param0->bufferManager);

    for (v1 = 0; v1 < 6 * 8; v1++) {
        v2 = v1 - 1;
        v2 += ov90_0225C31C[param1->unk_18 - 1].unk_00[param1->unk_1B] * 8;

        if (v2 < 0) {
            v2 += 192;
        }

        v0[v2] = param1->unk_1C.unk_00 >> FX32_SHIFT;
    }

    sub_02014A78(param0->bufferManager, 1);
}
