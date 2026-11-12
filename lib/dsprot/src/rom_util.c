// DS Protect 1.27's rom_util.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers
// the reference's elfcoder writes for its two functions (rom_util_decrypter.s). Black's ROM has the two functions
// encrypted with key 0xc20d. The ROM doesn't name them. The reference's comments say what the reads do.
#include "dsprot/dsprot_internal.h"
#include "types.h"

// <nitro/hw/io_reg.h>'s, as the reference has them
#define DSPROT_HW_REG_BASE 0x04000000
#define DSPROT_REG_EXMEMCNT_OFFSET 0x204
#define DSPROT_REG_MI_EXMEMCNT (*(REGType16v *)(DSPROT_HW_REG_BASE + DSPROT_REG_EXMEMCNT_OFFSET))
#define DSPROT_REG_MI_EXMEMCNT_MP_MASK 0x800
#define DSPROT_REG_A9ROM_OFFSET 0x4000
#define DSPROT_REG_SCFG_A9ROM_SEC_MASK 0x01

// The card registers, by offset from the register base
#define DSPROT_REG_CARD_MASTER_CNT_OFFSET 0x1a1
#define DSPROT_REG_CARDCNT_OFFSET 0x1a4
#define DSPROT_REG_CARD_CMD_OFFSET 0x1a8
#define DSPROT_REG_CARD_DATA_OFFSET 0x100010

#define DSPROT_CARD_ROM_PAGE_SIZE 0x200
#define DSPROT_CARDMST_ENABLE 0x80
#define DSPROT_CARD_DATA_READY 0x00800000
#define DSPROT_CARD_COMMAND_PAGE 0x01000000
#define DSPROT_CARD_COMMAND_MASK 0x07000000
#define DSPROT_CARD_RESET_HI 0x20000000
#define DSPROT_CARD_READ_MODE 0x00000000
#define DSPROT_CARD_START 0x80000000
#define DSPROT_MROMOP_G_READ_PAGE 0xb7000000

// The reference's ROMUtil_Read: reads num_bytes of the card's ROM at addr into dest by hand, 0x200 bytes at a time
// (its convoluted writing is what matches)
void func_ov230_02188eb4(void *dest, u32 addr, s32 num_bytes) {
    u8 buffer[8];
    REGType8v *vnull;
    u32 register_base_1;
    REGType8v *register_base_2;
    u32 card_ctrl_13;
    s32 card_ctrl_cmd;
    s32 addr_offset;
    u16 ext_mem_register_val_original;
    u32 output;
    int i;

    vnull = (REGType8v *)NULL;

    register_base_1 = 1;
    register_base_1 <<= 26;

    register_base_2 = (REGType8v *)DSPROT_HW_REG_BASE;

    ext_mem_register_val_original = DSPROT_REG_MI_EXMEMCNT;

    DSPROT_REG_MI_EXMEMCNT &= ~DSPROT_REG_MI_EXMEMCNT_MP_MASK;

    // 0x027ffe60 (0x02fffe60 on a DSi): the ROM header's port setting for normal commands
    card_ctrl_13 = 5;
    card_ctrl_13 += *(REGType8v *)(register_base_1 + DSPROT_REG_A9ROM_OFFSET) & DSPROT_REG_SCFG_A9ROM_SEC_MASK;
    card_ctrl_13 <<= 18;
    card_ctrl_13 -= 13;
    card_ctrl_13 <<= 5;

    card_ctrl_cmd = (*(vs32 *)card_ctrl_13 & ~DSPROT_CARD_COMMAND_MASK)
                    | (DSPROT_CARD_COMMAND_PAGE | DSPROT_CARD_READ_MODE | DSPROT_CARD_START | DSPROT_CARD_RESET_HI);

    addr_offset = 0 - (addr & (DSPROT_CARD_ROM_PAGE_SIZE - 1));

    while (*(REGType32v *)(register_base_1 + DSPROT_REG_CARDCNT_OFFSET) & DSPROT_CARD_START) {
        continue;
    }

    *(REGType8v *)(register_base_1 + DSPROT_REG_CARD_MASTER_CNT_OFFSET) = DSPROT_CARDMST_ENABLE;

    for (i = 0; i < 8; i++) {
        buffer[i] = *(vnull + DSPROT_HW_REG_BASE + DSPROT_REG_CARD_CMD_OFFSET + i);
    }

    addr += addr_offset;

    while (addr_offset < num_bytes) {
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 0] = DSPROT_MROMOP_G_READ_PAGE >> 24;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 1] = addr >> 24;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 2] = addr >> 16;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 3] = addr >> 8;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 4] = addr;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 5] = 0x00;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 6] = 0x00;
        register_base_2[DSPROT_REG_CARD_CMD_OFFSET + 7] = 0x00;

        *(REGType32v *)(register_base_1 + DSPROT_REG_CARDCNT_OFFSET) = card_ctrl_cmd;

        do {
            if (*(REGType32v *)(register_base_1 + DSPROT_REG_CARDCNT_OFFSET) & DSPROT_CARD_DATA_READY) {
                output = *(REGType32v *)(register_base_1 + DSPROT_REG_CARD_DATA_OFFSET);
                if (addr_offset >= 0 && addr_offset < num_bytes) {
                    *(u32 *)((u32)dest + addr_offset) = output;
                }
                addr_offset += 4;
            }
        } while (*(REGType32v *)(register_base_1 + DSPROT_REG_CARDCNT_OFFSET) & DSPROT_CARD_START);

        addr += DSPROT_CARD_ROM_PAGE_SIZE;
    }

    for (i = 0; i < 8; i++) {
        *(vnull + DSPROT_HW_REG_BASE + DSPROT_REG_CARD_CMD_OFFSET + i) = buffer[i];
    }

    *(REGType16v *)(register_base_1 + DSPROT_REG_EXMEMCNT_OFFSET) = ext_mem_register_val_original;
}

