#include "global.h"

#include "safari_zone.h"
#include "unk_02005D10.h"
#include "overlay_108_safari_area_customizer.h"

struct SafariZoneAreaSet;

void ov108_021E6B00(SafariAreaCustomizer *data);

void ov108_021E79A8(SafariAreaCustomizer *arg0, u8 arg1, s32 arg2, s32 arg3);

void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);

int ov108_021E6450(SafariAreaCustomizer *data);

int ov108_021E6450(SafariAreaCustomizer *data) {
    PlaySE(0x5DC);
    if ((data->unk_184E3) != 0) {
        ov108_021E6B00(data);
        return 1;
    }
    u8 areaNo = (u8)((data->cursor) + (data->page) * 6);
    (data->areaChanged) = 1;
    SafariZone_InitAreaInSet(&data->areaSet, (data->areaIdx), areaNo);
    u32 idx = (data->areaIdx);
    ov108_021E7BB4(data, idx, (data->areaSet.areas[idx].area_no));
    ov108_021E79A8(data, 1, 0, 0);
    return 2;
}
