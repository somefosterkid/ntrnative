// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_string.c, ported by tools/port_file.py.
// The battle's message strings: loads a message, fills its word set placeholders with the names and numbers its
// arguments give, and formats it, for btlv_core.c and btlv_scu.c. The name is a guess from Black and White's
// BTL_STR module, which does the same.

#include "types.h"
#include "constants/arc.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_setup.h"
#include "battle/btl_string.h"
#include "battle/trainer_data.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "save/player_info.h"
#include "stdarg.h"
#include "system/printsys.h"
#include "system/wordset.h"

typedef void (*BtlStringFunc)(StrBuf *strbuf, u16 message, const u32 *args);

// A standard message that has its own way to pick its variant
// Black 2's, not checked against Black
typedef struct BtlStringHandler {
    u16 message;
    BtlStringFunc func;
} BtlStringHandler;

// Black 2's, not checked against Black
typedef struct BtlStringWork {
    BtlMainModule *mainModule;
    const BtlPokeCon *pokeCon;
    WordSet *wordSet;
    StrBuf *strbuf;
    MsgData *msgData[8];
    u32 args[8];
    HeapID heapId;
    u32 clientId;
    u16 unk68;
    u16 trueNames;
} BtlStringWork;

static void func_ov093_021eed00(u8 monId, u32 index);
static void func_ov093_021eed24(u8 monId, u32 index);
static void func_ov093_021eed48(u8 monId, u8 index);
void func_ov093_021eed6c(WordSet *wordSet, u8 index, u8 clientId);
void func_ov093_021eed90(WordSet *wordSet, u8 index, u8 clientId);
static void func_ov093_021eeec8(StrBuf *strbuf, u16 message, const u32 *args);
static const u16 *func_ov093_021eef20(const u16 *str, u32 *category, u16 *index, u8 *param);
static u8 func_ov093_021eef98(const StrBuf *strbuf);
static void func_ov093_021eefe0(StrBuf *strbuf, const u32 *args, WordSet *wordSet);
static void func_ov093_021ef1cc(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef204(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef280(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef3a4(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef460(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef508(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef594(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef6b0(StrBuf *strbuf, u16 message, const u32 *args);
static void func_ov093_021ef6f0(StrBuf *strbuf, u16 message, const u32 *args);

// The message files, in the order of BtlStringWork's msgData
// pokebw2's sMsgFiles
static const u16 data_ov093_021f3990[] = { 15, 14, 13, 16, 182, 204, 233, 184 };

// The message files of the studio's movies
static const u32 sMovieMsgFiles[] = { 0x137, 0x138, 0x136, 0x135, 0x134 };

// The standard messages that have a variant for the other side, at the next ID
// pokebw2's sSideMessages
static const u16 data_ov093_021f39a0[] = {
    0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xb0, 0x84, 0x86, 0x80, 0x82, 0x7c,
    0x7e, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae,
};

// pokebw2's sHandlers
static const BtlStringHandler data_ov093_021f39d8[] = {
    { 0x1b, func_ov093_021ef460 },
    { 0x5a, func_ov093_021ef460 },
    { 0x99, func_ov093_021ef508 },
    { 0xae, func_ov093_021ef508 },
    { 0x3aa, func_ov093_021ef3a4 },
    { 0x21, func_ov093_021ef594 },
    { 0x414, func_ov093_021ef204 },
    { 0x417, func_ov093_021ef204 },
    { 0x14d, func_ov093_021ef204 },
    { 0x471, func_ov093_021ef204 },
    { 0x9, func_ov093_021ef204 },
    { 0xc, func_ov093_021ef204 },
    { 0x12, func_ov093_021ef204 },
    { 0x15, func_ov093_021ef204 },
};

// pokebw2's sWork
static BtlStringWork data_ov093_021f6398;

// Which variant of a message names a Pokémon: 0 for the player's side, 1 for a wild Pokémon, 2 for a foe's
static inline int GetMonVariant(u8 monId) {
    if (data_ov093_021f6398.unk68 == 0
        && func_ov093_021b8ce8(data_ov093_021f6398.mainModule, data_ov093_021f6398.clientId, func_ov093_021b8ddc(monId))) {
        if (BtlSetup_GetBattleType(data_ov093_021f6398.mainModule) == 0 || BtlSetup_GetBattleType(data_ov093_021f6398.mainModule) == 4) {
            return 1;
        }
        return 2;
    }
    return 0;
}

// The message file of a studio movie's script, one for each of the player's sexes
static inline u16 GetMovieScriptMsgFile(u8 movie, u8 male) {
    return 0xb5 + movie * 2 + male;
}


void func_ov093_021eec78(BtlMainModule *mainModule, u8 clientId, const BtlPokeCon *pokeCon, HeapID heapId) {
    int i;
    u16 fileId;

    data_ov093_021f6398.mainModule = mainModule;
    data_ov093_021f6398.pokeCon = pokeCon;
    data_ov093_021f6398.heapId = heapId;
    data_ov093_021f6398.clientId = clientId;
    data_ov093_021f6398.unk68 = BtlSetup_IsBattleType(mainModule, 0x200);
    data_ov093_021f6398.trueNames = FALSE;
    data_ov093_021f6398.wordSet = GFL_WordSetSystemCreateDefault(heapId);
    data_ov093_021f6398.strbuf = GFL_StrBufCreate(0x100, heapId);
    for (i = 0; i < 8; i++) {
        fileId = data_ov093_021f3990[i];
        data_ov093_021f6398.msgData[i] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, fileId, heapId);
    }
}

void func_ov093_021eecd8(void) {
    int i;

    GFL_WordSetSystemFree(data_ov093_021f6398.wordSet);
    GFL_StrBufFree(data_ov093_021f6398.strbuf);
    for (i = 0; i < 8; i++) {
        GFL_MsgDataFree(data_ov093_021f6398.msgData[i]);
    }
}

// A Pokémon's nickname as the battle shows it, the Illusion's when it has one
static void func_ov093_021eed00(u8 monId, u32 index) {
    loadPokemonNicknameToStrbuf(data_ov093_021f6398.wordSet, index,
                                func_ov093_021d5864(GetPokeParamConst(data_ov093_021f6398.pokeCon, monId)));
}

// A Pokémon's own nickname
static void func_ov093_021eed24(u8 monId, u32 index) {
    loadPokemonNicknameToStrbuf(data_ov093_021f6398.wordSet, index, GetSrcData(GetPokeParamConst(data_ov093_021f6398.pokeCon, monId)));
}

static void func_ov093_021eed48(u8 monId, u8 index) {
    setPartyPokemonSpeciesNameToStrbuf(data_ov093_021f6398.wordSet, index,
                                       func_ov093_021d5864(GetPokeParamConst(data_ov093_021f6398.pokeCon, monId)));
}

extern u32 func_ov093_021ba020(BtlMainModule *mainModule, u8 clientId);
void func_ov093_021eed6c(WordSet *wordSet, u8 index, u8 clientId) {
    u32 trainerClass = func_ov093_021ba020(data_ov093_021f6398.mainModule, clientId);
    loadTrainerTypeText(wordSet, index, (u8)trainerClass);
}

static u8 func_0202a3cc(u32 trainerClass);
void func_ov093_021eed90(WordSet *wordSet, u8 index, u8 clientId) {
    u32 trainerClass;
    u32 trainerId;

    if (clientId != 4) {
        if (func_ov093_021b9f70(data_ov093_021f6398.mainModule, clientId)) {
            if (BtlSetup_GetBattleType(data_ov093_021f6398.mainModule) == 1) {
                trainerId = func_ov093_021ba004(data_ov093_021f6398.mainModule, clientId);
                if (trainerId) {
                    loadTrainerNamesToStrbuf(wordSet, index, trainerId);
                }
            } else {
                StrBuf *name = func_ov093_021b9fac(data_ov093_021f6398.mainModule, clientId, &trainerClass);
                func_0201ee0c(wordSet, index, name, func_0202a3cc(trainerClass), 1, 2);
            }
        } else {
            copyVarForText(wordSet, index, func_ov093_021ba064(data_ov093_021f6398.mainModule, clientId));
        }
    } else {
        copyVarForText(wordSet, index, func_ov093_021ba080(data_ov093_021f6398.mainModule));
    }
}



// Black's layout: msgData has 8 entries, args has 8
void func_ov093_021eee24(StrBuf *strbuf, u16 message, u32 count, ...) {
    va_list list;
    u32 i;

    va_start(list, count);
    for (i = 0; i < count; i++) {
        data_ov093_021f6398.args[i] = va_arg(list, u32);
    }
    while (i < NELEMS(data_ov093_021f6398.args)) {
        data_ov093_021f6398.args[i++] = 0;
    }
    va_end(list);
    func_ov093_021eee80(strbuf, message, data_ov093_021f6398.args);
}

void func_ov093_021eee80(StrBuf *strbuf, u16 message, const u32 *args) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov093_021f39a0); i++) {
        if (message == data_ov093_021f39a0[i]) {
            if (!func_ov093_021b8bd0(data_ov093_021f6398.mainModule, args[0])) {
                message++;
            }
            break;
        }
    }
    func_ov093_021eeec8(strbuf, message, args);
}

static void func_ov093_021eeec8(StrBuf *strbuf, u16 message, const u32 *args) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[0], message, data_ov093_021f6398.strbuf);
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021eeef4(StrBuf *strbuf, u16 message, const u32 *args) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov093_021f39d8); i++) {
        if (message == data_ov093_021f39d8[i].message) {
            data_ov093_021f39d8[i].func(strbuf, message, args);
            return;
        }
    }
    func_ov093_021ef1cc(strbuf, message, args);
}

