#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyVipRecord;

struct WiFiHistory;

struct WiFiList;

typedef enum {
    UnkEnum_ov66_022336B8_00,
    UnkEnum_ov66_022336B8_01,
    UnkEnum_ov66_022336B8_02,
    UnkEnum_ov66_022336B8_03,
    UnkEnum_ov66_022336B8_04,
} UnkEnum_ov66_022336B8;

typedef struct WiFiHistory WiFiHistory;

UnkEnum_ov66_022336B8 ov45_0222F7A8(PPW_LOBBY_CHANNEL_KIND lobbyChannelKind);

s32 ov45_0222FB24(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);

void ov45_0222FDD8(s32 param0, PPW_LOBBY_CHANNEL_KIND param1, s32 param2, const u8 *param3, u32 param4);

void ov45_0222FDD8(s32 param0, PPW_LOBBY_CHANNEL_KIND param1, s32 param2, const u8 *param3, u32 param4) {
    s32 v2 = ov45_0222FB24(_022577C0, param0, 0);

    if (v2 == 0xffffffff) {
        return;
    }

    u32 v0;

    switch (param1) {
    case PPW_LOBBY_CHANNEL_KIND_MAIN:
        v0 = 0;
        break;
    case PPW_LOBBY_CHANNEL_KIND_FOOT1:
    case PPW_LOBBY_CHANNEL_KIND_FOOT2:
    case PPW_LOBBY_CHANNEL_KIND_CLOCK:
    case PPW_LOBBY_CHANNEL_KIND_NEWS:
        u32 v1 = ov45_0222F7A8(param1);

        if (_022577C0->unk_12D == v1) {
            v0 = 1;
        } else {
            return;
        }
        break;
    case PPW_LOBBY_CHANNEL_KIND_INVALID:
        v0 = 0;
        break;
    default:
        return;
    }

    if (_022577C0->unk_184[v0].unk_04 > param2) {
        GF_ASSERT(_022577C0->unk_184[v0].unk_00[param2].unk_04 <= param4);
        _022577C0->unk_184[v0].unk_00[param2].unk_00(param0, param3, _022577C0->unk_184[v0].unk_00[param2].unk_04, _022577C0->unk_184[v0].unk_08);
    } else {
        (void)0;
    }
}
