#include "global.h"

#include "field/area_light.h"
#include "field/model_attributes.h"

#include "ascii_util.h"
#include "error_handling.h"
#include "gf_rtc.h"
#include "heap.h"
#include "system.h"

extern const char *const ov01_02206450[5];

void LoadAreaOrDungeonLightTxt(u32 archiveID, ModelAttributes *modelAttributes);

void LoadAreaOrDungeonLightTxt(u32 archiveID, ModelAttributes *modelAttributes) {
    AreaLightTemplate *entries;
    s32 count;
    s32 sec;
    s32 i;
    u32 index;
    AreaLightTemplate *entry;

    ((archiveID < 5) ? (void)0 : GF_AssertFail());
    count = ov01_021EA3E0(ov01_02206450[archiveID], &entries);
    sec = GF_RTC_TimeToSec() / 2;
    i = 0;
    if (count > 0) {
        entry = entries;
        do {
            if (entry->endTime > (u32)sec) {
                index = i;
                break;
            }
            i++;
            entry++;
        } while (i < count);
    }
    ov01_021EA300(&entries[index], modelAttributes);
    ov01_021EA564(&entries);
}
