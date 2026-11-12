// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/card.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_CARD_H
#define BWPORT_NITRO_CARD_H

#include "types.h"
#include "nitro/os.h"

// The ROM headers of the DS and the DSi: CARDRomRegion, CARDRomHeader and CARDRomHeaderTWL
#include "nitro/card/types.h"

#define CARD_ROM_PAGE_SIZE 512
#define CARD_ROM_DOWNLOAD_SIGNATURE_SIZE 136

typedef enum {
    CARD_RESULT_SUCCESS,
    CARD_RESULT_FAILURE,
    CARD_RESULT_INVALID_PARAM,
    CARD_RESULT_UNSUPPORTED,
    CARD_RESULT_TIMEOUT,
    CARD_RESULT_ERROR,
    CARD_RESULT_NO_RESPONSE,
    CARD_RESULT_CANCELED,
} CARDResult;

// What the ARM9 asks the ARM7 for (the first word of a command)
typedef enum {
    CARD_REQ_INIT = 0,
    CARD_REQ_ACK,
    CARD_REQ_IDENTIFY,
    CARD_REQ_READ_ID,
    CARD_REQ_READ_ROM,
    CARD_REQ_WRITE_ROM,
    CARD_REQ_READ_BACKUP,
    CARD_REQ_WRITE_BACKUP,
    CARD_REQ_PROGRAM_BACKUP,
    CARD_REQ_VERIFY_BACKUP,
    CARD_REQ_ERASE_PAGE_BACKUP,
    CARD_REQ_ERASE_SECTOR_BACKUP,
    CARD_REQ_ERASE_CHIP_BACKUP,
    CARD_REQ_READ_STATUS,
    CARD_REQ_WRITE_STATUS,
    CARD_REQ_ERASE_SUBSECTOR_BACKUP,
    CARD_REQ_MAX
} CARDRequest;

typedef enum {
    CARD_REQUEST_MODE_RECV,
    CARD_REQUEST_MODE_SEND,
    CARD_REQUEST_MODE_SEND_VERIFY,
    CARD_REQUEST_MODE_SPECIAL,
} CARDRequestMode;

#define CARD_THREAD_PRIORITY_DEFAULT 4

#define CARD_RETRY_COUNT_MAX 10

// A backup type: the device, the log2 of its size and the vendor
typedef s32 CARDBackupType;

#define CARD_BACKUP_TYPE_DEVICE_SHIFT 0
#define CARD_BACKUP_TYPE_DEVICE_MASK 0xff
#define CARD_BACKUP_TYPE_DEVICE_EEPROM 1
#define CARD_BACKUP_TYPE_DEVICE_FLASH 2
#define CARD_BACKUP_TYPE_DEVICE_FRAM 3
#define CARD_BACKUP_TYPE_SIZEBIT_SHIFT 8
#define CARD_BACKUP_TYPE_SIZEBIT_MASK 0xff
#define CARD_BACKUP_TYPE_VENDER_SHIFT 16
#define CARD_BACKUP_TYPE_VENDER_MASK 0xff
#define CARD_BACKUP_TYPE_NOT_USE 0
#define CARD_BACKUP_TYPE_DEFINE(device, sizeShift, vendor) ((device) | ((sizeShift) << 8) | ((vendor) << 16))
#define CARD_BACKUP_TYPE_FLASH_4MBITS_EX CARD_BACKUP_TYPE_DEFINE(CARD_BACKUP_TYPE_DEVICE_FLASH, 19, 0xff)
typedef void (*MIDmaCallback)(void *arg);

// Called when the card is pulled out; returns whether to stop the system
typedef BOOL (*CARDPulledOutCallback)(void);

