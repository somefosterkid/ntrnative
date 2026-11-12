// TwlSDK's CARD library, card_backup.c: Black's main 0x020763bc-0x02076bbc. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"

static u8 CARDi_backup_cache_page_buf[256] ATTRIBUTE_ALIGN(32);

static inline void CARDi_SendPxi(u32 data) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_FS, data, TRUE) < 0) {
    }
}

void CARDi_OnFifoRecv(int tag, uPtr data, BOOL err) {
    if ((tag == PXI_FIFO_TAG_FS) && err) {
        CARDiCommon *p = &cardi_common;

        p->flag &= ~CARD_STAT_WAITFOR7ACK;
        OS_WakeupThreadDirect(p->current_thread_9);
    }
}

BOOL CARDi_Request(CARDiCommon *p, int req_type, int retry_count) {
    if ((p->flag & CARD_STAT_INIT_CMD) == 0) {
        p->flag |= CARD_STAT_INIT_CMD;
        while (!PXI_IsCallbackReady(PXI_FIFO_TAG_FS, PXI_PROC_ARM7)) {
            OS_SpinWaitSysCycles(50);
        }

        CARDi_Request(p, CARD_REQ_INIT, 1);
    }

    DC_FlushRange(p->cmd, sizeof(*p->cmd));
    DC_WaitWriteBufferEmpty();

    p->current_thread_9 = OS_GetCurrentThread();

    do {
        p->flag |= CARD_STAT_WAITFOR7ACK;
        CARDi_SendPxi((u32)req_type);

        switch (req_type) {
        case CARD_REQ_INIT:
            CARDi_SendPxi((u32)p->cmd);
            break;
        }
        {
            OSIntrMode bak_psr = OS_DisableInterrupts();
            while ((p->flag & CARD_STAT_WAITFOR7ACK) != 0) {
                OS_SleepThread(NULL);
            }
            OS_RestoreInterrupts(bak_psr);
        }
        DC_InvalidateRange(p->cmd, sizeof(*p->cmd));
    } while ((p->cmd->result == CARD_RESULT_TIMEOUT) && (--retry_count > 0));

    return (p->cmd->result == CARD_RESULT_SUCCESS);
}

void CARDi_RequestStreamCommandCore(CARDiCommon *p) {
    const int req_type = p->req_type;
    const int req_mode = p->req_mode;
    const int retry_count = p->req_retry;
    u32 size = sizeof(CARDi_backup_cache_page_buf);

    OSi_ReferSymbol(CARDi_BackupSignature);
    if (req_type == CARD_REQ_ERASE_SECTOR_BACKUP) {
        size = CARD_GetBackupSectorSize();
    } else if (req_type == CARD_REQ_ERASE_SUBSECTOR_BACKUP) {
        size = cardi_common.cmd->spec.subsect_size;
    }
    do {
        const u32 len = (size < p->len) ? size : p->len;
        p->cmd->len = len;
        if ((p->flag & CARD_STAT_CANCEL) != 0) {
            p->flag &= ~CARD_STAT_CANCEL;
            p->cmd->result = CARD_RESULT_CANCELED;
            break;
        }
        switch (req_mode) {
        case CARD_REQUEST_MODE_RECV:
            DC_InvalidateRange(CARDi_backup_cache_page_buf, len);
            p->cmd->src = p->src;
            p->cmd->dst = (u32)CARDi_backup_cache_page_buf;
            break;
        case CARD_REQUEST_MODE_SEND:
        case CARD_REQUEST_MODE_SEND_VERIFY:
            MI_CpuCopy8((const void *)p->src, CARDi_backup_cache_page_buf, len);
            DC_FlushRange(CARDi_backup_cache_page_buf, len);
            DC_WaitWriteBufferEmpty();
            p->cmd->src = (u32)CARDi_backup_cache_page_buf;
            p->cmd->dst = p->dst;
            break;
        case CARD_REQUEST_MODE_SPECIAL:
            p->cmd->src = p->src;
            p->cmd->dst = p->dst;
            break;
        }
        if (!CARDi_Request(p, req_type, retry_count)) {
            break;
        }
        if (req_mode == CARD_REQUEST_MODE_SEND_VERIFY) {
            if (!CARDi_Request(p, CARD_REQ_VERIFY_BACKUP, 1)) {
                break;
            }
        } else if (req_mode == CARD_REQUEST_MODE_RECV) {
            MI_CpuCopy8(CARDi_backup_cache_page_buf, (void *)p->dst, len);
        }
        p->src += len;
        p->dst += len;
        p->len -= len;
    } while (p->len > 0);
}

