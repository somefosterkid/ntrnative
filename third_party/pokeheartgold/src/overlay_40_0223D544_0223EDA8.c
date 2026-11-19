#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223EDA8_v0 {
    u8 unk_0[0x4D0];
    void *unk_4D0;
    MsgData *unk_4D4;
    s32 unk_4D8;
    void *unk_4DC;
} UnkStruct_ov40_0223EDA8_v0;

void ov40_0223EDA8(void *arg0) {
    UnkStruct_ov40_0223EDA8_v0 *v0;

    v0 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    DestroyMsgData(v0->unk_4D4);
    Heap_Free(v0->unk_4DC);
    Heap_Free(v0->unk_4D0);
    v0->unk_4D8 = 0;
}
