#include "field/union_msg.h"
#include "types.h"
#include "constants/language.h"
#include "constants/text_banks.h"
#include "field/field.h"
#include "field/field_msgbg.h"
#include "field/union_oneself.h"
#include "field/unity_tower.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/player_info.h"
#include "system/bmp_menulist.h"
#include "system/game_data.h"
#include "system/pms_word.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/union_msg.h"

// The Union Room's messages and menus: the lookups of message IDs, the message system the field talks through (the
// message data, the word set, the stream window), the menus of the battle rules and the regulation's rules printed.
// Names are not swan's

#define UNION_HEAPID 0x41

typedef struct {
    const ListMenuRow *rows;
    u8 count;
} UnionMenuInfo;

static void UnionMsg_SetupWordSet(UnionSystem *unisys);
static void UnionMsg_SetLanguageFlag(GameData *gameData, u32 language);
static void UnionMsg_CreateWordSet(UnionSystem *unisys);
static void UnionMsg_FreeWordSet(UnionSystem *unisys);
static void UnionMsg_FreeMsgSystem(UnionSystem *unisys);
static void UnionMsg_CreateMenu(UnionSystem *unisys, Field *field, const ListMenuRow *rows, u32 count,
                                ListMenuRequest *request);
static void UnionMsg_FreeMenu(UnionSystem *unisys);
static s32 UnionMsg_UpdateMenu(UnionSystem *unisys, Field *field);

// The rows of the summaries that end the menu: confirm the rules, or show them
#define UNION_RULE_MENU_CONFIRM 0x64
#define UNION_RULE_MENU_SHOW_RULES 0x96

// The menus of the battle rules, each row a message and a value or the menu it opens
static const ListMenuRow sMenuRules[3];
static const ListMenuRow sMenuLauncher[3];
static const ListMenuRow sMenuPlayers[3];
static const ListMenuRow sMenuFourRules[3];
static const ListMenuRow sMenuFourLauncher[3];
static const ListMenuRow sMenuFormat[5];
static const ListMenuRow sMenuFour[5];
static const ListMenuRow sMenuTwo[6];

// The tables are declared in the order that gives the ROM's .rodata: MWCC heapsorts the objects of equal size by where
// they are declared. The pairs are the messages of a gender, or of a language, by the lookups above
static const u16 sMsgPair_0214[2] = { UnionMsg_Text_092, UnionMsg_Text_093 };

static const u16 sMsgPair_0210[2] = { UnionMsg_Text_174, UnionMsg_Text_175 };

static const u16 sMsgPair_0208[2] = { UnionMsg_Text_126, UnionMsg_Text_127 };

static const u16 sMsgPair_01f0[2] = { UnionMsg_Text_162, UnionMsg_Text_163 };

static const u16 sMsgPair_0218[2] = { UnionMsg_Text_128, UnionMsg_Text_129 };

static const u16 sMsgPair_0220[2] = { UnionMsg_Text_172, UnionMsg_Text_173 };

static const u16 sMsgPair_021c[2] = { UnionMsg_Text_100, UnionMsg_Text_101 };

static const u16 sMsgPair_0228[2] = { UnionMsg_Text_072, UnionMsg_Text_073 };

static const u16 sMsgPair_01f4[2] = { UnionMsg_Text_102, UnionMsg_Text_103 };

static const u16 sMsgPair_01e0[2] = { UnionMsg_Text_076, UnionMsg_Text_077 };

static const u16 sMsgPair_0224[2] = { UnionMsg_Text_054, UnionMsg_Text_055 };

static const u16 sMsgPair_0204[2] = { UnionMsg_Text_056, UnionMsg_Text_057 };

static const u16 sMsgPair_01f8[2] = { UnionMsg_Text_160, UnionMsg_Text_161 };

static const u16 sMsgPair_0230[2] = { UnionMsg_Text_050, UnionMsg_Text_051 };

static const u16 sMsgPair_020c[2] = { UnionMsg_Text_172, UnionMsg_Text_173 };

static const u16 sMsgPair_01ec[2] = { UnionMsg_Text_098, UnionMsg_Text_099 };

static const u16 sMsgPair_0200[2] = { UnionMsg_Text_158, UnionMsg_Text_159 };

static const u16 sMsgPair_01e8[2] = { UnionMsg_Text_052, UnionMsg_Text_053 };

static const u16 sMsgPair_022c[2] = { UnionMsg_Text_074, UnionMsg_Text_075 };

static const u16 sMsgPair_01fc[2] = { UnionMsg_Text_096, UnionMsg_Text_097 };

static const u16 sMsgPair_01e4[2] = { UnionMsg_Text_104, UnionMsg_Text_105 };

static const u16 sMsgPair_01d8[2] = { UnionMsg_Text_078, UnionMsg_Text_079 };

static const u16 sMsgPair_01dc[2] = { UnionMsg_Text_080, UnionMsg_Text_081 };

static const u16 sMsgPair_0244[2] = { UnionMsg_Text_164, UnionMsg_Text_165 };

