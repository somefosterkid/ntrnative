#include "global.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"
#include "overlay_98_ov98text_sys.h"
struct UnkStruct_ov96_021EF610;

void ov96_021EF770(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EF770(UnkStruct_ov96_021EF610 *arg0) {
    s32 temp_r4;

    temp_r4 = (arg0->unk_20 * 3) + 0xD2;
    ov98_0221EBD8(arg0->textSys, 8, 0xF4, 0);
    ov98_0221EBEC(arg0->textSys, 9, 0xF2, 0, 0, 4);
    ov98_0221EBEC(arg0->textSys, 0xA, 0xF3, 0, 0, 4);
    ov98_0221EC08(arg0->textSys, 0xB, temp_r4, 0, 0);
}

void ov96_021EF7C4(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EF7C4(UnkStruct_ov96_021EF610 *arg0) {
    s32 temp_r4;

    temp_r4 = (arg0->unk_20 * 3) + 0xD3;
    ov98_0221EBD8(arg0->textSys, 8, 0xF5, 0);
    ov98_0221EBEC(arg0->textSys, 9, 0xF1, 0, 0, 4);
    ov98_0221EBEC(arg0->textSys, 0xA, 0xF3, 0, 0, 4);
    ov98_0221EC08(arg0->textSys, 0xB, temp_r4, 0, 0);
}
