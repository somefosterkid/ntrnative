#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "heap.h"
#include "msgdata.h"
#include "pokemon_types_def.h"
#include "pokedex.h"
#include "mail_misc.h"

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
enum EasyChatWordTextBank {
    EASY_CHAT_WORD_TEXT_BANK_SPECIES_NAME = 0,
    EASY_CHAT_WORD_TEXT_BANK_MOVE_NAMES_UPPERCASE,
    EASY_CHAT_WORD_TEXT_BANK_POKEMON_TYPE_NAMES,
    EASY_CHAT_WORD_TEXT_BANK_ABILITY_NAMES_UPPERCASE,
    EASY_CHAT_WORD_TEXT_BANK_TRAINER_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_PEOPLE_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_GREETINGS,
    EASY_CHAT_WORD_TEXT_BANK_LIFESTYLE_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_FEELINGS,
    EASY_CHAT_WORD_TEXT_BANK_TOUGH_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_UNION_WORDS,
    EASY_CHAT_WORD_TEXT_BANK_COUNT,
};
typedef struct UnlockedEasyChatWords {
    u32 unlockedGreetingBits;
    u32 unlockedToughWordBits;
} UnlockedEasyChatWords;
struct EasyChatWordLoader {
    enum HeapID heapID;
    MsgData *loaders[EASY_CHAT_WORD_TEXT_BANK_COUNT];
};
EasyChatManager *EasyChatManager_New(enum HeapID heapID);
typedef struct EasyChatWordList {
    const EasyChatArgs *easyChatArgs;
    EasyChatManager *wordLoader;
    u32 groupWordCounts[EASY_CHAT_GROUP_COUNT];
    u32 groupStartIndices[EASY_CHAT_GROUP_COUNT];
    u16 wordsByGroup[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u32 letterWordCounts[ABC_MODE_CHAR_COUNT];
    u32 letterStartIndices[ABC_MODE_CHAR_COUNT];
    u16 wordsByLetter[((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23)];
    u8 addedWordBits[((((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23) >> 3) + 1)];
} EasyChatWordList;
void sub_020918D4(EasyChatWordList *wordList);
void sub_02091B44(EasyChatWordList *wordList);
void EasyChatManager_Delete(EasyChatManager *wordLoader);

EasyChatWordList *sub_02091880(enum HeapID heapID, const EasyChatArgs *easyChatArgs);
void sub_020918C0(EasyChatWordList *wordList);

EasyChatWordList *sub_02091880(enum HeapID heapID, const EasyChatArgs *easyChatArgs) {
    EasyChatWordList *wordList = Heap_Alloc(heapID, sizeof(EasyChatWordList));
    wordList->easyChatArgs = easyChatArgs;
    wordList->wordLoader = EasyChatManager_New(heapID);

    for (int i = 0; i < ((((((((((((496 + 468) + 18) + 124) + 38) + 38) + 107) + 104) + 47) + 32) + 23) >> 3) + 1); i++) {
        wordList->addedWordBits[i] = 0;
    }

    sub_020918D4(wordList);
    sub_02091B44(wordList);

    return wordList;
}

void sub_020918C0(EasyChatWordList *wordList) {
    if (wordList) {
        EasyChatManager_Delete(wordList->wordLoader);
        Heap_Free(wordList);
    }
}
