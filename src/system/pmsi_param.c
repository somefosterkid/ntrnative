// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pmsi_param.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "save/event_work.h"
#include "save/save_control.h"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "system/pmsi_param.h"

// The parameters of the sentence input screen

struct PMSIParam {
    u8 mode;
    // Set until a result is set
    u8 canceled;
    // Whether the result differs from the starting value
    u8 changed;
    // Event flag 0x960
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 hasStartSentence : 1;
    u8 flag3 : 1;
    u32 unk4;
    // Save block 0x36
    void *unk8;
    PMSWordSave *wordSave;
    PMSData sentence;
    u16 words[PMS_SENTENCE_WORD_MAX];
    u32 unk1C;
};

static BOOL func_0202419c(const PMSIParam *param, const u16 *words, const PMSData *sentence);

// pokebw2's PMSIParam_Create
PMSIParam *func_02024044(u32 mode, u32 unused, const PMSData *sentence, BOOL flag3, SaveControl *save, u32 heapId) {
    PMSIParam *param = GFL_HeapAllocate(heapId, sizeof(PMSIParam), TRUE, "pmsi_param.c", 86);
    int i;

    param->mode = mode;
    param->unk8 = SaveControl_GetBlockPtr(save, 0x37);
    param->wordSave = getDexBlkAddress(save);
    param->flag0 = EventWork_FlagGet(getConstDataBlock(save), 0x960);
    param->flag1 = FALSE;
    param->canceled = TRUE;
    param->changed = FALSE;
    param->flag3 = flag3;
    if (mode == PMSI_MODE_SENTENCE) {
        if (sentence == NULL) {
            func_020242a4(&param->sentence, 0);
        } else {
            MI_CpuCopy8(sentence, &param->sentence, sizeof(PMSData));
            param->hasStartSentence = TRUE;
        }
    } else {
        for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
            param->words[i] = PMS_WORD_NULL;
        }
    }
    return param;
}

// pokebw2's PMSIParam_Free
void func_020240fc(PMSIParam *param) {
    GFL_HeapFree(param);
}

// pokebw2's PMSIParam_SetWord
void func_02024104(PMSIParam *param, u16 word) {
    param->words[0] = word;
}

// pokebw2's PMSIParam_SetSentence
void func_02024108(PMSIParam *param, const PMSData *sentence) {
    param->sentence = *sentence;
}

// pokebw2's PMSIParam_IsCanceled
BOOL func_0202411c(const PMSIParam *param) {
    return param->canceled;
}

// pokebw2's PMSIParam_IsChanged
BOOL func_02024120(const PMSIParam *param) {
    return param->changed;
}

// pokebw2's PMSIParam_GetWord
u16 func_02024124(const PMSIParam *param) {
    return param->words[0];
}

// pokebw2's PMSIParam_GetWords
void func_02024128(const PMSIParam *param, u16 *words) {
    words[0] = param->words[0];
    words[1] = param->words[1];
}

// pokebw2's PMSIParam_GetSentence
void func_02024134(const PMSIParam *param, PMSData *sentence) {
    func_02024570(sentence, &param->sentence);
}

// pokebw2's PMSIParam_GetMode
u32 func_02024144(const PMSIParam *param) {
    return param->mode;
}

// pokebw2's PMSIParam_GetPokeDex
void *func_02024148(const PMSIParam *param) {
    return param->unk8;
}

// pokebw2's PMSIParam_GetWordSave
PMSWordSave *func_0202414c(const PMSIParam *param) {
    return param->wordSave;
}

BOOL func_02024150(const PMSIParam *param) {
    return param->flag0;
}

// pokebw2's PMSIParam_HasStartSentence
BOOL func_02024158(const PMSIParam *param) {
    return param->hasStartSentence;
}

// pokebw2's PMSIParam_HasNumbers
BOOL func_02024160(const PMSIParam *param) {
    return param->flag3;
}

// pokebw2's PMSIParam_GetResult
void func_02024168(const PMSIParam *param, u16 *words, PMSData *sentence) {
    switch (param->mode) {
    case PMSI_MODE_WORD:
        words[0] = param->words[0];
        break;
    case PMSI_MODE_DOUBLE_WORD:
        words[0] = param->words[0];
        words[1] = param->words[1];
        break;
    case PMSI_MODE_SENTENCE:
        *sentence = param->sentence;
        break;
    }
}

// pokebw2's IsSameAsStart
static BOOL func_0202419c(const PMSIParam *param, const u16 *words, const PMSData *sentence) {
    switch (param->mode) {
    case PMSI_MODE_WORD:
        if (words[0] == param->words[0]) {
            return TRUE;
        }
        return FALSE;
    case PMSI_MODE_DOUBLE_WORD:
        if (words[0] == param->words[0] && words[1] == param->words[1]) {
            return TRUE;
        }
        return FALSE;
    case PMSI_MODE_SENTENCE:
    default:
        return func_02024534(&param->sentence, sentence);
    }
}

// pokebw2's PMSIParam_SetResult
void func_020241e0(PMSIParam *param, const u16 *words, const PMSData *sentence) {
    int i;

    param->changed = !func_0202419c(param, words, sentence);
    param->canceled = FALSE;
    for (i = 0; i < PMS_SENTENCE_WORD_MAX; i++) {
        param->words[i] = words[i];
    }
    param->sentence = *sentence;
}

BOOL func_0202421c(const PMSIParam *param) {
    return func_020355c4();
}

void func_02024224(PMSIParam *param, BOOL touch) {
    func_020355d8(touch);
}