// card_api.c
void CARD_Init(void);
BOOL CARD_IsEnabled(void);
void CARD_CheckEnabled(void);
void CARD_Enable(BOOL enable);
CARDResult CARD_GetResultCode(void);
const u8 *CARD_GetRomHeader(void);
const CARDRomHeader *CARD_GetOwnRomHeader(void);
const CARDRomHeaderTWL *CARD_GetOwnRomHeaderTWL(void);
void CARD_LockRom(u16 lockId);
void CARD_UnlockRom(u16 lockId);
void CARD_LockBackup(u16 lockId);
void CARD_UnlockBackup(u16 lockId);

// card_backup.c
BOOL CARD_IdentifyBackup(CARDBackupType type);
u32 CARD_GetBackupSectorSize(void);
BOOL CARD_WaitBackupAsync(void);
BOOL CARD_TryWaitBackupAsync(void);
void CARD_CancelBackupAsync(void);
BOOL CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len, MIDmaCallback callback, void *arg, BOOL async,
                                CARDRequest req_type, int req_retry, CARDRequestMode req_mode);

// NitroSDK's backup reads and writes (card/backup.h): the memory side of the stream command as a pointer
static inline BOOL CARDi_ReadBackup(u32 src, void *dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async) {
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async, CARD_REQ_READ_BACKUP, 1,
                                      CARD_REQUEST_MODE_RECV);
}

static inline BOOL CARDi_WriteAndVerifyBackup(u32 dst, const void *src, u32 len, MIDmaCallback callback, void *arg,
                                              BOOL is_async) {
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async, CARD_REQ_WRITE_BACKUP,
                                      CARD_RETRY_COUNT_MAX, CARD_REQUEST_MODE_SEND_VERIFY);
}

static inline void CARD_ReadBackupAsync(u32 src, void *dst, u32 len, MIDmaCallback callback, void *arg) {
    (void)CARDi_ReadBackup(src, dst, len, callback, arg, TRUE);
}

static inline BOOL CARD_ReadBackup(u32 src, void *dst, u32 len) {
    return CARDi_ReadBackup(src, dst, len, NULL, NULL, FALSE);
}

static inline void CARD_WriteAndVerifyBackupAsync(u32 dst, const void *src, u32 len, MIDmaCallback callback,
                                                  void *arg) {
    (void)CARDi_WriteAndVerifyBackup(dst, src, len, callback, arg, TRUE);
}

// card_rom.c
void CARDi_ReadRom(u32 dma, const void *src, void *dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async);
void CARD_WaitRomAsync(void);

// NitroSDK's CARD_ReadRom (card/rom.h): reads the card's ROM and waits
static inline void CARD_ReadRom(u32 dma, const void *src, void *dst, u32 len) {
    CARDi_ReadRom(dma, src, dst, len, NULL, NULL, FALSE);
}

// card_pullOut.c
void CARD_SetPulledOutCallback(CARDPulledOutCallback callback);
BOOL CARD_IsPulledOut(void);
void CARD_TerminateForPulledOut(void);

// What a TWL header gives a game to do with the card: the flags of its access control. The two backup flags are named
// for the access level they grant
typedef struct CARDAccessControl {
    u32 : 5;
    u32 gameCardPowerOn : 1;
    u32 : 2;
    u32 gameCardNtrMode : 1;
    u32 : 6;
    u32 backupRead : 1;
    u32 backupWrite : 1;
    u32 : 15;
} CARDAccessControl;

