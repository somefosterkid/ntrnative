#include "global.h"

#include "field/model_attributes.h"

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

void ModelAttributes_SetPolygonMode(ModelAttributes *modelAttributes, GXPolygonMode polygonMode, BOOL applyGlobal) {
    modelAttributes->polygonMode = polygonMode;
    if (applyGlobal == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x2000);
    }
}

void ModelAttributes_SetCullMode(ModelAttributes *modelAttributes, GXCull cullMode, BOOL applyGlobal) {
    modelAttributes->cullMode = cullMode;
    if (applyGlobal == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x4000);
    }
}

void ModelAttributes_SetAlpha(ModelAttributes *modelAttributes, int alpha, BOOL applyGlobal) {
    modelAttributes->alpha = alpha;
    if (applyGlobal == 1) {
        ModelAttributes_ApplyGlobal(modelAttributes, 0x10000);
    }
}

void ModelAttributes_SetMiscAttrEnabled(ModelAttributes *modelAttrs, int miscFlagsMask, BOOL enable, BOOL applyGlobal) {
    if (enable == TRUE) {
        if (modelAttrs->miscFlags & miscFlagsMask) {
            return;
        }
    } else {
        if (!(modelAttrs->miscFlags & miscFlagsMask)) {
            return;
        }
    }

    modelAttrs->miscFlags ^= miscFlagsMask;

    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttrs, MODEL_POLYGON_ATTRS);
    }
}