void CARDi_IdentifyBackupCore2(CARDBackupType type) {
    CARDiCommandArg *const p = cardi_common.cmd;

    MI_CpuFill8(&p->spec, 0, sizeof(p->spec));
    p->type = type;
    p->spec.caps = (CARD_BACKUP_CAPS_AVAILABLE | CARD_BACKUP_CAPS_READ_STATUS);
    if (type != CARD_BACKUP_TYPE_NOT_USE) {
        const u32 size = (u32)(1 << ((type >> CARD_BACKUP_TYPE_SIZEBIT_SHIFT) & CARD_BACKUP_TYPE_SIZEBIT_MASK));
        const int device = ((type >> CARD_BACKUP_TYPE_DEVICE_SHIFT) & CARD_BACKUP_TYPE_DEVICE_MASK);
        const int vender = ((type >> CARD_BACKUP_TYPE_VENDER_SHIFT) & CARD_BACKUP_TYPE_VENDER_MASK);

        p->spec.total_size = size;
        p->spec.initial_status = 0xff;
        if (device == CARD_BACKUP_TYPE_DEVICE_EEPROM) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x000200:
                p->spec.page_size = 0x10;
                p->spec.addr_width = 1;
                p->spec.program_page = 5;
                p->spec.initial_status = 0xf0;
                break;
            case 0x002000:
                p->spec.page_size = 0x20;
                p->spec.addr_width = 2;
                p->spec.program_page = 5;
                p->spec.initial_status = 0x00;
                break;
            case 0x010000:
                p->spec.page_size = 0x80;
                p->spec.addr_width = 2;
                p->spec.program_page = 10;
                p->spec.initial_status = 0x00;
                break;
            case 0x020000:
                p->spec.page_size = 0x100;
                p->spec.addr_width = 3;
                p->spec.program_page = 5;
                p->spec.initial_status = 0x00;
                break;
            }
            p->spec.sect_size = p->spec.page_size;
            p->spec.caps |= CARD_BACKUP_CAPS_READ;
            p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
        } else if (device == CARD_BACKUP_TYPE_DEVICE_FLASH) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x040000:
            case 0x080000:
            case 0x100000:
                p->spec.write_page = 25;
                p->spec.write_page_total = 300;
                p->spec.erase_page = 300;
                p->spec.erase_sector = 5000;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                break;
            case 0x200000:
                p->spec.write_page = 23;
                p->spec.write_page_total = 300;
                p->spec.erase_sector = 500;
                p->spec.erase_sector_total = 5000;
                p->spec.erase_chip = 10000;
                p->spec.erase_chip_total = 60000;
                p->spec.initial_status = 0x00;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                break;
            case 0x400000:
                p->spec.erase_sector = 600;
                p->spec.erase_sector_total = 3000;
                p->spec.erase_subsector = 70;
                p->spec.erase_subsector_total = 150;
                p->spec.erase_chip = 23000;
                p->spec.erase_chip_total = 800000;
                p->spec.initial_status = 0x00;
                p->spec.subsect_size = 0x1000;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_SUBSECTOR;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                break;
            case 0x800000:
                if (vender == 0) {
                    p->spec.erase_sector = 1000;
                    p->spec.erase_sector_total = 3000;
                    p->spec.erase_chip = 68000;
                    p->spec.erase_chip_total = 160000;
                    p->spec.initial_status = 0x00;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                } else if (vender == 1) {
                    p->spec.erase_sector = 1000;
                    p->spec.erase_sector_total = 3000;
                    p->spec.erase_chip = 68000;
                    p->spec.erase_chip_total = 160000;
                    p->spec.initial_status = 0x84;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                }
                break;
            }
            p->spec.sect_size = 0x10000;
            p->spec.page_size = 0x100;
            p->spec.addr_width = 3;
            p->spec.program_page = 5;
            p->spec.caps |= CARD_BACKUP_CAPS_READ;
            p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            p->spec.caps |= CARD_BACKUP_CAPS_ERASE_SECTOR;
        } else if (device == CARD_BACKUP_TYPE_DEVICE_FRAM) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x2000:
            case 0x8000:
                break;
            }
            p->spec.page_size = size;
            p->spec.sect_size = size;
            p->spec.addr_width = 2;
            p->spec.initial_status = 0x00;
            p->spec.caps |= CARD_BACKUP_CAPS_READ;
            p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
        } else {
        invalid_type:
            p->type = CARD_BACKUP_TYPE_NOT_USE;
            p->spec.total_size = 0;
            cardi_common.cmd->result = CARD_RESULT_UNSUPPORTED;
            return;
        }
    }
}

