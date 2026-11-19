#include "global.h"
#include "field/model_attributes.h"

struct ModelAttributes;

enum ModelAttributesShift {
    MODEL_LIGHT_VECTORS_SHIFT = 0,
    MODEL_LIGHT_COLORS_SHIFT = 4,
    MODEL_DIFFUSE_REFLECT_COLOR_SHIFT = 8,
    MODEL_AMBIENT_REFLECT_COLOR_SHIFT,
    MODEL_SPECULAR_REFLECT_COLOR_SHIFT,
    MODEL_EMISSION_COLOR_SHIFT,
    MODEL_LIGHT_ENABLE_FLAGS_SHIFT = 12,
    MODEL_POLYGON_ATTRS_SHIFT = 12,
    MODEL_POLYGON_MODE_SHIFT,
    MODEL_CULL_MODE_SHIFT,
    MODEL_POLYGON_ID_SHIFT,
    MODEL_ALPHA_SHIFT,
    MODEL_MISC_FOG_SHIFT,
    MODEL_MISC_DEPTHTEST_DECAL_SHIFT,
    MODEL_MISC_DISP_1DOT_SHIFT,
    MODEL_MISC_FAR_CLIPPING_SHIFT,
    MODEL_MISC_XLU_DEPTH_UPDATE_SHIFT,
    MODEL_ATTRIBUTES_LAST_SHIFT
};

enum ModelAttributesBit {
    MODEL_LIGHT_VECTOR_1 = 1 << MODEL_LIGHT_VECTORS_SHIFT,
    MODEL_LIGHT_VECTOR_2 = 1 << (MODEL_LIGHT_VECTORS_SHIFT + 1),
    MODEL_LIGHT_VECTOR_3 = 1 << (MODEL_LIGHT_VECTORS_SHIFT + 2),
    MODEL_LIGHT_VECTOR_4 = 1 << (MODEL_LIGHT_VECTORS_SHIFT + 3),
    MODEL_LIGHT_COLOR_1 = 1 << MODEL_LIGHT_COLORS_SHIFT,
    MODEL_LIGHT_COLOR_2 = 1 << (MODEL_LIGHT_COLORS_SHIFT + 1),
    MODEL_LIGHT_COLOR_3 = 1 << (MODEL_LIGHT_COLORS_SHIFT + 2),
    MODEL_LIGHT_COLOR_4 = 1 << (MODEL_LIGHT_COLORS_SHIFT + 3),
    MODEL_DIFFUSE_REFLECT_COLOR = 1 << MODEL_DIFFUSE_REFLECT_COLOR_SHIFT,
    MODEL_AMBIENT_REFLECT_COLOR = 1 << MODEL_AMBIENT_REFLECT_COLOR_SHIFT,
    MODEL_SPECULAR_REFLECT_COLOR = 1 << MODEL_SPECULAR_REFLECT_COLOR_SHIFT,
    MODEL_EMISSION_COLOR = 1 << MODEL_EMISSION_COLOR_SHIFT,
    MODEL_LIGHT_ENABLE_FLAGS = 1 << MODEL_LIGHT_ENABLE_FLAGS_SHIFT,
    MODEL_POLYGON_ATTRS = 1 << MODEL_POLYGON_ATTRS_SHIFT,
    MODEL_POLYGON_MODE = 1 << MODEL_POLYGON_MODE_SHIFT,
    MODEL_CULL_MODE = 1 << MODEL_CULL_MODE_SHIFT,
    MODEL_POLYGON_ID = 1 << MODEL_POLYGON_ID_SHIFT,
    MODEL_ALPHA = 1 << MODEL_ALPHA_SHIFT,
    MODEL_MISC_FOG = 1 << MODEL_MISC_FOG_SHIFT,
    MODEL_MISC_DEPTHTEST_DECAL = 1 << MODEL_MISC_DEPTHTEST_DECAL_SHIFT,
    MODEL_MISC_DISP_1DOT = 1 << MODEL_MISC_DISP_1DOT_SHIFT,
    MODEL_MISC_FAR_CLIPPING = 1 << MODEL_MISC_FAR_CLIPPING_SHIFT,
    MODEL_MISC_XLU_DEPTH_UPDATE = 1 << MODEL_MISC_XLU_DEPTH_UPDATE_SHIFT,
    MODEL_ATTRIBUTES_LAST_BIT = 1 << MODEL_ATTRIBUTES_LAST_SHIFT
};

void ModelAttributes_ApplyGlobal(ModelAttributes *modelAttrs, int attributes);

void ModelAttributes_ApplyGlobal(ModelAttributes *modelAttrs, int attributes) {
    for (int i = MODEL_LIGHT_VECTORS_SHIFT; i < MODEL_LIGHT_VECTORS_SHIFT + 4; i++) {
        if (attributes & (1 << i)) {
            NNS_G3dGlbLightVector((GXLightId)(i), modelAttrs->lightVectors[i].x, modelAttrs->lightVectors[i].y, modelAttrs->lightVectors[i].z);
        }

        if (attributes & (1 << (i + 4))) {
            NNS_G3dGlbLightColor((GXLightId)(i), modelAttrs->lightColors[i]);
        }
    }

    if (attributes & (MODEL_DIFFUSE_REFLECT_COLOR | MODEL_AMBIENT_REFLECT_COLOR)) {
        NNS_G3dGlbMaterialColorDiffAmb(
            modelAttrs->diffuse, modelAttrs->ambient, modelAttrs->setDiffuseColorAsVertexColor);
    }

    if (attributes & (MODEL_SPECULAR_REFLECT_COLOR | MODEL_EMISSION_COLOR)) {
        NNS_G3dGlbMaterialColorSpecEmi(
            modelAttrs->specular, modelAttrs->emission, modelAttrs->enableSpecularReflectShininessTable);
    }

    if (attributes >= MODEL_POLYGON_ATTRS) {
        NNS_G3dGlbPolygonAttr(modelAttrs->lightMask, modelAttrs->polygonMode, modelAttrs->cullMode, modelAttrs->id, modelAttrs->alpha, modelAttrs->miscFlags);
    }
}
