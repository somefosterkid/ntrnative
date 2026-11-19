#include "global.h"

#include "error_handling.h"
#include "overlay_00_thumb.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyVipRecord;

struct WiFiHistory;

typedef enum {
    UnkEnum_ov66_02232F38_00,
    UnkEnum_ov66_02232F38_01,
    UnkEnum_ov66_02232F38_02,
    UnkEnum_ov66_02232F38_03,
} UnkEnum_ov66_02232F38;

typedef struct WiFiHistory WiFiHistory;

typedef void (*NintendoWFCNewClientCallback)(u16 id, void *userParam);

void ov00_021E5CD0(NintendoWFCNewClientCallback callback, void *callbackArg);

int ov00_021E5CEC(u8 *matchmakingString, int maxPlayers, BOOL addSecondKey, u32 timeout);

BOOL ov45_0223021C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);

void ov45_02230390(u16 sLotoId, void *unused2);

void ov45_02230394(s32 param0, void *param1);

BOOL ov45_0222F350(UnkEnum_ov66_02232F38 param0);

BOOL ov45_0222F350(UnkEnum_ov66_02232F38 param0) {
    GF_ASSERT(_022577C0 != NULL);

    if (ov45_0223021C(_022577C0, param0) == 0) {
        return 0;
    }

    if (_022577C0->gameKind != UnkEnum_ov66_02232F38_03) {
        return 0;
    }

    if (_022577C0->unk_5B8[param0].lobbyRecruitInfo.paramVal == 1) {
        return 0;
    }

    _022577C0->gameKind = param0;

    if (ov00_021E5CEC((u8 *)_022577C0->unk_5B8[param0].lobbyRecruitInfo.matchMakingString, _022577C0->unk_5B8[param0].lobbyRecruitInfo.maxNum, 0, _022577C0->unk_5B8[param0].unk_46 / 30)) {
        (void)0;
    }

    ov00_021E5CD0(ov45_02230390, _022577C0);
    ov00_021E5CA0(ov45_02230394, _022577C0);

    return 1;
}