void CARDi_IdentifyBackupCore(CARDiCommon *p) {
    CARDi_Request(p, CARD_REQ_IDENTIFY, 1);

    p->cmd->src = 0;
    p->cmd->dst = (u32)CARDi_backup_cache_page_buf;
    p->cmd->len = 1;
    CARDi_Request(p, CARD_REQ_READ_BACKUP, 1);
}

void CARDi_BeginBackupCommand(CARDAccessLevel level, MIDmaCallback callback, void *arg) {
    OSi_ReferSymbol(CARDi_BackupSignature);
    CARD_CheckEnabled();
    if ((CARDi_GetAccessLevel() & level) != level) {
        OS_Terminate();
    }
    CARDi_WaitForTask(&cardi_common, TRUE, callback, arg);
}

BOOL CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async,
                                CARDRequest req_type, int req_retry, CARDRequestMode req_mode) {
    CARDAccessLevel level = (req_mode == CARD_REQUEST_MODE_RECV) ? CARD_ACCESS_LEVEL_BACKUP_R : CARD_ACCESS_LEVEL_BACKUP_W;

    CARDi_BeginBackupCommand(level, callback, arg);

    {
        CARDiCommon *p = &cardi_common;
        p->src = src;
        p->dst = dst;
        p->len = len;
        p->req_type = req_type;
        p->req_retry = req_retry;
        p->req_mode = req_mode;
    }

    return CARDi_ExecuteOldTypeTask(CARDi_RequestStreamCommandCore, is_async);
}

BOOL CARD_IdentifyBackup(CARDBackupType type) {
    if (type == CARD_BACKUP_TYPE_NOT_USE) {
        OS_Terminate();
    }

    CARDi_BeginBackupCommand(CARD_ACCESS_LEVEL_NONE, NULL, NULL);

    CARDi_IdentifyBackupCore2(type);

    return CARDi_ExecuteOldTypeTask(CARDi_IdentifyBackupCore, FALSE);
}

u32 CARD_GetBackupSectorSize(void) {
    return cardi_common.cmd->spec.sect_size;
}

BOOL CARD_WaitBackupAsync(void) {
    return CARDi_WaitAsync();
}

BOOL CARD_TryWaitBackupAsync(void) {
    return CARDi_TryWaitAsync();
}

void CARD_CancelBackupAsync(void) {
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    cardi_common.flag |= CARD_STAT_CANCEL;
    OS_RestoreInterrupts(bak_cpsr);
}
