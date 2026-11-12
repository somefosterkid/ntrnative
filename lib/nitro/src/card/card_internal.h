// TwlSDK's CARD library, its internal types and functions: the one header of card_api.c, card_backup.c,
// card_common.c, card_pullOut.c, card_rom.c, card_task.c and card_utility.c. From libntr (MIT,
// https://github.com/cybervisi0n/libntr) libraries/card/include/card_common.h, card_rom.h, card_task.h,
// card_utility.h and card_event.h for SDK 5, and kh-recoded-decomp's card_rom_internal.h (MIT), in the layout of
// Black's CARDiCommon (cardi_common, 0x520 bytes at 0x02150200).
#ifndef BWPORT_CARD_INTERNAL_H
#define BWPORT_CARD_INTERNAL_H

#include "types.h"
#include "nitro/card.h"
#include "nitro/os.h"

#define HW_TWL_ROM_HEADER_BUF 0x02ffe000
// The card ID that the IPL read (the SDK's HW_BOOT_CHECK_INFO_BUF)
#define CARD_IPL_CARD_ID_BUF 0x02fffc00
#define HW_LCD_HEIGHT 192
#define HW_CACHE_LINE_SIZE 32
#define OS_VALARM_DELAY_MAX 263
#define OS_LOCK_SUCCESS 0
#define MI_DMA_NOT_USE 0xffffffff
#ifndef MI_DMA_MAX_NUM
#define MI_DMA_MAX_NUM 3
#endif

enum {
    CARD_STAT_INIT = (1 << 0),
    CARD_STAT_INIT_CMD = (1 << 1),
    CARD_STAT_BUSY = (1 << 2),
    CARD_STAT_TASK = (1 << 3),
    CARD_STAT_WAITFOR7ACK = (1 << 5),
    CARD_STAT_CANCEL = (1 << 6)
};

typedef enum {
    CARD_TARGET_NONE,
    CARD_TARGET_ROM,
    CARD_TARGET_BACKUP,
    CARD_TARGET_RW
} CARDTargetMode;

typedef u32 CARDAccessLevel;
#define CARD_ACCESS_LEVEL_NONE 0x0000UL
#define CARD_ACCESS_LEVEL_BACKUP_R 0x0001UL
#define CARD_ACCESS_LEVEL_BACKUP_W 0x0002UL
#define CARD_ACCESS_LEVEL_BACKUP (u32)(CARD_ACCESS_LEVEL_BACKUP_R | CARD_ACCESS_LEVEL_BACKUP_W)
#define CARD_ACCESS_LEVEL_ROM 0x0004UL
#define CARD_ACCESS_LEVEL_FULL (u32)(CARD_ACCESS_LEVEL_BACKUP | CARD_ACCESS_LEVEL_ROM)

#define CARD_BACKUP_CAPS_AVAILABLE (u32)(CARD_BACKUP_CAPS_READ - 1)
#define CARD_BACKUP_CAPS_READ (u32)(1 << CARD_REQ_READ_BACKUP)
#define CARD_BACKUP_CAPS_WRITE (u32)(1 << CARD_REQ_WRITE_BACKUP)
#define CARD_BACKUP_CAPS_PROGRAM (u32)(1 << CARD_REQ_PROGRAM_BACKUP)
#define CARD_BACKUP_CAPS_VERIFY (u32)(1 << CARD_REQ_VERIFY_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_PAGE (u32)(1 << CARD_REQ_ERASE_PAGE_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_SECTOR (u32)(1 << CARD_REQ_ERASE_SECTOR_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_CHIP (u32)(1 << CARD_REQ_ERASE_CHIP_BACKUP)
#define CARD_BACKUP_CAPS_READ_STATUS (u32)(1 << CARD_REQ_READ_STATUS)
#define CARD_BACKUP_CAPS_WRITE_STATUS (u32)(1 << CARD_REQ_WRITE_STATUS)
#define CARD_BACKUP_CAPS_ERASE_SUBSECTOR (u32)(1 << CARD_REQ_ERASE_SUBSECTOR_BACKUP)

typedef s32 CARDiOwner;

// The command the ARM9 sends the ARM7 and the answer, in memory both see
typedef struct CARDiCommandArg {
    CARDResult result;
    CARDBackupType type;
    u32 id;
    u32 src;
    u32 dst;
    u32 len;
    struct {
        u32 total_size;
        u32 sect_size;
        u32 subsect_size;
        u32 page_size;
        u32 addr_width;
        u32 program_page;
        u32 write_page;
        u32 write_page_total;
        u32 erase_chip;
        u32 erase_chip_total;
        u32 erase_sector;
        u32 erase_sector_total;
        u32 erase_subsector;
        u32 erase_subsector_total;
        u32 erase_page;
        u8 initial_status;
        u8 padding1[3];
        u32 caps;
        u8 padding2[4];
    } spec;
} CARDiCommandArg;

