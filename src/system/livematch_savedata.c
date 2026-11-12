// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/livematch_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/records.h"
#include "app/livebattlematch_flow.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "nitro/os.h"

// The results of the live competition, which the record block (records.h) starts with: the console's MAC address,
// four counts and the last opponents. The ROM has no string for the file, so the name is a guess.

static void func_0200e938(LiveMatchData *data, u32 index, u8 *macAddress);

void func_0200e904(void *results) {
    LiveMatchData *data = results;

    MI_CpuFill8(data, 0, sizeof(LiveMatchData));
    OS_GetMacAddress(data->macAddress);
}

u32 func_0200e918(void *results, u32 index) {
    LiveMatchData *data = results;

    return data->counts[index];
}

void func_0200e920(void *results, u32 index, int value) {
    LiveMatchData *data = results;
    int count = data->counts[index] + value;

    if (count > LIVEMATCH_COUNT_MAX) {
        count = LIVEMATCH_COUNT_MAX;
    } else if (count < 0) {
        count = 0;
    }
    data->counts[index] = count;
}

// pokebw2's getOpponentMacAddress
static void func_0200e938(LiveMatchData *data, u32 index, u8 *macAddress) {
    MI_CpuCopy8(data->opponents[index % LIVEMATCH_OPPONENT_MAX].macAddress, macAddress, BATTLEMATCH_MAC_ADDRESS_SIZE);
}

void func_0200e958(void *results, u32 index, const u8 *macAddress) {
    LiveMatchData *data = results;

    MI_CpuCopy8(macAddress, data->opponents[index % LIVEMATCH_OPPONENT_MAX].macAddress, BATTLEMATCH_MAC_ADDRESS_SIZE);
}

void func_0200e978(void *results, u32 index, u32 field, u8 value) {
    LiveMatchData *data = results;

    data->opponents[index % LIVEMATCH_OPPONENT_MAX].params[field] = value;
}

BOOL func_0200e994(void *results, const u8 *macAddress) {
    LiveMatchData *data = results;
    u8 opponent[BATTLEMATCH_MAC_ADDRESS_SIZE];
    int i;
    int count = func_0200e918(data, 0);

    if (count > LIVEMATCH_OPPONENT_MAX) {
        count = LIVEMATCH_OPPONENT_MAX;
    }
    for (i = 0; i < count; i++) {
        func_0200e938(data, i, opponent);
        if (GFL_STD_MemCmp(macAddress, opponent, BATTLEMATCH_MAC_ADDRESS_SIZE) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
