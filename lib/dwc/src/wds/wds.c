// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/wds/wds.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// A beacon scanner of DWC (TWL-DWC 5.3, ARM9 main), built on NitroSDK's wireless manager. The file name is a guess,
// from the text "WDS!" that its RC4 key starts with; the ROM names none of it. A scan collects the wireless parents
// whose beacons carry the game ID 0x857; the 0x70 bytes of game information of each one are decrypted with a key of
// "WDS!" and four bytes of the beacon's BSSID, and kept when their CRC-16 is right. Its callers are the scan of
// dwc_wdsscan.c and the connection of dwc_connectinet.c, which decrypts a beacon with func_0205a7cc on its own.
//
// The WDS_ names are ours, taken from what the code does.

#include "dwc/wds.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include <stddef.h>

// What the scan is doing
enum {
    WDS_STATE_IDLE,     // the wireless manager was started
    WDS_STATE_SCANNING, // a scan was started
    WDS_STATE_SCANNED,  // a scan came back, or was stopped; the entries can be read
    WDS_STATE_ENDING,   // the wireless manager is being ended
};

#define WDS_GAME_ID 0x857
#define WDS_BEACON_VERSION 1
// The part of a beacon's game information that comes before the user's data
#define WDS_GAME_INFO_HEADER_SIZE 0x10
#define WDS_SCAN_BUF_SIZE 0x400

// Black 2's; twinned code confirms the offsets of scanBuf, scanParam, callback, infos, signals, tgids, count, next,
// state, crc, crcTable and rc4 in Black
typedef struct WDSWork {
    u8 wmBuf[0xf00]; // the wireless manager's system buffer
    u8 scanBuf[WDS_SCAN_BUF_SIZE];
    WMScanExParam scanParam;
    WMCallbackFunc callback; // the caller's, called when a scan is done
    WDSBeaconInfo infos[WDS_MAX_ENTRIES];
    u16 signals[WDS_MAX_ENTRIES];
    u16 tgids[WDS_MAX_ENTRIES]; // the TGID of each beacon taken in, to take each parent in once
    int count;                  // how many entries are in use
    int next;                   // where the next beacon goes; the entries are a ring
    int state;
    u16 crc;
    MATHCRC16Table crcTable;
    CRYPTORC4Context rc4;
} WDSWork;

static u8 WDSGetRssi8(u8 rssi);
static void func_0205a2dc(void *arg);
static int func_0205a688(int index, WDSEntry *out);
static BOOL func_0205a8c8(const u8 *text);

// pokebw2's sWdsWork
static WDSWork *data_02146abc;

// The RC4 key's first four bytes, "WDS!" as a four-character constant (the ROM has it as "!SDW")
// pokebw2's sKeyPrefix
static const u32 data_020a1618 = 'WDS!';

// Turns the RSSI of a scan result into the signal strength that the entries keep
// pokebw2's WDS_RssiToSignal
static u8 WDSGetRssi8(u8 rssi) {
    int strength = rssi >> 2;

    if ((rssi & 2) == 0) {
        strength += 25;
    }
    return strength;
}

// The callback of the scan. It takes in the beacons that are new and well-formed, then tells the caller
// pokebw2's WDS_OnScan
static void func_0205a2dc(void *arg) {
    WMStartScanExCallback *result = arg;
    // The SDK's u32 is unsigned long, which MWCC indexes with differently than unsigned int
    unsigned long i;
    u32 j;
    BOOL found;

    DC_InvalidateRange(data_02146abc->scanBuf, WDS_SCAN_BUF_SIZE);
    if (result->errcode == WM_ERRCODE_SUCCESS && result->state == WM_STATECODE_PARENT_FOUND) {
        for (i = 0; i < result->bssDescCount; i++) {
            if (result->bssDesc[i]->gameInfo.userGameInfoLength + WDS_GAME_INFO_HEADER_SIZE != sizeof(WMGameInfo) ||
                result->bssDesc[i]->gameInfo.magicNumber != WDS_BEACON_VERSION ||
                result->bssDesc[i]->gameInfo.ggid != WDS_GAME_ID) {
                continue;
            }

            found = FALSE;
            for (j = 0; j < data_02146abc->count; j++) {
                if (data_02146abc->tgids[j] == result->bssDesc[i]->gameInfo.tgid) {
                    found = TRUE;
                    break;
                }
            }
            if (found == TRUE) {
                continue;
            }

            if (func_0205a7cc(&data_02146abc->rc4, &data_02146abc->crc, &data_02146abc->crcTable, result->bssDesc[i],
                                      &data_02146abc->infos[data_02146abc->next])) {
                data_02146abc->signals[data_02146abc->next] = WDSGetRssi8(result->bssDesc[i]->rssi);
                data_02146abc->tgids[data_02146abc->next] = result->bssDesc[i]->gameInfo.tgid;
                data_02146abc->next = (data_02146abc->next + 1) % WDS_MAX_ENTRIES;
                data_02146abc->count++;
                if (data_02146abc->count > WDS_MAX_ENTRIES) {
                    data_02146abc->count = WDS_MAX_ENTRIES;
                }
            }
        }
    }

    data_02146abc->state = WDS_STATE_SCANNED;
    if (data_02146abc->callback != NULL) {
        data_02146abc->callback(result);
    }
}

// pokebw2's WDS_GetWorkSize
u32 func_0205a4c8(void) {
    return (sizeof(WDSWork) + 31) & ~31;
}

