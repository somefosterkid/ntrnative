// TwlSDK's CARD library, card_rom.c: Black's main 0x02076bbc-0x020773dc, and its twl/ltdmain_begin.h block in the LTD
// autoload's .ltdmain 0x0272e9e8-0x0272eb9c (.ltdmain.bss 0x02732da0-0x02732e34), which ltd_autoload_0's delinks.txt
// names card_rom@ltd.c (tools/two_modules.py: one object, two modules). The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"
#include "nitro/mi.h"
#include "nitro/math.h"

#define CARD_COMMAND_PAGE 0x01000000
#define CARD_COMMAND_ID 0x07000000
#define CARD_COMMAND_REFRESH 0x00000000
#define CARD_COMMAND_STAT CARD_COMMAND_ID
#define CARD_COMMAND_MASK 0x07000000
#define CARD_RESET_HI 0x20000000
#define CARD_START 0x80000000
#define CARD_DATA_READY 0x00800000
#define CARD_LATENCY1_MASK 0x00001fff
#define CARD_COMMAND_OP_G_READID 0xb8
#define CARD_COMMAND_OP_G_READPAGE 0xb7
#define CARD_COMMAND_OP_G_READSTAT 0xd6
#define CARD_COMMAND_OP_G_REFRESH 0xb5
#define CARD_COMMAND_OP_N_READID 0x90
#define CARD_COMMAND_OP_N_READPAGE 0x00

#define CARD_ROMID_REFRESH_MASK 0x20000000UL
#define CARD_ROMST_RFS_WARN_L2_MASK 0x00000008UL
#define CARD_ROMST_RFS_READY_MASK 0x00000020UL

#define HW_ITCM_SIZE 0x8000
#define HW_DTCM_SIZE 0x4000
#define OS_GetITCMAddress() 0x01ff8000

// The DSi's ROM hash check in the LTD autoload (CARDi_InitRomHash), below
void CARDi_InitTwlRom(void);

u32 cardi_rom_base;

static int (*CARDiReadRomFunction)(void *userdata, void *buffer, u32 offset, u32 length);
static CARDTransferInfo CARDiDmaReadInfo[1];
static CARDTransferInfo *CARDiDmaReadRegisteredInfo;

static u32 cache_page = 1;
static u8 CARDi_cache_buf[CARD_ROM_PAGE_SIZE] ATTRIBUTE_ALIGN(32);
static BOOL CARDiEnableCacheInvalidationIC = FALSE;
static BOOL CARDiEnableCacheInvalidationDC = TRUE;

static u8 CARDiOwnSignature[CARD_ROM_DOWNLOAD_SIGNATURE_SIZE] ATTRIBUTE_ALIGN(4);

static inline u32 CARDi_GetRomFlag(u32 flag) {
    u32 rom_ctrl = *(vu32 *)(HW_CARD_ROM_HEADER + 0x60);
    return (u32)(flag | CARD_START | CARD_RESET_HI | (rom_ctrl & ~CARD_COMMAND_MASK));
}

static void CARDi_SetRomOp(u32 command, u32 offset) {
    u32 cmd1 = (u32)((offset >> 8) | (command << 24));
    u32 cmd2 = (u32)(offset << 24);

    while ((reg_MI_MCCNT1 & CARD_START) != 0) {
    }

    reg_MI_MCCNT0 = (u16)((reg_MI_MCCNT0 & ~0x2000) | 0xc000);

    reg_MI_MCCMD0 = MI_SwapEndian32(cmd1);
    reg_MI_MCCMD1 = MI_SwapEndian32(cmd2);
}

static BOOL CARDi_IsNormalMode(void) {
    const CARDRomHeaderTWL *oh = CARD_GetOwnRomHeaderTWL();

    return OS_IsRunOnTwl() && (OS_GetBootType() != OS_BOOTTYPE_ROM) && oh->access_control.game_card_on &&
           !oh->access_control.game_card_nitro_mode;
}

static void CARDi_StartRomPageTransfer(u32 offset) {
    u8 op = CARD_COMMAND_OP_G_READPAGE;
    if (CARDi_IsNormalMode()) {
        op = CARD_COMMAND_OP_N_READPAGE;
    }

    CARDi_SetRomOp(op, offset);
    reg_MI_MCCNT1 = CARDi_GetRomFlag(CARD_COMMAND_PAGE);
}

