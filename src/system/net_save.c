// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/net_save.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/net_save.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/random.h"
#include "system/game_data.h"

// Saving the game on two machines at once, each waiting for the other between the steps so that both saves end
// together or not at all. The name is the ROM's string.

// The timing numbers of the steps
enum {
    NET_SAVE_TIMING_START = 0x71,
    NET_SAVE_TIMING_WRITTEN,
    NET_SAVE_TIMING_WAITED,
    NET_SAVE_TIMING_END
};

#define NET_SAVE_TIMING_GROUP 8
#define NET_SAVE_WAIT_MAX 60

typedef void (*NetSaveState)(NetSave *save);

struct NetSave {
    void (*func)(void *work);
    void *work;
    NetSaveState state;
    GameData *gameData;
    int wait;
};

static void func_02010888(NetSave *save, NetSaveState state);
static void func_0201093c(NetSave *save);
static void func_02010964(NetSave *save);
static void func_02010994(NetSave *save);
static void func_020109b8(NetSave *save);
static void func_020109e0(NetSave *save);
static void func_02010a04(NetSave *save);
static void func_02010a3c(NetSave *save);

// pokebw2's setState
static void func_02010888(NetSave *save, NetSaveState state) {
    save->state = state;
}

NetSave *func_0201088c(HeapID heapId, GameData *gameData) {
    NetSave *save = GFL_HeapAllocate(heapId, sizeof(NetSave), TRUE, "net_save.c", 117);

    save->gameData = gameData;
    func_02039c68(func_020399a8(), NET_SAVE_TIMING_START, NET_SAVE_TIMING_GROUP);
    func_02010888(save, func_0201093c);
    return save;
}

NetSave *func_020108c4(HeapID heapId, GameData *gameData, void (*func)(void *work), void *work) {
    NetSave *save = GFL_HeapAllocate(heapId, sizeof(NetSave), TRUE, "net_save.c", 135);

    save->func = func;
    save->work = work;
    save->gameData = gameData;
    func_02039c68(func_020399a8(), NET_SAVE_TIMING_START, NET_SAVE_TIMING_GROUP);
    func_02010888(save, func_0201093c);
    return save;
}

BOOL func_02010904(NetSave *save) {
    if (save->state != NULL) {
        save->state(save);
    }
    if (func_0200fb24()) {
        if (save->gameData != NULL) {
            func_02012e08(save->gameData);
        }
        return TRUE;
    }
    if (save->state == NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_02010934(NetSave *save) {
    GFL_HeapFree(save);
}

// pokebw2's stateStart
static void func_0201093c(NetSave *save) {
    if (func_02039cb4(func_020399a8(), NET_SAVE_TIMING_START, NET_SAVE_TIMING_GROUP)) {
        func_02012dac(save->gameData);
        func_02010888(save, func_02010964);
    }
}

// Writes all but the end of the save, then waits a random time, so that the two machines don't end together
// pokebw2's stateWrite
static void func_02010964(NetSave *save) {
    if (func_02012dd0(save->gameData) == 1) {
        func_02039c68(func_020399a8(), NET_SAVE_TIMING_WRITTEN, NET_SAVE_TIMING_GROUP);
        save->wait = GFL_RandomLC(NET_SAVE_WAIT_MAX);
        func_02010888(save, func_02010994);
    }
}

// pokebw2's stateWritten
static void func_02010994(NetSave *save) {
    if (func_02039cb4(func_020399a8(), NET_SAVE_TIMING_WRITTEN, NET_SAVE_TIMING_GROUP)) {
        func_02010888(save, func_020109b8);
    }
}

// pokebw2's stateWait
static void func_020109b8(NetSave *save) {
    save->wait--;
    if (save->wait < 0) {
        func_02039c68(func_020399a8(), NET_SAVE_TIMING_WAITED, NET_SAVE_TIMING_GROUP);
        func_02010888(save, func_020109e0);
    }
}

// pokebw2's stateWaited
static void func_020109e0(NetSave *save) {
    if (func_02039cb4(func_020399a8(), NET_SAVE_TIMING_WAITED, NET_SAVE_TIMING_GROUP)) {
        func_02010888(save, func_02010a04);
    }
}

// pokebw2's stateFinish
static void func_02010a04(NetSave *save) {
    if (save->func != NULL) {
        save->func(save->work);
    }
    if (func_02012dd0(save->gameData) == 2) {
        save->gameData = NULL;
        func_02039c68(func_020399a8(), NET_SAVE_TIMING_END, NET_SAVE_TIMING_GROUP);
        func_02010888(save, func_02010a3c);
    }
}

// pokebw2's stateEnd
static void func_02010a3c(NetSave *save) {
    if (func_02039cb4(func_020399a8(), NET_SAVE_TIMING_END, NET_SAVE_TIMING_GROUP)) {
        func_02010888(save, NULL);
    }
}
