#include "types.h"
// Metrowerks' fast floating-point and long-division runtime, which the compiler calls by name for float, double, 64-bit
// and division operations (the maintainers' notes, `_s32_div_f`). Shipped as ARM assembly, so each routine is to be written as
// an MWCC asm function (the rules). The routines' names are pokebw2's, which compiled code links against; the rest are
// unnamed. The file's name is a guess. Part of it is ARM's floating-point library as ~/ref's FP_fullI_v4t_LE.a (libntr's
// and NitroSDK's components/ichneumon) has it, byte for byte (tools/prebuilt_map.py): _f_cmp.o's eleven compares
// (_dgeq to _fneq, there _d_fge to _f_fne), _ll_div.o (_ll_mod), _ll_mul.o, _ll_sll.o, _s32_div_f.o and
// _u32_div_f.o; its other routines (_dadd, _dmul, the conversions) are another build's.

// Black's functions in order (Black's size), to write below:
// _dadd: 0x318
// _drsb: 0x18
// _dsub: 0x3b4
// _dmul: 0x364
// _d2f: 0x104
// _dflt: 0x40
// _dfltu: 0x3c
// _dgeq: 0x98
// _dgr: 0x98
// _dleq: 0xa4
// _dls: 0x9c
// _deq: 0x8c
// _dneq: 0x8c
// _fgeq: 0x5c
// _fgr: 0x5c
// _fleq: 0x68
// _fls: 0x5c
// _fneq: 0x68
// _f2d: 0x84
// _fflt: 0x48
// _ffltu: 0x48
// _ll_mod: 0x1c0
// _ll_udiv: 0x48
// _ll_mul: 0x20
// _ll_shl: 0x30
// _s32_div_f: 0x20c
// _u32_div_f: 0x1e4
// _ffix: 0x34
// _ffixu: 0x40
// _dfix: 0x4c
// __aeabi_d2ulz: 0x8c
// sqrt: 0x19c, zeldaret-st's name for the same function; it sets errno to EDOM
// _fmul: 0x1e0
// _fadd: 0x21c
// func_0209cb78: 0x8, the tail of _fadd's code (mvn r0, #0x80000000), then _frsb, which _fsub's code branches to
// _frsb: 0xc
// _fsub: 0x278
// _frdiv: 0xc
// _fdiv: 0x3b8
// _drdiv: 0x18
// _ddiv: 0x544


extern unsigned __builtin_clz(unsigned);

#pragma thumb off



asm double __aeabi_dadd(double a, double b) {
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4a0000cb  // bmi doubleSub + 0x10
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @L_0209ac30
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@L_0209ac30:
    mov lr, #0x80000000
    mov r12, r1, lsr #20
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r4, r12, lsl #21
    cmnne r4, #0x200000
    beq @L_0209ad2c
    mov r4, r3, lsr #20
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs lr, r4, lsl #21
    beq @L_0209ad74
@L_0209ac68:
    subs r4, r12, r4
    beq @L_0209acc0
    cmp r4, #0x20
    ble @L_0209aca4
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    blo @L_0209ace8
    b @L_0209accc
@L_0209aca4:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
@L_0209acc0:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @L_0209ace8
@L_0209accc:
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, r12, lsl #21
    cmn lr, #0x200000
    beq @L_0209aef8
@L_0209ace8:
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, lr}
    bx lr
@L_0209ad2c:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @L_0209ad98
    orrs r4, r0, r1, lsl #1
    bne @L_0209aed4
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq @L_0209aec0
    cmn r4, #0x200000
    bne @L_0209aec0
    orrs r4, r2, r3, lsl #1
    beq @L_0209aec0
    b @L_0209aed4
@L_0209ad74:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @L_0209ae04
    orrs r4, r2, r3, lsl #1
    bne @L_0209aed4
    b @L_0209aec0
@L_0209ad98:
    orrs r4, r0, r1, lsl #1
    beq @L_0209add8
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    cmnne r4, #0x200000
    mov r4, r4, lsr #21
    orr r4, r4, lr, lsr #20
    beq @L_0209ad74
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @L_0209ac68
@L_0209add8:
    mov r12, r3, lsr #20
    mov r1, r3, lsl #11
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #11
    movs r4, r12, lsl #21
    beq @L_0209ae8c
    cmn r4, #0x200000
    bne @L_0209ae8c
    orrs r4, r0, r1, lsl #1
    beq @L_0209aec0
    b @L_0209aed8
@L_0209ae04:
    orrs r4, r2, r3, lsl #1
    beq @L_0209ae9c
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl @L_0209ae28
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @L_0209ac68
@L_0209ae28:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @L_0209ae48
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
@L_0209ae48:
    cmp r1, #0
    subges r12, r12, #1
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqia sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
@L_0209ae8c:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
@L_0209ae9c:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@L_0209aec0:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@L_0209aed4:
    mov r1, r3
@L_0209aed8:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@L_0209aef8:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
}

// b - a: the operands swapped, then _dsub (FP_fullI_v4t_LE.a's _d_sub.o starts with it)
asm double _drsb(double a, double b) {
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}

