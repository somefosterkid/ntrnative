#ifndef POKEHEARTGOLD_OVERLAY_112_IR_STATE_H
#define POKEHEARTGOLD_OVERLAY_112_IR_STATE_H

// The ov112_021E5A68 unit's static data (Pokewalker IR link). mwcc reaches the unit's statics through two pooled
// base addresses, ov112_021FFAA4 and ov112_021FFB24, so the cut files see them as two objects. The other labels in
// this range are members: ov112_021FFAC0 = Ov112IrState.msgArray, ov112_021FFAF0 = &unk_4C, ov112_021FFAF8 = &unk_54;
// ov112_021FFB08 (OSMutex) and ov112_021FFB20 (OSMessageQueue) follow unk_60.
// Address slots hold RAM pointers or (cast) Pokewalker EEPROM addresses: they are pointers so they stay whole on 64-bit.

typedef struct Ov112IrLink {
    s8 unk_00;
    u8 pad_01[3];
    u32 unk_04;
    s32 unk_08;
    void *unk_0C; // callback (ov112_021E6A6C), set by ov112_021E5D38
    s32 unk_10;
    void *unk_14; // callback (ov112_021E6BDC), set by ov112_021E5D50
    s32 unk_18;
    void *unk_1C; // callback (ov112_021E6F60), set by ov112_021E5D44
    u32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
    u64 unk_30;
} Ov112IrLink;

// sizeof(Ov112IrLink) == 0x38 (_021FF9E0, the unit's first static block)

typedef struct Ov112IrState {
    u16 unk_00;
    u8 pad_02[2];
    u32 unk_04;
    u8 *unk_08;
    void *unk_0C;
    u8 *unk_10;
    s32 unk_14; // error code
    BOOL unk_18;
    void *msgArray[1];
    u8 *unk_20;
    u8 *unk_24;
    u8 *unk_28;
    u8 *unk_2C;
    u8 *unk_30;
    u8 *unk_34;
    u8 *unk_38;
    u64 unk_3C; // OS_GetTick() at the last packet
    u8 *unk_44;
    u8 *unk_48;
    u8 *unk_4C;
    u8 *unk_50;
    u8 *unk_54;
    u8 *unk_58;
    u8 *unk_5C;
    u8 *unk_60;
} Ov112IrState;

// sizeof(Ov112IrState) == 0x64 (mwcc: u64 is 4-aligned)

typedef struct Ov112IrXfer {
    u8 pad_00[0x1C]; // not reached through this base (ov112_021FFB20's message queue)
    u8 unk1C;
    u8 unk1D;
    u8 pad_1E[2];
    u8 *unk20; // source address (RAM pointer or Pokewalker address)
    u8 *unk24;
    u8 *unk28; // destination address
    u8 *unk2C;
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 unk36;
    u16 unk38;
    u16 unk3A;
    u16 unk3C;
    u8 pad_3E[2];
} Ov112IrXfer;

// sizeof(Ov112IrXfer) == 0x40

extern Ov112IrState ov112_021FFAA4;
extern Ov112IrXfer ov112_021FFB24;
extern Ov112IrLink _021FF9E0;

#endif // POKEHEARTGOLD_OVERLAY_112_IR_STATE_H
