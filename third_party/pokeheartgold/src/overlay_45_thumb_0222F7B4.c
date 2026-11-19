#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "save.h"
#include "constants/heap.h"
#include "heap.h"
#include "unk_0202C034.h"

struct WiFiHistory;

struct WiFiList;

PPW_LOBBY_RESULT ov45_022322A4(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char *key, const u8 *data, u32 dataSize);

typedef struct WiFiHistory WiFiHistory;

u8 WifiHistory_GetPlayerCountry(const WiFiHistory *wiFiHistory);

u8 WiFiHistory_GetPlayerRegion(const WiFiHistory *wiFiHistory);

BOOL ov45_0222F74C(PPW_LOBBY_RESULT param0);

extern const char *ov45_02254EF0[1];

BOOL ov45_0222F7B4(UnkStruct_ov66_022336C4 *param0);

void ov45_0222F848(void *param_1, s32 param_2, enum HeapID param_3);

BOOL ov45_0222F7B4(UnkStruct_ov66_022336C4 *param0) {
    u32 country = WifiHistory_GetPlayerCountry(param0->wiFiHistory);
    u32 region = WiFiHistory_GetPlayerRegion(param0->wiFiHistory);

    if (country == 0) {
        return 0;
    }

    int v2;

    for (v2 = 0; v2 < 20; v2++) {
        if (param0->unk_134[v2].unk_03 == 0) {
            break;
        }

        if ((param0->unk_134[v2].country == country) && (param0->unk_134[v2].region == region)) {
            return 0;
        }
    }

    if (v2 >= 20) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    param0->unk_134[v2].country = country;
    param0->unk_134[v2].region = region;
    param0->unk_134[v2].unk_03 = 1;

    PPW_LOBBY_RESULT lobbyResult = ov45_022322A4(-1, PPW_LOBBY_CHANNEL_KIND_MAIN, ov45_02254EF0[0], (const u8 *)param0->unk_134, sizeof(UnkStruct_ov66_022336C4_sub2) * 20);

    return ov45_0222F74C(lobbyResult);
}

void ov45_0222F848(void *param_1, s32 param_2, enum HeapID param_3) {
    void *pvVar1;

    pvVar1 = Heap_Alloc(param_3, param_2 * 0x14);
    _022577C0->unk_24 = pvVar1;
    memset(_022577C0->unk_24, 0, param_2 * 0x14);
    _022577C0->unk_28 = param_2;
    return;
}
