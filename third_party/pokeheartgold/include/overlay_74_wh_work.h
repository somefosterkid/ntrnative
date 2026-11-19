#ifndef POKEHEARTGOLD_OVERLAY_74_WH_WORK_H
#define POKEHEARTGOLD_OVERLAY_74_WH_WORK_H

#include "global.h"

#include "crypto_rc4.h"

// Overlay 74's "mywh" wireless-host module (a trimmed copy of the SDK's WH sample): it scans for a distribution parent
// and downloads its data in 104-byte blocks. ov74_0222CEE0 allocs ov74_02236980() bytes and hands them to ov74_02236680,
// which puts the WHWork 32-aligned inside; ov74_0223E2FC (8 bytes of asm .bss on the DS) holds it. The offsets in the
// comments are the DS ones.

typedef void (*WHCallback)(int);

typedef void (*WHPrintFunc)(const char *, ...);

// The user game info of a beacon: block `index` of the distributed data (index 0xFFFF: the parent's own info).
typedef struct WHBlock {
    u16 checksum; // 0x0
    u16 index; // 0x2
    u32 size; // 0x4: total size of the data
    u8 data[WM_SIZE_USER_GAMEINFO - 8]; // 0x8
} WHBlock; // 0x70

typedef struct WHWork {
    u8 sysBuf[WM_SYSTEM_BUF_SIZE] __attribute__((aligned(32))); // 0x0
    WMbssDesc bssDesc __attribute__((aligned(32))); // 0xF00
    WMscanParam scanParam __attribute__((aligned(32))); // 0xFC0
    u8 parentInfo[WM_SIZE_USER_GAMEINFO - 8]; // 0xFE0
    int hasParentInfo; // 0x1048
    CRYPTORC4Context rc4; // 0x104C
    int state; // 0x1150
    int errCode; // 0x1154
    u16 phase; // 0x1158
    u16 checksum; // 0x115A
    u16 blockCount; // 0x115C
    u16 blocksReceived; // 0x115E
    u32 dataSize; // 0x1160
    u32 mismatchCount; // 0x1164
    int lastBlock; // 0x1168
    int linkLevel; // 0x116C
    u8 parentBssid[WM_SIZE_BSSID]; // 0x1170
    u16 channel; // 0x1176
    u8 *recvBuf; // 0x1178
    WHCallback callback; // 0x117C
    u8 blockReceived[1261]; // 0x1180
} WHWork; // 0x1680

typedef struct WHStatics {
    WHWork *work; // 0x0
    WHPrintFunc print; // 0x4
} WHStatics;

extern WHStatics ov74_0223E2FC;

extern const char *const ov74_0223CFE4[];
extern const char ov74_0223D00C[];
extern const char ov74_0223D014[];
extern const char ov74_0223D018[];

void ov74_02236168(int state);
void ov74_022361B8(int errCode);
BOOL ov74_022361D4(void);
BOOL ov74_02236258(void);
void ov74_02236354(void *arg0);
void ov74_022364C0(void *arg0);
BOOL ov74_022364F0(void);
void ov74_02236514(void *arg0);
BOOL ov74_0223653C(void);
void ov74_02236570(void *arg0);
BOOL ov74_022365AC(void);
BOOL ov74_022365FC(void);
BOOL ov74_02236620(void);
void ov74_02236644(void *arg0);
BOOL ov74_02236680(void *userData, WHCallback callback, void *workBuf);
void ov74_022366E8(WHBlock *block);
BOOL ov74_02236768(WHBlock *block);
u16 ov74_022368A4(u8 *data, u32 size);
BOOL ov74_022368D4(void);
int ov74_02236980(void);
void *ov74_02236988(void);

#endif // POKEHEARTGOLD_OVERLAY_74_WH_WORK_H
