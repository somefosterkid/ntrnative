// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/game_beacon_set.c, ported by tools/port_file.py.
#include "types.h"
#include "app/name_entry.h"
#include "field/game_beacon_set.h"
#include "gfl/str.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/str_tool.h"
#include "system/game_beacon.h"
#include "nitro/rtc.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "gfl/net.h"
#include "system/game_system.h"

typedef struct {
    u32 unk_0;
    GameBeacon beacon;
} GameBeaconData;

typedef struct {
    GameBeaconData mine;
    GameBeaconData other;
} GameBeaconSet;

extern GameBeaconSet *data_02146940;

static void func_ov010_021650bc(GameBeacon *info, u16 value);
void func_ov010_021650cc(u16 value);
static void func_ov010_021650f4(GameBeacon *info, u16 value);
void func_ov010_02165104(u16 value);
static void func_ov010_0216512c(GameBeacon *info, u16 value);
void func_ov010_0216513c(u16 value);
static void func_ov010_02165164(GameBeacon *info, u16 value);
static void func_ov010_0216519c(GameBeacon *info, u16 itemId);
static void func_ov010_021651d4(GameBeacon *info, u16 itemId);
static void func_ov010_0216520c(GameBeacon *info, u16 itemId);
static void func_ov010_02165244(GameBeacon *info, u16 itemId);
static void func_ov010_0216527c(GameBeacon *info, u16 itemId);
static void func_ov010_021652b4(GameBeacon *info, u16 itemId);
static void func_ov010_021652ec(GameBeacon *info, u32 passPower);
static void func_ov010_02165334(GameBeacon *info, u16 value);
static void func_ov010_02165368(GameBeacon *info);
static void func_ov010_021653b4(GameBeacon *info, u32 value);
static void func_ov010_021653e8(GameBeacon *info);
static void func_ov010_02165418(GameBeacon *info);
static void func_ov010_0216544c(GameBeacon *info, const StrBuf *name);
static void func_ov010_02165490(GameBeacon *info, const StrBuf *name);
static void func_ov010_021654d4(GameBeacon *info, const StrBuf *name);
static void func_ov010_02165518(GameBeacon *info, const StrBuf *name);
static void func_ov010_0216555c(GameBeacon *info, u16 value);
static void func_ov010_02165590(GameBeacon *info);
static void func_ov010_021655c4(GameBeacon *info, u16 species);
void func_ov010_021655f8(GameBeacon *info);
void func_ov010_02165628(GameBeacon *info);
static void func_ov010_0216565c(GameBeacon *info, u32 value);
void func_ov010_02165690(GameBeacon *info);
static void func_ov010_021656c4(GameBeacon *info);
void func_ov010_021656f8(GameBeacon *info, u8 value);
static void func_ov010_02165730(GameBeacon *info);
static void func_ov010_02165760(GameBeacon *info);
static void func_ov010_02165794(GameBeacon *info, const StrBuf *name);
static void func_ov010_021657d8(GameBeacon *info, u32 passPower);
static void func_ov010_02165814(GameBeacon *info, GameData *gameData, u16 value);

static const u8 data_ov010_02171ca0[5] = {10, 30, 50, 100, 200};


void func_ov010_02165094(u16 value);

typedef struct {
    GameSystem *gsys;
    GameCommSys *comm;
    u8 addr[6];
} GameBeaconSend;


char data_ov010_021729bc[] = "game_beacon_search.c";

void func_ov010_02164fd0(GameBeaconReq *req, int flag)
{
    u8 addr[6];
    GameSystem *gsys;
    GameCommSys *comm;
    GameBeaconSend *send;

    gsys = req->gsys;
    MI_CpuCopy8(req->addr, addr, 6);
    comm = GSYS_GetGameCommSystem(gsys);
    GFL_HeapFree(req);
    if (flag == 0 && func_02012164(gsys) == 1 && func_020264ac(comm) == 0 && func_0203cd98() == 0 && GameCommSys_BootCheck(comm) == 0)
    {
        send = GFL_HeapAllocate(5, sizeof(GameBeaconSend), TRUE, data_ov010_021729bc, 0x287);
        send->gsys = gsys;
        send->comm = GSYS_GetGameCommSystem(gsys);
        MI_CpuCopy8(addr, send->addr, 6);
        func_020263d8(comm, 2, send);
    }
}

