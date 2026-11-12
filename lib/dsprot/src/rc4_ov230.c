// DS Protect 1.27's rc4.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decoder the reference's
// elfcoder writes for its functions (rc4_decoder.s), which runs from .ctor. Black's code at ov230
// 0x0218949c-0x02189954; the ROM doesn't name it (rc4_ov230.c: main has a file rc4.c).

#include "types.h"
#include "dsprot/dsprot_internal.h"

void func_ov230_0218949c(DSProtCipherState *state, const u8 *key, u32 keyLength) {
    // The order of these declarations sets the locals' registers
    int i;
    u32 j;
    u32 keyIndex;
    u32 *tableEnd;
    u32 value;
    u32 *table;

    state->offset = 0xaa;
    state->i = 0;
    state->j = 0;
    keyIndex = 0;
    j = 0;
    value = 0x03020100;
    table = (u32 *)state->table;
    tableEnd = (u32 *)(state->table + 256);
    do {
        *table = value;
        table++;
        value += 0x04040404;
    } while (table < tableEnd);
    for (i = 255; i >= 0; i--) {
        u8 temp;

        j = (state->table[i] + (j + key[keyIndex])) & 0xff;
        temp = state->table[j];
        state->table[j] = state->table[i];
        state->table[i] = temp;
        keyIndex++;
        if (keyIndex >= keyLength) {
            keyIndex = 0;
        }
    }
}

u32 func_ov230_02189528(DSProtCipherState *state) {
    u32 i = (state->i + 1 + state->offset) & 0xff;
    u8 atI = state->table[i];
    u32 j = (atI + state->j + state->offset) & 0xff;
    u8 atJ = state->table[j];

    state->i = i;
    state->j = j;
    state->table[j] = atI;
    state->table[i] = atJ;
    return state->table[(atI + atJ) & 0xff];
}

u32 func_ov230_02189580(u8 *sbox) {
    u32 x;
    u32 y;
    u32 *sbox_start;
    u32 *sbox_end;

    x = 0x03020100;
    y = 0x04040404;
    sbox_start = (u32 *)&sbox[0];
    sbox_end = (u32 *)&sbox[256];
    do {
        *sbox_start++ = x ^ 0xFFFFFFFF;
        x += y;
    } while (sbox_start < sbox_end);

    return 0;
}

static inline void func_ov230_EncryptByte(DSProtCipherState *state, u8 *src, u8 *dst) {
    int b;
    b = func_ov230_02189528(state) ^ *src;
    state->offset = b;
    *dst = b;
}
u32 func_ov230_021895b8(DSProtCipherState *state, u32 src, u32 dst, u32 size) {
    u8 sbox[256];
    unsigned long offset;
    u32 ins_word;
    u8 *src_bytes;
    u8 *dst_bytes;

    if (size & 3) {
        return -1;
    }

    src_bytes = (u8 *)src;
    dst_bytes = (u8 *)dst;

    func_ov230_02189580(&sbox[0]);

    for (offset = 0; offset < size; offset += 4) {
        ins_word = *(u32 *)(src_bytes + offset);

        switch (func_ov230_02188a0c(ins_word)) {
        case 1:
        case 2:
            {
                u32 opcode;
                u32 operands;
                u32 *src_addr = (u32 *)(src_bytes + offset);
                u32 *dst_addr = (u32 *)(dst_bytes + offset);

                *dst_addr = *src_addr;

                opcode = (*dst_addr & 0xFF000000) ^ (0x01 << 24);
                operands = ((*dst_addr & 0x00FFFFFF) + ((0x1600 >> 2) + 2)) & 0x00FFFFFF;

                state->offset += (opcode >> 24);

                *dst_addr = opcode | operands;
            }
            break;

        case 3:
            *(u32 *)(src_bytes + offset) ^= (0x01 << 24);
        default:
            func_ov230_EncryptByte(state, src_bytes + offset, dst_bytes + offset);
            func_ov230_EncryptByte(state, src_bytes + offset + 1, dst_bytes + offset + 1);

            dst_bytes[offset + 2] = sbox[src_bytes[offset + 2]];

            dst_bytes[offset + 3] = src_bytes[offset + 3];

            state->offset = (state->offset * dst_bytes[offset + 2]) - dst_bytes[offset + 3];
            break;
        }
    }

    return 0;
}

