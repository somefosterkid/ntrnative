#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

void ov73_021E7740(UnkStruct_020961E8 *arg0, NARC *arg1);

void ov73_021E7740(UnkStruct_020961E8 *arg0, NARC *narc) {
    arg0->unk_398[0] = GfGfxLoader_GetPlttData(NARC_a_1_0_0, 9, arg0->unk_3A0, HEAP_ID_50);
    arg0->unk_398[1] = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 0xB, &arg0->unk_3A0[1], HEAP_ID_50);
    arg0->unk_388[0] = GfGfxLoader_GetCharData(NARC_a_1_0_0, 0x2B, 1, arg0->unk_390, HEAP_ID_50);
    arg0->unk_388[1] = GfGfxLoader_GetCharDataFromOpenNarc(narc, 0xC, 1, &arg0->unk_390[1], HEAP_ID_50);
}
