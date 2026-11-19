#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

struct SoftwareSprite;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
void ov41_02246008(UnkStruct_ov22_02255040 *param0, BOOL param1);

void ov41_022489E4(UnkStruct_ov22_02259560 *param0, int param1);

void ov41_022489E4(UnkStruct_ov22_02259560 *param0, int param1) {
    UnkStruct_ov22_02259560 *v0 = param0->unk_08;

    while (v0 != (param0)) {
        if ((v0->unk_04 == 0) || (v0->unk_04 == 1) || (v0->unk_04 == 2)) {
            ov41_02246008(v0->unk_00, param1);
        }

        v0 = v0->unk_08;
    }
}
