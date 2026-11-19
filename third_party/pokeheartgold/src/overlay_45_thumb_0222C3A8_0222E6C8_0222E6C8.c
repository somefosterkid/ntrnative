#include "global.h"

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

u8 ov45_0222E96C(void);

void ov45_022302E4(UnkStruct_ov66_022336C4 *unused);

u32 ov45_02231514();

s32 ov45_022320C4(PPW_LobbyTimeInfo *);

u32 ov45_022321E0();

s32 ov45_0222E6C8(void);

s32 ov45_0222E6C8(void) {
    u32 v0;
    u32 v1;

    if (_022577C0 == NULL) {
        return 0;
    }
    v0 = ov45_02231514();
    switch (v0) {
    case 0:
        _022577C0->unk_12C = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        _022577C0->unk_12C = 1;
        break;
    case 8:
        _022577C0->unk_12C = 2;
        break;
    case 9:
        _022577C0->unk_12C = 3;
        break;
    case 10:
        _022577C0->unk_12C = 4;
        break;
    }
    if (_022577C0->unk_12C == 2) {
        ov45_022302E4(_022577C0);
        ov45_022320C4(&_022577C0->timeInfo);
        v1 = ov45_022321E0();
        switch (v1) {
        case 0:
            _022577C0->unk_12E = 0;
            _022577C0->unk_12D = 4;
            break;
        case 1:
            _022577C0->unk_12E = 1;
            break;
        case 2:
            if (_022577C0->unk_12E != 3) {
                _022577C0->unk_12E = 2;
            }
            break;
        case 3:
            _022577C0->unk_12E = 4;
            break;
        }
    }
    return (s32)ov45_0222E96C();
}
