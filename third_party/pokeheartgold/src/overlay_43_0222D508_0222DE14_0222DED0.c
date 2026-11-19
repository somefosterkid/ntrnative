#include "global.h"

#include "bg_window.h"

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    u32 unk_08;
    Window *unk_0C[7];
    u32 unk_28[7];
} UnkStruct_ov64_02230F98;

void ov43_0222DED0(UnkStruct_ov64_02230F98 *param0, u32 param1);

void ov43_0222DED0(UnkStruct_ov64_02230F98 *param0, u32 param1) {
    if (param0->unk_0C[param1] != NULL) {
        WindowArray_Delete(param0->unk_0C[param1], param0->unk_28[param1 + 1] & 0xff);
        param0->unk_0C[param1] = NULL;
        param0->unk_28[param1 + 1] = 0;
    }
    return;
}