static const u16 sMsgPair_0240[2] = { UnionMsg_Text_166, UnionMsg_Text_167 };

static const u16 sMsgPair_023c[2] = { UnionMsg_Text_170, UnionMsg_Text_171 };

static const u16 sMsgPair_0238[2] = { UnionMsg_Text_168, UnionMsg_Text_169 };

static const u16 sMsgPair_0234[2] = { UnionMsg_Text_048, UnionMsg_Text_049 };

static const u16 sMsgPairs_0248[2][2] = {
    { UnionMsg_Text_122, UnionMsg_Text_123 },
    { UnionMsg_Text_124, UnionMsg_Text_125 },
};

static const u16 sMsgPairs_0250[2][2] = {
    { UnionMsg_Text_062, UnionMsg_Text_063 },
    { UnionMsg_Text_064, UnionMsg_Text_065 },
};

static const u16 sMsgPairs_0258[4][2] = {
    { UnionMsg_Text_082, UnionMsg_Text_083 },
    { UnionMsg_Text_084, UnionMsg_Text_085 },
    { UnionMsg_Text_086, UnionMsg_Text_087 },
    { UnionMsg_Text_088, UnionMsg_Text_089 },
};

static const u16 sMsgPairs_0268[4][2] = {
    { UnionMsg_Text_108, UnionMsg_Text_109 },
    { UnionMsg_Text_110, UnionMsg_Text_111 },
    { UnionMsg_Text_112, UnionMsg_Text_113 },
    { UnionMsg_Text_114, UnionMsg_Text_115 },
};

static const u16 sMsgPairs_0278[4][2] = {
    { UnionMsg_Text_176, UnionMsg_Text_177 },
    { UnionMsg_Text_178, UnionMsg_Text_179 },
    { UnionMsg_Text_180, UnionMsg_Text_181 },
    { UnionMsg_Text_182, UnionMsg_Text_183 },
};

static const u16 sMsgPairs_04cc[25][2] = {
    { UnionMsg_Text_058, UnionMsg_Text_059 },
    { UnionMsg_Text_066, UnionMsg_Text_067 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_090, UnionMsg_Text_091 },
    { UnionMsg_Text_116, UnionMsg_Text_117 },
    { UnionMsg_Text_152, UnionMsg_Text_153 },
    { UnionMsg_Text_152, UnionMsg_Text_153 },
};

static const u16 sMsgPairs_0530[25][2] = {
    { UnionMsg_Text_060, UnionMsg_Text_061 },
    { UnionMsg_Text_070, UnionMsg_Text_071 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_094, UnionMsg_Text_095 },
    { UnionMsg_Text_120, UnionMsg_Text_121 },
    { UnionMsg_Text_156, UnionMsg_Text_157 },
    { UnionMsg_Text_134, UnionMsg_Text_135 },
};

static const u16 sTable_0424[26] = {
#include "asset/union_msg_sTable_0424.inc"
};

static const u16 sTable_0458[26] = {
#include "asset/union_msg_sTable_0458.inc"
};

static const ListMenuRow sMenuRules[3] = {
    { UnionMsg_Text_250, { 0x0 } },
    { UnionMsg_Text_251, { 0x1 } },
    { UnionMsg_Text_254, { .sub = sMenuTwo } },
};

static const ListMenuRow sMenuLauncher[3] = {
    { UnionMsg_Text_252, { 0x0 } },
    { UnionMsg_Text_253, { 0x1 } },
    { UnionMsg_Text_254, { .sub = sMenuTwo } },
};

static const ListMenuRow sMenuPlayers[3] = {
    { UnionMsg_Text_239, { .sub = sMenuTwo } },
    { UnionMsg_Text_240, { .sub = sMenuFour } },
    { UnionMsg_Text_241, { BMPMENULIST_CANCEL } },
};

static const ListMenuRow sMenuFourRules[3] = {
    { UnionMsg_Text_250, { 0x0 } },
    { UnionMsg_Text_251, { 0x1 } },
    { UnionMsg_Text_254, { .sub = sMenuFour } },
};

static const ListMenuRow sMenuFourLauncher[3] = {
    { UnionMsg_Text_252, { 0x0 } },
    { UnionMsg_Text_253, { 0x1 } },
    { UnionMsg_Text_254, { .sub = sMenuFour } },
};

// clang-format off
static const ListMenuRow sMenuFormat[5] = {
    { UnionMsg_Text_242, { 0x0 } },
    { UnionMsg_Text_243, { 0x1 } },
    { UnionMsg_Text_244, { 0x2 } },
    { UnionMsg_Text_245, { 0x3 } },
    { UnionMsg_Text_248, { .sub = sMenuTwo } },
};

// clang-format on

