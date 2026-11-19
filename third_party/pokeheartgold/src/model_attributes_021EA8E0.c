#include "global.h"

#include "constants/heap.h"

#include "heap.h"

struct FogManager;
typedef struct ModelAttributes {
    VecFx16 lightVectors[4];
    GXRgb lightColors[4];
    GXRgb diffuseReflectColor;
    GXRgb ambientReflectColor;
    GXRgb specularReflectColor;
    GXRgb emissionColor;
    BOOL setDiffuseColorAsVertexColor;
    BOOL enableSpecularReflectShininessTable;
    int enabledLightsMask;
    GXPolygonMode polygonMode;
    GXCull cullMode;
    int polygonID;
    int alpha;
    int miscFlags;
} ModelAttributes;
typedef struct FogManager FogManager;

ModelAttributes *ModelAttributes_Init(void);
void ModelAttributes_Free(FogManager **fogMan);

ModelAttributes *ModelAttributes_Init(void) {
    ModelAttributes *modelAttrs = Heap_Alloc(HEAP_ID_FIELD1, sizeof(ModelAttributes));
    MI_CpuClear32(modelAttrs, sizeof(ModelAttributes));

    return modelAttrs;
}

void ModelAttributes_Free(FogManager **fogMan) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, *fogMan);
    *fogMan = 0L;
}
