#include "global.h"

#include "constants/heap.h"

#include "pokedex.h"

struct EasyChatArgs;

struct EasyChatWordLoader;

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

typedef struct EasyChatWordLoader EasyChatWordLoader;

typedef struct EasyChatArgs EasyChatArgs;

const Pokedex *sub_02090D7C(const EasyChatArgs *args);

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

static inline void EasyChatWordList_FlagWordAsAdded(EasyChatWordList *wordList, u32 word) {
    wordList->addedWordBits[word >> 3] |= (1 << (word & 7));
}

u32 ECMenuBuild_Pokemon(EasyChatWordList *wordList, const u16 *words, u32 length, u16 *outList);

u32 ECMenuBuild_Pokemon(EasyChatWordList *wordList, const u16 *words, u32 length, u16 *outList) {
    u32 total = 0;
    const Pokedex *pokedex = sub_02090D7C(wordList->easyChatArgs);

    for (u32 i = 0; i < length; i++) {
        if (Pokedex_CheckMonSeenFlag(pokedex, words[i])) {
            EasyChatWordList_FlagWordAsAdded(wordList, words[i]);
            *outList++ = words[i];
            total++;
        }
    }

    return total;
}

u8 sub_02090D84(const EasyChatArgs *args);

u32 ECMenuBuild_Move(EasyChatWordList *wordList, const u16 *words, u32 length, u16 *outList);

u32 ECMenuBuild_Move(EasyChatWordList *wordList, const u16 *words, u32 length, u16 *outList) {
    if (sub_02090D84(wordList->easyChatArgs)) {
        for (u32 i = 0; i < length; i++) {
            EasyChatWordList_FlagWordAsAdded(wordList, *words);
            *outList++ = *words++;
        }

        return length;
    } else {
        return 0;
    }
}