static const ListMenuRow sMenuFour[5] = {
    { UnionMsg_Text_243, { .sub = sMenuFourRules } },
    { UnionMsg_Text_244, { .sub = sMenuFourLauncher } },
    { UnionMsg_Text_246, { UNION_RULE_MENU_CONFIRM } },
    { UnionMsg_Text_247, { UNION_RULE_MENU_SHOW_RULES } },
    { UnionMsg_Text_248, { .sub = sMenuPlayers } },
};

static const ListMenuRow sMenuTwo[6] = {
    { UnionMsg_Text_242, { .sub = sMenuFormat } },   { UnionMsg_Text_243, { .sub = sMenuRules } },
    { UnionMsg_Text_244, { .sub = sMenuLauncher } }, { UnionMsg_Text_246, { UNION_RULE_MENU_CONFIRM } },
    { UnionMsg_Text_247, { UNION_RULE_MENU_SHOW_RULES } },  { UnionMsg_Text_248, { .sub = sMenuPlayers } },
};

static const UnionMenuInfo sMenuInfo[8] = {
    { sMenuPlayers, 3 },  { sMenuTwo, 6 },  { sMenuFormat, 5 },    { sMenuRules, 3 },
    { sMenuLauncher, 3 }, { sMenuFour, 5 }, { sMenuFourRules, 3 }, { sMenuFourLauncher, 3 },
};

// The party and the rules a menu's choices lead to, by the battle format and the two options
static const u8 sRuleResult[5][2][2] = {
    { { UNION_ACTIVITY_SINGLE, UNION_ACTIVITY_SINGLE_NO_LAUNCHER },
      { UNION_ACTIVITY_SINGLE_FLAT, UNION_ACTIVITY_SINGLE_FLAT_NO_LAUNCHER } },
    { { UNION_ACTIVITY_DOUBLE, UNION_ACTIVITY_DOUBLE_NO_LAUNCHER },
      { UNION_ACTIVITY_DOUBLE_FLAT, UNION_ACTIVITY_DOUBLE_FLAT_NO_LAUNCHER } },
    { { UNION_ACTIVITY_TRIPLE, UNION_ACTIVITY_TRIPLE_NO_LAUNCHER },
      { UNION_ACTIVITY_TRIPLE_FLAT, UNION_ACTIVITY_TRIPLE_FLAT_NO_LAUNCHER } },
    { { UNION_ACTIVITY_ROTATION, UNION_ACTIVITY_ROTATION_NO_LAUNCHER },
      { UNION_ACTIVITY_ROTATION_FLAT, UNION_ACTIVITY_ROTATION_FLAT_NO_LAUNCHER } },
    { { UNION_ACTIVITY_MULTI, UNION_ACTIVITY_MULTI_NO_LAUNCHER },
      { UNION_ACTIVITY_MULTI_FLAT, UNION_ACTIVITY_MULTI_FLAT_NO_LAUNCHER } },
};

static const ListMenuRow sMenuMain[6] = {
    { UnionMsg_Text_000, { UNION_ACTIVITY_CARD } },      { UnionMsg_Text_002, { UNION_ACTIVITY_NONE } },
    { UnionMsg_Text_003, { UNION_ACTIVITY_TRADE } },     { UnionMsg_Text_001, { UNION_ACTIVITY_DRAW } },
    { UnionMsg_Text_005, { UNION_ACTIVITY_SPIN_TRADE } }, { UnionMsg_Text_004, { 0 } },
};

static const u32 sLanguageFlags[8] = { 0, 1, 2, 3, 4, 7, 5, 6 };

static const ListMenuRequest sMenuMainRequest = { 6, 6, 0, 0x0d, 0,    0, 1, 0xf,  2,    0,
                                                  0, 0, 1, 0x0c, 0x10, 1, 1, 0x0d, 0x0c, NULL };

static const ListMenuRequest sMenuRulesRequest = { 3, 3, 0, 0x0d, 0,    0, 1, 0xf,  2,    0,
                                                   0, 0, 1, 0x0c, 0x10, 1, 1, 0x13, 0x0e, NULL };

static const struct {
    u8 count;
    u8 unk01[3];
} sActivityPlayerCounts[27] = {
    { 2 }, { 2 }, { 2 }, { 5 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 },
    { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 4 }, { 4 }, { 4 }, { 4 }, { 2 }, { 5 }, { 5 },
};

u16 UnionMsg_GetNoReplyMessage(UnionSystem *unisys) {
    return sMsgPair_0204[getTrainerGender(unisys->param->playerInfo)];
}

u16 UnionMsg_GetContactGreetingMessage(u8 gender) {
    return sMsgPair_0234[gender];
}

u16 UnionMsg_GetJustAskMessage(u8 gender) {
    return sMsgPair_0218[gender];
}

u16 UnionMsg_GetBusyMessage(u8 gender) {
    return sMsgPair_0230[gender];
}

u16 UnionMsg_GetShowCardMessage(unsigned int row, u8 gender) {
    return sMsgPairs_04cc[row - 2][gender];
}

u16 UnionMsg_GetCardMissingMessage(unsigned int row, u8 gender) {
    return sMsgPairs_0530[row - 2][gender];
}

