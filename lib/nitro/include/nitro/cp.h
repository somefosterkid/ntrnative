// The ARM9's divider and square root unit. Names as in pret's pokeheartgold (lib/include/nitro/hw/ARM9/io_reg.h).
#ifndef BWPORT_NITRO_CP_H
#define BWPORT_NITRO_CP_H

#include "types.h"

// The 64-bit parameters are not volatile: declared volatile, their stores are scheduled unlike the ROM's
// (VEC_Normalize)
#define reg_CP_DIVCNT (*(vu16 *)0x04000280)
#define reg_CP_DIV_NUMER (*(s64 *)0x04000290)
#define reg_CP_DIV_DENOM (*(s64 *)0x04000298)
#define reg_CP_DIV_RESULT (*(vs64 *)0x040002a0)
#define reg_CP_SQRTCNT (*(vu16 *)0x040002b0)
#define reg_CP_SQRT_RESULT (*(vu32 *)0x040002b4)
#define reg_CP_SQRT_PARAM (*(u64 *)0x040002b8)

// Set while the unit computes
#define REG_CP_DIVCNT_BUSY_MASK 0x8000
#define REG_CP_SQRTCNT_BUSY_MASK 0x8000

// Snapshot of the divider and square-root hardware registers.
typedef struct {
    u32 divisionNumerator[2];
    u32 divisionDenominator[2];
    u32 squareRootValue[2];
    u16 divisionMode;
    u16 squareRootMode;
} CPContext;

// pokebw2's CP_SaveContext
void CP_SaveContext(CPContext *context);
// pokebw2's CPi_RestoreContext
void CPi_RestoreContext(const CPContext *context);

#define reg_CP_DIV_NUMER_LO (*(vu32 *)0x04000290)
#define reg_CP_DIV_RESULT_LO (*(vu32 *)0x040002a0)
#define reg_CP_DIVREM_RESULT (*(vu32 *)0x040002a8)
#define REG_CP_BUSY_MASK 0x8000

// The divider's registers by address, which NitroSDK's divider inlines write and read through plain pointers
#define REG_DIV_NUMER_ADDR 0x04000290

#define REG_DIV_DENOM_ADDR 0x04000298
#define REG_DIV_RESULT_ADDR 0x040002a0

// The divider's mode for a 64-bit numerator and a 32-bit denominator
#define CP_DIV_64_32BIT_MODE 1

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/cp/context.h, include/nitro/cp/divider.h, include/nitro/cp/sqrt.h
static inline void CP_SetDivControl (u16 parameter)
{
	reg_CP_DIVCNT = parameter;
}
static inline void CP_SetDivImm32_32_NS_ (u32 numer, u32 denom)
{
	*(REGType32 *)REG_DIV_NUMER_ADDR = numer;
	*(REGType64 *)REG_DIV_DENOM_ADDR = denom;
}
static inline void CP_SetDivImm64_32_NS_ (u64 numer, u32 denom)
{
	*(REGType64 *)REG_DIV_NUMER_ADDR = numer;
	*(REGType64 *)REG_DIV_DENOM_ADDR = denom;
}
static inline void CP_SetDivImm64_64_NS_ (u64 numer, u64 denom)
{
	*(REGType64 *)REG_DIV_NUMER_ADDR = numer;
	*(REGType64 *)REG_DIV_DENOM_ADDR = denom;
}
static inline s32 CP_IsDivBusy (void)
{
	return (reg_CP_DIVCNT & REG_CP_DIVCNT_BUSY_MASK);
}
static inline s64 CP_GetDivResultImm64 (void)
{
	return (s64)(*(REGType64 *)REG_DIV_RESULT_ADDR);
}
static inline s32 CP_GetDivResultImm32 (void)
{
	return (s32)(*(REGType32 *)REG_DIV_RESULT_ADDR);
}
static inline s16 CP_GetDivResultImm16 (void)
{
	return (s16)(*(REGType16 *)REG_DIV_RESULT_ADDR);
}
static inline s8 CP_GetDivResultImm8 (void)
{
	return (s8)(*(REGType8 *)REG_DIV_RESULT_ADDR);
}
static inline void CP_SetSqrtControl (u16 param)
{
	reg_CP_SQRTCNT = param;
}
static inline s32 CP_IsSqrtBusy (void)
{
	return (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK);
}
static inline void CP_SetDivImm32_32 (u32 numer, u32 denom)
{
	CP_SetDivImm32_32_NS_(numer, denom);
}
static inline void CP_SetDivImm64_32 (u64 numer, u32 denom)
{
	CP_SetDivImm64_32_NS_(numer, denom);
}
static inline void CP_SetDivImm64_64 (u64 numer, u64 denom)
{
	CP_SetDivImm64_64_NS_(numer, denom);
}
static inline void CP_SetDiv64_32 (u64 numer, u32 denom)
{
	reg_CP_DIVCNT = CP_DIV_64_32BIT_MODE;
	CP_SetDivImm64_32_NS_(numer, denom);
}
static inline void CP_WaitDiv (void)
{
	while (CP_IsDivBusy()) {
	}
}
static inline void CP_WaitSqrt (void)
{
	while (CP_IsSqrtBusy()) {
	}
}
static inline void CP_RestoreContext (const CPContext * context)
{
	CPi_RestoreContext(context);
	CP_WaitDiv();
}
static inline s64 CP_GetDivResult64 (void)
{
	CP_WaitDiv();
	return CP_GetDivResultImm64();
}
static inline s32 CP_GetDivResult32 (void)
{
	CP_WaitDiv();
	return CP_GetDivResultImm32();
}
static inline s16 CP_GetDivResult16 (void)
{
	CP_WaitDiv();
	return CP_GetDivResultImm16();
}

// The square root unit's registers by address and its 32-bit mode, as NitroSDK's sqrt inlines use them
#define REG_SQRT_RESULT_ADDR 0x040002b4
#define REG_SQRT_PARAM_ADDR 0x040002b8
#define CP_SQRT_32BIT_MODE 0
static inline void CP_SetSqrtImm32_NS_ (u32 param)
{
	*((REGType32 *)REG_SQRT_PARAM_ADDR) = param;
}
static inline void CP_SetSqrt32 (u32 param)
{
	reg_CP_SQRTCNT = CP_SQRT_32BIT_MODE;
	CP_SetSqrtImm32_NS_(param);
}
static inline u32 CP_GetSqrtResultImm32 (void)
{
	return (u32)(*((REGType32 *)REG_SQRT_RESULT_ADDR));
}
static inline u32 CP_GetSqrtResult32 (void)
{
	CP_WaitSqrt();
	return CP_GetSqrtResultImm32();
}
static inline s8 CP_GetDivResult8 (void)
{
	CP_WaitDiv();
	return CP_GetDivResultImm8();
}

#endif // BWPORT_NITRO_CP_H
