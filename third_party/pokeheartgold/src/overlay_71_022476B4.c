#include "global.h"
struct Trade3DModel;

#include "filesystem_files_def.h"
#include "overlay_71_trade3dmodel.h"

void ov71_022476B4(const Trade3DModel *tradeModel, VecFx32 *outPosition);

void ov71_022476D4(struct Trade3DModel *arg0, void *arg1);
void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);
void ov71_02247704(struct Trade3DModel *arg0, s32 arg1);

void ov71_022476B4(const Trade3DModel *tradeModel, VecFx32 *outPosition) {
    *outPosition = tradeModel->position;
}

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position) {
    tradeModel->position = *position;
}

void ov71_022476D4(Trade3DModel *arg0, void *arg1) {
    *(u16 *)arg1 = (u16) *((u16 *)&arg0->rotation.x);
    *(u16 *)((u8 *)arg1 + 2) = (u16) *((u16 *)&arg0->rotation.y);
    *(u16 *)((u8 *)arg1 + 4) = (u16) *((u16 *)&arg0->rotation.z);
}

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation) {
    *((u16 *)&tradeModel->rotation.x) = (u16) *((u16 *)&rotation->x);
    *((u16 *)&tradeModel->rotation.y) = (u16) *((u16 *)&rotation->y);
    *((u16 *)&tradeModel->rotation.z) = (u16) *((u16 *)&rotation->z);
}

void ov71_02247704(Trade3DModel *arg0, s32 arg1) {
    *((s32 *)&arg0->enabled) = arg1;
}
