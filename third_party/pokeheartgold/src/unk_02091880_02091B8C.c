#include "global.h"

#include "constants/heap.h"

#include "pm_string.h"
#include "mail_misc.h"

struct EasyChatArgs;
struct EasyChatManager;
enum EasyChatGroup {
    GROUP_POKEMON = 0,
    GROUP_POKEMON_2,
    GROUP_MOVE,
    GROUP_MOVE_2,
    GROUP_STATUS,
    GROUP_TRAINER,
    GROUP_PEOPLE,
    GROUP_GREETINGS,
    GROUP_LIFESTYLE,
    GROUP_FEELINGS,
    GROUP_TOUGH_WORDS,
    GROUP_UNION,
    EASY_CHAT_GROUP_COUNT,
    GROUP_MODE_CANCEL_INDEX = EASY_CHAT_GROUP_COUNT,
};
enum ABCModeChars {
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    EXCLAMATION,
    ABC_MODE_CHAR_COUNT,
};
typedef struct EasyChatManager EasyChatWordLoader;
typedef struct EasyChatWordList {
    const EasyChatArgs *easyChatArgs;
    EasyChatWordLoader *wordLoader;
    u32 groupWordCounts[EASY_CHAT_GROUP_COUNT];
    u32 groupStartIndices[EASY_CHAT_GROUP_COUNT];
    u16 wordsByGroup[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u32 letterWordCounts[ABC_MODE_CHAR_COUNT];
    u32 letterStartIndices[ABC_MODE_CHAR_COUNT];
    u16 wordsByLetter[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u8 addedWordBits[((((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23) >> 3) + 1)];
} EasyChatWordList;
static inline BOOL EasyChatWordList_WasWordAddedToGroup(const EasyChatWordList *wordList, u32 word) {
    return (wordList->addedWordBits[word >> 3] & (1 << (word & 7))) != 0;
}
u16 sub_02091CAC(const EasyChatWordList *wordList, u32 letter, u32 index);

u32 sub_02091B8C(EasyChatWordList *wordList, const u16 *words, u16 *outList, u32 outListIndex);
u32 sub_02091C38(const EasyChatWordList *wordList, u32 group);
void sub_02091C40(const EasyChatWordList *wordList, u32 group, u32 index, String *out);
u16 sub_02091C60(const EasyChatWordList *wordList, u32 group, u32 index);
u32 sub_02091C74(const EasyChatWordList *wordList, u32 letter);
void sub_02091C84(const EasyChatWordList *wordList, u32 letter, u32 index, String *out);
u16 sub_02091CAC(const EasyChatWordList *wordList, u32 letter, u32 index);

u32 sub_02091B8C(EasyChatWordList *wordList, const u16 *words, u16 *outList, u32 outListIndex) {
    u32 total = 0;

    while (*words != 0xFFFF) {
        if (*words == 0xFFFE) {
            words++;
            u32 duplicateCount = *words++;

            for (u32 i = 0; i < duplicateCount; i++) {
                if (EasyChatWordList_WasWordAddedToGroup(wordList, words[i])) {
                    outList[outListIndex++] = words[i];
                    total++;
                    break;
                }
            }

            words += duplicateCount;
        } else {
            if (EasyChatWordList_WasWordAddedToGroup(wordList, *words)) {
                outList[outListIndex++] = *words;
                total++;
            }

            words++;
        }
    }

    return total;
}

u32 sub_02091C38(const EasyChatWordList *wordList, u32 group) {
    return wordList->groupWordCounts[group];
}

void sub_02091C40(const EasyChatWordList *wordList, u32 group, u32 index, String *out) {
    u32 startIndex = wordList->groupStartIndices[group];
    EasyChatManager_ReadWordIntoString(wordList->wordLoader, wordList->wordsByGroup[startIndex + index], out);
}

u16 sub_02091C60(const EasyChatWordList *wordList, u32 group, u32 index) {
    u32 startIndex = wordList->groupStartIndices[group];
    return wordList->wordsByGroup[startIndex + index];
}

u32 sub_02091C74(const EasyChatWordList *wordList, u32 letter) {
    return wordList->letterWordCounts[letter];
}

void sub_02091C84(const EasyChatWordList *wordList, u32 letter, u32 index, String *out) {
    if (wordList->letterWordCounts[letter]) {
        u16 word = sub_02091CAC(wordList, letter, index);
        EasyChatManager_ReadWordIntoString(wordList->wordLoader, word, out);
    }
}

u16 sub_02091CAC(const EasyChatWordList *wordList, u32 letter, u32 index) {
    if (wordList->letterWordCounts[letter]) {
        u32 wordIndex = wordList->letterStartIndices[letter] + index;
        return wordList->wordsByLetter[wordIndex];
    } else {
        return 0xFFFF;
    }
}
