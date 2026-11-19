#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

struct GF_2DGfxRawResMan *ov01_021FA1DC(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_E8;
}

struct GF_2DGfxRawResMan *ov01_021FA1E4(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_EC;
}

void ov01_021FA1EC(UnkStruct_ov5_021ED0A4 *gfx, struct GF_3DGfxRawResMan *resMan) {
    gfx->unk_F0 = resMan;
}

struct GF_3DGfxRawResMan *ov01_021FA1F4(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_F0;
}

int *ov01_021FA1FC(UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_60;
}

int *ov01_021FA200(UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_20;
}

int *ov01_021FA204(UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_40;
}

void ov01_021FA208(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_08 = value;
}

int ov01_021FA20C(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_08;
}

void ov01_021FA210(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_0C = value;
}

int ov01_021FA214(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_0C;
}

void ov01_021FA218(UnkStruct_ov5_021ED0A4 *gfx, int value) {
    gfx->unk_10 = value;
}

int ov01_021FA21C(const UnkStruct_ov5_021ED0A4 *gfx) {
    return gfx->unk_10;
}
