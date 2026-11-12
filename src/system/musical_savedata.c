// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/musical_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/musical_savedata.h"
#include "gfl/std.h"
#include "save/save_control.h"

// The musical's save block: the photo of the last finale, the props owned and the ones not yet looked at, and the
// musical's counters. The ROM has no string for the file, so the name is a guess (swan's
// getAddressOfMusicalDataInfo). Block 0x2a.

#define SAVE_BLOCK_MUSICAL 0x2a

u32 func_0200a53c(void) {
    return sizeof(MusicalSave);
}

void func_0200a544(MusicalSave *musical) {
    u8 i;

    MI_CpuFill8(musical, 0, sizeof(MusicalSave));
    func_0200a604(musical);
    for (i = 0; i < 10; i++) {
        musical->unk1E0[i].unk0 = 4;
    }
    musical->unk29D = 0;
    musical->unk29E = 0;
    func_0200a6bc(musical, 0x30);
    func_0200a6bc(musical, 0x46);
    func_0200a6bc(musical, 0x53);
    func_0200a6bc(musical, 0x4b);
    func_0200a6bc(musical, 0x0);
    func_0200a6bc(musical, 0x1);
    func_0200a6bc(musical, 0xa);
    func_0200a6bc(musical, 0x1c);
    func_0200a6bc(musical, 0x8);
    func_0200a6bc(musical, 0x3b);
    func_0200a6bc(musical, 0x3f);
    func_0200a6bc(musical, 0x42);
    func_0200a6bc(musical, 0x10);
    func_0200a6bc(musical, 0x59);
    func_0200a6bc(musical, 0x4c);
}

MusicalSave *getAddressOfMusicalDataInfo(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_MUSICAL);
}

void func_0200a604(MusicalSave *musical) {
    u8 i;

    for (i = 0; i < 8; i++) {
        musical->unk1B0[i].unk0 = 10;
    }
}

MusicalSaveUnk1B0 *func_0200a628(MusicalSave *musical) {
    return musical->unk1B0;
}

BOOL func_0200a630(MusicalSave *musical) {
    if (musical->shot.month) {
        return TRUE;
    }
    return FALSE;
}

MusicalShot *func_0200a640(MusicalSave *musical) {
    return &musical->shot;
}

BOOL func_0200a644(MusicalSave *musical, u8 prop) {
    if (prop < MUSICAL_PROP_MAX) {
        u8 byte = prop / 8;
        u8 bit = prop % 8;
        if ((musical->props[byte] & (1 << bit)) > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_0200a680(MusicalSave *musical, u8 prop) {
    if (prop < MUSICAL_PROP_MAX) {
        u8 byte = prop / 8;
        u8 bit = prop % 8;
        if ((musical->newProps[byte] & (1 << bit)) > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_0200a6bc(MusicalSave *musical, u8 prop) {
    if (prop < MUSICAL_PROP_MAX) {
        u8 shift = prop % 8;
        u32 mask = 1 << shift;
        u8 byte = prop / 8;
        musical->props[byte] |= mask;
        musical->newProps[byte] |= mask;
    }
}

void func_0200a6f8(MusicalSave *musical) {
    u8 i;

    for (i = 0; i < MUSICAL_PROP_BYTES; i++) {
        musical->newProps[i] = 0;
    }
}

u32 func_0200a714(MusicalSave *musical) {
    u32 count = 0;
    s32 i;

    for (i = 0; i < 256; i++) {
        if (func_0200a644(musical, i) == TRUE) {
            count++;
        }
    }
    return count;
}

BOOL func_0200a73c(MusicalSave *musical) {
    if (func_0200a714(musical) == MUSICAL_PROP_MAX) {
        return TRUE;
    }
    return FALSE;
}

MusicalSaveUnk1E0 *func_0200a750(MusicalSave *musical, u8 index) {
    return &musical->unk1E0[index];
}

u16 func_0200a75c(MusicalSave *musical) {
    return musical->unk252;
}

void func_0200a768(MusicalSave *musical) {
    if (musical->unk252 < 0xffff) {
        musical->unk252++;
    }
}

u16 func_0200a780(MusicalSave *musical) {
    return musical->unk254;
}

void func_0200a788(MusicalSave *musical) {
    if (musical->unk254 < 0xffff) {
        musical->unk254++;
    }
}

u8 func_0200a7a0(MusicalSave *musical, u8 index) {
    return musical->unk298[index];
}

void func_0200a7ac(MusicalSave *musical, u8 index, u8 value) {
    musical->unk298[index] = value;
}

u8 func_0200a7b8(MusicalSave *musical) {
    return musical->unk29C;
}

void func_0200a7c0(MusicalSave *musical, u8 value) {
    musical->unk29C = value;
}

u8 func_0200a7c8(MusicalSave *musical) {
    return musical->unk29D;
}

void func_0200a7d4(MusicalSave *musical, u8 value) {
    musical->unk29D = value;
}

u8 func_0200a7e0(MusicalSave *musical) {
    return musical->unk29E;
}

void func_0200a7ec(MusicalSave *musical, u8 value) {
    musical->unk29E = value;
}

const u16 *func_0200a7f8(MusicalSave *musical) {
    return musical->name;
}

void func_0200a800(MusicalSave *musical, u16 value) {
    s32 sum = musical->unk256 + value;
    if (sum < 0xffff) {
        musical->unk256 = sum;
    } else {
        musical->unk256 = 0xffff;
    }
}

u16 func_0200a81c(MusicalSave *musical) {
    return musical->unk256;
}

BOOL func_0200a828(MusicalSave *musical) {
    return musical->dressingRoomVisited;
}

void func_0200a834(MusicalSave *musical, BOOL value) {
    musical->dressingRoomVisited = value;
}

u32 func_0200a840(void) {
    return 0x2dc;
}

void func_0200a848(CityStateBlock *block) {
    func_ov010_02161d98(&block->state);
    func_ov010_02161eec(block->unk_1e8);
    func_ov010_02161f00(&block->unk_1fc);
}

void *func_0200a864(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, 0x2b);
}