// Finds the next word set command in a string, and returns the string after it, or NULL at its end
static const u16 *func_ov093_021eef20(const u16 *str, u32 *category, u16 *index, u8 *param) {
    if (str != NULL) {
        u16 eom = GFL_StrBufGetTerminator();
        u16 tag = GFL_StrCmdGetIdentChar();

        while (*str != eom) {
            if (*str == tag) {
                if (GFL_StrCmdIsWordSet(str)) {
                    *param = GFL_WordSetGetCommandParameter(str, 0);
                    *category = GFL_StrCmdGetCommandCategory(str);
                    *index = GFL_StrCmdGetCommandIndex(str);
                    return GFL_StrCmdSkipCommand(str);
                }
                str = GFL_StrCmdSkipCommand(str);
            } else {
                str++;
            }
        }
        return NULL;
    }
    return NULL;
}

// Counts the Pokémon names in a message
static u8 func_ov093_021eef98(const StrBuf *strbuf) {
    u8 param;
    u16 index;
    u32 category;
    u8 count;
    const u16 *str;

    str = GFL_StrBufGetStringPtr(strbuf);
    count = 0;
    while (str != NULL) {
        str = func_ov093_021eef20(str, &category, &index, &param);
        if (str != NULL && category == 1) {
            switch (index) {
            case 1:
            case 2:
            case 12:
                count++;
                break;
            }
        }
    }
    return count;
}

