#ifndef POKEHEARTGOLD_FIELD_AREA_LIGHT_H
#define POKEHEARTGOLD_FIELD_AREA_LIGHT_H

#include "field/model_attributes.h"

typedef struct AreaLightTemplate {
    u32 endTime;
    u8 validLightsMask;
    GXRgb lightColors[4];
    VecFx16 lightVectors[4];
    GXRgb diffuseReflectColor;
    GXRgb ambientReflectColor;
    GXRgb specularReflectColor;
    GXRgb emissionColor;
} AreaLightTemplate;

typedef struct AreaLightManager {
    u32 templateCount;
    AreaLightTemplate *templates;
    int activeTemplateIndex;
    ModelAttributes *areaModelAttrs;
    BOOL applyToAreaModelAttrs;
} AreaLightManager;

AreaLightManager *AreaLightManager_New(ModelAttributes *areaModelAttributes, const u32 archiveID);
void AreaLightManager_Free(AreaLightManager **areaLightManager);
void AreaLightManager_UpdateActiveTemplate(AreaLightManager *areaLightManager);
void ov01_021EA300(const AreaLightTemplate *template, ModelAttributes *modelAttrs);
void ov01_021EA398(AreaLightManager *areaLightMan);
u32 ov01_021EA3E0(const char *path, AreaLightTemplate **templatesOut);
void ov01_021EA564(AreaLightTemplate **templates);

#endif // POKEHEARTGOLD_FIELD_AREA_LIGHT_H
