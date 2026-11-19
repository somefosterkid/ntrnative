#include "global.h"
#include "billboard.h"

void *sub_02020838(NNSG3dResTex *, u8);

void sub_02020820(struct BillboardVRAMTransfer *arg0, u8 *arg1);

void sub_02020820(BillboardVRAMTransfer *arg0, u8 *arg1) {
    sub_02020838(*((void **)&arg0->texture), *arg1);
}

void *sub_02020888(NNSG3dResTex *, u8);

void sub_0202082C(struct BillboardVRAMTransfer *arg0, void *arg1);

void sub_0202082C(BillboardVRAMTransfer *arg0, void *arg1) {
    sub_02020888(*((void **)&arg0->texture), *(u8 *)((u8 *)arg1 + 1));
}