// Fills the word set from the arguments, by the commands in the string
static void func_ov093_021eefe0(StrBuf *strbuf, const u32 *args, WordSet *wordSet) {
    u16 eom;
    u16 tag;
    u8 skip[8];
    u32 i;
    const u16 *str;

    eom = GFL_StrBufGetTerminator();
    tag = GFL_StrCmdGetIdentChar();
    for (i = 0; i < 8; i++) {
        skip[i] = 0;
    }

    // A trainer name command takes no argument of its own, so the words after it take earlier arguments
    str = GFL_StrBufGetStringPtr(strbuf);
    while (*str != eom) {
        if (*str == tag) {
            if (GFL_StrCmdIsWordSet(str)) {
                u16 param = GFL_WordSetGetCommandParameter(str, 0);
                u8 category = GFL_StrCmdGetCommandCategory(str);
                if (category == 1 && GFL_StrCmdGetCommandIndex(str) == 14) {
                    for (i = param + 1; i < 8; i++) {
                        skip[i]++;
                    }
                }
            }
            str = GFL_StrCmdSkipCommand(str);
        } else {
            str++;
        }
    }

    str = GFL_StrBufGetStringPtr(strbuf);
    while (*str != eom) {
        if (*str == tag) {
            if (GFL_StrCmdIsWordSet(str)) {
                u32 param = GFL_WordSetGetCommandParameter(str, 0);
                u32 arg = param;
                u8 category = GFL_StrCmdGetCommandCategory(str);

                if (param >= skip[param]) {
                    arg = param - skip[param];
                }
                if (category == 2) {
                    // The command's index is the number's digits less one
                    u8 digits = GFL_StrCmdGetCommandIndex(str) + 1;
                    WordSetNumber(wordSet, param, args[arg], digits, 0, 1);
                } else {
                    switch (GFL_StrCmdGetCommandIndex(str)) {
                    case 14:
                        func_ov093_021eed6c(wordSet, param, args[arg]);
                        break;
                    case 0:
                        func_ov093_021eed90(wordSet, param, args[arg]);
                        break;
                    case 2:
                        if (data_ov093_021f6398.trueNames == FALSE) {
                            func_ov093_021eed00(args[arg], param);
                        } else {
                            func_ov093_021eed24(args[arg], param);
                        }
                        break;
                    case 12:
                        func_ov093_021eed24(args[arg], param);
                        break;
                    case 1:
                        func_ov093_021eed48(args[arg], param);
                        break;
                    case 3:
                        loadTypeTextToStrbuf(wordSet, param, args[arg]);
                        break;
                    case 6:
                        loadAbilityNameToStrbuf(wordSet, param, args[arg]);
                        break;
                    case 7:
                        loadMoveNameToStrbuf(wordSet, param, args[arg]);
                        break;
                    case 9:
                        loadItemNameToStrbuf(wordSet, param, args[arg]);
                        break;
                    case 15:
                        loadTypeTextToStrbuf(wordSet, param, args[arg]);
                        break;
                    }
                }
            }
            str = GFL_StrCmdSkipCommand(str);
        } else {
            str++;
        }
    }
}

