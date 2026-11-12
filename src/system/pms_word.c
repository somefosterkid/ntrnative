// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pms_word.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/abilities.h"
#include "constants/arc.h"
#include "constants/language.h"
#include "constants/moves.h"
#include "constants/pms_words.h"
#include "constants/text_banks.h"
#include "constants/types.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/save_control.h"
#include "system/pms_data.h"
#include "system/pms_word.h"

// The words that fill sentences, and the save block of sentences

#define PMS_SAVE_SENTENCE_COUNT 20

struct PMSWordBank {
    u32 heapId;
    MsgData *msgData[PMS_WORD_CATEGORY_COUNT];
};

struct PMSWordSave {
    u32 languageFlags;
    u32 unk4;
    PMSData sentences[PMS_SAVE_SENTENCE_COUNT];
};

typedef struct {
    u8 language;
    // The language's bit in the flags
    u8 flag;
} PMSLanguageFlag;

// Groups of words that count as the same word
typedef struct {
    const u16 *words;
    int count;
} PMSWordGroup;

// The number of words in each category
// pokebw2's sCategoryWordCounts
static const u16 data_0209f792[PMS_WORD_CATEGORY_COUNT] = {
    PMS_WORD_COUNT_SPECIES,  PMS_WORD_COUNT_MOVES,  PMS_WORD_COUNT_TYPES,      PMS_WORD_COUNT_ABILITIES,
    PMS_WORD_COUNT_TRAINER,  PMS_WORD_COUNT_PEOPLE, PMS_WORD_COUNT_GREETINGS,  PMS_WORD_COUNT_LIFESTYLE,
    PMS_WORD_COUNT_FEELINGS, PMS_WORD_COUNT_TERM,   PMS_WORD_COUNT_CONNECTION, PMS_WORD_COUNT_ANIMATED,
    PMS_WORD_COUNT_VOICE,
};

// The message file of each category
// pokebw2's sCategoryMsgFiles
static const u16 data_0209f7ac[PMS_WORD_CATEGORY_COUNT] = {
    0x11c, 0x11e,     0x11f,
    0x11d, 0x95,    0x96,
    0x97, 0x98,  0x99,
    0x9a,      0x9b, 0x94,
    0x9c,
};

// pokebw2's sWordGroup0
static const u16 data_0209f74c[] = { PMS_WORD_PEOPLE_YOU_2, PMS_WORD_PEOPLE_YOU };
// pokebw2's sWordGroup1
static const u16 data_0209f77c[] = { PMS_WORD_PEOPLE_I_3, PMS_WORD_PEOPLE_I, PMS_WORD_PEOPLE_I_2, PMS_WORD_PEOPLE_I_4 };
// pokebw2's sWordGroup2
static const u16 data_0209f778[] = { PMS_WORD_PEOPLE_KIDS_2, PMS_WORD_PEOPLE_KIDS };
// pokebw2's sWordGroup3
static const u16 data_0209f774[] = { PMS_WORD_VOICE_OK_2, PMS_WORD_VOICE_OK };
// pokebw2's sWordGroup4
static const u16 data_0209f770[] = { PMS_WORD_VOICE_WOW_2, PMS_WORD_VOICE_WOW };
// pokebw2's sWordGroup5
static const u16 data_0209f76c[] = { PMS_WORD_GREETINGS_WELCOME, PMS_WORD_GREETINGS_WELCOME_2 };
// pokebw2's sWordGroup6
static const u16 data_0209f768[] = { PMS_WORD_GREETINGS_HELLO_2, PMS_WORD_GREETINGS_HELLO };
// pokebw2's sWordGroup7
static const u16 data_0209f754[] = { PMS_WORD_TYPE(TYPE_PSYCHIC), PMS_WORD_MOVE(MOVE_PSYCHIC) };
// pokebw2's sWordGroup8
static const u16 data_0209f750[] = { PMS_WORD_FEELINGS_SIMPLE, PMS_WORD_ABILITY(ABILITY_SIMPLE) };
// pokebw2's sWordGroup9
static const u16 data_0209f75c[] = { PMS_WORD_FEELINGS_NO_WAY, PMS_WORD_GREETINGS_NO_WAY };
// pokebw2's sWordGroup10
static const u16 data_0209f758[] = { PMS_WORD_FEELINGS_ANTICIPATION, PMS_WORD_ABILITY(ABILITY_ANTICIPATION) };
// pokebw2's sWordGroup11
static const u16 data_0209f764[] = { PMS_WORD_VOICE_HUH, PMS_WORD_VOICE_HUH_2 };
// pokebw2's sWordGroup12
static const u16 data_0209f760[] = { PMS_WORD_VOICE_HEY, PMS_WORD_VOICE_HEY_4 };

// pokebw2's sWordGroups
static const PMSWordGroup data_0209f7c8[] = {
    { data_0209f74c, NELEMS(data_0209f74c) },   { data_0209f77c, NELEMS(data_0209f77c) },
    { data_0209f778, NELEMS(data_0209f778) },   { data_0209f774, NELEMS(data_0209f774) },
    { data_0209f770, NELEMS(data_0209f770) },   { data_0209f76c, NELEMS(data_0209f76c) },
    { data_0209f768, NELEMS(data_0209f768) },   { data_0209f754, NELEMS(data_0209f754) },
    { data_0209f750, NELEMS(data_0209f750) },   { data_0209f75c, NELEMS(data_0209f75c) },
    { data_0209f758, NELEMS(data_0209f758) }, { data_0209f764, NELEMS(data_0209f764) },
    { data_0209f760, NELEMS(data_0209f760) },
};