u32 CARDi_ReadRomIDCore(void) {
    u8 op = CARD_COMMAND_OP_G_READID;
    if (CARDi_IsNormalMode()) {
        op = CARD_COMMAND_OP_N_READID;
    }

    CARDi_SetRomOp(op, 0);
    reg_MI_MCCNT1 = (u32)(CARDi_GetRomFlag(CARD_COMMAND_ID) & ~CARD_LATENCY1_MASK);
    while ((reg_MI_MCCNT1 & CARD_DATA_READY) == 0) {
    }
    return reg_MI_MCD1;
}

u32 CARDi_ReadRomStatusCore(void) {
    u32 iplCardID = *(u32 *)(CARD_IPL_CARD_ID_BUF);

    if (!(iplCardID & CARD_ROMID_REFRESH_MASK)) {
        return CARD_ROMST_RFS_READY_MASK;
    }

    CARDi_SetRomOp(CARD_COMMAND_OP_G_READSTAT, 0);
    reg_MI_MCCNT1 = (u32)(CARDi_GetRomFlag(CARD_COMMAND_STAT) & ~CARD_LATENCY1_MASK);
    while ((reg_MI_MCCNT1 & CARD_DATA_READY) == 0) {
    }
    return reg_MI_MCD1;
}

void CARDi_RefreshRom(u32 warn_mask) {
    if (CARDi_ReadRomStatusCore() & warn_mask) {
        CARDi_RefreshRomCore();

        while (!(CARDi_ReadRomStatusCore() & CARD_ROMST_RFS_READY_MASK)) {
            if (OSi_IsThreadInitialized && OS_IsAlarmAvailable()) {
                OS_Sleep(1);
            }
        }
    }
}

void CARDi_RefreshRomCore(void) {
    CARDi_SetRomOp(CARD_COMMAND_OP_G_REFRESH, 0);
    reg_MI_MCCNT1 = (u32)(CARDi_GetRomFlag(CARD_COMMAND_REFRESH) & ~CARD_LATENCY1_MASK);
    while (reg_MI_MCCNT1 & CARD_START) {
    }
}

int CARDi_ReadRomWithCPU(void *userdata, void *buffer, u32 offset, u32 length) {
    int retval = (int)length;

    u32 cachedPage = cache_page;
    u8 *const cacheBuffer = CARDi_cache_buf;
    while (length > 0) {
        u8 *ptr = (u8 *)buffer;
        u32 n = CARD_ROM_PAGE_SIZE;
        u32 pos = offset & ~(CARD_ROM_PAGE_SIZE - 1);

        if (pos == cachedPage) {
            ptr = cacheBuffer;
        } else {
            if (((pos != offset) || (((u32)buffer & 3) != 0) || (length < n))) {
                cachedPage = pos;
                ptr = cacheBuffer;
            }

            CARDi_StartRomPageTransfer(pos);
            {
                u32 word = 0;
                for (;;) {
                    u32 ctrl = reg_MI_MCCNT1;
                    if ((ctrl & CARD_DATA_READY) != 0) {
                        u32 data = reg_MI_MCD1;
                        if (word < (CARD_ROM_PAGE_SIZE / sizeof(u32))) {
                            ((u32 *)ptr)[word++] = data;
                        }
                    }

                    if ((ctrl & CARD_START) == 0) {
                        break;
                    }
                }
            }
        }

        if (ptr == cacheBuffer) {
            u32 mod = offset - pos;
            n = (length <= CARD_ROM_PAGE_SIZE - mod) ? length : CARD_ROM_PAGE_SIZE - mod;
            MI_CpuCopy8(cacheBuffer + mod, buffer, n);
        }
        buffer = (u8 *)buffer + n;
        offset += n;
        length -= n;
    }

    CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());

    cache_page = cachedPage;
    return retval;
}