// The header of a DSi game: the load tables of the DS header in its first 0x60 bytes, then where the DSi loads its own
// two images, and the keyed hashes of the ranges that the boot code verifies before it loads them. Only the fields that
// the boot code and TwlSDK's card_hash.c read are named, and each hash's name says which range it verifies
typedef struct CARDTwlRomHeader {
    u8 unk_000[0x1c];
    // 0x1c: the card's security flag; bit 0 says the boot code checks the images against the hashes. The card library
    // reads the same byte as its CARD_TWL_BOOT_FLAGS
    u8 bootFlags;
    u8 unk_01d[0x3];
    u32 arm9RomOffset;
    u32 arm9Entry;
    ROMHeaderLoad arm9;
    u32 arm7RomOffset;
    u32 arm7Entry;
    ROMHeaderLoad arm7;
    u8 unk_040[0x174];
    CARDAccessControl accessControl;
    u8 unk_1b8[0x8];
    u32 arm9iRomOffset;
    u32 arm9iEntry;
    ROMHeaderLoad arm9i;
    u32 arm7iRomOffset;
    u32 arm7iEntry;
    ROMHeaderLoad arm7i;
    // 0x1e0: the parts of the DS and the DSi images that TwlSDK's card library checks as it reads them, the tables of
    // the sectors' hashes and of the blocks' hashes (a block hashes the hashes of a run of sectors), a sector's size,
    // and the sectors of a block
    CARDRomRegion digestNtr;
    CARDRomRegion digestTwl;
    CARDRomRegion digestSectorHashes;
    CARDRomRegion digestBlockHashes;
    u32 digestSectorSize;
    u32 digestSectorsPerBlock;
    u8 unk_208[0x10c];
    u8 arm7Hash[0x14];
    u8 digestBlockHashesHash[0x14];
    u8 unk_33c[0x14];
    u8 arm9iHash[0x14];
    u8 arm7iHash[0x14];
    u8 unk_378[0x28];
    u8 arm9Hash[0x14];
} CARDTwlRomHeader;

// TwlSDK's card/hash.h (libntr's reconstruction, MIT): the ROM reads of the LTD autoload's card_hash.c, which check
// each sector against the keyed hashes of the DSi's ROM header. A sector is checked against its hash in the table of
// the sectors' hashes, and a run of those hashes, a block, against its hash in the table of the blocks' hashes; the
// context caches the last sectors and blocks read
#define CARD_ROM_HASH_SIZE 20

// The SDK's MIDeviceReadFunction (mi/device.h)
typedef int (*MIDeviceReadFunction)(void *userdata, void *buffer, u32 offset, u32 length);

typedef struct CARDRomHashSector {
    struct CARDRomHashSector *next;
    u32 index;
    u32 offset;
    void *image;
} CARDRomHashSector;

typedef struct CARDRomHashBlock {
    struct CARDRomHashBlock *next;
    u32 index;
    u32 offset;
    u8 *hash;
    u8 *hash_aligned;
} CARDRomHashBlock;

typedef struct CARDRomHashContext {
    CARDRomRegion area_ntr;
    CARDRomRegion area_ltd;
    CARDRomRegion sector_hash;
    CARDRomRegion block_hash;
    u32 bytes_per_sector;
    u32 sectors_per_block;
    u32 block_max;
    u32 sector_max;
    void *userdata;
    MIDeviceReadFunction ReadSync;
    MIDeviceReadFunction ReadAsync;
    OSThread *loader;
    void *recent_load;
    CARDRomHashSector *loading_sector; // waiting to be read
    CARDRomHashSector *loaded_sector;  // read, waiting to be checked
    CARDRomHashSector *valid_sector;   // checked
    CARDRomHashBlock *loading_block;
    CARDRomHashBlock *loaded_block;
    CARDRomHashBlock *valid_block;
    u8 *master_hash; // the table of the blocks' hashes
    u8 *images;      // the sectors' images
    u8 *hashes;      // the hashes of the blocks' sectors
    CARDRomHashSector *sectors;
    CARDRomHashBlock *blocks;
} CARDRomHashContext;

void CARD_InitRomHashContext(CARDRomHashContext *context, const CARDRomHeaderTWL *header, void *buffer, u32 length,
                             MIDeviceReadFunction sync, MIDeviceReadFunction async, void *userdata);
u32 CARD_CalcRomHashBufferLength(const CARDRomHeaderTWL *header);
void CARD_NotifyRomHashReadAsync(CARDRomHashContext *context);
void CARD_ReadRomHashImage(CARDRomHashContext *context, void *buffer, u32 offset, u32 length);

#endif // BWPORT_NITRO_CARD_H
