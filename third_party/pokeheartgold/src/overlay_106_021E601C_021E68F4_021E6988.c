#include "global.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"

typedef s32 (*UnkFunc_ov106_021E6988)(LegendaryCinematic *cinematic, s32 *script);

extern const UnkFunc_ov106_021E6988 ov106_021E7044[];

extern const s32 ov106_021E7090[];

u32 ov106_021E6988(LegendaryCinematic *cinematic);

u32 ov106_021E6988(LegendaryCinematic *cinematic) {
    s32 **pScript;
    ArceusCinematic *data = cinematic->arceus;
    pScript = &data->scriptPos;
    s32 *script;
    s32 ret;

    while (TRUE) {
        script = data->scriptPos;
        if (*script == 0x13) {
            return 1;
        }
        ret = ov106_021E7044[*script](cinematic, script);
        if (ret == 0) {
            break;
        }
        if (ret == 1) {
            *pScript += ov106_021E7090[*data->scriptPos];
        } else if (ret == 2) {
            data->scriptPos += ov106_021E7090[*data->scriptPos];
            break;
        }
    }
    return cinematic->state;
}