static void CARDi_DmaReadPageCallback(void) {
    CARDTransferInfo *info = CARDiDmaReadRegisteredInfo;
    if (info) {
        info->src += CARD_ROM_PAGE_SIZE;
        info->dst += CARD_ROM_PAGE_SIZE;
        info->len -= CARD_ROM_PAGE_SIZE;

        if (info->len > 0) {
            CARDi_StartRomPageTransfer(info->src);
        } else {
            cardi_common.DmaCall->Stop(cardi_common.dma);
            OS_DisableIrqMask(OS_IE_CARD_DATA);
            OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
            CARDiDmaReadRegisteredInfo = NULL;

            CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());
            if (info->callback) {
                (*info->callback)(info->userdata);
            }
        }
    }
}

void CARDi_ReadRomWithDMA(CARDTransferInfo *info) {
    OSIntrMode bak_psr = OS_DisableInterrupts();
    CARDiDmaReadRegisteredInfo = info;

    OS_SetIrqFunction(OS_IE_CARD_DATA, CARDi_DmaReadPageCallback);
    OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
    OS_EnableIrqMask(OS_IE_CARD_DATA);
    OS_RestoreInterrupts(bak_psr);

    cardi_common.DmaCall->Recv(cardi_common.dma, (void *)&reg_MI_MCD1, (void *)info->dst, CARD_ROM_PAGE_SIZE);

    CARDi_StartRomPageTransfer(info->src);
}

static void CARDi_DmaReadDone(void *userdata) {
    CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());

    CARDi_RefreshRom(CARD_ROMST_RFS_WARN_L2_MASK);

    cardi_common.cmd->result = CARD_RESULT_SUCCESS;
    CARDi_EndTask(&cardi_common);
}

static BOOL CARDi_IsRomDmaAvailable(u32 dma, void *dst, u32 src, u32 len) {
    return (dma <= MI_DMA_MAX_NUM) && (len > 0) && (((u32)dst & 31) == 0) &&
           (((u32)dst + len <= OS_GetITCMAddress()) || ((u32)dst >= OS_GetITCMAddress() + HW_ITCM_SIZE)) &&
           (((u32)dst + len <= OS_GetDTCMAddress()) || ((u32)dst >= OS_GetDTCMAddress() + HW_DTCM_SIZE)) &&
           (((src | len) & (CARD_ROM_PAGE_SIZE - 1)) == 0);
}

static void CARDi_ReadRomSyncCore(CARDiCommon *c) {
    (*CARDiReadRomFunction)(NULL, (void *)c->dst, c->src, c->len);

    CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());

    CARDi_RefreshRom(CARD_ROMST_RFS_WARN_L2_MASK);

    cardi_common.cmd->result = CARD_RESULT_SUCCESS;
}

void CARDi_ReadRom(u32 dma, const void *src, void *dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async) {
    CARDiCommon *c = &cardi_common;

    CARD_CheckEnabled();

    if ((CARDi_GetAccessLevel() & CARD_ACCESS_LEVEL_ROM) == 0) {
        OS_Terminate();
    }
    CARDi_WaitForTask(c, TRUE, callback, arg);
    c->DmaCall = CARDi_GetDmaInterface(dma);
    c->dma = (u32)((c->DmaCall != NULL) ? (dma & 3) : MI_DMA_NOT_USE);
    if (c->dma <= MI_DMA_MAX_NUM) {
        c->DmaCall->Stop(c->dma);
    }
    c->src = (u32)((u32)src + cardi_rom_base);
    c->dst = (u32)dst;
    c->len = (u32)len;

    {
        CARDTransferInfo *info = CARDiDmaReadInfo;
        info->callback = CARDi_DmaReadDone;
        info->userdata = NULL;
        info->src = c->src;
        info->dst = c->dst;
        info->len = c->len;
        info->work = 0;
    }

    if ((CARDiReadRomFunction == CARDi_ReadRomWithCPU) && CARDi_IsRomDmaAvailable(c->dma, (void *)c->dst, c->src, c->len)) {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        if (CARDiEnableCacheInvalidationIC) {
            CARDi_ICInvalidateSmart((void *)c->dst, c->len, c->flush_threshold_ic);
        }
        if (CARDiEnableCacheInvalidationDC) {
            CARDi_DCInvalidateSmart((void *)c->dst, c->len, c->flush_threshold_dc);
        }
        OS_RestoreInterrupts(bak_psr);

        CARDi_ReadRomWithDMA(CARDiDmaReadInfo);

        if (!is_async) {
            CARD_WaitRomAsync();
        }
    } else {
        if (CARDiEnableCacheInvalidationIC) {
            CARDi_ICInvalidateSmart((void *)c->dst, c->len, c->flush_threshold_ic);
        }

        CARDi_ExecuteOldTypeTask(CARDi_ReadRomSyncCore, is_async);
    }
}

