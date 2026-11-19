#include "global.h"
#include "field/model_attributes.h"
#include "model_attributes.h"

void ov01_021EAAB8(ModelAttributes *modelAttributes, GXLightId lightId, fx16 x, fx16 y, fx16 z) {
    modelAttributes->lightVectors[lightId].x = x;
    modelAttributes->lightVectors[lightId].y = y;
    modelAttributes->lightVectors[lightId].z = z;
    NNS_G3dGlbLightVector(lightId, modelAttributes->lightVectors[lightId].x, modelAttributes->lightVectors[lightId].y, modelAttributes->lightVectors[lightId].z);
}

void ov01_021EAAE0(ModelAttributes *modelAttributes, GXLightId lightId, GXRgb color) {
    modelAttributes->lightColors[lightId] = color;
    NNS_G3dGlbLightColor(lightId, modelAttributes->lightColors[lightId]);
}

void ov01_021EAAF4(ModelAttributes *modelAttributes, u16 arg1, s32 arg2, s32 arg3) {
    modelAttributes->diffuse = arg1;
    modelAttributes->setDiffuseColorAsVertexColor = arg2;
    if (arg3 == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x100);
    }
}

void ov01_021EAB08(ModelAttributes *modelAttributes, u16 arg1, s32 arg2) {
    modelAttributes->ambient = arg1;
    if (arg2 == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x200);
    }
}

void ov01_021EAB1C(ModelAttributes *modelAttributes, u16 arg1, s32 arg2, s32 arg3) {
    modelAttributes->specular = arg1;
    modelAttributes->enableSpecularReflectShininessTable = arg2;
    if (arg3 == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x400);
    }
}

void ov01_021EAB30(ModelAttributes *modelAttributes, u16 arg1, s32 arg2) {
    modelAttributes->emission = arg1;
    if (arg2 == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x800);
    }
}
