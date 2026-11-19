#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void *ReadMModelFromNarcInternal(const struct MapObjectManager *param0, u32 param1, int param2);
void *FldObjSys_ReadMModelFromNarc(UnkStruct_ov5_021ED0A4 *param0, u32 param1, int param2);

void ov01_021FA220(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_14 = value;
}

int ov01_021FA224(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_14;
}

void ov01_021FA228(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_18 = value;
}

int ov01_021FA22C(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_18;
}

void ov01_021FA230(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_1C = value;
}

int ov01_021FA234(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_1C;
}

void *FldObjSys_ReadMModelFromNarc(UnkStruct_ov5_021ED0A4 *param0, u32 param1, int param2) {
    return ReadMModelFromNarcInternal(param0->unk_104, param1, param2);
}
