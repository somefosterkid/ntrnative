#include "global.h"

#include "overlay_01_02203A18.h"
#include "unk_020689C8.h"

void sub_02023E50(void *, VecFx32 *);

void ov01_02203DC0(void *param_1, UnkOv01_02203A18_Work *param_2);

void ov01_02203DC0(void *param_1, UnkOv01_02203A18_Work *param_2) {
    VecFx32 VStack_18;

    if (param_2->unk10 != 1) {
        sub_02068DB8(param_1, &VStack_18);
        VStack_18.x = VStack_18.x + param_2->unk18;
        VStack_18.y = VStack_18.y + param_2->unk1C;
        VStack_18.z = VStack_18.z + param_2->unk20;
        sub_02023E50(param_2->unk3C, &VStack_18);
    }
    return;
}
