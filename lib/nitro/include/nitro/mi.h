// NitroSDK's memory functions. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/nitro/include/nitro/mi.h, trimmed to what Black's C uses so far.
#ifndef BWPORT_NITRO_MI_H
#define BWPORT_NITRO_MI_H

#include "types.h"
#include "stddef.h"
#include "nitro/misc.h"

typedef enum {
    MI_PROCESSOR_ARM9,
    MI_PROCESSOR_ARM7,
} MIProcessor;

// Copy or fill size bytes a byte at a time: NitroSDK's MI_CpuCopy8 and MI_CpuFill8; a word at a time, 32 bytes at a
// time where it can: MIi_CpuCopy32 and MIi_CpuCopyFast
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u32 value, u32 size);
// TwlSDK's MI_CpuFill: fills size bytes, a word at a time where it can (mi_memory.c)
void MI_CpuFill(void *dest, u8 data, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void MIi_CpuCopyFast(const void *src, void *dest, u32 size);

// Copy or fill size bytes a halfword or a word at a time: NitroSDK's MIi_CpuCopy16, MIi_CpuClear16 and MIi_CpuClear32
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void MIi_CpuClear16(u16 value, void *dest, u32 size);
void MIi_CpuClear32(u32 value, void *dest, u32 size);
// A DMA fill of size bytes by the DMA channel dmaNo (g3x.c's and g2d_OAM.c's; its body is in another file)
void MIi_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size, BOOL dmaEnable);
// DMA copies of size bytes by the DMA channel dmaNo, a halfword or a word at a time, the async one calling
// callback(arg) when it ends, and the wait for a channel to end (main_020821c0.c)
void MIi_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size, BOOL dmaEnable);
void MIi_DmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size, BOOL dmaEnable);
void MIi_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *arg), void *arg,
                        BOOL dmaEnable);
void MI_WaitDma(u32 dmaNo);
void MI_StopDma(u32 dmaNo);
// Stops the game if DMA0 would read the I/O registers or the cartridge from src on (main_020821c0.c)
void MIi_CheckDma0SourceAddress(u32 dmaNo, uintptr_t src, u32 size, u32 dir);
// Stops the game if another channel than dmaNo runs an automatic DMA that clashes with one of dmaType
void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType);
// Writes a channel's three registers: src the address to read, or for MIi_DMA_MODE_SRC32/16 the value to fill with
// (mi_dma_itcm.c)
void MIi_DmaSetParameters(u32 dmaNo, uintptr_t src, uintptr_t dest, u32 ctrl, u32 flags);
// The same on the DSi's new DMA channels (gx.c's and g3x.c's)
void MI_NDmaFill(u32 dmaNo, void *dest, u32 data, u32 size);
void MI_Copy36B(const void *src, void *dest);
void MI_Copy64B(const void *src, void *dest);

// Fills size bytes, a multiple of 4, 32 bytes at a time where it can: NitroSDK's MIi_CpuClearFast
void MIi_CpuClearFast(u32 value, void *dest, u32 size);

// Copies size bytes, a multiple of 4, word by word to a fixed destination such as a FIFO: NitroSDK's MIi_CpuSend32
void MIi_CpuSend32(const void *src, volatile void *dest, u32 size);

// Decompresses LZ77 data, which starts with a word holding its decompressed size in its upper 24 bits.
// NitroSDK's MI_UncompressLZ8
void MI_UncompressLZ8(const void *src, void *dest);

static inline u32 MI_GetUncompressedSize(const void *src) {
    return *(u32 *)src >> 8;
}

#define reg_MI_EXMEMCNT (*(vu16 *)0x04000204)
#define REG_MI_EXMEMCNT_EP_MASK 0x8000
#define REG_MI_EXMEMCNT_EP_SHIFT 15

// NitroSDK's MI_CpuCopy16, an inline over MIi_CpuCopy16 (MIi_CpuCopy16). MWCC evaluates an inline call's arguments
// from the last, so the size is read before a destination that a call gives
static inline void MI_CpuCopy16(const void *src, void *dest, u32 size) {
    MIi_CpuCopy16(src, dest, size);
}

static inline void MI_CpuClear16(void *dest, u32 size) {
    MIi_CpuClear16(0, dest, size);
}

static inline void MI_CpuClear32(void *dest, u32 size) {
    MIi_CpuClear32(0, dest, size);
}

static inline void MI_CpuClearFast(void *dest, u32 size) {
    MIi_CpuClearFast(0, dest, size);
}

// Compares size bytes, returning the difference of the first that differ, or 0
int sys_memcmp(const void *a, const void *b, u32 size);

