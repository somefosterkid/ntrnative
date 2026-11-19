#include "global.h"
#include "unk_02095EAC.h"
#include "touch_pad_data_buffer.h"

void ov37_021E78E0(UnkStruct_ov58_021D2820 *param0, TouchPadDataBuffer *param1, int param2, int param3);

void ov37_021E78E0(UnkStruct_ov58_021D2820 *param0, TouchPadDataBuffer *param1, int param2, int param3) {
    int v0, v1;

    if (param1->bufferSize != 0) {
        param0->unk_00[0] = param1->buffer[0].x;
        param0->unk_04[0] = param1->buffer[0].y;

        v1 = param1->bufferSize - 1;

        param0->unk_00[1] = param1->buffer[v1].x;
        param0->unk_04[1] = param1->buffer[v1].y;

        for (v0 = 0; v0 < 2; v0++) {
            if ((param0->unk_00[v0] + param0->unk_04[v0]) == 0) {
                param1->bufferSize = 0;
            }
        }
    }

    if (param1->bufferSize >= 2) {
        param0->unk_08_3 = 2;
    } else {
        param0->unk_08_3 = param1->bufferSize;
    }

    param0->unk_08_0 = param2;
    param0->unk_08_6 = param3;
}
