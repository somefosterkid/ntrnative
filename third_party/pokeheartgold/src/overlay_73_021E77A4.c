#include "global.h"

#include "unk_0205B3DC.h"
#include "heap.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

extern const u16 ov73_021EA520[5];

void ov73_021E77A4(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4);

void ov73_021E77E8(UnkStruct_020961E8 *arg0);
s32 ov73_021E7818(void);

void ov73_021E77A4(NNSG2dCharacterData *param0[2], NNSG2dPaletteData *param1[2], int param2, int param3, int param4) {
    int v0;
    u8 *v1, *v2;

    v0 = SpriteToUnionRoomAvatarIdx(param4, param3);

    v1 = (u8 *)param0[1]->pRawData;
    v2 = (u8 *)param1[1]->pRawData;

    GX_LoadOBJ(&v1[(3 * 4 * 4) * v0 * 0x20], ov73_021EA520[param2], (3 * 4 * 4) * 0x20);
    GX_LoadOBJPltt(&v2[v0 * 32], (param2 + 7) * 32, 32);
}

void ov73_021E77E8(UnkStruct_020961E8 *arg0) {
    Heap_Free(arg0->unk_398[0]);
    Heap_Free(arg0->unk_398[1]);
    Heap_Free(arg0->unk_388[0]);
    Heap_Free(arg0->unk_388[1]);
}

s32 ov73_021E7818(void) {
    return 1;
}
