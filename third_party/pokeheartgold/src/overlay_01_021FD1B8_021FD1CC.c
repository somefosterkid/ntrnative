#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FD1B8.h"

void ov01_021FD21C(UnkStruct_021F114C *param0);

void ov01_021FD1CC(void *param0) {
    UnkStruct_021F114C *v0 = param0;

    ov01_021FD21C(v0);

    if (v0->unk_08 != 0L) {
        ov01_021F1448(v0->unk_08);
    }

    ov01_021F1448(v0);
}