u16 UnionMsg_GetOfferMessage(unsigned int row) {
    return sTable_0424[row - 2];
}

u16 UnionMsg_GetDeclinedMessage(unsigned int row) {
    return sTable_0458[row - 2];
}

u16 UnionMsg_GetWhatToDoMessage(u8 gender) {
    return sMsgPair_01e8[gender];
}

u16 UnionMsg_GetBattleStartMessage(u8 gender) {
    return sMsgPair_0214[gender];
}

u16 UnionMsg_GetOnePokemonCannotTradeMessage(u8 gender) {
    return sMsgPair_0208[gender];
}

u16 UnionMsg_GetNeedEggMessage(u8 gender) {
    return sMsgPair_0200[gender];
}

u16 UnionMsg_GetEggCannotTradeMessage(UnionSystem *unisys) {
    return sMsgPair_0210[getTrainerGender(unisys->param->playerInfo)];
}

u16 UnionMsg_GetEggCannotTradeResultMessage(UnionSystem *unisys) {
    return sMsgPair_020c[getTrainerGender(unisys->param->playerInfo)];
}

u16 UnionMsg_GetBattleChatMessage(u8 gender) {
    return sMsgPairs_0268[GFL_RandomLC(4)][gender];
}

u16 UnionMsg_GetBusyTrainerMessage(u8 gender) {
    return sMsgPair_0224[gender];
}

u16 UnionMsg_GetCardChatMessage(u8 gender) {
    return sMsgPairs_0250[GFL_RandomLC(2)][gender];
}

u16 UnionMsg_GetDrawChatMessage(u8 gender) {
    return sMsgPairs_0258[GFL_RandomLC(4)][gender];
}

u16 UnionMsg_GetTradeChatMessage(u8 gender) {
    return sMsgPairs_0248[GFL_RandomLC(2)][gender];
}

u16 UnionMsg_GetSpinTradeChatMessage(u8 gender) {
    return sMsgPairs_0278[GFL_RandomLC(4)][gender];
}

u16 UnionMsg_GetJoinMultiBattleMessage(u8 gender) {
    return sMsgPair_01fc[gender];
}

u16 UnionMsg_GetJoinSpinTradeMessage(u8 gender) {
    return sMsgPair_01f8[gender];
}

u16 UnionMsg_GetJoinDrawMessage(u8 gender) {
    return sMsgPair_0228[gender];
}

u16 UnionMsg_GetBattleDeclineMessage(u8 gender) {
    return sMsgPair_01ec[gender];
}

u16 UnionMsg_GetEggTradeDeclineMessage(u8 gender) {
    return sMsgPair_01f0[gender];
}

u16 UnionMsg_GetDrawDeclineMessage(u8 gender) {
    return sMsgPair_022c[gender];
}

u16 UnionMsg_GetNoEggMessage(u8 gender) {
    return sMsgPair_0238[gender];
}

u16 UnionMsg_GetTargetEggCannotTradeMessage(u8 gender) {
    return sMsgPair_0220[gender];
}

u16 UnionMsg_GetEggTradeAcceptMessage(u8 gender) {
    return sMsgPair_0244[gender];
}

u16 UnionMsg_GetSpinTradeStartMessage(u8 gender) {
    return sMsgPair_0240[gender];
}

u16 UnionMsg_GetEggTradeBusyMessage(u8 gender) {
    return sMsgPair_023c[gender];
}

u16 UnionMsg_GetDrawAcceptMessage(u8 gender) {
    return sMsgPair_01e0[gender];
}

u16 UnionMsg_GetDrawStartMessage(u8 gender) {
    return sMsgPair_01d8[gender];
}

u16 UnionMsg_GetDrawBusyMessage(u8 gender) {
    return sMsgPair_01dc[gender];
}

u16 UnionMsg_GetBattleAcceptMessage(u8 gender) {
    return sMsgPair_021c[gender];
}

u16 UnionMsg_GetMultiBattleStartMessage(u8 gender) {
    return sMsgPair_01f4[gender];
}

u16 UnionMsg_GetBattleBusyMessage(u8 gender) {
    return sMsgPair_01e4[gender];
}

u32 UnionMsg_GetOriginMessage(UnionSystem *unisys, u8 country, u8 province) {
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(unisys->param->gameData);
    u8 myCountry = UnityTowerVisitor_GetCountry(playerInfo);
    u8 myProvince = UnityTowerVisitor_GetProvince(playerInfo);

    if (country == 0) {
        return UnionMsg_Text_015;
    }
    if (myCountry != country) {
        if (province == 0) {
            return UnionMsg_Text_013;
        }
        return UnionMsg_Text_012;
    }
    if (province == 0 || myProvince == province) {
        return UnionMsg_Text_015;
    }
    return UnionMsg_Text_014;
}