// Which processor has priority on the main memory
static inline void MI_SetMainMemoryPriority(MIProcessor proc) {
    reg_MI_EXMEMCNT = (u16)((reg_MI_EXMEMCNT & ~REG_MI_EXMEMCNT_EP_MASK) | (proc << REG_MI_EXMEMCNT_EP_SHIFT));
}

// Swan's MAT43_Copy
void MI_Copy48B(const void *src, void *dest);

// The shared WRAM's bank control: which processor gets which half of it
#define reg_MI_WRAMCNT (*(vu8 *)0x04000247)

// Who gets the shared WRAM's two 16 KiB halves: the ARM7 gets those that the ARM9 doesn't
typedef enum {
    MI_WRAM_ARM9_ALL,
    MI_WRAM_ARM9_2ND_HALF,
    MI_WRAM_ARM9_1ST_HALF,
    MI_WRAM_ARM7_ALL,
} MIWramBank;

// The DMA channels: three words each, the source and destination addresses and the control word
#define REG_DMA0SAD_ADDR 0x040000b0

#define REG_GXFIFO_ADDR 0x04000400
// NitroSDK's nitro/hw/ARM9/ioreg_MI.h: a DMA channel's enable bit
#define REG_MI_DMA0CNT_E_MASK 0x80000000

typedef struct MIDmaRegs {
    vu32 src;
    vu32 dest;
    vu32 cnt;
} MIDmaRegs;

#define MI_DMA_REGS ((MIDmaRegs *)REG_DMA0SAD_ADDR)

// The bits of a DMA control word
// NitroSDK's: no DMA, a CPU copy
#define MI_DMA_NOT_USE 0xffffffff
#define MI_DMA_ENABLE 0x80000000

#define MI_DMA_IF_ENABLE 0x40000000
#define MI_DMA_TIMING_MASK 0x38000000
#define MI_DMA_TIMING_IMM 0x00000000
#define MI_DMA_TIMING_V_BLANK 0x08000000
#define MI_DMA_TIMING_H_BLANK 0x10000000
#define MI_DMA_TIMING_DISP 0x18000000
#define MI_DMA_TIMING_DISP_MMEM 0x20000000
#define MI_DMA_TIMING_CARD 0x28000000
#define MI_DMA_TIMING_CARTRIDGE 0x30000000
#define MI_DMA_TIMING_GXFIFO 0x38000000
#define MI_DMA_CONTINUOUS_ON 0x02000000
#define MI_DMA_32BIT_BUS 0x04000000
#define MI_DMA_SRC_INC 0x00000000
#define MI_DMA_SRC_DEC 0x00800000
#define MI_DMA_SRC_FIX 0x01000000
#define MI_DMA_DEST_INC 0x00000000
#define MI_DMA_DEST_DEC 0x00200000
#define MI_DMA_DEST_FIX 0x00400000
#define MIi_DMA_MODE_NOINT 0x01
#define MIi_DMA_MODE_WAIT 0x02
#define MIi_DMA_MODE_NOCLEAR 0x04
#define MIi_DMA_MODE_SRC32 0x10
#define MIi_DMA_MODE_SRC16 0x20

// NitroSDK's MI_SwapEndian32: the word with its bytes in the other order, as the card's commands go
#define MI_SwapEndian32(x)                                                                                             \
    ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8) | ((x) << 24))

// NitroSDK's MI_Init: gives the shared WRAM to the ARM7, sets up the new DMA on a DSi and stops DMA channel 0. OS_Init
// calls it
void MI_Init(void);
void MI_StopAllDma(void);   // swan's dma_endAll
// Copies 32 bytes, a register's table: NitroSDK's MI_Copy32B
void MI_Copy32B(const void *src, void *dest); // swan's gfxUploadFogTable

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/mi/byteAccess.h, include/nitro/mi/endian.h, include/nitro/mi/exMemory.h, include/nitro/mi/memory.h
    static inline u8 MI_ReadByte (const void * address)
    {
        return *(u8 *)address;
    }
    static inline void MI_WriteByte (void * address, u8 value)
    {
        *(u8 *)address = value;
    }
