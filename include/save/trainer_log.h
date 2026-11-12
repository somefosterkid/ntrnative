// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/trainer_log.h: the lines in use so far.
#ifndef BWPORT_SAVE_TRAINER_LOG_H
#define BWPORT_SAVE_TRAINER_LOG_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/rtc.h"

// A day has passed
void func_0200ab04(TrainerLogSave *log);

#define TRAINER_LOG_MAX 30
#define TRAINER_LOG_COUNT_MAX 999
#define TRAINER_LOG_NONE 0x7f
#define TRAINER_LOG_NO_SLOT 0xff

// Black 2's; twinned code confirms the offsets of name, value, count, unk10_17, unk10_18, unk10_22, used, id, unk18,
// unk1A, unk1B_0, unk1B_6 and today, and its size in Black
struct TrainerLogEntry {
    u16 name[8];
    // What the fortune of func_0200b11c is matched against, 0 to 100
    u32 value : 7;
    // How many times the trainer was met, up to TRAINER_LOG_COUNT_MAX
    u32 count : 10;
    // Whether the name takes no article in the scripts' text
    u32 unk10_17 : 1;
    u32 unk10_18 : 4;
    u32 unk10_22 : 4;
    u32 unk10_26 : 5;
    u32 used : 1;
    u32 id;
    u16 unk18;
    u8 unk1A;
    u8 unk1B_0 : 6;
    u8 unk1B_6 : 1;
    // Set when the trainer is met, cleared each day
    u8 today : 1;
}; // size: 0x1c

// Black 2's; twinned code confirms the offsets of entries and lastSlot, and its size in Black
struct TrainerLogSave {
    TrainerLogEntry entries[TRAINER_LOG_MAX];
    // The slot the last trainer went into, TRAINER_LOG_NONE at first
    u32 lastSlot : 8;
    u32 unk348_8 : 24;
}; // size: 0x34c

TrainerLogSave *func_0200a8a0(SaveControl *save);
// How many entries are in use, counted up to the first free one
u32 func_0200a8ac(TrainerLogSave *log);
// An entry, whose name comes first
const u16 *func_0200a8c8(TrainerLogSave *log, u32 index);
u8 func_0200a8d0(TrainerLogSave *log, u32 index);
u16 func_0200a8e4(TrainerLogSave *log, u32 index);
BOOL func_0200a8f8(TrainerLogSave *log, u32 index);
u8 func_0200a930(TrainerLogSave *log);
// The entry whose value is nearest the fortune of the birthday and date, at random between two as near
u32 func_0200a940(TrainerLogSave *log, u8 month, u8 day, RTCDate *date);
// Whether the trainer with the ID was met today, and whether they are in the log
BOOL func_0200a9a4(TrainerLogSave *log, u32 id);
BOOL func_0200a9d4(TrainerLogSave *log, u32 id);
// A fortune from 0 to 100, a cosine of how far the date is from the birthday within a month
u8 func_0200aa00(u8 month, u8 day, const RTCDate *date);
// Logs a trainer met: in the slot their value earns, counting the meetings of one already logged. Returns the slot
u8 func_0200aa68(TrainerLogSave *log, const u16 *name, u8 value, u8 unk10_17, u8 unk10_18, u8 unk10_22, u32 id,
                 u16 unk18, u8 unk1A, u8 unk1B_0, u8 unk1B_6);
// The size of the block and its init
u32 func_0200ab24(void);
void func_0200ab2c(TrainerLogSave *log);
// Puts the entry in the first slot whose value is not above its own, moving the rest down. Returns the slot
u8 func_0200ab50(TrainerLogSave *log, s32 count, const TrainerLogEntry *entry);
BOOL func_0200abe0(TrainerLogSave *log, s32 count, const TrainerLogEntry *entry);
void func_0200ac14(TrainerLogEntry *entry, const u16 *name, u8 value, u32 count, u8 unk10_17, u8 unk10_18,
                   u8 unk10_22, u32 id, u16 unk18, u8 unk1A, u8 unk1B_0, u8 unk1B_6);
TrainerLogEntry *func_0200acd0(TrainerLogSave *log, s32 count, u32 id);
// Counts one more meeting for every entry with the entry's ID
void func_0200ad00(TrainerLogSave *log, s32 count, const TrainerLogEntry *entry);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_0200a908(const TrainerLogSave *log, u32 index, u16 *unk18, u8 *unk1A, u8 *unk1B_0, u8 *unk1B_6);

#endif // BWPORT_SAVE_TRAINER_LOG_H