// The words of the message system: the player or the one spoken to, their greeting by language and their origin
static void UnionMsg_SetupWordSet(UnionSystem *unisys) {
    UnionSelf *situ = &unisys->situ;
    PlayerInfo *playerInfo = unisys->param->playerInfo;
    const u16 *name;
    u8 language;
    u8 gender;
    StrBuf *strbuf;
    u32 messageId;
    UnionBeaconEntry *target;

    GFL_ASSERT(unisys->msgdata != NULL);
    GFL_ASSERT(unisys->wordset != NULL);

    target = situ->mycomm.target;
    if (target != NULL) {
        name = target->beacon.name;
        language = target->beacon.language;
        gender = target->beacon.gender;
    } else {
        name = GetPlayerName(playerInfo);
        language = LANGUAGE_ENGLISH;
        gender = getTrainerGender(playerInfo);
    }

    strbuf = GFL_StrBufCreate(0x20, UNION_HEAPID);
    GFL_StrBufLoadString(strbuf, name);
    GFL_WordSetSetStrbuf(unisys->wordset, 0, strbuf, gender, TRUE, language);
    GFL_StrBufFree(strbuf);
    copyVarForText(unisys->wordset, 1, playerInfo);

    switch (language) {
    case LANGUAGE_JAPANESE:
        messageId = UnionMsg_Text_231;
        break;
    case LANGUAGE_ENGLISH:
        messageId = UnionMsg_Text_232;
        break;
    case LANGUAGE_FRENCH:
        messageId = UnionMsg_Text_233;
        break;
    case LANGUAGE_ITALIAN:
        messageId = UnionMsg_Text_234;
        break;
    case LANGUAGE_GERMAN:
        messageId = UnionMsg_Text_235;
        break;
    case LANGUAGE_SPANISH:
        messageId = UnionMsg_Text_236;
        break;
    case LANGUAGE_KOREAN:
        messageId = UnionMsg_Text_238;
        break;
    default:
        messageId = UnionMsg_Text_237;
        break;
    }
    strbuf = GFL_MsgDataLoadStrbufNew(unisys->msgdata, messageId);
    GFL_WordSetSetStrbuf(unisys->wordset, 2, strbuf, gender, TRUE, language);
    GFL_StrBufFree(strbuf);

    if (situ->mycomm.target != NULL && situ->mycomm.target->beacon.country != 0) {
        loadCountryToStrbuf(unisys->wordset, 3, situ->mycomm.target->beacon.country);
        if (situ->mycomm.target->beacon.province != 0) {
            loadCountryAreaToStrbuf(unisys->wordset, 4, situ->mycomm.target->beacon.country,
                                    situ->mycomm.target->beacon.province);
        }
    }
    UnionMsg_SetLanguageFlag(unisys->param->gameData, language);
}

// Marks the language as seen in the player's Pokemon Wonder word list
static void UnionMsg_SetLanguageFlag(GameData *gameData, u32 language) {
    PMSWordSave *save = getDexBlkAddress(GameData_GetSaveControl(gameData));
    u16 index = language - 1;

    if (language >= 1 && language <= 8) {
        if (index < 8) {
            PMSWordSave_SetLanguageFlag(save, sLanguageFlags[index]);
        } else {
            PMSWordSave_SetLanguageFlag(save, 7);
        }
    } else {
        PMSWordSave_SetLanguageFlag(save, 7);
    }
}

static void UnionMsg_CreateWordSet(UnionSystem *unisys) {
    if (unisys->wordset == NULL) {
        unisys->wordset = GFL_WordSetSystemCreate(5, 0x40, UNION_HEAPID);
        UnionMsg_SetupWordSet(unisys);
    }
}

static void UnionMsg_FreeWordSet(UnionSystem *unisys) {
    if (unisys->wordset != NULL) {
        GFL_WordSetSystemFree(unisys->wordset);
        unisys->wordset = NULL;
    }
}

s32 UnionMsg_GetActivityPlayerCount(s32 index) {
    if (index >= 27) {
        return 0;
    }
    return sActivityPlayerCounts[index].count;
}

void UnionMsg_FreeAll(UnionSystem *unisys) {
    UnionMsg_FreeRulesWindow(unisys);
    UnionMsg_FreeInfoWindow(unisys);
    UnionMsg_FreeYesNo(unisys);
    UnionMsg_FreeMenu(unisys);
    UnionMsg_FreeMsgSystem(unisys);
}

void UnionMsg_CreateMsgSystem(UnionSystem *unisys, Field *field) {
    void *fldmsg_bg;

    if (unisys->fld_msgwin_stream == NULL) {
        GFL_ASSERT(unisys->msgdata == NULL);
        fldmsg_bg = Field_GetMsgBGSys(field);
        unisys->msgdata = MsgBGPrinter_CreateMsgData(fldmsg_bg, TEXT_BANK_UNION_MSG);
        unisys->msgbuf = GFL_StrBufCreate(0x100, UNION_HEAPID);
        unisys->fmtbuf = GFL_StrBufCreate(0x100, UNION_HEAPID);
        UnionMsg_CreateWordSet(unisys);
        unisys->fld_msgwin_stream = MsgBGPrinter_CreatePlainWin(fldmsg_bg, unisys->msgdata, 0, 1, 0x13, 0x1e, 4);
    } else {
        MsgBGPrinter_ClearPlainWin(unisys->fld_msgwin_stream);
    }
}

