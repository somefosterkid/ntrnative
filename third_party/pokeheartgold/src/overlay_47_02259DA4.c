#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_45_wifi_plaza_system.h"

u32 ov47_02259DA4(const UnkStruct_ov68_0225DC74 *param0, u32 param1, u32 param2);

struct UnkStruct_ov66_0222DFF8_t;
struct WiFiPlazaProfile;
typedef enum {
    UnkEnum_ov66_022335C0_00,
    UnkEnum_ov66_022335C0_01,
    UnkEnum_ov66_022335C0_02,
} UnkEnum_ov66_022335C0;
const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);
u8 ov45_0222AA54(const WiFiPlazaProfile *profile);
void ov45_0222AB0C(const WiFiPlazaProfile *profile, UnkStruct_ov66_0222E908 *param1);
u8 ov45_0222F6B0(UnkEnum_ov66_022335C0 param0, u32 language);
void ov47_02259DCC(UnkStruct_ov68_0225DC74 *param0, const WiFiPlazaSystem *param1);

u32 ov47_02259DA4(const UnkStruct_ov68_0225DC74 *param0, u32 param1, u32 param2) {
    u64 v0, v1;

    GF_ASSERT(param1 < 3);

    v0 = param2;
    v1 = param0->unk_00[param1];

    return (u32)(((u64)(v0 * v1)) / param0->unk_0C);
}

void ov47_02259DCC(UnkStruct_ov68_0225DC74 *param0, const WiFiPlazaSystem *param1) {
    const WiFiPlazaProfile *v0;
    UnkStruct_ov66_0222E908 v1;
    int v2;
    u32 language;
    BOOL v4;

    memset(param0, 0, sizeof(UnkStruct_ov68_0225DC74));

    for (v2 = 0; v2 < 20; v2++) {
        v0 = ov45_0222A578(param1, v2);

        if (v0 != NULL) {
            language = ov45_0222AA54(v0);
            ov45_0222AB0C(v0, &v1);
            v4 = ov45_0222F6B0(UnkEnum_ov66_022335C0_00, language);

            if ((v4 == 1) && (v1.selectedAnswer < 3)) {
                param0->unk_00[v1.selectedAnswer]++;
                param0->unk_0C++;
            }
        }
    }
}
