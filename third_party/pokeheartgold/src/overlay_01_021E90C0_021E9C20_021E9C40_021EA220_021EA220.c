#include "global.h"

#include "field/area_light.h"
#include "field/model_attributes.h"

#include "ascii_util.h"
#include "error_handling.h"
#include "gf_rtc.h"
#include "heap.h"
#include "system.h"

void ov01_021EA398(AreaLightManager *areaLightManager);

extern const char *const ov01_02206450[5];

AreaLightManager *AreaLightManager_New(ModelAttributes *areaModelAttributes, const u32 archiveID) {
    AreaLightManager *areaLightManager;
    s32 sec;
    u32 i;
    AreaLightTemplate *entry;

    ((archiveID < 5) ? (void)0 : GF_AssertFail());
    areaLightManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaLightManager));
    areaLightManager->areaModelAttrs = areaModelAttributes;
    areaLightManager->templateCount = ov01_021EA3E0(ov01_02206450[archiveID], &areaLightManager->templates);
    areaLightManager->activeTemplateIndex = 0;
    sec = GF_RTC_TimeToSec() / 2;
    i = 0;
    if (i < areaLightManager->templateCount) {
        entry = areaLightManager->templates;
        do {
            if (entry->endTime > (u32)sec) {
                areaLightManager->activeTemplateIndex = i;
                break;
            }
            i++;
            entry++;
        } while (i < areaLightManager->templateCount);
    }
    areaLightManager->applyToAreaModelAttrs = 1;
    ov01_021EA398(areaLightManager);
    return areaLightManager;
}
