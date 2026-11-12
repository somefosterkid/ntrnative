// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/cp/cp_context.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitro/cp.h"

// Save the math coprocessor registers, including the current operation modes.
// pokebw2's CP_SaveContext
#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): the registers read in the asm's
// order, through the platform layer's registers at those addresses
void CP_SaveContext(CPContext *context) {
    vu32 *div = (vu32 *)REG_DIV_NUMER_ADDR;
    vu32 *sqrtParam = (vu32 *)&reg_CP_SQRT_PARAM;
    u16 divMode;

    context->divisionNumerator[0] = div[0];
    context->divisionNumerator[1] = div[1];
    context->divisionDenominator[0] = div[2];
    context->divisionDenominator[1] = div[3];
    divMode = reg_CP_DIVCNT;
    context->squareRootValue[0] = sqrtParam[0];
    context->squareRootValue[1] = sqrtParam[1];
    context->divisionMode = divMode & 3;
    context->squareRootMode = reg_CP_SQRTCNT & 1;
}
#else
asm void CP_SaveContext(CPContext *context) {
    ldr r1, [pc, #0x34]
    stmfd sp!, {r4}
    ldmia r1, {r2-r4, r12}
    stmia r0!, {r2-r4, r12}
    ldrh r12, [r1, #-16]
    add r1, r1, #40
    ldmia r1, {r2-r3}
    stmia r0!, {r2-r3}
    and r12, r12, #3
    ldrh r2, [r1, #-8]
    strh r12, [r0]
    and r2, r2, #1
    strh r2, [r0, #2]
    ldmfd sp!, {r4}
    bx lr
    dcd 0x04000290
}
#endif

// Restore the saved register values and modes.
// pokebw2's CPi_RestoreContext
#ifndef __MWERKS__
// C for a PC build: the parameters, then the modes, then the square root's parameter, as the asm writes them
void CPi_RestoreContext(const CPContext *context) {
    vu32 *div = (vu32 *)REG_DIV_NUMER_ADDR;
    vu32 *sqrtParam = (vu32 *)&reg_CP_SQRT_PARAM;

    div[0] = context->divisionNumerator[0];
    div[1] = context->divisionNumerator[1];
    div[2] = context->divisionDenominator[0];
    div[3] = context->divisionDenominator[1];
    reg_CP_DIVCNT = context->divisionMode;
    reg_CP_SQRTCNT = context->squareRootMode;
    sqrtParam[0] = context->squareRootValue[0];
    sqrtParam[1] = context->squareRootValue[1];
}
#else
asm void CPi_RestoreContext(const CPContext *context) {
    stmfd sp!, {r4}
    ldr r1, [pc, #0x2c]
    ldmia r0, {r2-r4, r12}
    stmia r1, {r2-r4, r12}
    ldrh r2, [r0, #24]
    ldrh r3, [r0, #26]
    strh r2, [r1, #-16]
    strh r3, [r1, #32]
    add r0, r0, #16
    add r1, r1, #40
    ldmia r0, {r2-r3}
    stmia r1, {r2-r3}
    ldmfd sp!, {r4}
    bx lr
    dcd 0x04000290
}
#endif
