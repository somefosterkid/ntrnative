#include "global.h"

#include "heap.h"
#include "overlay_71_trade3dmodel.h"

void ov71_022475C4(Trade3DModel *tradeModel);

void ov71_022475C4(Trade3DModel *tradeModel) {
    tradeModel->enabled = 0;
    tradeModel->modelData = NULL;
    tradeModel->rotation.z = 0;
    tradeModel->rotation.y = tradeModel->rotation.z;
    tradeModel->rotation.x = tradeModel->rotation.y;
    tradeModel->scale.z = 0x1000;
    tradeModel->scale.y = 0x1000;
    tradeModel->scale.x = 0x1000;
    tradeModel->position.z = 0;
    tradeModel->position.y = 0;
    tradeModel->position.x = 0;
}

void ov71_022475F8(Trade3DModel *tradeModel);

void ov71_022475F8(Trade3DModel *tradeModel) {
    void *ptr;

    ptr = tradeModel->modelData;
    if (ptr != NULL) {
        Heap_Free(ptr);
        tradeModel->modelData = NULL;
        tradeModel->enabled = 0;
    }
}
