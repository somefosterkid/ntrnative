#include "global.h"

#include "unk_0208DE40.h"

typedef struct SpiderGraphVtxBounds {
    VecFx16 max;
    VecFx16 min;
    VecFx16 valueLength;
} SpiderGraphVtxBounds;

void sub_0208DF9C(const SpiderGraphVtxBounds *bounds, VecFx16 *outVec, u8 statValue);

void sub_0208DF9C(const SpiderGraphVtxBounds *bounds, VecFx16 *outVec, u8 statValue) {
    if (statValue == 255) {
        *outVec = bounds->max;
    } else if (statValue == 0) {
        *outVec = bounds->min;
    } else {
        outVec->x = bounds->min.x + bounds->valueLength.x * statValue;
        outVec->y = bounds->min.y + bounds->valueLength.y * statValue;
        outVec->z = bounds->min.z + bounds->valueLength.z * statValue;
    }
}

void sub_0208DFF8(VecFx16 *startVec, VecFx16 *endVec, VecFx16 *outVec) {
    outVec->x = ((fx16)(((((f32)((endVec->x - startVec->x) / (f32)(1 << 12))) / 4) > 0) ? (fx16)((((f32)((endVec->x - startVec->x) / (f32)(1 << 12))) / 4) * (1 << 12) + 0.5f) : (fx16)((((f32)((endVec->x - startVec->x) / (f32)(1 << 12))) / 4) * (1 << 12) - 0.5f)));
    outVec->y = ((fx16)(((((f32)((endVec->y - startVec->y) / (f32)(1 << 12))) / 4) > 0) ? (fx16)((((f32)((endVec->y - startVec->y) / (f32)(1 << 12))) / 4) * (1 << 12) + 0.5f) : (fx16)((((f32)((endVec->y - startVec->y) / (f32)(1 << 12))) / 4) * (1 << 12) - 0.5f)));
    outVec->z = ((fx16)(((((f32)((endVec->z - startVec->z) / (f32)(1 << 12))) / 4) > 0) ? (fx16)((((f32)((endVec->z - startVec->z) / (f32)(1 << 12))) / 4) * (1 << 12) + 0.5f) : (fx16)((((f32)((endVec->z - startVec->z) / (f32)(1 << 12))) / 4) * (1 << 12) - 0.5f)));
}