static void func_ov093_021ef1cc(StrBuf *strbuf, u16 message, const u32 *args) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], message, data_ov093_021f6398.strbuf);
    if (func_ov093_021eef98(data_ov093_021f6398.strbuf) == 2) {
        func_ov093_021ef280(strbuf, message, args);
    } else {
        func_ov093_021ef204(strbuf, message, args);
    }
}

// A message about one Pokémon, args[0]
static void func_ov093_021ef204(StrBuf *strbuf, u16 message, const u32 *args) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], (u16)(message + GetMonVariant(args[0])), data_ov093_021f6398.strbuf);
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

// A message about two Pokémon, args[0] and args[1], in seven variants: three for the player's Pokémon by the other's
// variant, and two each for a wild or a foe's Pokémon, by whether the other is of the same kind
static void func_ov093_021ef280(StrBuf *strbuf, u16 message, const u32 *args) {
    u8 target = args[1];
    int variant = GetMonVariant(args[0]);

    switch (variant) {
    case 0:
        message = message + GetMonVariant(target);
        // Only this layout matches: the original's case 0 jumps past the test of the other cases
        goto load;
    case 1:
        message += 3;
        break;
    case 2:
        message += 5;
        break;
    }
    if (variant == GetMonVariant(target)) {
        message += 1;
    }
load:
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], message, data_ov093_021f6398.strbuf);
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

// An item raised a stat: args are the Pokémon, the item, the stat and how many stages
static void func_ov093_021ef3a4(StrBuf *strbuf, u16 message, const u32 *args) {
    u8 stat = args[2] - 1;
    int degree = 0;
    int variant;
    int base;

    if ((int)args[3] >= 3) {
        degree = 2;
    }
    if (args[3] == 2) {
        degree = 1;
    }
    message += degree * 21;
    variant = GetMonVariant(args[0]);
    func_ov093_021eed00(args[0], 0);
    loadItemNameToStrbuf(data_ov093_021f6398.wordSet, 1, args[1]);
    // The stat's message, before the variant is added: only a variable of its own puts the variant second in the add
    base = message + stat * 3;
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], (u16)(base + variant), data_ov093_021f6398.strbuf);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

