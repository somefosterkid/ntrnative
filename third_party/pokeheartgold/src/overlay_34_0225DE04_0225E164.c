#include "global.h"

#include "task.h"
#include "overlay_34.h"

void ov34_0225E164(Ov34Work *work) {
    s32 v0;
    s32 v1;

    v1 = -1;
    if (FieldSystem_TaskIsRunning(work->fieldSystem) == 0) {
        v0 = ov34_0225DE94(work);
        v1 = ov34_0225E020(work);
        if ((v0 == -1) && (v1 == -1)) {
            ov34_0225E0E4(work);
        }
    }
    ov34_0225E1C4(work->bgConfig, work->scrnData, work->rows, v1 - 1, work->chatLog->count, &work->unk290);
}
