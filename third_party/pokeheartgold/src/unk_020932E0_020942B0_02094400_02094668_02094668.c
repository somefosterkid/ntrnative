#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "unk_020932E0.h"

void sub_02094668(UnkStruct_020932E0 *arg0, u8 arg1, s32 arg2, s32 arg3) {
    void *buf;
    NARC *narc;
    s32 i;
    s32 slot = arg1;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, arg0->unk_4);
    buf = Heap_AllocAtEnd(arg0->unk_4, 0x1000);
    for (i = 0; i < 30; i++) {
        arg0->unk_8D4[i].unk_8 = NULL;
    }
    if (arg3 != 0) {
        arg0->unk_7E8[slot].unk_4 = arg0->unk_7E8[arg2].unk_4;
        sub_02094C08(&arg0->unk_7E8[arg2].unk_4, arg0->unk_8D4, slot, arg0->unk_7E8[slot].unk_0, buf, narc, 0x222);
    } else {
        arg0->unk_7E8[slot].unk_4 = arg0->unk_198[arg2].unk_4;
        sub_02094C08(&arg0->unk_198[arg2].unk_4, arg0->unk_8D4, slot, arg0->unk_7E8[slot].unk_0, buf, narc, 0x222);
    }
    Heap_Free(buf);
    NARC_Delete(narc);
    arg0->unk_4644 = sub_02094758;
}
