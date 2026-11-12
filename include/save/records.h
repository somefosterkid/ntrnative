// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/records.h, the lines in use so far.
#ifndef BWPORT_SAVE_RECORDS_H
#define BWPORT_SAVE_RECORDS_H

#include "types.h"
#include "struct_decls.h"

u32 RecordAddOne(GameRecords *records, u32 id);

void EncryptRecordStorage(GameRecords *records, u32 id);
void DecryptRecordStorage(GameRecords *records, u32 id);
u32 GetRecordCounter(GameRecords *records, int id);
u32 SetRecordCounter(GameRecords *records, int id, u32 value);
u32 GetRecordCounterMax(u32 id);
u32 RecordGet(GameRecords *records, u32 id);

#define RECORD_COUNT_WORD 0x47
#define RECORD_COUNT 0xa7

struct GameRecords {
    u32 words[RECORD_COUNT_WORD];
    u16 halfwords[RECORD_COUNT - RECORD_COUNT_WORD];
    u16 checksum;
    u16 seedHigh;
};

u32 RecordAdd(GameRecords *records, u32 id, u32 value);
// Sets a record to value, or to its maximum
u32 func_02008d1c(GameRecords *records, u32 id, u32 value);
// Sets a record to value if that is higher, up to the record's maximum
u32 func_02008d58(GameRecords *records, u32 id, u32 value);
void func_02008e30(GameRecords *records);
// The Trial House's best rank and best points
void func_02008e68(GameRecords *records, u8 rank);
// The Battle Test rank
u8 func_02008e78(GameRecords *records);
void func_02008e88(GameRecords *records, u32 points);
// Record 0x7c
u32 func_02008ea0(GameRecords *records);

#define BATTLEMATCH_MAC_ADDRESS_SIZE 6
#define LIVEMATCH_COUNT_MAX 0xff
#define LIVEMATCH_OPPONENT_MAX 10

// An opponent of the live competition
typedef struct {
    u8 macAddress[BATTLEMATCH_MAC_ADDRESS_SIZE];
    u8 params[2];
} LiveMatchOpponent;

// The results of the live competition (livematch_savedata.c)
typedef struct {
    // The console the competition was entered on
    u8 macAddress[BATTLEMATCH_MAC_ADDRESS_SIZE];
    // The first is the number of opponents
    u8 counts[4];
    // A ring of the last opponents
    LiveMatchOpponent opponents[LIVEMATCH_OPPONENT_MAX];
} LiveMatchData; // size: 0x5a

#define RNDMATCH_TYPE_MAX 11
#define RNDMATCH_FIELD_MAX 3
#define RNDMATCH_VALUE_MAX 0xffff

// The results of the random matches and the competitions (rndmatch_savedata.c): three counts for each of 11 kinds of
// match
typedef struct {
    u16 values[RNDMATCH_TYPE_MAX][RNDMATCH_FIELD_MAX];
} RndMatchData; // size: 0x42

// Block 0x3c (battlematch_savedata.c). Black's lacks the match-in-progress mark that Black 2's has after these
struct RecordSave {
    /* 0x00 */ LiveMatchData liveMatch;
    /* 0x5a */ RndMatchData rndMatch;
}; // size: 0x9c

// livematch_savedata.c, whose other functions app/livebattlematch_flow.h declares. A count of the live competition's
// results
u32 func_0200e918(void *results, u32 index);

// pokebw2's RndMatchData_Init
// rndmatch_savedata.c
void func_0200dfcc(void *results);
// pokebw2's RndMatchData_GetParam
u16 func_0200dfd8(void *results, u32 index, u32 field);
// pokebw2's RndMatchData_SetParam
// Sets a field of a result, from 0 to 0xffff
void func_0200dfe4(void *results, u32 index, u32 field, s32 value);
// pokebw2's RndMatchData_IncParam
// Adds 1 to a field of a result, up to 0xffff
void func_0200e008(void *results, u32 index, u32 field);

// The results of the random matches (pokebw2's RecordSave_GetRndMatch), which the Wi-Fi competition's card reads
void *func_0200ea00(RecordSave *record);

// The results of the live competition (pokebw2's RecordSave_GetLiveMatch)
void *func_0200ea08(RecordSave *record);
// The results of the random matches (pokebw2's RecordSave_GetRndMatchData)
void *func_0200ea04(RecordSave *record);

void initTrainerCardInfoBlk(GameRecords *records);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The size of the records
u32 func_02008c20(void);

#endif // BWPORT_SAVE_RECORDS_H
