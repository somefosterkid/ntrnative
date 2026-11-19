#include "global.h"
#include "overlay_49_unk_struct_ov70_02269800.h"
struct UnkStruct_ov70_02269800;

void ov49_02266D7C(void *, void *);

BOOL ov49_02266E78(void *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266D60(void *param_1, struct UnkStruct_ov70_02269800 *param_2);

void ov49_02266D60(void *param_1, UnkStruct_ov70_02269800 *param_2) {
    unsigned char new_var;
    new_var = *((s8 *)&param_2->unk_965);
    if (new_var == 0) {
        ov49_02266D7C(param_1, param_2);
        return;
    }
    ov49_02266E78(param_1, param_2);
    return;
}