asm double doubleSub(double a, double b) {
    stmdb sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4affff33  // bmi __aeabi_dadd + 0x10
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @L_0209af64
    eor lr, lr, #0x80000000
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@L_0209af64:
    mov lr, #0x80000000
    mov r12, r1, lsr #20
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r4, r12, lsl #21
    cmnne r4, #0x200000
    beq @L_0209b168
    mov r4, r3, lsr #20
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs lr, r4, lsl #21
    beq @L_0209b1b0
@L_0209af9c:
    subs r4, r12, r4
    beq @L_0209b044
    cmp r4, #0x20
    ble @L_0209afd8
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi @L_0209b000
    b @L_0209b0f0
@L_0209afd8:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl @L_0209b0f0
@L_0209b000:
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqia sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b044:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq @L_0209b2d4
    mov lr, r12, lsl #20
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bmi @L_0209b0cc
    bne @L_0209b07c
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @L_0209b098
@L_0209b07c:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
@L_0209b098:
    cmp r12, #0
    bgt @L_0209b0d4
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b0cc:
    cmp r1, #0
    subges r12, r12, #1
@L_0209b0d4:
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b0f0:
    mov lr, r12, lsl #20
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bne @L_0209b114
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @L_0209b130
@L_0209b114:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub r12, r12, r4
@L_0209b130:
    cmp r12, #0
    orrgt r12, r12, lr, lsr #20
    bgt @L_0209b000
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b168:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @L_0209b1d4
    orrs r4, r0, r1, lsl #1
    bne @L_0209b2b0
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq @L_0209b29c
    cmn r4, #0x200000
    bne @L_0209b29c
    orrs r4, r2, r3, lsl #1
    beq @L_0209b2c4
    b @L_0209b2b0
@L_0209b1b0:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @L_0209b24c
    orrs r4, r2, r3, lsl #1
    bne @L_0209b2b0
    b @L_0209b29c
@L_0209b1d4:
    orrs r4, r0, r1, lsl #1
    beq @L_0209b214
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    cmnne r4, #0x200000
    mov r4, r4, lsr #21
    orr r4, r4, lr, lsr #20
    beq @L_0209b1b0
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @L_0209af9c
@L_0209b214:
    mov r12, r3, lsr #20
    mov r1, r3, lsl #11
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #11
    movs r4, r12, lsl #21
    beq @L_0209b240
    cmn r4, #0x200000
    bne @L_0209b268
    orrs r4, r0, r1, lsl #1
    bne @L_0209b2b4
    b @L_0209b29c
@L_0209b240:
    orrs r4, r0, r1, lsl #1
    beq @L_0209b2d4
    b @L_0209b268
@L_0209b24c:
    orrs r4, r2, r3, lsl #1
    beq @L_0209b278
    mov r4, #1
    bic r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @L_0209af9c
@L_0209b268:
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b278:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b29c:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b2b0:
    mov r1, r3
@L_0209b2b4:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b2c4:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, lr}
    bx lr
@L_0209b2d4:
    mov r1, #0
    mov r0, #0
    ldmia sp!, {r4, lr}
    bx lr
}


