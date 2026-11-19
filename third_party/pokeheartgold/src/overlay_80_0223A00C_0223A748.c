#include "global.h"

#include "obj_char_transfer.h"
#include "font.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;
void FontOAM_Delete(FontOAM *param0);
typedef struct {
    FontOAM *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

void ov80_0223A748(UnkStruct_ov113_0225E250 *param0);

void ov80_0223A75C(String *arg0, enum Font arg1, s32 *arg2, s32 *arg3);

void ov80_0223A748(UnkStruct_ov113_0225E250 *param0) {
    FontOAM_Delete(param0->unk_00);
    sub_02021B5C(&param0->unk_04);
}

void ov80_0223A75C(String *string, enum Font fontId, s32 *arg2, s32 *arg3) {
    s32 a;
    s32 v0;

    a = (s32)FontID_String_GetWidth(fontId, string, 0U);
    v0 = (s32)(a + ((u32)(a >> 2) >> 0x1D)) >> 3;
    if (FX_ModS32(a, 8) != 0) {
        v0 += 1;
    }
    *arg2 = a;
    *arg3 = v0;
}