static inline u8 MI_LoadLE8 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u8)((src[0] << 0UL));
}
static inline u16 MI_LoadLE16 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u16)((src[0] << 0UL) | (src[1] << 8UL));
}
static inline u32 MI_LoadLE32 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u32)((src[0] << 0UL) | (src[1] << 8UL) | (src[2] << 16UL) | (src[3] << 24UL));
}
static inline u8 MI_LoadBE8 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u8)((src[0] << 0UL));
}
static inline u16 MI_LoadBE16 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u16)((src[0] << 8UL) | (src[1] << 0UL));
}
static inline u32 MI_LoadBE32 (const void * ptr)
{
	const u8 * src = (const u8 *)ptr;
	return (u32)((src[0] << 24UL) | (src[1] << 16UL) | (src[2] << 8UL) | (src[3] << 0UL));
}
static inline void MI_StoreLE8 (void * ptr, u8 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 0UL);
}
static inline void MI_StoreLE16 (void * ptr, u16 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 0UL);
	src[1] = (u8)(val >> 8UL);
}
static inline void MI_StoreLE32 (void * ptr, u32 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 0UL);
	src[1] = (u8)(val >> 8UL);
	src[2] = (u8)(val >> 16UL);
	src[3] = (u8)(val >> 24UL);
}
static inline void MI_StoreBE8 (void * ptr, u8 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 0UL);
}
static inline void MI_StoreBE16 (void * ptr, u16 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 8UL);
	src[1] = (u8)(val >> 0UL);
}
static inline void MI_StoreBE32 (void * ptr, u32 val)
{
	u8 * src = (u8 *)ptr;
	src[0] = (u8)(val >> 24UL);
	src[1] = (u8)(val >> 16UL);
	src[2] = (u8)(val >> 8UL);
	src[3] = (u8)(val >> 0UL);
}
static inline MIProcessor MI_GetMainMemoryPriority (void)
{
	return (MIProcessor)((reg_MI_EXMEMCNT & REG_MI_EXMEMCNT_EP_MASK) >> REG_MI_EXMEMCNT_EP_SHIFT);
}
static inline void MI_CpuFill32 (void * dest, u32 data, u32 size)
{
	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "destination address must be in 4-byte alignment");
	MIi_CpuClear32(data, dest, size);
}
static inline void MI_CpuCopy32 (const void * src, void * dest, u32 size)
{
	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)src & 3) == 0, "source address must be in 4-byte alignment");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "destination address must be in 4-byte alignment");
	MIi_CpuCopy32(src, dest, size);
}
static inline void MI_CpuSend32 (const void * src, volatile void * dest, u32 size)
{
	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)src & 3) == 0, "source address must be in 4-byte alignment");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "destination address must be in 4-byte alignment");
	MIi_CpuSend32(src, dest, size);
}
static inline void MI_CpuFill16 (void * dest, u16 data, u32 size)
{
	SDK_ASSERTMSG((size & 1) == 0, "size & 1 must be 0");
	SDK_ASSERTMSG(((u32)dest & 1) == 0, "source address must be in 2-byte alignment");
	MIi_CpuClear16(data, dest, size);
}
static inline void MI_CpuFillFast (void * dest, u32 data, u32 size)
{
	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "source address must be in 4-byte alignment");
	MIi_CpuClearFast(data, dest, size);
}
static inline void MI_CpuCopyFast (const void * src, void * dest, u32 size)
{
	SDK_ASSERTMSG((size & 3) == 0, "size & 3 must be 0");
	SDK_ASSERTMSG(((u32)src & 3) == 0, "source address must be in 4-byte alignment");
	SDK_ASSERTMSG(((u32)dest & 3) == 0, "destination address must be in 4-byte alignment");
	MIi_CpuCopyFast(src, dest, size);
}
static inline void MI_CpuClear8 (void * dest, u32 size)
{
	MI_CpuFill8(dest, 0, size);
}
static inline u64 MI_LoadLE64(const void *ptr) {
  const u8 *src = (const u8 *)ptr;
  return ((u64)MI_LoadLE32(src + 4) << 32) | (u64)MI_LoadLE32(src);
}
static inline u64 MI_LoadBE64(const void *ptr) {
  const u8 *src = (const u8 *)ptr;
  return ((u64)MI_LoadLE32(src) << 32) | (u64)MI_LoadLE32(src + 4);
}
static inline void MI_StoreLE64(void *ptr, u64 val) {
  u8 *src = (u8 *)ptr;
  MI_StoreLE32(src, (u32)(val >> 0));
  MI_StoreLE32(src + 4, (u32)(val >> 32));
}
static inline void MI_StoreBE64(void *ptr, u64 val) {
  u8 *src = (u8 *)ptr;
  MI_StoreLE32(src, (u32)(val >> 32));
  MI_StoreLE32(src + 4, (u32)(val >> 0));
}

// pokebw2's MI_InitWramManager
// On a DSi, sets up TwlSDK's shared WRAM manager, which is in the LTD autoload (MIi_InitWramManagerCore). OS_Init calls
// it
typedef void (*MIDmaCallback)(void *arg);

void func_02082198(void);

void MI_SendGXCommandAsync(u32 dmaNo, const void *src, u32 length, MIDmaCallback callback, void *arg);

void MI_SendGXCommandAsyncFast(u32 dmaNo, const void *src, u32 length, MIDmaCallback callback, void *arg);

void MI_Zero36B(void *dest);



#endif // BWPORT_NITRO_MI_H