static void UnionMsg_FreeMsgSystem(UnionSystem *unisys) {
    if (unisys->fld_msgwin_stream != NULL) {
        MsgBGPrinter_OpenPlainWin(unisys->fld_msgwin_stream);
        MsgBGPrinter_DeletePlainWin(unisys->fld_msgwin_stream);
        unisys->fld_msgwin_stream = NULL;
        unisys->msgPrinting = 0;
        GFL_StrBufFree(unisys->msgbuf);
        GFL_StrBufFree(unisys->fmtbuf);
        unisys->msgbuf = NULL;
        unisys->fmtbuf = NULL;
    }
    if (unisys->msgdata != NULL) {
        UnionMsg_FreeWordSet(unisys);
        MsgBGPrinter_DeleteMsgData(unisys->msgdata);
        unisys->msgdata = NULL;
    }
}

void UnionMsg_PrintMessage(UnionSystem *unisys, u32 messageId) {
    GFL_ASSERT(unisys->fld_msgwin_stream != NULL);
    GFL_MsgDataLoadStrbuf(unisys->msgdata, messageId, unisys->msgbuf);
    GFL_WordSetFormatStrbuf(unisys->wordset, unisys->fmtbuf, unisys->msgbuf);
    MsgBGPrinter_PlainWinPrintStream(unisys->fld_msgwin_stream, 0, 0, unisys->fmtbuf);
    unisys->msgPrinting = 1;
}

BOOL UnionMsg_IsPrintDone(UnionSystem *unisys) {
    if (unisys->fld_msgwin_stream == NULL || unisys->msgPrinting == 0) {
        return TRUE;
    }
    return MsgBGPrinter_PlainWinMain(unisys->fld_msgwin_stream);
}

void UnionMsg_CreateAndPrint(UnionSystem *unisys, Field *field, u32 messageId) {
    UnionMsg_CreateMsgSystem(unisys, field);
    UnionMsg_SetupWordSet(unisys);
    UnionMsg_PrintMessage(unisys, messageId);
}

void UnionMsg_CreateYesNo(UnionSystem *unisys, Field *field) {
    if (unisys->fldmenu_yesno_func == NULL) {
        unisys->fldmenu_yesno_func = MsgBGPrinter_CreateYesNoMenu(Field_GetMsgBGSys(field), 0);
    }
}

void UnionMsg_FreeYesNo(UnionSystem *unisys) {
    if (unisys->fldmenu_yesno_func != NULL) {
        MsgBGPrinter_DeleteMenuFunc(unisys->fldmenu_yesno_func);
        unisys->fldmenu_yesno_func = NULL;
    }
}

BOOL UnionMsg_UpdateYesNo(UnionSystem *unisys, BOOL *yes) {
    u32 answer;

    GFL_ASSERT(unisys->fldmenu_yesno_func != NULL);
    answer = MsgBGPrinter_YesNoMenuMain(unisys->fldmenu_yesno_func);
    if (answer == MSGBG_YES_NO_WAITING) {
        return FALSE;
    }
    *yes = answer == MSGBG_YES_NO_YES;
    return TRUE;
}

static void UnionMsg_CreateMenu(UnionSystem *unisys, Field *field, const ListMenuRow *rows, u32 count,
                                ListMenuRequest *request) {
    FieldMsgBG *msgBGSys = Field_GetMsgBGSys(field);

    if (unisys->fldmenu_func == NULL) {
        ListMenuOption *options = MsgBGPrinter_CreateMenuItemsFromTable(rows, count, unisys->msgdata, UNION_HEAPID);

        unisys->fldmenu_func = ListMenuUI_Create(msgBGSys, request, options, NULL, NULL, 0, 0, TRUE);
    }
}

static void UnionMsg_FreeMenu(UnionSystem *unisys) {
    if (unisys->fldmenu_func != NULL) {
        MsgBGPrinter_DeleteMenuFunc(unisys->fldmenu_func);
        unisys->fldmenu_func = NULL;
    }
}

static s32 UnionMsg_UpdateMenu(UnionSystem *unisys, Field *field) {
    void *fldmsg_bg = Field_GetMsgBGSys(field);

    GFL_ASSERT(fldmsg_bg != NULL);
    if (fldmsg_bg != NULL && !MsgBGPrinter_IsPrintQueFinished(fldmsg_bg)) {
        return BMPMENULIST_NULL;
    }
    GFL_ASSERT(unisys->fldmenu_func != NULL);
    return ListMenuUI_Update(unisys->fldmenu_func);
}

void UnionMsg_CreateMainMenu(UnionSystem *unisys, Field *field) {
    UnionMsg_CreateMenu(unisys, field, sMenuMain, 6, (ListMenuRequest *)&sMenuMainRequest);
}