typedef struct CARDDmaInterface {
    void (*Recv)(u32 channel, const void *src, void *dst, u32 len);
    void (*Stop)(u32 channel);
} CARDDmaInterface;

typedef struct CARDiCommon {
    CARDiCommandArg *cmd;
    volatile u32 flag;
    u32 priority;
    u32 flush_threshold_ic;
    u32 flush_threshold_dc;
    volatile CARDiOwner lock_owner;
    volatile int lock_ref;
    OSThreadQueue lock_queue[1];
    CARDTargetMode lock_target;
    struct {
        OSThread context[1];
        u8 stack[0x400];
    } thread;
    void (*task_func)(struct CARDiCommon *);
    MIDmaCallback callback;
    void *callback_arg;
    OSThreadQueue busy_q[1];
    u32 src;
    u32 dst;
    u32 len;
    u32 dma;
    const CARDDmaInterface *DmaCall;
    CARDRequest req_type;
    int req_retry;
    CARDRequestMode req_mode;
    OSThread *current_thread_9;
} CARDiCommon;

extern CARDiCommon cardi_common;
extern u32 cardi_rom_base;

// card_common.c
void CARDi_LockResource(CARDiOwner owner, CARDTargetMode target);
void CARDi_UnlockResource(CARDiOwner owner, CARDTargetMode target);
CARDAccessLevel CARDi_GetAccessLevel(void);
void CARDi_InitResourceLock(void);
void CARDi_InitCommand(void);
BOOL CARDi_WaitAsync(void);
BOOL CARDi_TryWaitAsync(void);

// card_task.c
BOOL CARDi_ExecuteOldTypeTask(void (*task)(CARDiCommon *), BOOL async);
BOOL CARDi_WaitForTask(CARDiCommon *p, BOOL restart, MIDmaCallback callback, void *callback_arg);
void CARDi_EndTask(CARDiCommon *p);
void CARDi_OldTypeTaskThread(void *arg);

// card_backup.c
// The library's middleware signature, "[SDK+NINTENDO:BACKUP]" (the SDK's cardi_backup_assert, in its .version section),
// which lies in the version strings at the start of the program
extern const char CARDi_BackupSignature[];
void CARDi_OnFifoRecv(int tag, uPtr data, BOOL err);
BOOL CARDi_Request(CARDiCommon *p, int req_type, int retry_count);
void CARDi_RequestStreamCommandCore(CARDiCommon *p);
void CARDi_IdentifyBackupCore2(CARDBackupType type);
void CARDi_IdentifyBackupCore(CARDiCommon *p);
void CARDi_BeginBackupCommand(CARDAccessLevel level, MIDmaCallback callback, void *arg);

// card_utility.c
const CARDDmaInterface *CARDi_GetDmaInterface(u32 channel);
void CARDi_ICInvalidateSmart(void *buffer, u32 length, u32 threshold);
void CARDi_DCInvalidateSmart(void *buffer, u32 length, u32 threshold);

// card_pullOut.c
void CARD_InitPulledOutCallback(void);

// card_hook.c
typedef u32 CARDEvent;
#define CARD_EVENT_PULLEDOUT 0x00000001
#define CARD_EVENT_SLOTRESET 0x00000002

typedef void (*CARDHookFunction)(void *, CARDEvent, void *);

typedef struct CARDHookContext {
    struct CARDHookContext *next;
    void *userdata;
    CARDHookFunction callback;
} CARDHookContext;

void CARDi_NotifyEvent(CARDEvent event, void *arg);

// card_pullOut.c
#define CARD_PXI_COMMAND_MASK 0x0000003f
#define CARD_PXI_COMMAND_TERMINATE 0x0001
#define CARD_PXI_COMMAND_PULLED_OUT 0x0011
#define CARD_PXI_COMMAND_RESET_SLOT 0x0002

void CARDi_CheckPulledOutCore(u32 id);

// card_rom.c
typedef struct CARDTransferInfo {
    u32 command;
    void (*callback)(void *userdata);
    void *userdata;
    u32 src;
    u32 dst;
    u32 len;
    u32 work;
} CARDTransferInfo;

int CARDi_ReadRomWithCPU(void *userdata, void *buffer, u32 offset, u32 length);
void CARDi_ReadRomWithDMA(CARDTransferInfo *info);
void CARDi_InitRom(void);
u32 CARDi_ReadRomIDCore(void);
u32 CARDi_ReadRomStatusCore(void);
void CARDi_RefreshRom(u32 warn_mask);
void CARDi_RefreshRomCore(void);
const u8 *CARDi_GetOwnSignature(void);

#endif // BWPORT_CARD_INTERNAL_H
