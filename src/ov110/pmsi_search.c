// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsi_search.c, which is the same code in
// Black 2/White 2's overlay 185 but for the message files' numbers. The names are Black's dsd names; pokebw2's guesses
// are in the header and the comments.
#include "app/pmsi_search.h"
#include "types.h"
#include "app/ui/msgsearch.h"
#include "app/pms_input_data.h"
#include "app/pmsi_initial_data.h"
#include "constants/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"

// The phrase input's search: the letters typed are kept as initials, and the words of the first letter's initial
// whose names start with them are listed (pmsi_search.c, named by the ROM's embedded string)

// The words of the others' initial that the search leaves out
#define PMSI_SEARCH_EXCLUDED_COUNT 10
// How many words a search lists
#define PMSI_SEARCH_RESULT_MAX 264

struct PMSISearch {
    const PMSInputWork *mwk;
    const PMSInputData *dwk;
    u16 heapId;
    // The names of the words of each initial
    MsgData *msgData[PMSI_INITIAL_COUNT];
    MsgSearch *search;
    MsgSearchResult results[PMSI_SEARCH_RESULT_MAX];
    u16 words[PMSI_SEARCH_RESULT_MAX + 2];
    StrBuf *inputStr;
    u16 input[PMSI_SEARCH_INPUT_MAX + 1];
    u8 inputLen;
    u8 resultCount;
};

// sPMSISearchExcludedWords
static const u16 data_ov110_021c375c[PMSI_SEARCH_EXCLUDED_COUNT] = {
    1732, 1733, 1734, 1735, 1736, 1737, 1738, 1739, 1740, 1741,
};

// The message files of the words of each initial (sPMSISearchInitialMsgFiles)
static const u16 data_ov110_021c3770[PMSI_INITIAL_COUNT] = {
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
};

PMSISearch *func_ov110_021baedc(const PMSInputWork *mwk, const PMSInputData *dwk, HeapID heapId) {
    int i;
    PMSISearch *ss;

    ss = GFL_HeapAllocate(heapId, sizeof(PMSISearch), TRUE, "pmsi_search.c", 137);
    ss->mwk = mwk;
    ss->dwk = dwk;
    ss->heapId = heapId;
    func_ov110_021bafe4(ss);
    for (i = 0; i < PMSI_INITIAL_COUNT; i++) {
        ss->msgData[i] = GFL_MsgSysLoadData(TRUE, ARCID_SYSTEM_MESSAGE, data_ov110_021c3770[i], heapId);
    }
    ss->search = func_ov067_021b6c74(ss->msgData, PMSI_INITIAL_COUNT, ss->heapId);
    ss->inputStr = GFL_StrBufCreate(27, heapId);
    return ss;
}

void func_ov110_021baf4c(PMSISearch *ss) {
    int i;

    GFL_StrBufFree(ss->inputStr);
    func_ov067_021b6ccc(ss->search);
    for (i = 0; i < PMSI_INITIAL_COUNT; i++) {
        GFL_MsgDataFree(ss->msgData[i]);
    }
    GFL_HeapFree(ss);
}

void func_ov110_021baf7c(PMSISearch *ss, u16 initial) {
    int len;

    ss->input[ss->inputLen] = initial;
    len = ss->inputLen + 1;
    ss->inputLen = len > PMSI_SEARCH_INPUT_MAX - 1 ? PMSI_SEARCH_INPUT_MAX - 1 : (len < 0 ? 0 : len);
}

BOOL func_ov110_021bafac(PMSISearch *ss) {
    if (ss->inputLen == 0) {
        return FALSE;
    }
    if (ss->input[PMSI_SEARCH_INPUT_MAX - 1] == PMSI_SEARCH_INPUT_NONE) {
        ss->inputLen--;
    }
    ss->input[ss->inputLen] = PMSI_SEARCH_INPUT_NONE;
    return TRUE;
}

u8 func_ov110_021bafdc(PMSISearch *ss) {
    return ss->inputLen;
}

void func_ov110_021bafe4(PMSISearch *ss) {
    int i;

    for (i = 0; i < PMSI_SEARCH_INPUT_MAX; i++) {
        ss->input[i] = PMSI_SEARCH_INPUT_NONE;
    }
    ss->inputLen = 0;
    ss->resultCount = 0;
}

void func_ov110_021bb00c(PMSISearch *ss, StrBuf *buf) {
    u16 str[PMSI_SEARCH_INPUT_MAX + 1];
    int i;

    for (i = 0; i < PMSI_SEARCH_INPUT_MAX; i++) {
        if (ss->input[i] == PMSI_SEARCH_INPUT_NONE) {
            str[i] = GFL_StrBufGetTerminator();
            break;
        }
        str[i] = func_ov110_021bb214(ss->input[i]);
    }
    str[PMSI_SEARCH_INPUT_MAX] = GFL_StrBufGetTerminator();
    GFL_StrBufLoadString(buf, str);
}

BOOL func_ov110_021bb054(PMSISearch *ss) {
    u32 count;
    u32 initial;
    u32 i;

    initial = ss->input[0];
    if (initial >= PMSI_INITIAL_COUNT) {
        ss->resultCount = 0;
        return FALSE;
    }
    GFL_StrBufClear(ss->inputStr);
    func_ov110_021bb00c(ss, ss->inputStr);
    if (initial == PMSI_INITIAL_COUNT - 1) {
        count = GFL_MsgDataGetLineCount(ss->msgData[initial]);
        u32 n = 0;
        u16 words[PMSI_SEARCH_RESULT_MAX + 1] = { 0 };

        for (i = 0; i < count; i++) {
            u16 word = func_ov110_021b99f0(ss->dwk, initial, i);
            BOOL excluded = FALSE;
            u32 j;

            for (j = 0; j < PMSI_SEARCH_EXCLUDED_COUNT; j++) {
                if (word == data_ov110_021c375c[j]) {
                    excluded = TRUE;
                    break;
                }
            }
            if (!excluded) {
                ss->results[n].file = initial;
                ss->results[n].line = i;
                words[n] = word;
                n++;
            }
        }
        words[n] = func_ov110_021b9a48(ss->dwk);
        ss->resultCount = func_ov110_021b99d8(ss->dwk, words, ss->words);
        if (ss->resultCount) {
            return TRUE;
        }
        return FALSE;
    } else {
        u16 words[PMSI_SEARCH_RESULT_MAX + 1] = { 0 };
        u32 n;

        n = func_ov067_021b6ce0(ss->search, initial, 0, ss->inputStr, ss->results, PMSI_SEARCH_RESULT_MAX);
        for (i = 0; i < n; i++) {
            words[i] = func_ov110_021b99f0(ss->dwk, initial, ss->results[i].line);
        }
        words[n] = func_ov110_021b9a48(ss->dwk);
        ss->resultCount = func_ov110_021b99d8(ss->dwk, words, ss->words);
        if (ss->resultCount) {
            return TRUE;
        }
        return FALSE;
    }
}

u8 func_ov110_021bb1c0(PMSISearch *ss) {
    return ss->resultCount;
}

void func_ov110_021bb1cc(PMSISearch *ss, u8 index, StrBuf *buf) {
    func_ov110_021b9a3c(ss->dwk, ss->words[index], buf);
}

u16 func_ov110_021bb1e4(PMSISearch *ss, u32 index) {
    return ss->words[index];
}