void UnionMsg_FreeMainMenu(UnionSystem *unisys) {
    UnionMsg_FreeMenu(unisys);
}

s32 UnionMsg_UpdateMainMenu(UnionSystem *unisys, Field *field) {
    return UnionMsg_UpdateMenu(unisys, field);
}

// Opens the menu of the battle rules: the kind says which of sMenuInfo's, and which choices the rows show
void UnionMsg_CreateRuleMenu(UnionSystem *unisys, Field *field, int kind, u8 *ruleSel) {
    ListMenuRow *rows;
    u8 count;

    ruleSel[3] = kind;
    ListMenuRequest request = sMenuRulesRequest;
    count = sMenuInfo[kind].count;
    request.count = count;
    request.maxShown = count;
    request.height = request.count * 2;
    if (kind == UNION_RULE_MENU_TWO || kind == UNION_RULE_MENU_FOUR) {
        request.width = 30;
    }
    if (kind >= UNION_RULE_MENU_FOUR) {
        request.y = 5;
    }

    if (kind == UNION_RULE_MENU_TWO) {
        rows = GFL_HeapAllocate(UNION_HEAPID, sizeof(ListMenuRow) * 6, TRUE, "union_msg.c", 0x647);
        MI_CpuCopy8(sMenuTwo, rows, sizeof(ListMenuRow) * 6);
        rows[0].messageId = UnionMsg_Text_255 + ruleSel[0];
        rows[1].messageId = UnionMsg_Text_260 + ruleSel[1];
        rows[2].messageId = UnionMsg_Text_262 + ruleSel[2];
        UnionMsg_CreateMenu(unisys, field, rows, count, &request);
        GFL_HeapFree(rows);
    } else if (kind == UNION_RULE_MENU_FOUR) {
        rows = GFL_HeapAllocate(UNION_HEAPID, sizeof(ListMenuRow) * 5, TRUE, "union_msg.c", 0x657);
        MI_CpuCopy8(sMenuFour, rows, sizeof(ListMenuRow) * 5);
        rows[0].messageId = UnionMsg_Text_260 + ruleSel[1];
        rows[1].messageId = UnionMsg_Text_262 + ruleSel[2];
        UnionMsg_CreateMenu(unisys, field, rows, count, &request);
        GFL_HeapFree(rows);
    } else {
        UnionMsg_CreateMenu(unisys, field, sMenuInfo[kind].rows, count, &request);
    }
}

void UnionMsg_FreeRuleMenu(UnionSystem *unisys) {
    UnionMsg_FreeMenu(unisys);
}

// Whether the number is one of sMenuInfo's menus
static inline BOOL UnionMsg_IsMenuKind(u32 kind) {
    if (kind < NELEMS(sMenuInfo)) {
        return TRUE;
    }
    return FALSE;
}

// The entry of sMenuInfo for a menu kind
static inline const UnionMenuInfo *UnionMsg_GetMenuInfo(int kind) {
    return &sMenuInfo[kind];
}

// Runs the menu of the battle rules and takes the choice: the value is a submenu or a rule. picked says the choice
// changed the rules or opened a menu, isFinal that the rules are confirmed
u32 UnionMsg_UpdateRuleMenu(UnionSystem *unisys, BOOL *picked, u8 *ruleSel, BOOL *isFinal, Field *field) {
    u32 ret;
    u32 i;

    *picked = FALSE;
    *isFinal = FALSE;
    ret = UnionMsg_UpdateMenu(unisys, field);

    switch (ret) {
    case BMPMENULIST_NULL:
        return ret;
    case BMPMENULIST_CANCEL:
        if (ruleSel[3] == UNION_RULE_MENU_PLAYERS) {
            return ret;
        }
        // The last row goes back to the menu that opened this one
        ret = sMenuInfo[ruleSel[3]].rows[UnionMsg_GetMenuInfo(ruleSel[3])->count - 1].value;
        break;
    case UNION_RULE_MENU_CONFIRM:
        return sRuleResult[ruleSel[0]][ruleSel[1]][ruleSel[2]];
    case UNION_RULE_MENU_SHOW_RULES:
        *isFinal = TRUE;
        return sRuleResult[ruleSel[0]][ruleSel[1]][ruleSel[2]];
    }

    if (ruleSel[3] == UNION_RULE_MENU_PLAYERS) {
        if (ret == (uintptr_t)sMenuTwo) {
            if (ruleSel[0] == 4) {
                ruleSel[0] = 0;
            }
        } else if (ret == (uintptr_t)sMenuFour) {
            ruleSel[0] = 4;
        }
    }

    for (i = 0; UnionMsg_IsMenuKind(i); i++) {
        if (UnionMsg_GetMenuInfo(i)->rows == (const ListMenuRow *)ret) {
            *picked = TRUE;
            return i;
        }
        switch (ruleSel[3]) {
        case UNION_RULE_MENU_FORMAT:
            if (ret < 5) {
                ruleSel[0] = ret;
                *picked = TRUE;
                return UNION_RULE_MENU_TWO;
            }
            break;
        case UNION_RULE_MENU_RULES:
        case UNION_RULE_MENU_FOUR_RULES:
            if (ret < 2) {
                ruleSel[1] = ret;
                *picked = TRUE;
                if (ruleSel[3] == UNION_RULE_MENU_FOUR_RULES) {
                    return UNION_RULE_MENU_FOUR;
                }
                return UNION_RULE_MENU_TWO;
            }
            break;
        case UNION_RULE_MENU_LAUNCHER:
        case UNION_RULE_MENU_FOUR_LAUNCHER:
            if (ret < 2) {
                ruleSel[2] = ret;
                *picked = TRUE;
                if (ruleSel[3] == UNION_RULE_MENU_FOUR_LAUNCHER) {
                    return UNION_RULE_MENU_FOUR;
                }
                return UNION_RULE_MENU_TWO;
            }
            break;
        }
    }
    GFL_ASSERT_MSG(FALSE, "menu_ret=%d\n", ret);
    return 0;
}