asm double __aeabi_dmul(double a, double b) {
    // Unpacks a: lr = the sign of the product, r12 = a's exponent, r1:r0 = its mantissa with the integer bit at bit 63
    stmdb sp!, {r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov r12, r1, lsr #20
    mov r1, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r6, r12, lsl #21
    cmnne r6, #0x200000
    beq @a_special
    orr r1, r1, #0x80000000
    bic r12, r12, #0x800
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq @b_special
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
@multiply:
    // r12 = the sum of the exponents. The 128-bit product of the mantissas is r1:r0:r4:r5, whose low words fold into
    // a sticky bit in r0
    add r12, r4, r12
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt @bias
    sub r12, r12, #1
    adds r0, r0, r0
    adc r1, r1, r1
@bias:
    // Removes the bias and sends results with an exponent of 0 or less to underflow
    add r12, r12, #2
    subs r12, r12, #0x400
    bmi @underflow
    beq @underflow
    mov r6, r12, lsl #20
    cmn r6, #0x100000
    bmi @overflow
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@a_special:
    bics r12, r12, #0x800
    beq @a_denormal
    orrs r6, r0, r1, lsl #1
    bne @nan
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq @a_inf_b_exp0
    cmn r5, #0x200000
    bne @inf
    orrs r5, r2, r3, lsl #1
    beq @inf
    b @nan
@a_inf_b_exp0:
    orrs r5, r3, r2
    beq @nan_inf_zero
    b @inf
@b_special:
    bics r4, r4, #0x800
    beq @b_denormal
    orrs r6, r2, r3, lsl #1
    bne @nan
    b @inf
@a_denormal:
    orrs r6, r0, r1, lsl #1
    beq @a_zero
    mov r12, #1
    cmp r1, #0
    bne @a_normalize
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @unpack_b
@a_normalize:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #0x20
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #0x20
    mov r0, r0, lsl r6
    sub r12, r12, r6
@unpack_b:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq @b_special
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b @multiply
@a_zero:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq @zero
    cmn r5, #0x200000
    bne @zero
    orrs r6, r2, r3, lsl #1
    beq @nan_inf_zero
    b @nan
@b_denormal:
    orrs r5, r2, r3, lsl #1
    beq @zero
    mov r4, #1
    cmp r3, #0
    bne @b_normalize
    sub r4, r4, #0x20
    movs r3, r2
    mov r2, #0
    bmi @multiply
@b_normalize:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #0x20
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #0x20
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b @multiply
@underflow:
    cmn r12, #0x34
    beq @half_min
    bmi @zero_underflow
    mov r2, r1
    mov r3, r0
    add r4, r12, #0x34
    cmp r4, #0x20
    movge r2, r3
    movge r3, #0
    subge r4, r4, #0x20
    rsb r5, r4, #0x20
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    cmp r2, #0
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqia sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@half_min:
    orr r0, r0, r1, lsl #1
    b @min_denormal
@inf:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@nan:
    mov r1, r3
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@nan_inf_zero:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@min_denormal:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    addne r0, r0, #1
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@zero_underflow:
    mov r1, lr
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@overflow:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
@zero:
    mov r1, lr
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
}

asm float __aeabi_d2f(double x) {
    // r2 = the sign; r12 = the exponent, rebased from 0x3ff to 0x7f at 0x380
    and r2, r1, #0x80000000
    mov r12, r1, lsr #20
    bics r12, r12, #0x800
    beq @zero_or_denormal
    mov r3, r12, lsl #21
    cmn r3, #0x200000
    bhs @inf_or_nan
    subs r12, r12, #0x380
    bls @denormal_result
    cmp r12, #0xff
    bge @overflow
    mov r1, r1, lsl #12
    orr r3, r2, r1, lsr #9
    orr r3, r3, r0, lsr #29
    movs r1, r0, lsl #3
    orr r0, r3, r12, lsl #23
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@inf_or_nan:
    orrs r3, r0, r1, lsl #12
    bne @nan
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@nan:
    mvn r0, #0x80000000
    bx lr
@zero_or_denormal:
    orrs r3, r0, r1, lsl #12
    bne @zero
    mov r0, r2
    bx lr
@denormal_result:
    cmn r12, #0x17
    beq @min_denormal
    bmi @zero
    mov r1, r1, lsl #11
    orr r1, r1, #0x80000000
    mov r3, r1, lsr #8
    orr r3, r3, r0, lsr #29
    rsb r12, r12, #1
    movs r1, r0, lsl #3
    orr r0, r2, r3, lsr r12
    rsb r12, r12, #0x20
    mov r3, r3, lsl r12
    orrne r3, r3, #1
    movs r1, r3
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@min_denormal:
    orr r0, r0, r1, lsl #12
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@zero:
    mov r0, r2
    bx lr
@overflow:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
}

asm double __aeabi_i2d(int x) {
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    movs r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}

asm double __aeabi_ui2d(unsigned int x) {
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    bmi @pack
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@pack:
    mov r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}

asm int _dgeq(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    movhs r0, #1
    movlo r0, #0
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhs r0, #1
    movlo r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int __aeabi_dcmpgt(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #1
    movls r0, #0
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #1
    movls r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int _dleq(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    movls r0, #1
    movhi r0, #0
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    bne @compare
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    orr r12, r12, #0x40000000
    msr cpsr_f, r12
    bxeq lr
@compare:
    cmp r3, r1
    cmpeq r2, r0
    movls r0, #1
    movhi r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int _dls(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    movlo r0, #1
    movhs r0, #0
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    bne @compare
    mrs r12, cpsr
    orr r12, r12, #0x20000000
    msr cpsr_f, r12
    bxeq lr
@compare:
    cmp r3, r1
    cmpeq r2, r0
    movlo r0, #1
    movhs r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int _deq(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #1
    movne r0, #0
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #1
    movne r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int _dneq(double a, double b) {
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @a_inf_or_nan
    cmn r12, r3, lsl #1
    bhs @b_inf_or_nan
@ordered:
    orrs r12, r3, r1
    bmi @negative
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #1
    moveq r0, #0
    bx lr
@unordered:
    mov r0, #1
    mrs r12, cpsr
    bic r12, r12, #0x40000000
    msr cpsr_f, r12
    bx lr
@negative:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #1
    moveq r0, #0
    bx lr
@a_inf_or_nan:
    bne @unordered
    cmp r0, #0
    bhi @unordered
    cmn r12, r3, lsl #1
    blo @ordered
@b_inf_or_nan:
    bne @unordered
    cmp r2, #0
    bhi @unordered
    b @ordered
}

asm int _fgeq(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @unordered
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movge r0, #1
    movlt r0, #0
    mrs r12, cpsr
    biclt r12, r12, #0x20000000
    orrge r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
}

asm int _fgr(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @unordered
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movgt r0, #1
    movle r0, #0
    mrs r12, cpsr
    bicle r12, r12, #0x20000000
    orrgt r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
@unordered:
    mov r0, #0
    mrs r12, cpsr
    bic r12, r12, #0x20000000
    msr cpsr_f, r12
    bx lr
}

int _fleq(float a, float b)
{
    __asm
    {
        mov r3, #0xFF000000
        cmp r3, r0, lsl #1
        cmphs r3, r1, lsl #1
        blo done
        cmp r0, #0
        bicmi r0, r0, #0x80000000
        rsbmi r0, r0, #0
        cmp r1, #0
        bicmi r1, r1, #0x80000000
        rsbmi r1, r1, #0
        cmp r0, r1
        movle r0, #1
        movgt r0, #0
        mrs ip, cpsr
        orrgt ip, ip, #0x20000000
        bicgt ip, ip, #0x40000000
        bicle ip, ip, #0x20000000
        orrle ip, ip, #0x40000000
        msr cpsr_f, ip
        bx lr
    done:
        mov r0, #0
        mrs ip, cpsr
        bic ip, ip, #0x40000000
        orr ip, ip, #0x20000000
        msr cpsr_f, ip
    }
}

int _fls(float a, float b)
{
    __asm
    {
        mov r3, #0xFF000000
        cmp r3, r0, lsl #1
        cmphs r3, r1, lsl #1
        blo done
        cmp r0, #0
        bicmi r0, r0, #0x80000000
        rsbmi r0, r0, #0
        cmp r1, #0
        bicmi r1, r1, #0x80000000
        rsbmi r1, r1, #0
        cmp r0, r1
        movlt r0, #1
        movge r0, #0
        mrs ip, cpsr
        orrge ip, ip, #0x20000000
        biclt ip, ip, #0x20000000
        msr cpsr_f, ip
        bx lr
    done:
        mov r0, #0
        mrs ip, cpsr
        orr ip, ip, #0x20000000
        msr cpsr_f, ip
    }
}

int _fneq(float a, float b)
{
    __asm
    {
        mov r3, #0xFF000000
        cmp r3, r0, lsl #1
        blo done
        cmp r3, r1, lsl #1
        blo done
        orr r3, r0, r1
        movs r3, r3, lsl #1
        moveq r0, #0
        bne notzero
        mrs ip, cpsr
        orr ip, ip, #0x40000000
        msr cpsr_f, ip
        bx lr
    notzero:
        cmp r0, r1
        movne r0, #1
        moveq r0, #0
        mrs ip, cpsr
        bicne ip, ip, #0x40000000
        orreq ip, ip, #0x40000000
        msr cpsr_f, ip
        bx lr
    done:
        mov r0, #1
        mrs ip, cpsr
        bic ip, ip, #0x40000000
        msr cpsr_f, ip
    }
}

asm double __aeabi_f2d(float x) {
    and r2, r0, #0x80000000
    mov r12, r0, lsr #23
    mov r3, r0, lsl #9
    ands r12, r12, #0xff
    beq @zero_or_denormal
    cmp r12, #0xff
    beq @inf_or_nan
@pack:
    add r12, r12, #0x380
    mov r0, r3, lsl #20
    orr r1, r2, r3, lsr #12
    orr r1, r1, r12, lsl #20
    bx lr
@zero_or_denormal:
    cmp r3, #0
    bne @denormal
    mov r1, r2
    mov r0, #0
    bx lr
@denormal:
    mov r3, r3, lsr #1
    clz r12, r3
    movs r3, r3, lsl r12
    rsb r12, r12, #1
    add r3, r3, r3
    b @pack
@inf_or_nan:
    cmp r3, #0
    bhi @nan
    ldr r1, =0x7ff00000
    orr r1, r1, r2
    mov r0, #0
    bx lr
@nan:
    mvn r0, #0
    bic r1, r0, #0x80000000
    bx lr
}

float __aeabi_i2f(int x)
{
    __asm
    {
        ands r2, r0, #0x80000000
        rsbmi r0, r0, #0x0
        cmp r0, #0x0
        bxeq lr
        clz r3, r0
        movs r0, r0, lsl r3
        rsb r3, r3, #0x9e
        ands r1, r0, #0xff
        add r0, r0, r0
        orr r0, r2, r0, lsr #0x9
        orr r0, r0, r3, lsl #0x17
        bxeq lr
        tst r1, #0x80
        bxeq lr
        ands r3, r1, #0x7f
        andeqs r3, r0, #0x1
        addne r0, r0, #0x1
    }
}

asm float _ffltu(unsigned int x) {
    cmp r0, #0
    bxeq lr
    mov r3, #0x9e
    bmi @pack
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@pack:
    ands r2, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r2, #0x80
    bxeq lr
    ands r1, r2, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
}

asm long long _ll_mod(long long a, long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    b @L_0209be6c
@_ll_sdiv:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
@L_0209be6c:
    orrs r5, r3, r2
    bne @L_0209be7c
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L_0209be7c:
    mov r5, r0, lsr #31
    add r5, r5, r1
    mov r6, r2, lsr #31
    add r6, r6, r3
    orrs r6, r5, r6
    bne @L_0209beb0
    mov r1, r2
    dcd 0xeb000081  // bl _s32_div_f
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #31
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L_0209beb0:
    cmp r1, #0
    bge @L_0209bec0
    rsbs r0, r0, #0
    rsc r1, r1, #0
@L_0209bec0:
    cmp r3, #0
    bge @L_0209bed0
    rsbs r2, r2, #0
    rsc r3, r3, #0
@L_0209bed0:
    orrs r5, r1, r0
    beq @L_0209bff4
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi @L_0209befc
@L_0209bee8:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl @L_0209bee8
    add r6, r6, r5
@L_0209befc:
    cmp r1, #0
    blt @L_0209bf1c
@L_0209bf04:
    cmp r6, #1
    beq @L_0209bf1c
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl @L_0209bf04
@L_0209bf1c:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b @L_0209bf44
@L_0209bf2c:
    orr r12, r12, #1
    subs r6, r6, #1
    beq @L_0209bf9c
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
@L_0209bf44:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @L_0209bf2c
@L_0209bf60:
    subs r6, r6, #1
    beq @L_0209bf94
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @L_0209bf2c
    b @L_0209bf60
@L_0209bf94:
    adds r0, r0, r2
    adc r1, r1, r3
@L_0209bf9c:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq @L_0209bfd4
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge @L_0209bff8
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b @L_0209bfd4
    mov r0, r1, lsr r7
    mov r1, #0
@L_0209bfd4:
    cmp r4, #0
    blt @L_0209bfe4
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L_0209bfe4:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L_0209bff4:
    mov r0, #0
@L_0209bff8:
    mov r1, #0
    cmp r4, #0
    blt @L_0209bfe4
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}

asm unsigned long long __aeabi_uldivmod(unsigned long long a, unsigned long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #0
    b @L_0209c020
@_ull_mod:
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #1
@L_0209c020:
    orrs r5, r3, r2
    bne @L_0209c030
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L_0209c030:
    orrs r5, r1, r3
    dcd 0x1affffa5  // bne _ll_mod + 0x84
    mov r1, r2
    dcd 0xeb00009d  // bl __aeabi_uidivmod + 0x8
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}



asm long long _ull_mul(long long a, long long b) {
    stmdb sp!, {r4, r5, lr}
    umull r5, r4, r0, r2
    mla r4, r0, r3, r4
    mla r4, r2, r1, r4
    mov r1, r4
    mov r0, r5
    ldmia sp!, {r4, r5, lr}
    bx lr
}

u64 __aeabi_llsl(u64 val, int shift)
{
    __asm
    {
        ands r2, r2, #0x3f
        bxeq lr
        subs r3, r2, #0x20
        bge high
        rsb r3, r2, #0x20
        mov r1, r1, lsl r2
        orr r1, r1, r0, lsr r3
        mov r0, r0, lsl r2
        bx lr
    high:
        mov r1, r0, lsl r3
        mov r0, #0x0
    }
}

asm int _s32_div_f(int a, int b) {
    eor r12, r0, r1
    and r12, r12, #0x80000000
    cmp r0, #0
    rsblt r0, r0, #0
    addlt r12, r12, #1
    cmp r1, #0
    rsblt r1, r1, #0
    beq @L_0209c29c
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0
    blo @L_0209c29c
    mov r2, #0x1c
    mov r3, r0, lsr #4
    cmp r1, r3, lsr #12
    suble r2, r2, #0x10
    movle r3, r3, lsr #16
    cmp r1, r3, lsr #4
    suble r2, r2, #8
    movle r3, r3, lsr #8
    cmp r1, r3
    suble r2, r2, #4
    movle r3, r3, lsr #4
    mov r0, r0, lsl r2
    rsb r1, r1, #0
    adds r0, r0, r0
    add r2, r2, r2, lsl #1
    add pc, pc, r2, lsl #2
    mov r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
@L_0209c29c:
    ands r3, r12, #0x80000000
    rsbne r0, r0, #0
    ands r3, r12, #1
    rsbne r1, r1, #0
    bx lr
}

asm unsigned int __aeabi_uidivmod(unsigned int a, unsigned int b) {
    cmp r1, #0
    bxeq lr
@L_0209c2b8:
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0
    bxlo lr
    mov r2, #0x1c
    mov r3, r0, lsr #4
    cmp r1, r3, lsr #12
    suble r2, r2, #0x10
    movle r3, r3, lsr #16
    cmp r1, r3, lsr #4
    suble r2, r2, #8
    movle r3, r3, lsr #8
    cmp r1, r3
    suble r2, r2, #4
    movle r3, r3, lsr #4
    mov r0, r0, lsl r2
    rsb r1, r1, #0
    adds r0, r0, r0
    add r2, r2, r2, lsl #1
    add pc, pc, r2, lsl #2
    mov r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
    bx lr
}



int _ffix(float f)
{
    __asm
    {
        bic r1, r0, #0x80000000
        mov r2, #0x9e
        subs r2, r2, r1, lsr #0x17
        ble done
        mov r1, r1, lsl #0x8
        orr r1, r1, #0x80000000
        cmp r0, #0x0
        mov r0, r1, lsr r2
        rsbmi r0, r0, #0x0
        bx lr
    done:
        mvn r0, r0, asr #0x1f
        add r0, r0, #0x80000000
    }
}

unsigned __aeabi_f2uiz(float f)
{
    __asm
    {
        tst r0, #0x80000000
        bne neg_case
        mov r1, #0x9e
        subs r1, r1, r0, lsr #0x17
        blt overflow
        mov r2, r0, lsl #0x8
        orr r0, r2, #0x80000000
        mov r0, r0, lsr r1
        bx lr
    neg_case:
        mov r2, #0xff000000
        cmp r2, r0, lsl #0x1
        movhs r0, #0x0
        mvnlo r0, #0x0
        bx lr
    overflow:
        mvn r0, #0x0
    }
}

asm int __aeabi_d2iz(double x) {
    bic r3, r1, #0x80000000
    // 0x41e is the exponent of 2^31
    ldr r2, =0x41e
    subs r2, r2, r3, lsr #20
    ble @saturate
    cmp r2, #0x20
    bge @zero
    mov r3, r1, lsl #11
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    cmp r1, #0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0
    bx lr
@zero:
    mov r0, #0
    bx lr
@saturate:
    mvn r0, r1, asr #31
    add r0, r0, #0x80000000
    bx lr
}

asm unsigned long long __aeabi_d2ulz(double x) {
    tst r1, #0x80000000
    // 0x43e is the exponent of 2^63
    bne @negative
    ldr r2, =0x43e
    subs r2, r2, r1, lsr #20
    blt @overflow
    cmp r2, #0x40
    bge @zero
    mov r12, r1, lsl #11
    orr r12, r12, #0x80000000
    orr r12, r12, r0, lsr #21
    cmp r2, #0x20
    ble @wide
    sub r2, r2, #0x20
    mov r1, #0
    mov r0, r12, lsr r2
    bx lr
@wide:
    mov r3, r0, lsl #11
    mov r1, r12, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #0x20
    orr r0, r0, r12, lsl r2
    bx lr
@zero:
    mov r1, #0
    mov r0, #0
    bx lr
@negative:
    cmn r1, #0x100000
    cmpeq r0, #0
    bhi @overflow
    mov r1, #0
    mov r0, #0
    bx lr
@overflow:
    mvn r1, #0
    mvn r0, #0
    bx lr
}

extern int errno;
asm double sqrt(double x) {
    stmdb sp!, {r4, r5, r6, lr}
    ldr r2, =0x7ff00000
    cmp r1, r2
    bhs @L_0209c72c
    movs r12, r1, lsr #20
    beq @L_0209c6d8
    bic r1, r1, r2
    orr r1, r1, #0x100000
@L_0209c600:
    movs r12, r12, asr #1
    bhs @L_0209c614
    sub r12, r12, #1
    movs r0, r0, lsl #1
    adc r1, r1, r1
@L_0209c614:
    movs r3, r0, lsl #1
    adc r1, r1, r1
    mov r2, #0
    mov r4, #0
    mov lr, #0x200000
@L_0209c628:
    add r6, r4, lr
    cmp r6, r1
    addle r4, r6, lr
    suble r1, r1, r6
    addle r2, r2, lr
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @L_0209c628
    mov r0, #0
    mov r5, #0
    cmp r1, r4
    cmpeq r3, #0x80000000
    blo @L_0209c670
    subs r3, r3, #0x80000000
    sbc r1, r1, r4
    add r4, r4, #1
    mov r0, #0x80000000
@L_0209c670:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    mov lr, #0x40000000
@L_0209c67c:
    add r6, r5, lr
    cmp r4, r1
    cmpeq r6, r3
    bhi @L_0209c69c
    add r5, r6, lr
    subs r3, r3, r6
    sbc r1, r1, r4
    add r0, r0, lr
@L_0209c69c:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @L_0209c67c
    orrs r1, r1, r3
    biceq r0, r0, #1
    movs r1, r2, lsr #1
    movs r0, r0, rrx
    adcs r0, r0, #0
    adc r1, r1, #0
    add r1, r1, #0x20000000
    sub r1, r1, #0x100000
    add r1, r1, r12, lsl #20
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@L_0209c6d8:
    cmp r1, #0
    bne @L_0209c708
    cmp r0, #0
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    mvn r12, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub r12, r12, r5
    mov r1, r0, lsr #11
    mov r0, r0, lsl #21
    b @L_0209c600
@L_0209c708:
    clz r2, r1
    movs r1, r1, lsl r2
    rsb r2, r2, #0x2b
    mov r1, r1, lsr #11
    orr r1, r1, r0, lsr r2
    rsb r2, r2, #0x20
    mov r0, r0, lsl r2
    rsb r12, r2, #1
    b @L_0209c600
@L_0209c72c:
    tst r1, #0x80000000
    beq @L_0209c748
    bics r3, r1, #0x80000000
    cmpeq r0, #0
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    b @L_0209c754
@L_0209c748:
    orrs r2, r0, r1, lsl #12
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
@L_0209c754:
    ldr r2, =0x7ff80000
    orr r1, r1, r2
    ldr r3, =errno
    mov r4, #0x21
    str r4, [r3, #0]
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
}


asm float _fmul(float a, float b) {
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov r12, #0xff
    ands r3, r12, r0, lsr #23
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq @a_special
    orr r0, r0, #0x80000000
    ands r12, r12, r1, lsr #23
    mov r1, r1, lsl #8
    cmpne r12, #0xff
    beq @b_special
    orr r1, r1, #0x80000000
@multiply:
    add r12, r3, r12
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl r12, r12, #1
    subs r12, r12, #0x7f
    bmi @underflow
    cmp r12, #0xfe
    bge @overflow
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, r12, lsl #23
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #25
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@a_special:
    cmp r3, #0
    beq @a_denormal
    movs r0, r0, lsl #1
    bne @nan
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @a_inf_b_exp0
    cmp r12, #0xff
    blt @inf
    cmp r1, #0
    beq @inf
    b @nan
@a_inf_b_exp0:
    cmp r1, #0
    beq @nan_inf_zero
    b @inf
@b_special:
    cmp r12, #0
    beq @b_denormal
@b_inf_or_nan:
    movs r1, r1, lsl #1
    bne @nan
    b @inf
@a_denormal:
    movs r0, r0, lsl #1
    beq @a_zero
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov r12, r1, lsr #23
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @b_denormal
    cmp r12, #0xff
    beq @b_inf_or_nan
    orr r1, r1, #0x80000000
    b @multiply
@a_zero:
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @zero
    cmp r12, #0xff
    blt @zero
    cmp r1, #0
    beq @nan_inf_zero
    b @nan
@b_denormal:
    movs r1, r1, lsl #1
    beq @zero
    mov r1, r1, lsr #1
    clz r12, r1
    movs r1, r1, lsl r12
    rsb r12, r12, #1
    b @multiply
@underflow:
    cmn r12, #0x18
    beq @half_min
    bmi @zero_underflow
    cmp r1, #0
    orrne r0, r0, #1
    mov r3, r0
    mov r0, r0, lsr #8
    rsb r12, r12, #0
    orr r0, r2, r0, lsr r12
    rsb r12, r12, #0x18
    movs r1, r3, lsl r12
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@half_min:
    mov r0, r0, lsl #1
    b @min_denormal
@inf:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@nan:
    mvn r0, #0x80000000
    bx lr
@nan_inf_zero:
    mvn r0, #0x80000000
    bx lr
@overflow:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@min_denormal:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@zero_underflow:
    mov r0, r2
    bx lr
@zero:
    mov r0, r2
    bx lr
}

asm float _fadd(float a, float b) {
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a00008b  // bmi _fsub + 0xc
    subs r12, r0, r1
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq @L_0209c9fc
    mov r12, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq @L_0209ca3c
@L_0209c99c:
    subs r12, r3, r12
    beq @L_0209c9b4
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
@L_0209c9b4:
    adds r0, r0, r1
    blo @L_0209c9d4
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq @L_0209cb44
@L_0209c9d4:
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L_0209c9fc:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @L_0209ca60
    movs r0, r0, lsl #1
    bne @L_0209cb70
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @L_0209cb64
    cmp r12, #0xff
    blt @L_0209cb64
    cmp r1, #0
    beq @L_0209cb64
    b @L_0209cb70
@L_0209ca3c:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq @L_0209cabc
@L_0209ca54:
    movs r1, r1, lsl #1
    bne @L_0209cb70
    b @L_0209cb64
@L_0209ca60:
    movs r0, r0, lsl #1
    beq @L_0209ca98
    mov r3, #1
    mov r0, r0, lsr #1
    mov r12, r1, lsr #23
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @L_0209cabc
    cmp r12, #0xff
    beq @L_0209ca54
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @L_0209c99c
@L_0209ca98:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq @L_0209cb24
    cmp r3, #0xff
    blt @L_0209cb24
    cmp r0, #0
    beq @L_0209cb64
    b @L_0209cb5c
@L_0209cabc:
    movs r1, r1, lsl #1
    beq @L_0209cb2c
    mov r1, r1, lsr #1
    mov r12, #1
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    cmp r0, #0
    bmi @L_0209c99c
    adds r0, r0, r1
    blo @L_0209caf0
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r12, r12, #1
@L_0209caf0:
    cmp r0, #0
    subge r12, r12, #1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r12, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L_0209cb24:
    mov r0, r1
    bx lr
@L_0209cb2c:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L_0209cb44:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L_0209cb5c:
    mvn r0, #0x80000000
    bx lr
@L_0209cb64:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L_0209cb70:
    mvn r0, #0x80000000
    bx lr
}




















#pragma thumb reset

#pragma thumb reset


#pragma thumb reset

u32 func_0209cb78(void) {
    return 0x7fffffff;
}

// b - a: the operands swapped, then _fsub (_f_sub.o starts with it)
asm float _frsb(float a, float b) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

asm float _fsub(float a, float b) {
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4affff73  // bmi _fadd + 0xc
    subs r12, r0, r1
    eorlo r12, r12, #0x80000000
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands r12, r3, #0xff
    cmpne r12, #0xff
    beq @L_0209ccb4
    mov r12, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, r12, #0xff
    beq @L_0209ccf4
@L_0209cbd0:
    subs r12, r3, r12
    beq @L_0209cc18
    rsb r2, r12, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl @L_0209cc5c
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L_0209cc18:
    subs r0, r0, r1
    beq @L_0209cdc0
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @L_0209cc4c
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@L_0209cc4c:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L_0209cc5c:
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @L_0209cc88
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@L_0209cc88:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L_0209ccb4:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @L_0209cd1c
    movs r0, r0, lsl #1
    bne @L_0209cdf4
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @L_0209cde8
    cmp r12, #0xff
    blt @L_0209cde8
    cmp r1, #0
    beq @L_0209cdfc
    b @L_0209cdf4
@L_0209ccf4:
    cmp r12, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands r12, r12, #0xff
    beq @L_0209cd84
@L_0209cd0c:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne @L_0209cdf4
    b @L_0209cde8
@L_0209cd1c:
    movs r0, r0, lsl #1
    beq @L_0209cd54
    mov r0, r0, lsr #1
    mov r3, #1
    mov r12, r1, lsr #23
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @L_0209cd84
    cmp r12, #0xff
    beq @L_0209cd0c
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @L_0209cbd0
@L_0209cd54:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq @L_0209cd78
    cmp r2, #0xff
    blt @L_0209cda0
    cmp r0, #0
    bne @L_0209cde0
    b @L_0209cde8
@L_0209cd78:
    cmp r0, #0
    beq @L_0209cdc0
    b @L_0209cda0
@L_0209cd84:
    movs r1, r1, lsl #1
    beq @L_0209cda8
    mov r1, r1, lsr #1
    mov r12, #1
    orr r12, r12, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b @L_0209cbd0
@L_0209cda0:
    mov r0, r1
    bx lr
@L_0209cda8:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L_0209cdc0:
    mov r0, #0
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L_0209cde0:
    mvn r0, #0x80000000
    bx lr
@L_0209cde8:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L_0209cdf4:
    mvn r0, #0x80000000
    bx lr
@L_0209cdfc:
    mvn r0, #0x80000000
    bx lr
}


// b / a: the operands swapped, then _fdiv (_f_div.o starts with it)
asm float _frdiv(float a, float b) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

asm float _fdiv(float a, float b) {
    stmdb sp!, {lr}
    mov r12, #0xff
    ands r3, r12, r0, lsr #23
    cmpne r3, #0xff
    beq @a_special
    ands r12, r12, r1, lsr #23
    cmpne r12, #0xff
    beq @b_special
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
@divide:
    cmp r2, lr
    movlo r2, r2, lsl #1
    sublo r3, r3, #1
    // N = the signs differ: the sign of the quotient, which orrmi puts in the result
    teq r0, r1
    // r0 = the reciprocal table less 0x100 entries: the divisor's mantissa bits 22 to 15 and its integer bit index it
    sub r0, pc, #0x94
    ldrb r1, [r0, lr, lsr #15]
    rsb lr, lr, #0
    mov r0, lr, asr #1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #6
    mul r0, r1, r0
    mov r0, r0, lsr #14
    mul r1, lr, r0
    sub r12, r3, r12
    mov r1, r1, lsr #12
    mul r1, r0, r1
    mov r0, r0, lsl #14
    add r0, r0, r1, lsr #15
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds r12, r12, #0x7e
    bmi @underflow
    cmp r12, #0xfe
    bge @overflow
    add r0, r0, r12, lsl #23
    mov r12, r1, lsr #28
    cmp r12, #7
    beq @exact_tie
    add r0, r0, r1, lsr #31
    ldmia sp!, {lr}
    bx lr
    // The reciprocal table, one byte per mantissa from 1.0 to 2.0, from 0xff down to 0x83
    // Each word holds four of the table's bytes, the first in the low byte
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@exact_tie:
    mov r1, r3, lsl #1
    add r1, r1, #1
    rsb lr, lr, #0
    mul r1, lr, r1
    cmp r1, r2, lsl #24
    addmi r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@a_special:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0
    beq @a_denormal
    movs r0, r0, lsl #9
    bne @nan
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @inf
    cmp r12, #0xff
    blt @inf
    cmp r1, #0
    beq @nan_inf_inf
    b @nan_b
@b_special:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r12, #0
    beq @b_denormal
@b_inf_or_nan:
    movs r1, r1, lsl #9
    bne @nan_b
    b @zero
@a_denormal:
    movs r2, r0, lsl #9
    beq @a_zero
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0
    mov r2, r2, lsr #8
    ands r12, r12, r1, lsr #23
    beq @a_denormal_b_denormal
    cmp r12, #0xff
    beq @b_inf_or_nan
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b @divide
@a_zero:
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @a_zero_b_exp0
    cmp r12, #0xff
    blt @zero
    cmp r1, #0
    beq @zero
    b @nan_b
@a_zero_b_exp0:
    cmp r1, #0
    beq @nan_inf_inf
    b @zero
@b_denormal:
    movs r12, r1, lsl #9
    beq @inf
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b @divide
@a_denormal_b_denormal:
    movs r12, r1, lsl #9
    beq @inf
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    b @divide
@underflow:
    and r0, r0, #0x80000000
    cmn r12, #0x18
    beq @half_min
    bmi @return_sign
    add r1, r12, #0x17
    mov r2, r2, lsl r1
    rsb r12, r12, #0
    mov r3, r3, lsr r12
    orr r0, r0, r3
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2
    ldmeqia sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq @round_up
    addmi r0, r0, #1
    subpl r1, r1, lr
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #1
    and r3, r0, #1
    addmi r0, r0, #1
    addeq r0, r0, r3
    ldmia sp!, {lr}
    bx lr
@round_up:
    add r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@half_min:
    cmn r2, lr
    addne r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@nan_b:
    mov r0, r1
    b @nan
@inf:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@nan:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@nan_inf_inf:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@overflow:
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #1
    moveq r0, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@return_sign:
    ldmia sp!, {lr}
    bx lr
@zero:
    mov r0, lr
    ldmia sp!, {lr}
    bx lr
}

// b / a: the operands swapped, then _ddiv (_d_div.o starts with it)
asm double _drdiv(double a, double b) {
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}

asm double __aeabi_ddiv(double a, double b) {
    stmdb sp!, {r4, r5, r6, lr}
    ldr lr, =0xffe
    eor r4, r1, r3
    ands r12, lr, r1, lsr #19
    cmpne r12, lr
    beq @a_special
    bic r1, r1, lr, lsl #20
    orr r1, r1, #0x100000
    add r12, r12, r4, lsr #31
@unpack_b:
    ands r4, lr, r3, lsr #19
    cmpne r4, lr
    beq @b_special
    bic r3, r3, lr, lsl #20
    orr r3, r3, #0x100000
@divide:
    sub r12, r12, r4
    cmp r1, r3
    cmpeq r0, r2
    bhs @reciprocal
    adds r0, r0, r0
    adc r1, r1, r1
    sub r12, r12, #2
@reciprocal:
    // r4 = the reciprocal table less 0x100 entries: the divisor's mantissa bits 19 to 12 and its integer bit index it
    sub r4, pc, #0x24
    ldrb lr, [r4, r3, lsr #12]
    rsbs r2, r2, #0
    rsc r3, r3, #0
    mov r4, #0x20000000
    mla r5, lr, r3, r4
    mov r6, r3, lsl #10
    mov r5, r5, lsr #7
    mul lr, r5, lr
    orr r6, r6, r2, lsr #22
    mov lr, lr, lsr #13
    mul r5, lr, r6
    mov r6, r1, lsl #10
    orr r6, r6, r0, lsr #22
    mov r5, r5, lsr #16
    mul r5, lr, r5
    mov lr, lr, lsl #14
    add lr, lr, r5, lsr #16
    umull r5, r6, lr, r6
    umull r4, r5, r6, r2
    mla r5, r3, r6, r5
    mov r4, r4, lsr #26
    orr r4, r4, r5, lsl #6
    add r4, r4, r0, lsl #2
    umull lr, r5, r4, lr
    mov r4, #0
    adds r5, r5, r6, lsl #24
    adc r4, r4, r6, lsr #8
    cmp r12, #0x800
    bge @overflow
    add r12, r12, #0x7f0
    adds r12, r12, #0xc
    bmi @underflow
    orr r1, r4, r12, lsl #31
    bic r12, r12, #1
    add r1, r1, r12, lsl #19
    tst lr, #0x80000000
    bne @round_up
    rsbs r2, r2, #0
    mov r4, r4, lsl #1
    add r4, r4, r5, lsr #31
    mul lr, r2, r4
    mov r6, #0
    mov r4, r5, lsl #1
    orr r4, r4, #1
    umlal r6, lr, r4, r2
    rsc r3, r3, #0
    mla lr, r4, r3, lr
    cmp lr, r0, lsl #21
    bmi @round_up
    mov r0, r5
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@round_up:
    adds r0, r5, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // The reciprocal table again
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@overflow:
    movs r1, r12, lsl #31
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf00000
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@underflow:
    mvn r6, r12, asr #1
    cmp r6, #0x34
    bgt @zero_underflow
    beq @half_min
    cmp r6, #0x14
    bge @underflow_wide
    rsb r6, r6, #0x13
    mov lr, r0, lsl r6
    rsb r6, r6, #0x14
    mov r0, r5, lsr r6
    rsb r6, r6, #0x20
    orr r0, r0, r4, lsl r6
    rsb r6, r6, #0x20
    mov r4, r4, lsr r6
    orr r1, r4, r12, lsl #31
    mov r12, lr
    mov lr, #0
    b @underflow_round
@underflow_wide:
    rsb r6, r6, #0x33
    mov lr, r1, lsl r6
    mov r1, r12, lsl #31
    rsb r6, r6, #0x20
    orr r12, lr, r0, lsr r6
    rsb r6, r6, #0x20
    mov lr, r0, lsl r6
    mov r5, r5, lsr #21
    orr r5, r5, r4, lsl #11
    rsb r6, r6, #0x1f
    mov r0, r5, lsr r6
    mov r4, #0
@underflow_round:
    rsbs r2, r2, #0
    mul r4, r2, r4
    mov r5, #0
    umlal r5, r4, r2, r0
    rsc r3, r3, #0
    mla r4, r0, r3, r4
    cmp r4, r12
    cmpeq r5, lr
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    adds r5, r5, r2
    adc r4, r4, r3
    cmp r4, r12
    bmi @increment_check
    bne @underflow_exact
    cmp r5, lr
    beq @increment
    blo @increment_check
@underflow_exact:
    subs r5, r5, r2
    sbc r4, r4, r3
@underflow_check:
    adds r5, r5, r5
    adc r4, r4, r4
    adds r5, r5, r2
    adc r4, r4, r3
    adds lr, lr, lr
    adc r12, r12, r12
    cmp r4, r12
    bmi @increment
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    cmp r5, lr
    blo @increment
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    tst r0, #1
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
@increment:
    adds r0, r0, #1
    adc r1, r1, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@increment_check:
    adds r0, r0, #1
    adc r1, r1, #0
    b @underflow_check
@half_min:
    rsbs r2, r2, #0
    rsc r3, r3, #0
    cmp r1, r3
    cmpeq r0, r2
    mov r1, r12, lsl #31
    mov r0, #0
    movne r0, #1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@zero_underflow:
    mov r1, r12, lsl #31
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@a_special:
    orrs r5, r0, r1, lsl #1
    beq @a_zero
    cmp r12, lr
    beq @a_inf_or_nan
    movs r1, r1, lsl #12
    beq @a_denormal_low
    clz r5, r1
    movs r1, r1, lsl r5
    sub r12, r12, r5
    add r5, r12, #0x1f
    mov r1, r1, lsr #11
    orr r1, r1, r0, lsr r5
    rsb r5, r5, #0x20
    mov r0, r0, lsl r5
    mov r12, r12, lsl #1
    orr r12, r12, r4, lsr #31
    b @unpack_b
@a_denormal_low:
    mvn r12, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub r12, r12, r5
    mov r1, r0, lsr #11
    mov r0, r0, lsl #21
    mov r12, r12, lsl #1
    orr r12, r12, r4, lsr #31
    b @unpack_b
@a_inf_or_nan:
    orrs r5, r0, r1, lsl #12
    bne @a_nan
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    bhs @a_inf_b_special
    and r5, r3, #0x80000000
    eor r1, r5, r1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@a_inf_b_special:
    orrs r5, r2, r3, lsl #12
    bne @nan_b
    b @nan
@b_special:
    orrs r5, r2, r3, lsl #1
    beq @inf
    cmp r4, lr
    beq @b_inf_or_nan
    movs r3, r3, lsl #12
    beq @b_denormal_low
    clz r5, r3
    movs r3, r3, lsl r5
    sub r4, r4, r5
    add r5, r4, #0x1f
    mov r3, r3, lsr #11
    orr r3, r3, r2, lsr r5
    rsb r5, r5, #0x20
    mov r2, r2, lsl r5
    mov r4, r4, lsl #1
    b @divide
@b_denormal_low:
    mvn r4, #0x13
    clz r5, r2
    movs r2, r2, lsl r5
    sub r4, r4, r5
    mov r3, r2, lsr #11
    mov r2, r2, lsl #21
    mov r4, r4, lsl #1
    b @divide
@b_inf_or_nan:
    orrs r5, r2, r3, lsl #12
    bne @nan_b
    mov r1, r12, lsl #31
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@inf:
    mov r1, r12, lsl #31
    orr r1, r1, lr, lsl #19
    mov r0, #0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@a_zero:
    orrs r5, r2, r3, lsl #1
    beq @nan
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    cmpeq r2, #0
    bhi @nan_b
    eor r1, r1, r3
    and r1, r1, #0x80000000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@a_nan:
    tst r1, #0x80000
    beq @nan
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #19
    cmpeq r2, #0
    bhi @nan_b
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@nan_b:
    tst r3, #0x80000
    beq @nan
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
@nan:
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf80000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
}


