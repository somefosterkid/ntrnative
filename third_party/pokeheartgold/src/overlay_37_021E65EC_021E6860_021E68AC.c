#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"

void ov37_021E72E8(Window *param0, UnkStruct_ov58_021D2820 *param1, UnkStruct_ov58_021D2754 *param2, int param3);

void ov37_021E73B4(UnkStruct_02095EAC *param0);

void ov37_021E68AC(UnkStruct_02095EAC *param0);

void ov37_021E68AC(UnkStruct_02095EAC *param0) {
    ov37_021E73B4(param0);
    ov37_021E72E8(&param0->unk_32C, param0->unk_43E6, param0->unk_4418, 0);
}
