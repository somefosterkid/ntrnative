// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/record.c, ported by tools/port_file.py.
#include "types.h"
#include "save/records.h"
#include "gfl/std.h"

// The counters of the records the game keeps (the ROM has no string for the file, so the name is a guess). Their
// maximums are 0, or one of the values of nines, as the byte of the record in data_020a70c4 says.

static const struct {
    u16 destination;
    u16 source;
// pokebw2's sRecordMoves
} data_0209dda0[] = {
    {26, 27},
    {81, 82},
    {83, 84},
    {85, 86},
    {87, 88},
};

static const u32 nines[] = {999999999, 999999, 65535, 9999, 999, 6};

// pokebw2's sRecordMaxKind
static u8 data_020a70c4[] = {
    0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 4, 3, 3, 5, 3, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 0,
};

void EncryptRecordStorage(GameRecords *records, u32 id) {
    if (id != 0) {
        records->checksum = addUpDataStream((u8 *)&records->words[1], 0x1d8);
        decryptData(&records->words[1], 0x1d8, records->checksum + (records->seedHigh << 16));
    }
}

void DecryptRecordStorage(GameRecords *records, u32 id) {
    if (id != 0) {
        _decryptData(&records->words[1], 0x1d8, records->checksum + (records->seedHigh << 16));
    }
}

u32 GetRecordCounter(GameRecords *records, int id) {
    if (id < RECORD_COUNT_WORD) {
        return records->words[id];
    }
    if (id < RECORD_COUNT) {
        return records->halfwords[id - RECORD_COUNT_WORD];
    }
    return 0;
}

u32 SetRecordCounter(GameRecords *records, int id, u32 value) {
    if (id < RECORD_COUNT_WORD) {
        records->words[id] = value;
    } else if (id < RECORD_COUNT) {
        records->halfwords[id - RECORD_COUNT_WORD] = value;
    }
    return GetRecordCounter(records, id);
}

u32 GetRecordCounterMax(u32 id) {
    int kind = data_020a70c4[id];

    if (kind >= 0xa7) {
        return 0;
    }
    if (kind >= 6) {
        return 0;
    }
    return nines[kind];
}

u32 func_02008d1c(GameRecords *records, u32 id, u32 value) {
    u32 max = GetRecordCounterMax(id);
    u32 result;

    DecryptRecordStorage(records, id);
    if (value < max) {
        result = SetRecordCounter(records, id, value);
    } else {
        result = SetRecordCounter(records, id, max);
    }
    EncryptRecordStorage(records, id);
    return result;
}

u32 func_02008d58(GameRecords *records, u32 id, u32 value) {
    u32 max = GetRecordCounterMax(id);
    u32 current;

    DecryptRecordStorage(records, id);
    current = GetRecordCounter(records, id);
    if (value > max) {
        value = max;
    }
    if (current < value) {
        current = SetRecordCounter(records, id, value);
    } else if (current > max) {
        current = SetRecordCounter(records, id, max);
    }
    EncryptRecordStorage(records, id);
    return current;
}

u32 RecordAdd(GameRecords *records, u32 id, u32 value) {
    u32 max = GetRecordCounterMax(id);
    u32 current;
    u32 result;

    DecryptRecordStorage(records, id);
    current = GetRecordCounter(records, id);
    if (current + value < max) {
        result = SetRecordCounter(records, id, current + value);
    } else {
        result = SetRecordCounter(records, id, max);
    }
    EncryptRecordStorage(records, id);
    return result;
}

u32 RecordAddOne(GameRecords *records, u32 id) {
    return RecordAdd(records, id, 1);
}

u32 RecordGet(GameRecords *records, u32 id) {
    u32 max = GetRecordCounterMax(id);
    u32 value;

    DecryptRecordStorage(records, id);
    value = GetRecordCounter(records, id);
    EncryptRecordStorage(records, id);
    if (value > max) {
        return max;
    }
    return value;
}

void func_02008e30(GameRecords *records) {
    u32 i;

    for (i = 0; i < 5; i++) {
        u32 source = data_0209dda0[i].source;

        func_02008d58(records, data_0209dda0[i].destination, RecordGet(records, source));
        func_02008d1c(records, source, 0);
    }
}

void func_02008e68(GameRecords *records, u8 rank) {
    if (rank < 7) {
        func_02008d58(records, 0x7b, rank);
    }
}

u8 func_02008e78(GameRecords *records) {
    return RecordGet(records, 0x7b);
}

void func_02008e88(GameRecords *records, u32 points) {
    if (points < 9999) {
        func_02008d58(records, 0x7c, points);
    }
}

u32 func_02008ea0(GameRecords *records) {
    return RecordGet(records, 0x7c);
}