#include "twl/ltdmain_begin.h"

u8 *CARDiHashBufferAddress = NULL;
u32 CARDiHashBufferLength = 0;
static CARDRomHashContext context[1];

static int CARDi_ReadCardWithHash(void *userdata, void *buffer, u32 offset, u32 length) {
    (void)userdata;
    CARD_ReadRomHashImage(context, buffer, offset, length);
    return (int)length;
}

static int CARDi_ReadCardWithHashInternalAsync(void *userdata, void *buffer, u32 offset, u32 length) {
    if (cardi_common.dma == MI_DMA_NOT_USE) {
        length = 0;
    } else {
        CARDRomHashContext *context = (CARDRomHashContext *)userdata;
        static CARDTransferInfo card_hash_info[1];
        DC_FlushRange(buffer, length);
        card_hash_info->callback = (void (*)(void *))CARD_NotifyRomHashReadAsync;
        card_hash_info->userdata = context;
        card_hash_info->src = offset;
        card_hash_info->dst = (u32)buffer;
        card_hash_info->len = length;
        card_hash_info->command = 0;
        card_hash_info->work = 0;
        CARDi_ReadRomWithDMA(card_hash_info);
    }
    return (int)length;
}

void CARDi_InitTwlRom(void) {
    u8 *lo = (u8 *)MATH_ROUNDUP((u32)OS_GetArenaLo(OS_ARENA_MAIN), 32);
    u8 *hi = (u8 *)OS_GetArenaHi(OS_ARENA_MAIN);
    u32 len = CARD_CalcRomHashBufferLength(CARD_GetOwnRomHeaderTWL());
    if (&lo[len] > (u8 *)MATH_ROUNDDOWN((u32)hi, 32)) {
        OS_Terminate();
    } else {
        OS_SetArenaLo(OS_ARENA_MAIN, &lo[len]);
        CARDiHashBufferAddress = lo;
        CARDiHashBufferLength = len;

        if ((OS_GetBootType() == OS_BOOTTYPE_ROM) && ((((const u8 *)HW_TWL_ROM_HEADER_BUF)[0x1C] & 0x01) != 0)) {
            cardi_common.dma = MI_DMA_NOT_USE;
            CARDiReadRomFunction = CARDi_ReadCardWithHash;
            {
                u16 id = (u16)OS_GetLockID();
                CARD_LockRom(id);
                CARD_InitRomHashContext(context, CARD_GetOwnRomHeaderTWL(), CARDiHashBufferAddress,
                                        CARDiHashBufferLength, CARDi_ReadRomWithCPU,
                                        CARDi_ReadCardWithHashInternalAsync, context);
                CARDiHashBufferAddress = NULL;
                CARDiHashBufferLength = 0;
                CARD_UnlockRom(id);
                OS_ReleaseLockID(id);
            }
        }
    }
}

#include "twl/ltdmain_end.h"

void CARDi_InitRom(void) {
    CARDiReadRomFunction = CARDi_ReadRomWithCPU;

    if ((OS_GetBootType() == OS_BOOTTYPE_ROM) && CARD_GetOwnRomHeader()->rom_size) {
        u16 id = (u16)OS_GetLockID();
        CARD_LockRom(id);
        CARDi_ReadRomWithCPU(NULL, CARDiOwnSignature, CARD_GetOwnRomHeader()->rom_size,
                             CARD_ROM_DOWNLOAD_SIGNATURE_SIZE);
        CARD_UnlockRom(id);
        OS_ReleaseLockID(id);
    }
    if (OS_IsRunOnTwl()) {
        CARDi_InitTwlRom();
    }
}

void CARD_WaitRomAsync(void) {
    CARDi_WaitAsync();
}

const u8 *CARDi_GetOwnSignature(void) {
    return CARDiOwnSignature;
}
