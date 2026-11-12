// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pms_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/abilities.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/pms_words.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "system/printsys.h"
#include "system/wordset.h"



// Sentences with words filled in. The file's name is a guess: the ROM has no string for it

// The word a word that isn't valid is replaced with
#define PMS_WORD_DEFAULT PMS_WORD_TERM_POKEMON
// The numbers a number word can be, from PMS_NUMBER_MIN to below PMS_NUMBER_END
#define PMS_NUMBER_MIN 1
#define PMS_NUMBER_END 11

static u32 func_02024498(u16 type, u16 id, HeapID heapId);
static BOOL func_02024768(u16 word);

// pokebw2's sSentenceCounts
static const u8 data_0209f71c[PMS_SENTENCE_TYPE_COUNT] = { 20, 20, 20, 20, 20, 21, 2 };

// pokebw2's sSentenceMsgFiles
static const u16 data_0209f724[PMS_SENTENCE_TYPE_COUNT] = {
    TEXT_BANK_0171, TEXT_BANK_0173, TEXT_BANK_0176, TEXT_BANK_0170, TEXT_BANK_0175, TEXT_BANK_0172, TEXT_BANK_0174,
};

// pokebw2's PMSData_Clear
void func_0202428c(PMSData *data) {
    int i;

    data->type = PMS_WORD_NULL;
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        data->words[i] = PMS_WORD_NULL;
    }
}

// pokebw2's PMSData_Init
void func_020242a4(PMSData *data, u16 type) {
    int i;

    data->type = type;
    data->id = 0;
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        data->words[i] = PMS_WORD_NULL;
    }
}

// pokebw2's PMSData_InitWithSentence
void func_020242c0(PMSData *data, u16 type, u16 id) {
    func_020242a4(data, type);
    func_02024598(data, type, id);
}

void func_020242d8(PMSData *data, u32 preset) {
    switch (preset) {
    case 0:
        func_020242a4(data, 1);
        data->id = 0;
        data->words[0] = PMS_WORD_GREETINGS_LETS_GO;
        break;
    case 1:
        func_020242a4(data, 2);
        data->id = 0;
        data->words[0] = PMS_WORD_GREETINGS_THANKS;
        break;
    case 2:
        func_020242a4(data, 3);
        data->id = 0;
        data->words[0] = PMS_WORD_FEELINGS_REGRET;
        break;
    case 3:
        func_020242a4(data, 2);
        data->id = 13;
        data->words[0] = PMS_WORD_TRAINER_NO1;
        break;
    }
}

void func_02024344(PMSData *data) {
    func_020242a4(data, 0);
    data->id = 1;
    data->words[0] = PMS_WORD_TRAINER_TRAINER;
}

// pokebw2's PMSData_ToString
StrBuf *func_0202435c(const PMSData *data, u32 heapId) {
    return func_02024368(data, heapId, PMS_SENTENCE_WORD_MAX);
}

// pokebw2's PMSData_ToStringWithWords
StrBuf *func_02024368(const PMSData *data, u32 heapId, int wordCount) {
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(heapId);
    int i;
    MsgData *msgData;
    StrBuf *strbuf;
    StrBuf *sentence;

    for (i = 0; i < wordCount; i++) {
        if (data->words[i] != PMS_WORD_NULL) {
            if ((data->words[i] >> PMS_WORD_NUMBER_SHIFT) & 1) {
                func_0201effc(wordSet, i, data->words[i] & PMS_WORD_INDEX_MASK);
            } else {
                loadSayingForDisplay(wordSet, i, data->words[i]);
            }
        } else {
            func_0201effc(wordSet, i, 1);
        }
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_0209f724[data->type], heapId);
    strbuf = GFL_StrBufCreate(0x100, heapId);
    sentence = GFL_MsgDataLoadStrbufNew(msgData, data->id);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, sentence);
    GFL_StrBufFree(sentence);
    GFL_MsgDataFree(msgData);
    GFL_WordSetSystemFree(wordSet);
    return strbuf;
}

