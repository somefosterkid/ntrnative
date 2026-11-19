#include "global.h"
#include "constants/heap.h"
#include "mail_misc.h"

struct EasyChatArgs;

struct EasyChatWordList;

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

extern const u16 *const _02106C9C[27];

u32 sub_02091B8C(EasyChatWordList *wordList, const u16 *words, u16 *outList, u32 outListIndex);

EasyChatWordList *sub_02091880(enum HeapID heapID, const EasyChatArgs *easyChatArgs);

void sub_02091B44(EasyChatWordList *wordList);

void sub_02091B44(EasyChatWordList *wordList) {
    int total = 0;

    for (int i = 0; i < NELEMS(_02106C9C); i++) {
        wordList->letterStartIndices[i] = total;
        wordList->letterWordCounts[i] = sub_02091B8C(wordList, _02106C9C[i], wordList->wordsByLetter, total);
        total += wordList->letterWordCounts[i];
    }
}