void UnionMsg_CreateInfoWindow(UnionSystem *unisys, Field *field) {
    if (unisys->fldmsgwin == NULL) {
        FieldMsgBG *msgBGSys = Field_GetMsgBGSys(field);

        unisys->fldmsgwin = MsgBGPrinter_CreatePlainWin(msgBGSys, unisys->msgdata, 0, 1, 1, 0x1e, 2);
        MsgBGPrinter_PlainWinPrintMsg(unisys->fldmsgwin, 0, 0, UnionMsg_Text_259);
    }
}

void UnionMsg_FreeInfoWindow(UnionSystem *unisys) {
    if (unisys->fldmsgwin != NULL) {
        MsgBGPrinter_DeletePlainWin(unisys->fldmsgwin);
        unisys->fldmsgwin = NULL;
    }
}

// Opens the window that lists the regulation's rules, each in the color of whether the party meets it
void UnionMsg_CreateRulesWindow(UnionSystem *unisys, Field *field, u32 okMask, u32 hasLauncher, u32 mode) {
    FieldMsgBG *msgBGSys = Field_GetMsgBGSys(field);
    BOOL launcher;
    int i;

    GFL_ASSERT(unisys->wordset != NULL);
    GFL_ASSERT(unisys->fldmsgwin == NULL);
    GFL_ASSERT(unisys->alloc.rpm == NULL);

    launcher = TRUE;
    if (hasLauncher != 1) {
        launcher = FALSE;
    }
    unisys->fldmsgwin = MsgBGPrinter_CreatePlainWin(msgBGSys, unisys->msgdata, 0, 1, 1, 0x1a, 0x10);
    unisys->alloc.rpm =
        PokeRegulation_CreatePrintMsg(unisys->alloc.regulation, unisys->wordset, UNION_HEAPID, launcher);
    UnionMsg_SetupWordSet(unisys);

    if (mode == 0) {
        StrBuf *title = makeStrbufLoadBattleTypeText(unisys->alloc.regulation, UNION_HEAPID);

        MsgBGPrinter_PlainWinPrintStr(unisys->fldmsgwin, 0, 0, title);
        GFL_StrBufFree(title);
    } else {
        MsgBGPrinter_PlainWinPrintMsg(unisys->fldmsgwin, 0, 0,
                                      mode == 1 ? UnionMsg_Text_335 : UnionMsg_Text_336);
    }

    for (i = 0; i < 6; i++) {
        RegulationPrintMsg *rpm;
        u16 color;

        if (okMask & (1 << i)) {
            color = PRINT_COLOR(4, 2, 0);
        } else {
            color = PRINT_COLOR(1, 2, 0);
        }

        rpm = unisys->alloc.rpm;
        MsgBGPrinter_PlainWinPrintStrColor(unisys->fldmsgwin, 0, (i * 2 + 4) * 8, rpm->rule[i], color);
        rpm = unisys->alloc.rpm;
        MsgBGPrinter_PlainWinPrintStrColor(unisys->fldmsgwin, 0x70, (i * 2 + 4) * 8, rpm->value[i], color);
    }
}

BOOL UnionMsg_IsRulesPrinted(UnionSystem *unisys, Field *field) {
    void *queue = MsgBGPrinter_GetPrintQue(Field_GetMsgBGSys(field));

    if (unisys->alloc.rpm == NULL) {
        return TRUE;
    }
    return PrintQueue_IsEmpty(queue);
}

void UnionMsg_FreeRulesWindow(UnionSystem *unisys) {
    if (unisys->alloc.rpm != NULL) {
        MsgBGPrinter_DeletePlainWin(unisys->fldmsgwin);
        unisys->fldmsgwin = NULL;
        PokeRegulation_FreePrintMsg(unisys->alloc.rpm);
        unisys->alloc.rpm = NULL;
    }
}
