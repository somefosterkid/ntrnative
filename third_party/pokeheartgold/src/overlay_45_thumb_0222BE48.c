#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222BE48(void *arg0);

struct PlttTransferTask;
typedef struct PlttTransferTask PlttTransferTask;
typedef struct PlttTransferTaskManager {
    PlttTransferTask *tasks;
    int capacity;
    int length;
    u32 offsetMain;
    u32 offsetSub;
    u32 extPlttOffsetMain;
    u32 extPlttOffsetSub;
    u32 extPlttVramSizeMain;
    u32 extPlttVramSizeSub;
    u16 vramTransferMain;
    u16 vramTransferSub;
} PlttTransferTaskManager;
void ov45_0222BE54(PlttTransferTaskManager *manager);
void ov45_0222BE5C(UnkStruct_ov66_0222FC68 *param0);
BOOL ov45_0222BE74(const UnkStruct_ov66_0222FC68 *param0);

void ov45_0222BE48(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x24) = 1;
    *(s16 *)((u8 *)arg0 + 0x26) = 0;
}

void ov45_0222BE54(PlttTransferTaskManager *manager) {
    manager->vramTransferMain = 0;
    manager->vramTransferSub = 0;
}

void ov45_0222BE5C(UnkStruct_ov66_0222FC68 *param0) {
    if (param0->unk_24 == 1) {
        if ((param0->unk_26 + 1) <= 900) {
            param0->unk_26++;
        }
    }
}

BOOL ov45_0222BE74(const UnkStruct_ov66_0222FC68 *param0) {
    if (param0->unk_24 == 0) {
        return 1;
    }

    if (param0->unk_26 >= 900) {
        return 0;
    }

    return 1;
}
