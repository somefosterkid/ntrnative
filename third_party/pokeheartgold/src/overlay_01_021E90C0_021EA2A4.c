#include "global.h"

#include "field/area_light.h"
#include "field/model_attributes.h"

#include "gf_rtc.h"

void ov01_021EA398(AreaLightManager *areaLightMan);

void ov01_021EAAB8(ModelAttributes *modelAttrs, int index, fx16 x, fx16 y, fx16 z);
void ov01_021EAAE0(ModelAttributes *modelAttrs, int index, GXRgb lightColor);
void ov01_021EAAF4(ModelAttributes *modelAttrs, GXRgb diffuseReflectColor, BOOL setDiffuseColorAsVertexColor, BOOL applyGlobal);
void ov01_021EAB08(ModelAttributes *modelAttrs, GXRgb ambientReflectColor, BOOL applyGlobal);
void ov01_021EAB1C(ModelAttributes *modelAttrs, GXRgb specularReflectColor, BOOL enableSpecularReflectShininessTable, BOOL applyGlobal);
void ov01_021EAB30(ModelAttributes *modelAttrs, GXRgb emissionColor, BOOL applyGlobal);
void ov01_021EA300(const AreaLightTemplate *template, ModelAttributes *modelAttrs);
void ov01_021EA300(const AreaLightTemplate *template, ModelAttributes *modelAttrs);
void ov01_021EA3B0(NNSG3dResMdl *model);

void AreaLightManager_UpdateActiveTemplate(AreaLightManager *areaLightMan) {
    ((areaLightMan) ? (void)0 : GF_AssertFail());

    int currentTime = GF_RTC_TimeToSec() / 2;

    if (areaLightMan->templateCount > 1) {
        int previousEndTime = areaLightMan->activeTemplateIndex - 1 >= 0
            ? areaLightMan->templates[areaLightMan->activeTemplateIndex - 1].endTime
            : 0;

        int activeEndTime = areaLightMan->templates[areaLightMan->activeTemplateIndex].endTime;

        if (currentTime >= activeEndTime || currentTime < previousEndTime) {
            areaLightMan->activeTemplateIndex++;

            if (areaLightMan->activeTemplateIndex >= areaLightMan->templateCount) {
                areaLightMan->activeTemplateIndex = 0;
            }

            if (areaLightMan->applyToAreaModelAttrs) {
                ov01_021EA398(areaLightMan);
            }
        }
    }
}

void ov01_021EA300(const AreaLightTemplate *template, ModelAttributes *modelAttrs) {
    for (int i = 0; i < 4; i++) {
        int currentLightMask = (1 << i);

        if ((template->validLightsMask & currentLightMask) != 0) {
            ov01_021EAAB8(modelAttrs, i, template->lightVectors[i].x, template->lightVectors[i].y, template->lightVectors[i].z);
            ov01_021EAAE0(modelAttrs, i, template->lightColors[i]);
        } else {
            ov01_021EAAB8(modelAttrs, i, 0, 0, 0);
            ov01_021EAAE0(modelAttrs, i, ((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10)))));
        }
    }

    ov01_021EAAF4(modelAttrs, template->diffuseReflectColor, 0, 0);
    ov01_021EAB08(modelAttrs, template->ambientReflectColor, 1);
    ov01_021EAB1C(modelAttrs, template->specularReflectColor, 0, 0);
    ov01_021EAB30(modelAttrs, template->emissionColor, 1);
}

void ov01_021EA398(AreaLightManager *areaLightMan) {
    AreaLightTemplate *activeTemplate = &areaLightMan->templates[areaLightMan->activeTemplateIndex];
    ov01_021EA300(activeTemplate, areaLightMan->areaModelAttrs);
}

void ov01_021EA3B0(NNSG3dResMdl *model) {
    NNS_G3dMdlUseGlbDiff(model);
    NNS_G3dMdlUseGlbAmb(model);
    NNS_G3dMdlUseGlbSpec(model);
    NNS_G3dMdlUseGlbEmi(model);
}