// pokebw2's WDS_Initialize
int func_0205a4d4(void *work, WMCallbackFunc callback, u16 dmaNo) {
    int result;

    if (callback == NULL) {
        return -1;
    }

    data_02146abc = work;
    MI_CpuFill8(data_02146abc, 0, func_0205a4c8());
    data_02146abc->state = WDS_STATE_IDLE;
    data_02146abc->crc = 0;
    MATHi_CRC16InitTableRev(&data_02146abc->crcTable, 0xa001);

    result = WM_Initialize(data_02146abc, callback, dmaNo);
    if (result == WM_ERRCODE_OPERATING) {
        result = 0;
    }
    return result;
}

// pokebw2's WDS_End
int func_0205a558(WMCallbackFunc callback) {
    int result;

    if (callback == NULL) {
        return -1;
    }

    data_02146abc->state = WDS_STATE_ENDING;
    data_02146abc = NULL;

    result = WM_End(callback);
    if (result == WM_ERRCODE_OPERATING) {
        result = 0;
    }
    return result;
}

// pokebw2's WDS_StartScan
int func_0205a598(WMCallbackFunc callback) {
    int result;

    if (callback == NULL) {
        return -1;
    }

    data_02146abc->scanParam.scanBuf = (WMBssDesc *)data_02146abc->scanBuf;
    data_02146abc->scanParam.scanBufSize = WDS_SCAN_BUF_SIZE;
    data_02146abc->scanParam.channelList = WM_GetAllowedChannel();
    data_02146abc->scanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    data_02146abc->scanParam.scanType = 1;
    MI_CpuFill8(data_02146abc->scanParam.bssid, 0xff, sizeof(data_02146abc->scanParam.bssid));
    data_02146abc->callback = callback;
    data_02146abc->state = WDS_STATE_SCANNING;

    result = WM_StartScanEx(func_0205a2dc, &data_02146abc->scanParam);
    if (result == WM_ERRCODE_OPERATING) {
        result = 0;
    }
    return result;
}

// pokebw2's WDS_EndScan
int WDS_EndScan(WMCallbackFunc callback) {
    int result;

    if (callback == NULL) {
        return -1;
    }

    data_02146abc->state = WDS_STATE_SCANNED;

    result = WM_EndScan(callback);
    if (result == WM_ERRCODE_OPERATING) {
        result = 0;
    }
    return result;
}

// Gives out the entry at an index of the ring, with its signal strength
// pokebw2's WDS_GetEntry
static int func_0205a688(int index, WDSEntry *out) {
    if (data_02146abc->state != WDS_STATE_SCANNED) {
        return -1;
    }
    if (index < 0 || index >= data_02146abc->count) {
        return -1;
    }

    MI_CpuFill8(out, 0, sizeof(WDSEntry));
    out->valid = TRUE;
    out->signal = data_02146abc->signals[index];
    MI_CpuCopy8(&data_02146abc->infos[index], &out->info, sizeof(WDSBeaconInfo));
    return 0;
}

// pokebw2's WDS_GetEntries
int func_0205a724(WDSEntry *entries) {
    int i;

    if (data_02146abc->state != WDS_STATE_SCANNED) {
        return -1;
    }

    MI_CpuFill8(entries, 0, WDS_MAX_ENTRIES * sizeof(WDSEntry));
    for (i = 0; i < WDS_MAX_ENTRIES; i++) {
        entries[i].valid = FALSE;
    }

    for (i = 0; i < data_02146abc->count; i++) {
        if (func_0205a688(i, &entries[i]) == -1) {
            break;
        }
    }
    return 0;
}

BOOL func_0205a7cc(CRYPTORC4Context *context, u16 *crc, MATHCRC16Table *table, const WMBssDesc *desc,
                           WDSBeaconInfo *out) {
    u8 key[8];
    u16 calculated;

    if (desc == NULL || context == NULL || crc == NULL || table == NULL || out == NULL) {
        return FALSE;
    }

    MI_CpuCopy8(desc->gameInfo.userGameInfo, out, sizeof(WDSBeaconInfo));
    MI_CpuCopy8(&data_020a1618, key, 4);
    MI_CpuCopy8(&desc->bssid[2], &key[4], 4);
    CRYPTO_RC4Init(context, key, sizeof(key));
    CRYPTO_RC4Encrypt(context, out, sizeof(WDSBeaconInfo), out);

    // The accumulator is updated here, but the check below does its own calculation
    MATHi_CRC16UpdateRev(table, crc, out, offsetof(WDSBeaconInfo, crc));
    calculated = MATH_CalcCRC16(table, out, offsetof(WDSBeaconInfo, crc));
    if (calculated == out->crc || out->crc == 0) {
        return TRUE;
    }

    MI_CpuFill8(out, 0, sizeof(WDSBeaconInfo));
    return FALSE;
}

// Whether ten bytes are all printable ASCII
// pokebw2's WDS_IsPrintable
static BOOL func_0205a8c8(const u8 *text) {
    int i;

    for (i = 0; i < 10; i++) {
        if (text[i] < 0x20 || text[i] > 0x7e) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's WDS_IsBeaconInfoValid
BOOL func_0205a8fc(const WDSBeaconInfo *info) {
    if ((info->flags & 2) && func_0205a8c8(info->spotInfo) && info->wepMode <= 3) {
        return TRUE;
    }
    return FALSE;
}