// A stat rose or fell: args are the Pokémon, the stat and how many stages
static void func_ov093_021ef460(StrBuf *strbuf, u16 message, const u32 *args) {
    u8 stat = args[1] - 1;
    int degree = 0;

    if ((int)args[2] >= 3) {
        degree = 2;
    }
    if (args[2] == 2) {
        degree = 1;
    }
    message += degree * 21;
    func_ov093_021eed00(args[0], 0);
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], (u16)(message + stat * 3 + GetMonVariant(args[0])), data_ov093_021f6398.strbuf);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

// A stat can't go any higher or lower: args are the Pokémon and the stat
static void func_ov093_021ef508(StrBuf *strbuf, u16 message, const u32 *args) {
    u8 stat = args[1] - 1;

    func_ov093_021eed00(args[0], 0);
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[1], (u16)(message + stat * 3 + GetMonVariant(args[0])), data_ov093_021f6398.strbuf);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

// Picks one of three messages by whose client args[0] is
static void func_ov093_021ef594(StrBuf *strbuf, u16 message, const u32 *args) {
    u8 clientId = args[0];

    if (clientId != GetPlayerClientID(data_ov093_021f6398.mainModule)) {
        if (func_ov093_021b9f70(data_ov093_021f6398.mainModule, clientId)) {
            message = 0x23;
        } else {
            message = 0x22;
        }
    } else {
        message = 0x21;
    }
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[0], message, data_ov093_021f6398.strbuf);
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021ef5e8(StrBuf *strbuf, u8 monId, u16 move) {
    int variant = GetMonVariant(monId);

    func_ov093_021eed00(monId, 0);
    move *= 3;
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[2], (u16)(move + variant), data_ov093_021f6398.strbuf);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021ef664(StrBuf *strbuf, u16 arg1, u32 arg2) {
    if (arg1 != 0) {
        u16 message = (arg1 - 1) * 3;
        if (arg2 == 0) {
            message += 1;
        } else {
            message += 2;
        }
        GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[7], message, strbuf);
    }
}


void func_ov093_021ef694(StrBuf *strbuf, u16 message) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[3], message, strbuf);
}

void func_ov093_021ef6a8(StrBuf *strbuf, u16 message, const u32 *args) {
    func_ov093_021ef6b0(strbuf, message, args);
}

static void func_ov093_021ef6b0(StrBuf *strbuf, u16 message, const u32 *args) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[5], message, data_ov093_021f6398.strbuf);
    data_ov093_021f6398.trueNames = TRUE;
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    data_ov093_021f6398.trueNames = FALSE;
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021ef6e8(StrBuf *strbuf, u16 message, const u32 *args) {
    func_ov093_021ef6f0(strbuf, message, args);
}

static void func_ov093_021ef6f0(StrBuf *strbuf, u16 message, const u32 *args) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[6], message, data_ov093_021f6398.strbuf);
    func_ov093_021eefe0(data_ov093_021f6398.strbuf, args, data_ov093_021f6398.wordSet);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021ef71c(StrBuf *strbuf, int hp, int attack, int defense, int spAttack, int spDefense, int speed) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[3], 0x10, data_ov093_021f6398.strbuf);
    WordSetNumber(data_ov093_021f6398.wordSet, 0, hp, 2, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 1, attack, 2, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 2, defense, 2, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 3, spAttack, 2, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 4, spDefense, 2, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 5, speed, 2, 0, 1);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

void func_ov093_021ef7ac(StrBuf *strbuf, int hp, int attack, int defense, int spAttack, int spDefense, int speed) {
    GFL_MsgDataLoadStrbuf(data_ov093_021f6398.msgData[3], 0x11, data_ov093_021f6398.strbuf);
    WordSetNumber(data_ov093_021f6398.wordSet, 0, hp, 3, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 1, attack, 3, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 2, defense, 3, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 3, spAttack, 3, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 4, spDefense, 3, 0, 1);
    WordSetNumber(data_ov093_021f6398.wordSet, 5, speed, 3, 0, 1);
    GFL_WordSetFormatStrbuf(data_ov093_021f6398.wordSet, strbuf, data_ov093_021f6398.strbuf);
}

