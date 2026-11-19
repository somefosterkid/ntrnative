#include "global.h"

#include "obj_char_transfer.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;
void FontOAM_Delete(FontOAM *param0);
typedef struct {
    FontOAM *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

void ov89_0225A354(UnkStruct_ov113_0225E250 *param0);

void ov89_0225A354(UnkStruct_ov113_0225E250 *param0) {
    FontOAM_Delete(param0->unk_00);
    sub_02021B5C(&param0->unk_04);
}