void func_ov010_0216505c(u16 species) {
    if (func_0202708c(species)) {
        func_ov010_02165104(species);
    } else {
        func_ov010_02165094(species);
    }
}

void func_ov010_02165078(u16 species) {
    if (func_0202708c(species)) {
        func_ov010_0216513c(species);
    } else {
        func_ov010_021650cc(species);
    }
}


void func_02026fb8(GameBeacon *info);
void func_ov010_02165094(u16 value) {
    if (func_02027060(2)) {
        func_ov010_021650bc(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021650bc(GameBeacon *info, u16 value) {
    info->type = 2;
    info->arg.value = value;
    func_020273c4(info, value);
}

void func_ov010_021650cc(u16 value) {
    if (func_02027060(3)) {
        func_ov010_021650f4(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021650f4(GameBeacon *info, u16 value) {
    info->type = 3;
    info->arg.value = value;
    func_0202744c(info);
}

void func_ov010_02165104(u16 value) {
    if (func_02027060(4)) {
        func_ov010_0216512c(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216512c(GameBeacon *info, u16 value) {
    info->type = 4;
    info->arg.value = value;
    func_020273dc(info, value);
}

void func_ov010_0216513c(u16 value) {
    if (func_02027060(5)) {
        func_ov010_02165164(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165164(GameBeacon *info, u16 value) {
    info->type = 5;
    info->arg.value = value;
    func_0202744c(info);
}

void func_ov010_02165174(u16 itemId) {
    if (func_02027060(6)) {
        func_ov010_0216519c(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216519c(GameBeacon *info, u16 itemId) {
    info->type = 6;
    info->payload.value = itemId;
    func_020273f8(info, itemId);
}

void func_ov010_021651ac(u16 itemId) {
    if (func_02027060(7)) {
        func_ov010_021651d4(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021651d4(GameBeacon *info, u16 itemId) {
    info->type = 7;
    info->payload.value = itemId;
    func_0202744c(info);
}

void func_ov010_021651e4(u16 itemId) {
    if (func_02027060(8)) {
        func_ov010_0216520c(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216520c(GameBeacon *info, u16 itemId) {
    info->type = 8;
    info->payload.value = itemId;
    func_02027414(info, itemId);
}

void func_ov010_0216521c(u16 itemId) {
    if (func_02027060(9)) {
        func_ov010_02165244(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165244(GameBeacon *info, u16 itemId) {
    info->type = 9;
    info->payload.value = itemId;
    func_0202744c(info);
}

void func_ov010_02165254(u16 itemId) {
    if (func_02027060(0xa)) {
        func_ov010_0216527c(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216527c(GameBeacon *info, u16 itemId) {
    info->type = 10;
    info->payload.value = itemId;
    func_02027430(info, itemId);
}

void func_ov010_0216528c(u16 itemId) {
    if (func_02027060(0xb)) {
        func_ov010_021652b4(&data_02146940->mine.beacon, itemId);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021652b4(GameBeacon *info, u16 itemId) {
    info->type = 11;
    info->payload.value = itemId;
    func_0202744c(info);
}

void func_ov010_021652c4(u32 passPower) {
    if (func_02027060(0x12)) {
        func_ov010_021652ec(&data_02146940->mine.beacon, passPower);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021652ec(GameBeacon *info, u32 passPower) {
    info->type = 0x12;
    info->passPower = passPower;
    func_0202744c(info);
}

void func_ov010_0216530c(u16 value) {
    if (func_02027060(0x13)) {
        func_ov010_02165334(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165334(GameBeacon *info, u16 value) {
    info->type = 0x13;
    info->payload.value = value;
    func_0202744c(info);
}

void func_ov010_02165344(void) {
    if (func_02027060(0x15)) {
        func_ov010_02165368(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165368(GameBeacon *info) {
    info->type = 0x15;
    func_0202744c(info);
}

void func_ov010_02165374(u32 value) {
    u32 i;

    if (func_02027060(0x16)) {
        for (i = 0; i < 5; i++) {
            if (value == data_ov010_02171ca0[i]) {
                func_ov010_021653b4(&data_02146940->mine.beacon, value);
                func_02026fb8(&data_02146940->mine.beacon);
                return;
            }
        }
    }
}


static void func_ov010_021653b4(GameBeacon *info, u32 value) {
    info->type = 0x16;
    info->payload.value32 = value;
    func_0202744c(info);
}

void func_ov010_021653c4(void) {
    if (func_02027060(0x17)) {
        func_ov010_021653e8(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021653e8(GameBeacon *info) {
    info->type = 0x17;
    func_0202744c(info);
}

void func_ov010_021653f4(u16 value) {
    if (func_02027060(0x1e)) {
        func_ov010_02165418(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165418(GameBeacon *info) {
    info->type = 0x1e;
    func_0202744c(info);
}

void func_ov010_02165424(const StrBuf *name) {
    if (func_02027060(0x1f)) {
        func_ov010_0216544c(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216544c(GameBeacon *info, const StrBuf *name) {
    info->type = 0x1f;
    func_020270ac(name, info->payload.text);
    func_0202744c(info);
}

void func_ov010_02165468(const StrBuf *name) {
    if (func_02027060(0x20)) {
        func_ov010_02165490(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165490(GameBeacon *info, const StrBuf *name) {
    info->type = 0x20;
    func_020270ac(name, info->payload.text);
    func_0202744c(info);
}

void func_ov010_021654ac(const StrBuf *name) {
    if (func_02027060(0x21)) {
        func_ov010_021654d4(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021654d4(GameBeacon *info, const StrBuf *name) {
    info->type = 0x21;
    func_020270ac(name, info->payload.text);
    func_0202744c(info);
}

void func_ov010_021654f0(const StrBuf *name) {
    if (func_02027060(0x22)) {
        func_ov010_02165518(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165518(GameBeacon *info, const StrBuf *name) {
    info->type = 0x22;
    func_020270ac(name, info->payload.text);
    func_0202744c(info);
}

void func_ov010_02165534(u16 value) {
    if (func_02027060(0x24)) {
        func_ov010_0216555c(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216555c(GameBeacon *info, u16 value) {
    info->type = 0x24;
    info->payload.value = value;
    func_0202744c(info);
}

void func_ov010_0216556c(void) {
    if (func_02027060(0x25)) {
        func_ov010_02165590(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165590(GameBeacon *info) {
    info->type = 0x25;
    func_0202744c(info);
}

void func_ov010_0216559c(u16 species) {
    if (func_02027060(0x26)) {
        func_ov010_021655c4(&data_02146940->mine.beacon, species);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021655c4(GameBeacon *info, u16 species) {
    info->type = 0x26;
    info->arg.value = species;
    func_0202744c(info);
}

void func_ov010_021655d4(void) {
    if (func_02027060(0x27)) {
        func_ov010_021655f8(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


void func_ov010_021655f8(GameBeacon *info) {
    info->type = 0x27;
    func_0202744c(info);
}

void func_ov010_02165604(void) {
    if (func_02027060(0x28)) {
        func_ov010_02165628(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


void func_ov010_02165628(GameBeacon *info) {
    info->type = 0x28;
    func_0202744c(info);
}

void func_ov010_02165634(u32 value) {
    if (func_02027060(0x29)) {
        func_ov010_0216565c(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_0216565c(GameBeacon *info, u32 value) {
    info->type = 0x29;
    info->payload.value32 = value;
    func_0202744c(info);
}

void func_ov010_0216566c(void) {
    if (func_02027060(0x2a)) {
        func_ov010_02165690(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


void func_ov010_02165690(GameBeacon *info) {
    info->type = 0x2a;
    info->payload.value32 = 0;
    func_0202744c(info);
}

void func_ov010_021656a0(void) {
    if (func_02027060(0x2b)) {
        func_ov010_021656c4(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021656c4(GameBeacon *info) {
    info->type = 0x2b;
    func_0202744c(info);
}

void func_ov010_021656d0(u8 value) {
    if (func_02027060(0x2c)) {
        func_ov010_021656f8(&data_02146940->mine.beacon, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


void func_ov010_021656f8(GameBeacon *info, u8 value) {
    info->type = 0x2c;
    info->payload.value8 = value;
    func_0202744c(info);
}

void GameBeacon_BroadcastFerrisWheel(void) {
    if (func_02027060(0x2d)) {
        func_ov010_02165730(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165730(GameBeacon *info) {
    info->type = 0x2d;
    func_0202744c(info);
}

void func_ov010_0216573c(void) {
    if (func_02027060(0x2e)) {
        func_ov010_02165760(&data_02146940->mine.beacon);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165760(GameBeacon *info) {
    info->type = 0x2e;
    func_0202744c(info);
}

void func_ov010_0216576c(const StrBuf *name) {
    if (func_02027060(0x2f)) {
        func_ov010_02165794(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165794(GameBeacon *info, const StrBuf *name) {
    info->type = 0x2f;
    func_020270ac(name, info->payload.text);
    func_0202744c(info);
}

void func_ov010_021657b0(u32 passPower) {
    if (func_02027060(0x30)) {
        func_ov010_021657d8(&data_02146940->mine.beacon, passPower);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_021657d8(GameBeacon *info, u32 passPower) {
    info->type = 0x30;
    info->payload.value = passPower;
    func_0202744c(info);
}

void func_ov010_021657e8(GameData *gameData, u16 value) {
    if (func_02027060(0x18)) {
        func_ov010_02165814(&data_02146940->mine.beacon, gameData, value);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}


static void func_ov010_02165814(GameBeacon *info, GameData *gameData, u16 value) {
    wcharsncpy(func_0200c20c(getTrainerGameInfoAddress(GameData_GetSaveControl(gameData))), info->payload.named.name, 9);
    info->type = 0x18;
    info->payload.named.value = value;
    func_0202744c(info);
}

void func_ov010_0216586c(GameBeacon *info, StrBuf *name);
void func_ov010_02165844(StrBuf *name)
{
    if (func_02027060(0x31) != 0) {
        func_ov010_0216586c(&data_02146940->mine.beacon, name);
        func_02026fb8(&data_02146940->mine.beacon);
    }
}

void func_ov010_0216586c(GameBeacon *info, StrBuf *name)
{
    GFL_StrBufStoreString(name, info->payload.text, 9);
    info->type = 0x31;
    func_0202744c(info);
}

int func_ov010_02165960(GameBeaconInfoTable *table, GameBeacon *beacon, u16 value, BOOL overwrite);
int func_ov010_02165888(GameBeaconInfoTable *table)
{
    RTCTime time;
    int cursor = 0;
    int index;
    u16 packed;
    int count = 0;

    RTC_GetCachedTime(&time);
    packed = (time.hour << 8) | time.minute;
    while (TRUE)
    {
        index = func_02026ca0(&cursor);
        if (index == GAME_BEACON_LOG_MAX)
            break;
        if (func_ov010_02165960(table, func_02026c1c(index), packed, 0) == GAME_BEACON_LOG_MAX)
            break;
        func_02026ce0(index);
        count++;
    }
    return count;
}

BOOL func_ov010_021658d4(GameBeaconInfoTable *table, GameBeacon *dest, u16 *time)
{
    int i;

    if (table->beacons[0].targetVersions == 0)
        return FALSE;
    *dest = table->beacons[0];
    *time = table->times[0];
    for (i = 1; i < GAME_BEACON_LOG_MAX; i++)
    {
        table->beacons[i - 1] = table->beacons[i];
        table->times[i - 1] = table->times[i];
    }
    MI_CpuFill8(&table->beacons[GAME_BEACON_LOG_MAX - 1], 0, sizeof(GameBeacon));
    return TRUE;
}

void func_ov010_02165940(GameBeacon *info)
{
    int i;
    for (i = 0; i < 30; i++)
    {
        if (info[i].targetVersions == 0)
            break;
        func_02026d04(&info[i]);
    }
}

int func_ov010_02165960(GameBeaconInfoTable *table, GameBeacon *beacon, u16 value, BOOL overwrite)
{
    int i;

    for (i = 0; i < GAME_BEACON_LOG_MAX; i++)
    {
        if (table->beacons[i].targetVersions == 0 || beacon->trainerId == table->beacons[i].trainerId)
        {
            table->beacons[i] = *beacon;
            table->times[i] = value;
            return i;
        }
    }
    if (overwrite == TRUE)
    {
        MIi_CpuCopy32(&table->beacons[1], table->beacons, sizeof(GameBeacon) * (GAME_BEACON_LOG_MAX - 1));
        MIi_CpuCopy16(&table->times[1], table->times, sizeof(u16) * (GAME_BEACON_LOG_MAX - 1));
        table->beacons[GAME_BEACON_LOG_MAX - 1] = *beacon;
        table->times[GAME_BEACON_LOG_MAX - 1] = value;
        return GAME_BEACON_LOG_MAX - 1;
    }
    return GAME_BEACON_LOG_MAX;
}

u32 func_ov010_021659f0(GameBeaconInfoTable *table, s32 offset)
{
    s32 index = table->head;
    if (offset < 0)
        index = index + offset + 30;
    else
        index = index + offset;
    return index % 30;
}


u32 func_ov010_02165a10(GameBeaconInfoTable *table, GameBeacon *beacon, u16 value, u32 *notFound)
{
    int i;
    int index;

    if (notFound != NULL)
        *notFound = FALSE;
    for (i = 0; i < GAME_BEACON_LOG_MAX; i++)
    {
        index = func_ov010_021659f0(table, i);
        if (table->beacons[index].targetVersions == 0)
            break;
        if (beacon->trainerId == table->beacons[index].trainerId)
        {
            table->beacons[index] = *beacon;
            table->times[index] = value;
            return i;
        }
    }
    if (notFound != NULL)
        *notFound = TRUE;
    index = func_ov010_021659f0(table, -1);
    table->beacons[index] = *beacon;
    table->times[index] = value;
    table->head = index;
    if (table->count < GAME_BEACON_LOG_MAX)
        table->count++;
    return 0;
}

BOOL func_ov010_02165ac4(GameBeaconInfoTable *table, GameBeacon *dest, u16 *time, int offset)
{
    int index = func_ov010_021659f0(table, offset);
    if (table->beacons[index].targetVersions == 0)
        return FALSE;
    *dest = table->beacons[index];
    *time = table->times[index];
    return TRUE;
}


u8 func_ov010_02165b04(GameBeaconInfoTable *table)
{
    return table->count;
}

#include "gfl/heap.h"

void *func_ov010_02165b10(u16 line)
{
    return GFL_HeapAllocate(line, 0x5c, 1, "game_beacon_accessor.c", 0x1b3);
}

void *func_ov010_02165b2c(HeapID heapId)
{
    GameBeaconInfoTable *table = GFL_HeapAllocate(heapId, 0xb08, TRUE, "game_beacon_accessor.c", 0x1c1);
    table->head = 0;
    table->count = 0;
    return table;
}