// pokebw2's sLanguageFlags
static const PMSLanguageFlag data_0209f784[] = {
    { LANGUAGE_JAPANESE, 0 }, { LANGUAGE_ENGLISH, 1 }, { LANGUAGE_FRENCH, 2 }, { LANGUAGE_ITALIAN, 3 },
    { LANGUAGE_GERMAN, 4 },   { LANGUAGE_SPANISH, 5 }, { LANGUAGE_KOREAN, 6 },
};

// pokebw2's PMSWordBank_Create
PMSWordBank *func_020247a8(u32 heapId) {
    int i;
    PMSWordBank *bank = GFL_HeapAllocate(heapId, sizeof(PMSWordBank), FALSE, "pms_word.c", 50);

    for (i = 0; i < PMS_WORD_CATEGORY_COUNT; i++) {
        bank->heapId = heapId;
        bank->msgData[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, data_0209f7ac[i], heapId);
    }
    return bank;
}

// pokebw2's PMSWordBank_Free
void func_020247f0(PMSWordBank *bank) {
    int i;

    for (i = 0; i < PMS_WORD_CATEGORY_COUNT; i++) {
        GFL_MsgDataFree(bank->msgData[i]);
    }
    GFL_HeapFree(bank);
}

// pokebw2's PMSWordBank_LoadWord
void func_02024810(PMSWordBank *bank, u16 word, StrBuf *strbuf) {
    u32 category;
    u32 index;

    func_020248d8(word, &category, &index);
    GFL_MsgDataLoadStrbuf(bank->msgData[category], index, strbuf);
}

void loadSayingToString(u16 saying, StrBuf *strbuf, HeapID heapId) {
    u32 fileId;
    u32 index;
    MsgData *msgData;

    if (saying != PMS_WORD_NULL) {
        func_020248d8(saying, &fileId, &index);
        // The category becomes its message file in place: a separate variable doesn't match
        fileId = data_0209f7ac[fileId];
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, fileId, heapId);
        GFL_MsgDataLoadStrbuf(msgData, index, strbuf);
        GFL_MsgDataFree(msgData);
    } else {
        GFL_StrBufClear(strbuf);
    }
}

// pokebw2's PMSWord_FromMessage
u16 func_02024888(u16 fileId, u16 index) {
    u32 i;
    u16 base;
    u16 j;

    for (i = 0; i < PMS_WORD_CATEGORY_COUNT; i++) {
        if (fileId == data_0209f7ac[i]) {
            for (j = 0, base = 0; j < i; j++) {
                base += data_0209f792[j];
            }
            return base + index;
        }
    }
    return PMS_WORD_NULL;
}

// pokebw2's PMSWord_GetMessage
BOOL func_020248d8(u32 word, u32 *category, u32 *index) {
    u32 i;
    u32 end;

    word &= PMS_WORD_INDEX_MASK;
    end = 0;
    for (i = 0; i < PMS_WORD_CATEGORY_COUNT; i++) {
        end += data_0209f792[i];
        if (word < end) {
            *category = i;
            *index = word - (end - data_0209f792[i]);
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's PMSWordSave_GetSentence
PMSData *func_02024910(PMSWordSave *save, int index) {
    return &save->sentences[index];
}

// pokebw2's PMSWordSave_SetSentence
void func_02024918(PMSWordSave *save, int index, const PMSData *sentence) {
    if (index >= 0 && index < PMS_SAVE_SENTENCE_COUNT) {
        MI_CpuCopy8(sentence, &save->sentences[index], sizeof(PMSData));
    }
}

// pokebw2's PMSWordSave_GetSize
u32 func_02024934(void) {
    return sizeof(PMSWordSave);
}

// pokebw2's PMSWordSave_Init
void func_02024938(PMSWordSave *save) {
    int i;
    int j;

    save->languageFlags = 0;
    save->unk4 = 0;
    for (i = 0; i < PMS_SAVE_SENTENCE_COUNT; i++) {
        for (j = 0; j < PMS_SENTENCE_WORD_MAX; j++) {
            save->sentences[i].words[j] = PMS_WORD_NULL;
        }
    }
    for (i = 0; i < NELEMS(data_0209f784); i++) {
        if (data_0209f784[i].language == GAME_LANGUAGE) {
            func_020249a0(save, data_0209f784[i].flag);
            break;
        }
    }
}

PMSWordSave *getDexBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_PMS);
}

// pokebw2's PMSWordSave_GetLanguageFlag
BOOL func_02024994(PMSWordSave *save, u32 bit) {
    return (save->languageFlags >> bit) & 1;
}

// pokebw2's PMSWordSave_SetLanguageFlag
void func_020249a0(PMSWordSave *save, u32 bit) {
    save->languageFlags |= 1 << bit;
}

// pokebw2's PMSWord_AreEquivalent
BOOL func_020249b0(u16 word, u16 other) {
    u32 i;
    int j;
    int k;

    if (word == other) {
        return TRUE;
    }
    for (i = 0; i < NELEMS(data_0209f7c8); i++) {
        for (j = 0; j < data_0209f7c8[i].count; j++) {
            if (word == data_0209f7c8[i].words[j]) {
                for (k = 0; k < data_0209f7c8[i].count; k++) {
                    if (other == data_0209f7c8[i].words[k]) {
                        return TRUE;
                    }
                }
                return FALSE;
            }
        }
    }
    return FALSE;
}
