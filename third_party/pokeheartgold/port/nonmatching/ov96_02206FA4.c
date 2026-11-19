/* PORTOK hk42 */
typedef struct Words2 { u32 w[2]; } Words2;
void ov96_02206FA4(s32 arg0, void *arg1, void *arg2) {
    s32 temp_r1;
    s32 temp_r3;
    s32 temp_r4_2;
    s32 temp_r5_2;
    u16 temp_r4;
    u32 temp_r5;

    temp_r5 = *(u16 *)arg1;
    temp_r4 = *(u16 *)((u8 *)arg1 + 2);
    temp_r3 = temp_r5 - 0x10;
    *(s32 *)arg2 = temp_r3;
    temp_r1 = temp_r4 - 0x10;
    *(s32 *)((u8 *)arg2 + 4) = temp_r1;
    temp_r5_2 = temp_r5 + 0x10;
    *(s32 *)((u8 *)arg2 + 8) = temp_r5_2;
    *(s32 *)((u8 *)arg2 + 0xC) = temp_r1;
    *(s32 *)((u8 *)arg2 + 0x10) = temp_r5_2;
    temp_r4_2 = temp_r4 + 0x10;
    *(s32 *)((u8 *)arg2 + 0x14) = temp_r4_2;
    *(s32 *)((u8 *)arg2 + 0x18) = temp_r3;
    *(s32 *)((u8 *)arg2 + 0x1C) = temp_r4_2;
    *(Words2 *)((u8 *)arg2 + 0x20) = *(Words2 *)((u8 *)arg2);
    *(Words2 *)((u8 *)arg2 + 0x28) = *(Words2 *)((u8 *)arg2 + 8);
    *(Words2 *)((u8 *)arg2 + 0x30) = *(Words2 *)((u8 *)arg2 + 8);
    *(Words2 *)((u8 *)arg2 + 0x38) = *(Words2 *)((u8 *)arg2 + 0x10);
    *(Words2 *)((u8 *)arg2 + 0x40) = *(Words2 *)((u8 *)arg2 + 0x10);
    *(Words2 *)((u8 *)arg2 + 0x48) = *(Words2 *)((u8 *)arg2 + 0x18);
    *(Words2 *)((u8 *)arg2 + 0x50) = *(Words2 *)((u8 *)arg2 + 0x18);
    *(Words2 *)((u8 *)arg2 + 0x58) = *(Words2 *)((u8 *)arg2);
    *(s32 *)((u8 *)arg2 + 0x24) = (s32) (*(s32 *)((u8 *)arg2 + 0x24) - arg0);
    *(s32 *)((u8 *)arg2 + 0x2C) = (s32) (*(s32 *)((u8 *)arg2 + 0x2C) - arg0);
    *(s32 *)((u8 *)arg2 + 0x30) = (s32) (*(s32 *)((u8 *)arg2 + 0x30) + arg0);
    *(s32 *)((u8 *)arg2 + 0x38) = (s32) (*(s32 *)((u8 *)arg2 + 0x38) + arg0);
    *(s32 *)((u8 *)arg2 + 0x44) = (s32) (*(s32 *)((u8 *)arg2 + 0x44) + arg0);
    *(s32 *)((u8 *)arg2 + 0x4C) = (s32) (*(s32 *)((u8 *)arg2 + 0x4C) + arg0);
    *(s32 *)((u8 *)arg2 + 0x50) = (s32) (*(s32 *)((u8 *)arg2 + 0x50) - arg0);
    *(s32 *)((u8 *)arg2 + 0x58) = (s32) (*(s32 *)((u8 *)arg2 + 0x58) - arg0);
}
