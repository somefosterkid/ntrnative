#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

typedef struct WiFiHistory WiFiHistory;

struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
};

BOOL ov45_0222F7B4(UnkStruct_ov66_022336C4 *param0);

s32 ov45_0222ED7C(void);

s32 ov45_0222ED7C(void) {
    s32 v0;

    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    v0 = ov45_0222F7B4(_022577C0);
    if (v0 != 0) {
        _022577C0->unk_130 = 1;
    }
    return v0;
}