// The reference's ROMUtil_CRC32 (its eight steps written out, as the reference's must be to match)
u32 func_ov230_02189004(void *buf, u32 size) {
    u32 crc;
    u32 poly;
    u8 *byte_ptr;

    byte_ptr = (u8 *)buf;
    crc = 0xffffffff;
    poly = 0xedb88320;

    while (size-- != 0) {
        crc ^= *byte_ptr++;
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
        if (crc & 1) { crc = (crc >> 1); } else { crc = poly ^ (crc >> 1); }
    }

    return ~crc;
}

// The reference's RunEncrypted_ROMUtil_Read: decrypts func_ov230_02188eb4, runs it and encrypts it again
asm void func_ov230_02189098(void *dest, u32 addr, s32 num_bytes) {
    orr ip, pc, pc
    ands ip, ip, ip
    moveq ip, #0x0
    addne ip, ip, #0x1c
    ldr ip, [ip, #0x14]
    sub ip, ip, #0x1600
    stmdb sp!, {ip}
    orr ip, pc, pc
    ldmia sp!, {pc}
    dcd DSProt_BSS_ov230 + 1
    dcd DSProt_BSS_ov230 + 0xc20d + 0x1600
    dcd func_ov230_02188eb4 + 0x1600
    dcd DSProt_BSS_ov230 + 0x14c + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_ROMUtil_CRC32
asm u32 func_ov230_021890d4(void *buf, u32 size) {
    orr ip, pc, pc
    ands ip, ip, ip
    moveq ip, #0x0
    addne ip, ip, #0x1c
    ldr ip, [ip, #0x14]
    sub ip, ip, #0x1600
    stmdb sp!, {ip}
    orr ip, pc, pc
    ldmia sp!, {pc}
    dcd DSProt_BSS_ov230 + 1
    dcd DSProt_BSS_ov230 + 0xc20d + 0x1600
    dcd func_ov230_02189004 + 0x1600
    dcd DSProt_BSS_ov230 + 0x90 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}