static inline void func_ov230_DecryptByte(DSProtCipherState *state, u8 *src, u8 *dst) {
    int b;
    b = func_ov230_02189528(state) ^ *src;
    state->offset = *src;
    *dst = b;
}
u32 func_ov230_021896d0(DSProtCipherState *state, u32 src, u32 dst, u32 size) {
    u8 sbox[256];
    unsigned long offset;
    u32 ins_word;
    u8 *src_bytes;
    u8 *dst_bytes;

    if (size & 3) {
        return -1;
    }

    src_bytes = (u8 *)src;
    dst_bytes = (u8 *)dst;

    func_ov230_02189580(&sbox[0]);

    for (offset = 0; offset < size; offset += 4) {
        ins_word = *(u32 *)(src_bytes + offset);

        switch (func_ov230_02188a0c(ins_word)) {
        case 1:
        case 3:
            {
                u32 opcode;
                u32 operands;
                u32 *dst_addr = (u32 *)(dst_bytes + offset);

                state->offset += (ins_word >> 24);

                opcode = (ins_word & 0xFF000000) ^ (0x01 << 24);
                operands = ((ins_word & 0x00FFFFFF) - ((0x1600 >> 2) + 2)) & 0x00FFFFFF;

                *dst_addr = opcode | operands;
            }
            break;

        case 2:
            func_ov230_DecryptByte(state, src_bytes + offset, dst_bytes + offset);
            func_ov230_DecryptByte(state, src_bytes + offset + 1, dst_bytes + offset + 1);

            state->offset = (src_bytes[offset + 2] * state->offset) - src_bytes[offset + 3];

            dst_bytes[offset + 2] = sbox[src_bytes[offset + 2]];

            dst_bytes[offset + 3] = src_bytes[offset + 3];

            *(u32 *)(src_bytes + offset) ^= (0x01 << 24);
            break;

        default:
            func_ov230_DecryptByte(state, src_bytes + offset, dst_bytes + offset);
            func_ov230_DecryptByte(state, src_bytes + offset + 1, dst_bytes + offset + 1);

            state->offset = (src_bytes[offset + 2] * state->offset) - src_bytes[offset + 3];

            dst_bytes[offset + 2] = sbox[src_bytes[offset + 2]];

            dst_bytes[offset + 3] = src_bytes[offset + 3];
            break;
        }
    }

    return 0;
}



u32 func_ov230_02189858(u32 *key, u32 dst, u32 src, u32 size) {
    DSProtCipherState state;

    func_ov230_0218949c(&state, (u8 *)key, 16);
    return func_ov230_021895b8(&state, dst, src, size) == -1 ? -1 : 0;
}


u32 func_ov230_021898b0(u32 *key, u32 dst, u32 src, u32 size) {
    DSProtCipherState state;

    func_ov230_0218949c(&state, (u8 *)key, 16);
    return -(func_ov230_021896d0(&state, dst, src, size) == 0xFFFFFFFF);
}


// The RC4 functions' decoder (the reference's NitroStaticInit of rc4_decoder.s): has func_ov230_02188a48 decode the
// seven functions of the table of {function, size} pairs after its code
// clang-format off
asm void __sinit_ov230_02189908(void) {
    orr r0, pc, #0x0
    adds r0, r0, #0x4
    bne func_ov230_02188a48
    dcd func_ov230_0218949c + 0x1600
    dcd DSProt_BSS_ov230 + 0x1684
    dcd func_ov230_02189580 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1630
    dcd func_ov230_021895b8 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1718
    dcd func_ov230_021896d0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1784
    dcd func_ov230_02189858 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1658
    dcd func_ov230_021898b0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1658
    dcd func_ov230_02189528 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1658
    dcd 0
    dcd 0
}
// clang-format on

#pragma define_section ctor ".ctor" ".ctor" ".ctor" abs32 RW
__declspec(ctor) static void (*const sDecodeRc4)(void) = __sinit_ov230_02189908;