// pokebw2's PMSData_GetSentenceString
StrBuf *func_0202441c(const PMSData *data, u32 heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_0209f724[data->type], heapId);
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(msgData, data->id);

    GFL_MsgDataFree(msgData);
    return strbuf;
}

// pokebw2's PMSData_IsNotEmpty
BOOL func_02024450(const PMSData *data) {
    return data->type != PMS_WORD_NULL;
}

// pokebw2's PMSData_IsComplete
BOOL func_02024464(const PMSData *data, HeapID heapId) {
    u32 count = func_02024498(data->type, data->id, heapId);
    u32 i;

    for (i = 0; i < count; i++) {
        if (data->words[i] == PMS_WORD_NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's GetSentenceWordCount
static u32 func_02024498(u16 type, u16 id, HeapID heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_0209f724[type], heapId);
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(msgData, id);
    u32 count;

    GFL_MsgDataFree(msgData);
    count = GFL_StrCmdGetWordSetCommandCount(strbuf);
    GFL_StrBufFree(strbuf);
    return count;
}

u16 func_020244d4(const PMSData *data, u32 index) {
    u16 word = data->words[index];

    if (word == PMS_WORD_NULL) {
        return PMS_WORD_NULL;
    }
    return word & PMS_WORD_INDEX_MASK;
}

// pokebw2's PMSData_GetWord
u16 func_020244f0(const PMSData *data, u32 index) {
    return data->words[index];
}

BOOL func_020244f8(const PMSData *data, u32 index) {
    return func_02024508(data->words[index]);
}

// pokebw2's PMSWord_IsNumber
BOOL func_02024508(u16 word) {
    if (word == PMS_WORD_NULL) {
        return FALSE;
    }
    return (word >> PMS_WORD_NUMBER_SHIFT) & 1;
}

// pokebw2's PMSWord_GetNumber
int func_02024520(u16 word) {
    return word & PMS_WORD_INDEX_MASK;
}

// pokebw2's PMSData_GetType
u16 func_0202452c(const PMSData *data) {
    return data->type;
}

// pokebw2's PMSData_GetID
u16 func_02024530(const PMSData *data) {
    return data->id;
}

// pokebw2's PMSData_Equals
BOOL func_02024534(const PMSData *data, const PMSData *other) {
    int i;

    if (data->type != other->type || data->id != other->id) {
        return FALSE;
    }
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        if (data->words[i] != other->words[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's PMSData_Copy
void func_02024570(PMSData *dest, const PMSData *src) {
    *dest = *src;
}

// pokebw2's PMSData_GetSentenceCount
u32 func_02024584(u32 type) {
    if (type < PMS_SENTENCE_TYPE_COUNT) {
        return data_0209f71c[type];
    }
    return 0;
}

// pokebw2's PMSData_SetSentence
void func_02024598(PMSData *data, u32 type, u32 id) {
    data->type = type;
    data->id = id;
}

// pokebw2's PMSData_SetWord
void func_020245a0(PMSData *data, u32 index, u16 word) {
    if (index < PMS_SENTENCE_WORD_MAX) {
        data->words[index] = word;
    }
}

// pokebw2's PMSData_ClearUnusedWords
void func_020245ac(PMSData *data, HeapID heapId) {
    u32 i;

    for (i = func_02024498(data->type, data->id, heapId); i < PMS_SENTENCE_WORD_MAX; i++) {
        data->words[i] = PMS_WORD_NULL;
    }
}

// pokebw2's PMSData_IsValid
BOOL func_020245d4(const PMSData *data, u32 heapId) {
    u16 id;
    u32 count;
    u32 i;
    BOOL valid;
    u32 index;
    u32 category;

    if (data->type < PMS_SENTENCE_TYPE_COUNT) {
        id = data->id;
        if (id < func_02024584(data->type)) {
            count = func_02024498(data->type, id, HEAPID_TAIL(heapId));
            valid = TRUE;
            for (i = 0; i < count; i++) {
                if (func_02024508(data->words[i])) {
                    int number = func_02024520(data->words[i]);

                    if (number < PMS_NUMBER_MIN || number >= PMS_NUMBER_END) {
                        valid = FALSE;
                        break;
                    }
                } else if (!func_020248d8(data->words[i], &category, &index)) {
                    valid = FALSE;
                    break;
                }
            }
            return valid;
        }
    }
    return FALSE;
}

// pokebw2's PMSData_Validate
BOOL func_0202465c(PMSData *data, BOOL allowEmpty, HeapID heapId) {
    BOOL valid = TRUE;
    u32 count;
    u32 i;

    if (data->type >= PMS_SENTENCE_TYPE_COUNT || data->id >= func_02024584(data->type)) {
        valid = FALSE;
        data->type = 0;
        data->id = 0;
    }
    count = func_02024498(data->type, data->id, HEAPID_TAIL(heapId));
    if (count > PMS_SENTENCE_WORD_MAX) {
        count = PMS_SENTENCE_WORD_MAX;
    }
    for (i = 0; i < count; i++) {
        if (!func_020246c8(&data->words[i], allowEmpty, TRUE)) {
            valid = FALSE;
        }
    }
    return valid;
}

// pokebw2's PMSWord_Validate
BOOL func_020246c8(u16 *word, BOOL allowEmpty, BOOL allowNumber) {
    BOOL valid = TRUE;
    u32 index;
    u32 category;

    if (allowEmpty) {
        if (*word == PMS_WORD_NULL) {
            return valid;
        }
    } else if (*word == PMS_WORD_NULL) {
        *word = PMS_WORD_DEFAULT;
        return FALSE;
    }
    if (func_02024508(*word)) {
        if (allowNumber) {
            int number = func_02024520(*word);

            if (number < PMS_NUMBER_MIN || number >= PMS_NUMBER_END) {
                *word = PMS_WORD_DEFAULT;
                valid = FALSE;
            }
        } else {
            *word = PMS_WORD_DEFAULT;
            valid = FALSE;
        }
    } else if (func_02024768(*word) || !func_020248d8(*word, &category, &index)) {
        *word = PMS_WORD_DEFAULT;
        valid = FALSE;
    }
    return valid;
}

// pokebw2's PMSNumber_Validate
BOOL func_0202473c(int *number, BOOL allowZero) {
    BOOL valid = TRUE;

    if (allowZero) {
        if (*number == 0) {
            return valid;
        }
    } else if (*number == 0) {
        *number = PMS_NUMBER_MIN;
        return FALSE;
    }
    if (*number < PMS_NUMBER_MIN || *number >= PMS_NUMBER_END) {
        *number = PMS_NUMBER_MIN;
        valid = FALSE;
    }
    return valid;
}

// Words that sentences can't use
// pokebw2's IsExcludedWord
static BOOL func_02024768(u16 word) {
    const u16 excluded[] = {
        PMS_WORD_SPECIES(SPECIES_NONE),   PMS_WORD_SPECIES_BAD_EGG,         PMS_WORD_SPECIES_EGG,
        PMS_WORD_MOVE(MOVE_NONE),         PMS_WORD_MOVE(MOVE_TECHNO_BLAST), PMS_WORD_MOVE(MOVE_RELIC_SONG),
        PMS_WORD_MOVE(MOVE_SECRET_SWORD), PMS_WORD_MOVE(MOVE_FREEZE_SHOCK), PMS_WORD_MOVE(MOVE_ICE_BURN),
        PMS_WORD_MOVE(MOVE_SNARL),        PMS_WORD_MOVE(MOVE_V_CREATE),     PMS_WORD_ABILITY(ABILITY_NONE)
    };
    u8 i;

    for (i = 0; i < NELEMS(excluded); i++) {
        if (word == excluded[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
