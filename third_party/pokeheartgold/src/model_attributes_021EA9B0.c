#include "global.h"

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

void ov01_021EA9B0(ModelAttributes *modelAttrs, NNSG3dResMdl *model, int attributes);

void ov01_021EA9B0(ModelAttributes *modelAttrs, NNSG3dResMdl *model, int attributes) {
    if (attributes & MODEL_DIFFUSE_REFLECT_COLOR) {
        NNS_G3dMdlSetMdlDiffAll(model, modelAttrs->diffuseReflectColor);
    }

    if (attributes & MODEL_AMBIENT_REFLECT_COLOR) {
        NNS_G3dMdlSetMdlAmbAll(model, modelAttrs->ambientReflectColor);
    }

    if (attributes & MODEL_SPECULAR_REFLECT_COLOR) {
        NNS_G3dMdlSetMdlSpecAll(model, modelAttrs->specularReflectColor);
    }

    if (attributes & MODEL_EMISSION_COLOR) {
        NNS_G3dMdlSetMdlEmiAll(model, modelAttrs->emissionColor);
    }

    if (attributes & MODEL_LIGHT_ENABLE_FLAGS) {
        NNS_G3dMdlSetMdlLightEnableFlagAll(model, modelAttrs->enabledLightsMask);
    }

    if (attributes & MODEL_POLYGON_MODE) {
        NNS_G3dMdlSetMdlPolygonModeAll(model, modelAttrs->polygonMode);
    }

    if (attributes & MODEL_CULL_MODE) {
        NNS_G3dMdlSetMdlCullModeAll(model, modelAttrs->cullMode);
    }

    if (attributes & MODEL_POLYGON_ID) {
        NNS_G3dMdlSetMdlPolygonIDAll(model, modelAttrs->polygonID);
    }

    if (attributes & MODEL_ALPHA) {
        NNS_G3dMdlSetMdlAlphaAll(model, modelAttrs->alpha);
    }

    if (attributes & MODEL_MISC_FOG) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(model, modelAttrs->miscFlags & GX_POLYGON_ATTR_MISC_FOG);
    }

    if (attributes & MODEL_MISC_DEPTHTEST_DECAL) {
        NNS_G3dMdlSetMdlDepthTestCondAll(model, modelAttrs->miscFlags & GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL);
    }

    if (attributes & MODEL_MISC_DISP_1DOT) {
        NNS_G3dMdlSetMdl1DotAll(model, modelAttrs->miscFlags & GX_POLYGON_ATTR_MISC_DISP_1DOT);
    }

    if (attributes & MODEL_MISC_FAR_CLIPPING) {
        NNS_G3dMdlSetMdlFarClipAll(model, modelAttrs->miscFlags & GX_POLYGON_ATTR_MISC_FAR_CLIPPING);
    }

    if (attributes & MODEL_MISC_XLU_DEPTH_UPDATE) {
        NNS_G3dMdlSetMdlXLDepthUpdateAll(model, modelAttrs->miscFlags & GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE);
    }
}
