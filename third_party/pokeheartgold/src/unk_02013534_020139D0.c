#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_struct_02013034.h"
#include "unk_02013534_font_system.h"

struct FontOAM;
struct UnkStruct_02012B20_t;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
int sub_02013948(const UnkStruct_02012B20 *param0, int param1);
struct UnkStruct_02012B20_t {
    UnkStruct_02013034 unk_00;
    int unk_14;
};
void sub_02013D88(const Window *param0, char *param1, const UnkStruct_02013034 *param2, int param3, int param4);

void TextOBJ_CopyFromBGWindow(FontOAM *param0, const UnkStruct_02012B20 *param1, const Window *param2, enum HeapID heapID);
void sub_02013A50(const Window *window, int width, int height, int x, int y, char *output);

void TextOBJ_CopyFromBGWindow(FontOAM *param0, const UnkStruct_02012B20 *param1, const Window *param2, enum HeapID heapID) {
    int v0;
    char *v1;
    NNSG2dImageProxy *v2;
    Sprite *v3 = param0->unk_00[0].unk_00;
    int v4 = Sprite_GetVramType(v3);
    v0 = sub_02013948(param1, v4);
    v1 = (char *)Heap_AllocAtEnd(heapID, v0);

    memset(v1, 0, v0);

    sub_02013D88(param2, v1, &param1->unk_00, v4, heapID);
    DC_FlushRange(v1, v0);

    v2 = Sprite_GetImageProxy(v3);

    if (v4 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(v1, NNS_G2dGetImageLocation(v2, NNS_G2D_VRAM_TYPE_2DMAIN), v0);
    } else {
        GXS_LoadOBJ(v1, NNS_G2dGetImageLocation(v2, NNS_G2D_VRAM_TYPE_2DSUB), v0);
    }

    Heap_Free(v1);
}

void sub_02013A50(
    const Window *window,
    int width,
    int height,
    int x,
    int y,
    char *output) {

    int i;
    int dstOffset;
    int srcOffset;

    ((window->width >= (width + x)) ? (void)0 : GF_AssertFail());
    ((window->height >= (height + y)) ? (void)0 : GF_AssertFail());

    for (i = 0; i < height; i++) {
        dstOffset = i * width;
        dstOffset *= 32;
        srcOffset = ((i + y) * window->width);
        srcOffset += x;
        srcOffset *= 32;

        memcpy(output + dstOffset, (char *)(window->pixelBuffer) + srcOffset, 32 * width);
    }
}
