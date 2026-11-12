// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov034/union_msg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "field/union_msg.h"
#include "types.h"
#include "constants/language.h"
#include "constants/text_banks.h"
#include "field/field.h"
#include "field/field_msgbg.h"
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
#include "system/wordset.h"
#include "text/system/0399.h"
#include "battle/regulation.h"
#include "field/union_comm.h"

// The Union Room's messages and menus: the lookups of message IDs, the message system the field talks through (the
// message data, the word set, the stream window), the menus of the battle rules and the regulation's rules printed.
// Names are not swan's

#define UNION_HEAPID 0x41

typedef struct {
    const ListMenuRow *rows;
    u8 count;
} UnionMenuInfo;

static void func_ov019_0217ee90(UnionSystem *unisys);
static void func_ov019_0217efe0(GameData *gameData, u32 language);
static void func_ov019_0217f020(UnionSystem *unisys);
static void func_ov019_0217f044(UnionSystem *unisys);
void func_ov019_0217f128(UnionSystem *unisys);
static void func_ov019_0217f2a8(UnionSystem *unisys, Field *field, const ListMenuRow *rows, u32 count,
                                const ListMenuRequest *request);
static void func_ov019_0217f2f4(UnionSystem *unisys);
intptr_t func_ov019_0217f310(UnionSystem *unisys, Field *field);

// The menus of the battle rules, each row a message and a value or the menu it opens
// pokebw2's sMenu_029c
static const ListMenuRow data_ov019_021803b4[3];
// pokebw2's sMenu_02b4
static const ListMenuRow data_ov019_021803cc[3];
// pokebw2's sMenu_02cc
static const ListMenuRow data_ov019_021803e4[3];
// pokebw2's sMenu_02e4
static const ListMenuRow data_ov019_021803fc[3];
// pokebw2's sMenu_02fc
static const ListMenuRow data_ov019_02180414[3];
// pokebw2's sMenu_0374
static const ListMenuRow data_ov019_0218048c[5];
// pokebw2's sMenu_039c
static const ListMenuRow data_ov019_021804b4[5];
// pokebw2's sMenu_03f4
static const ListMenuRow data_ov019_0218050c[6];

// The tables are declared in the order that gives the ROM's .rodata: MWCC heapsorts the objects of equal size by where
// they are declared. The pairs are the messages of a gender, or of a language, by the lookups above
// pokebw2's sMsgPair_0214
static const u16 data_ov019_0218032c[2] = { Bank0399_Text_092, Bank0399_Text_093 };

// pokebw2's sMsgPair_0210
static const u16 data_ov019_02180328[2] = { Bank0399_Text_174, Bank0399_Text_175 };

// pokebw2's sMsgPair_0208
static const u16 data_ov019_02180320[2] = { Bank0399_Text_126, Bank0399_Text_127 };

// pokebw2's sMsgPair_01f0
static const u16 data_ov019_02180308[2] = { Bank0399_Text_162, Bank0399_Text_163 };

// pokebw2's sMsgPair_0218
static const u16 data_ov019_02180330[2] = { Bank0399_Text_128, Bank0399_Text_129 };

// pokebw2's sMsgPair_0220
static const u16 data_ov019_02180338[2] = { Bank0399_Text_172, Bank0399_Text_173 };

// pokebw2's sMsgPair_021c
static const u16 data_ov019_02180334[2] = { Bank0399_Text_100, Bank0399_Text_101 };

// pokebw2's sMsgPair_0228
static const u16 data_ov019_02180340[2] = { Bank0399_Text_072, Bank0399_Text_073 };

// pokebw2's sMsgPair_01f4
static const u16 data_ov019_0218030c[2] = { Bank0399_Text_102, Bank0399_Text_103 };

// pokebw2's sMsgPair_01e0
static const u16 data_ov019_021802f8[2] = { Bank0399_Text_076, Bank0399_Text_077 };

// pokebw2's sMsgPair_0224
static const u16 data_ov019_0218033c[2] = { Bank0399_Text_054, Bank0399_Text_055 };

// pokebw2's sMsgPair_0204
static const u16 data_ov019_0218031c[2] = { Bank0399_Text_056, Bank0399_Text_057 };

// pokebw2's sMsgPair_01f8
static const u16 data_ov019_02180310[2] = { Bank0399_Text_160, Bank0399_Text_161 };

// pokebw2's sMsgPair_0230
static const u16 data_ov019_02180348[2] = { Bank0399_Text_050, Bank0399_Text_051 };

// pokebw2's sMsgPair_020c
static const u16 data_ov019_02180324[2] = { Bank0399_Text_172, Bank0399_Text_173 };

// pokebw2's sMsgPair_01ec
const u16 data_ov019_02180304[2] = { Bank0399_Text_098, Bank0399_Text_099 };

// pokebw2's sMsgPair_0200
static const u16 data_ov019_02180318[2] = { Bank0399_Text_158, Bank0399_Text_159 };

// pokebw2's sMsgPair_01e8
static const u16 data_ov019_02180300[2] = { Bank0399_Text_052, Bank0399_Text_053 };

// pokebw2's sMsgPair_022c
static const u16 data_ov019_02180344[2] = { Bank0399_Text_074, Bank0399_Text_075 };

// pokebw2's sMsgPair_01fc
static const u16 data_ov019_02180314[2] = { Bank0399_Text_096, Bank0399_Text_097 };

// pokebw2's sMsgPair_01e4
static const u16 data_ov019_021802fc[2] = { Bank0399_Text_104, Bank0399_Text_105 };

// pokebw2's sMsgPair_01d8
static const u16 data_ov019_021802f0[2] = { Bank0399_Text_078, Bank0399_Text_079 };

// pokebw2's sMsgPair_01dc
static const u16 data_ov019_021802f4[2] = { Bank0399_Text_080, Bank0399_Text_081 };

// pokebw2's sMsgPair_0244
static const u16 data_ov019_0218035c[2] = { Bank0399_Text_164, Bank0399_Text_165 };

// pokebw2's sMsgPair_0240
static const u16 data_ov019_02180358[2] = { Bank0399_Text_166, Bank0399_Text_167 };

// pokebw2's sMsgPair_023c
static const u16 data_ov019_02180354[2] = { Bank0399_Text_170, Bank0399_Text_171 };

// pokebw2's sMsgPair_0238
static const u16 data_ov019_02180350[2] = { Bank0399_Text_168, Bank0399_Text_169 };

// pokebw2's sMsgPair_0234
static const u16 data_ov019_0218034c[2] = { Bank0399_Text_048, Bank0399_Text_049 };

// pokebw2's sMsgPairs_0248
static const u16 data_ov019_02180360[2][2] = {
    { Bank0399_Text_122, Bank0399_Text_123 },
    { Bank0399_Text_124, Bank0399_Text_125 },
};

// pokebw2's sMsgPairs_0250
static const u16 data_ov019_02180368[2][2] = {
    { Bank0399_Text_062, Bank0399_Text_063 },
    { Bank0399_Text_064, Bank0399_Text_065 },
};

// pokebw2's sMsgPairs_0258
static const u16 data_ov019_02180370[4][2] = {
    { Bank0399_Text_082, Bank0399_Text_083 },
    { Bank0399_Text_084, Bank0399_Text_085 },
    { Bank0399_Text_086, Bank0399_Text_087 },
    { Bank0399_Text_088, Bank0399_Text_089 },
};

// pokebw2's sMsgPairs_0268
static const u16 data_ov019_02180380[4][2] = {
    { Bank0399_Text_108, Bank0399_Text_109 },
    { Bank0399_Text_110, Bank0399_Text_111 },
    { Bank0399_Text_112, Bank0399_Text_113 },
    { Bank0399_Text_114, Bank0399_Text_115 },
};

// pokebw2's sMsgPairs_0278
static const u16 data_ov019_02180390[4][2] = {
    { Bank0399_Text_176, Bank0399_Text_177 },
    { Bank0399_Text_178, Bank0399_Text_179 },
    { Bank0399_Text_180, Bank0399_Text_181 },
    { Bank0399_Text_182, Bank0399_Text_183 },
};

// pokebw2's sMsgPairs_04cc
static const u16 data_ov019_021805e4[25][2] = {
    { Bank0399_Text_058, Bank0399_Text_059 },
    { Bank0399_Text_066, Bank0399_Text_067 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_090, Bank0399_Text_091 },
    { Bank0399_Text_116, Bank0399_Text_117 },
    { Bank0399_Text_152, Bank0399_Text_153 },
    { Bank0399_Text_152, Bank0399_Text_153 },
};

// pokebw2's sMsgPairs_0530
static const u16 data_ov019_02180648[25][2] = {
    { Bank0399_Text_060, Bank0399_Text_061 },
    { Bank0399_Text_070, Bank0399_Text_071 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_094, Bank0399_Text_095 },
    { Bank0399_Text_120, Bank0399_Text_121 },
    { Bank0399_Text_156, Bank0399_Text_157 },
    { Bank0399_Text_134, Bank0399_Text_135 },
};

// pokebw2's sTable_0424
static const u16 data_ov019_0218053c[26] = {
#include "asset/union_msg_data_ov019_0218053c.inc"
};

// pokebw2's sTable_0458
static const u16 data_ov019_02180570[26] = {
#include "asset/union_msg_data_ov019_02180570.inc"
};

static const ListMenuRow data_ov019_021803b4[3] = {
    { Bank0399_Text_250, { 0x0 } },
    { Bank0399_Text_251, { 0x1 } },
    { Bank0399_Text_254, { .sub = data_ov019_0218050c } },
};

static const ListMenuRow data_ov019_021803cc[3] = {
    { Bank0399_Text_252, { 0x0 } },
    { Bank0399_Text_253, { 0x1 } },
    { Bank0399_Text_254, { .sub = data_ov019_0218050c } },
};

static const ListMenuRow data_ov019_021803e4[3] = {
    { Bank0399_Text_239, { .sub = data_ov019_0218050c } },
    { Bank0399_Text_240, { .sub = data_ov019_021804b4 } },
    { Bank0399_Text_241, { BMPMENULIST_CANCEL } },
};

static const ListMenuRow data_ov019_021803fc[3] = {
    { Bank0399_Text_250, { 0x0 } },
    { Bank0399_Text_251, { 0x1 } },
    { Bank0399_Text_254, { .sub = data_ov019_021804b4 } },
};

static const ListMenuRow data_ov019_02180414[3] = {
    { Bank0399_Text_252, { 0x0 } },
    { Bank0399_Text_253, { 0x1 } },
    { Bank0399_Text_254, { .sub = data_ov019_021804b4 } },
};

static const ListMenuRow data_ov019_0218048c[5] = {
    { Bank0399_Text_242, { 0x0 } },
    { Bank0399_Text_243, { 0x1 } },
    { Bank0399_Text_244, { 0x2 } },
    { Bank0399_Text_245, { 0x3 } },
    { Bank0399_Text_248, { .sub = data_ov019_0218050c } },
};

static const ListMenuRow data_ov019_021804b4[5] = {
    { Bank0399_Text_243, { .sub = data_ov019_021803fc } },
    { Bank0399_Text_244, { .sub = data_ov019_02180414 } },
    { Bank0399_Text_246, { 0x64 } },
    { Bank0399_Text_247, { 0x96 } },
    { Bank0399_Text_248, { .sub = data_ov019_021803e4 } },
};

static const ListMenuRow data_ov019_0218050c[6] = {
    { Bank0399_Text_242, { .sub = data_ov019_0218048c } },
    { Bank0399_Text_243, { .sub = data_ov019_021803b4 } },
    { Bank0399_Text_244, { .sub = data_ov019_021803cc } },
    { Bank0399_Text_246, { 0x64 } },
    { Bank0399_Text_247, { 0x96 } },
    { Bank0399_Text_248, { .sub = data_ov019_021803e4 } },
};

// pokebw2's sMenuInfo
static const UnionMenuInfo data_ov019_021805a4[8] = {
    { data_ov019_021803e4, 3 }, { data_ov019_0218050c, 6 }, { data_ov019_0218048c, 5 }, { data_ov019_021803b4, 3 },
    { data_ov019_021803cc, 3 }, { data_ov019_021804b4, 5 }, { data_ov019_021803fc, 3 }, { data_ov019_02180414, 3 },
};

// The party and the rules a menu's choices lead to, by the battle format and the two options
// pokebw2's sRuleResult
static const u8 data_ov019_021803a0[5][2][2] = {
    { { 4, 5 }, { 6, 7 } },     { { 8, 9 }, { 10, 11 } },   { { 12, 13 }, { 14, 15 } },
    { { 16, 17 }, { 18, 19 } }, { { 20, 21 }, { 22, 23 } },
};

// pokebw2's sMenuMain
static const ListMenuRow data_ov019_021804dc[6] = {
    { Bank0399_Text_000, { 2 } },  { Bank0399_Text_002, { 0x1b } }, { Bank0399_Text_003, { 0x18 } },
    { Bank0399_Text_001, { 3 } },   { Bank0399_Text_005, { 0x19 } },   { Bank0399_Text_004, { 0 } },
};

// pokebw2's sLanguageFlags
static const u32 data_ov019_0218044c[8] = { 0, 1, 2, 3, 4, 7, 5, 6 };

// pokebw2's sMenuMainRequest
static const ListMenuRequest data_ov019_0218042c = { 6, 6, 0, 0x0d, 0, 0, 1, 0xf, 2, 0, 0, 0, 0, 1, 0, 0x0c, 0x10, 1, 1, 0x0d, 0x0c, 0 };

// pokebw2's sMenuRulesRequest
static const ListMenuRequest data_ov019_0218046c = { 3, 3, 0, 0x0d, 0, 0, 1, 0xf, 2, 0, 0, 0, 0, 1, 0, 0x0c, 0x10, 1, 1, 0x13, 0x0e, 0 };

static const struct {
    u8 value;
    u8 unk01[3];
// pokebw2's sMsgIndexTable
} data_ov019_021806ac[27] = {
    { 2 }, { 2 }, { 2 }, { 5 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 4 }, { 4 }, { 4 }, { 4 }, { 2 }, { 5 }, { 5 },
};

u16 func_ov019_0217ec04(UnionSystem *unisys)
{
    return data_ov019_0218031c[getTrainerGender(unisys->param->playerInfo)];
}

u16 func_ov019_0217ec1c(u8 gender)
{
    return data_ov019_0218034c[gender];
}

u16 func_ov019_0217ec28(u8 gender)
{
    return data_ov019_02180330[gender];
}

u16 func_ov019_0217ec34(u8 gender)
{
    return data_ov019_02180348[gender];
}

u16 func_ov019_0217ec40(u32 row, u8 gender)
{
    return data_ov019_021805e4[row - 2][gender];
}

u16 func_ov019_0217ec54(u32 row, u8 gender)
{
    return data_ov019_02180648[row - 2][gender];
}

u16 func_ov019_0217ec68(u32 row)
{
    return data_ov019_0218053c[row - 2];
}

u16 func_ov019_0217ec78(u32 row)
{
    return data_ov019_02180570[row - 2];
}

u16 func_ov019_0217ec88(u8 gender)
{
    return data_ov019_02180300[gender];
}

u16 func_ov019_0217ec94(u8 gender)
{
    return data_ov019_0218032c[gender];
}

u16 func_ov019_0217eca0(u8 gender)
{
    return data_ov019_02180320[gender];
}

u16 func_ov019_0217ecac(u8 gender)
{
    return data_ov019_02180318[gender];
}

u16 func_ov019_0217ecb8(UnionSystem *unisys)
{
    return data_ov019_02180328[getTrainerGender(unisys->param->playerInfo)];
}

u16 func_ov019_0217ecd0(UnionSystem *unisys)
{
    return data_ov019_02180324[getTrainerGender(unisys->param->playerInfo)];
}

u16 func_ov019_0217ece8(u8 gender)
{
    return data_ov019_02180380[GFL_RandomLC(4)][gender];
}

u16 func_ov019_0217ed04(u8 gender)
{
    return data_ov019_0218033c[gender];
}

u16 func_ov019_0217ed10(u8 gender)
{
    return data_ov019_02180368[GFL_RandomLC(2)][gender];
}

u16 func_ov019_0217ed2c(u8 gender)
{
    return data_ov019_02180370[GFL_RandomLC(4)][gender];
}

u16 func_ov019_0217ed48(u8 gender)
{
    return data_ov019_02180360[GFL_RandomLC(2)][gender];
}

u16 func_ov019_0217ed64(u8 gender)
{
    return data_ov019_02180390[GFL_RandomLC(4)][gender];
}

u16 func_ov019_0217ed80(u8 gender)
{
    return data_ov019_02180314[gender];
}

u16 func_ov019_0217ed8c(u8 gender)
{
    return data_ov019_02180310[gender];
}

u16 func_ov019_0217ed98(u8 gender)
{
    return data_ov019_02180340[gender];
}

u16 func_ov019_0217eda4(u8 gender)
{
    return data_ov019_02180304[gender];
}

u16 func_ov019_0217edb0(u8 gender)
{
    return data_ov019_02180308[gender];
}

u16 func_ov019_0217edbc(u8 gender)
{
    return data_ov019_02180344[gender];
}

u16 func_ov019_0217edc8(u8 gender)
{
    return data_ov019_02180350[gender];
}

u16 func_ov019_0217edd4(u8 gender)
{
    return data_ov019_02180338[gender];
}

u16 func_ov019_0217ede0(u8 gender)
{
    return data_ov019_0218035c[gender];
}

u16 func_ov019_0217edec(u8 gender)
{
    return data_ov019_02180358[gender];
}

u16 func_ov019_0217edf8(u8 gender)
{
    return data_ov019_02180354[gender];
}

u16 func_ov019_0217ee04(u8 gender)
{
    return data_ov019_021802f8[gender];
}

u16 func_ov019_0217ee10(u8 gender)
{
    return data_ov019_021802f0[gender];
}

u16 func_ov019_0217ee1c(u8 gender)
{
    return data_ov019_021802f4[gender];
}

u16 func_ov019_0217ee28(u8 gender)
{
    return data_ov019_02180334[gender];
}

u16 func_ov019_0217ee34(u8 gender)
{
    return data_ov019_0218030c[gender];
}

u16 func_ov019_0217ee40(u8 gender)
{
    return data_ov019_021802fc[gender];
}

u32 func_ov019_0217ee4c(UnionSystem *unisys, u8 country, u8 province)
{
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(unisys->param->gameData);
    u8 myCountry = UnityTowerVisitor_GetCountry(playerInfo);
    u8 myProvince = UnityTowerVisitor_GetProvince(playerInfo);

    if (country == 0) {
        return Bank0399_Text_015;
    }
    if (myCountry != country) {
        if (province == 0) {
            return Bank0399_Text_013;
        }
        return Bank0399_Text_012;
    }
    if (province == 0 || myProvince == province) {
        return Bank0399_Text_015;
    }
    return Bank0399_Text_014;
}

// The words of the message system: the player or the one spoken to, their greeting by language and their origin
static void func_ov019_0217ee90(UnionSystem *unisys)
{
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
    func_0201ee0c(unisys->wordset, 0, strbuf, gender, TRUE, language);
    GFL_StrBufFree(strbuf);
    copyVarForText(unisys->wordset, 1, playerInfo);

    switch (language) {
    case LANGUAGE_JAPANESE:
        messageId = Bank0399_Text_231;
        break;
    case LANGUAGE_ENGLISH:
        messageId = Bank0399_Text_232;
        break;
    case LANGUAGE_FRENCH:
        messageId = Bank0399_Text_233;
        break;
    case LANGUAGE_ITALIAN:
        messageId = Bank0399_Text_234;
        break;
    case LANGUAGE_GERMAN:
        messageId = Bank0399_Text_235;
        break;
    case LANGUAGE_SPANISH:
        messageId = Bank0399_Text_236;
        break;
    case LANGUAGE_KOREAN:
        messageId = Bank0399_Text_238;
        break;
    default:
        messageId = Bank0399_Text_237;
        break;
    }
    strbuf = GFL_MsgDataLoadStrbufNew(unisys->msgdata, messageId);
    func_0201ee0c(unisys->wordset, 2, strbuf, gender, TRUE, language);
    GFL_StrBufFree(strbuf);

    if (situ->mycomm.target != NULL && situ->mycomm.target->beacon.country != 0) {
        loadCountryToStrbuf(unisys->wordset, 3, situ->mycomm.target->beacon.country);
        if (situ->mycomm.target->beacon.province != 0) {
            loadCountryAreaToStrbuf(unisys->wordset, 4, situ->mycomm.target->beacon.country,
                                    situ->mycomm.target->beacon.province);
        }
    }
    func_ov019_0217efe0(unisys->param->gameData, language);
}

// Marks the language as seen in the player's Pokemon Wonder word list
static void func_ov019_0217efe0(GameData *gameData, u32 language)
{
    PMSWordSave *save = getDexBlkAddress(GameData_GetSaveControl(gameData));
    u16 index = language - 1;

    if (language >= 1 && language <= 8) {
        if (index < 8) {
            func_020249a0(save, data_ov019_0218044c[index]);
        } else {
            func_020249a0(save, 7);
        }
    } else {
        func_020249a0(save, 7);
    }
}

static void func_ov019_0217f020(UnionSystem *unisys)
{
    if (unisys->wordset == NULL) {
        unisys->wordset = GFL_WordSetSystemCreate(5, 0x40, UNION_HEAPID);
        func_ov019_0217ee90(unisys);
    }
}

static void func_ov019_0217f044(UnionSystem *unisys)
{
    if (unisys->wordset != NULL) {
        GFL_WordSetSystemFree(unisys->wordset);
        unisys->wordset = NULL;
    }
}

int func_ov019_0217f060(s32 index)
{
    if (index >= 27) {
        return 0;
    }
    return data_ov019_021806ac[index].value;
}

void func_ov019_0217f074(UnionSystem *unisys)
{
    func_ov019_0217f7b8(unisys);
    func_ov019_0217f624(unisys);
    func_ov019_0217f24c(unisys);
    func_ov019_0217f2f4(unisys);
    func_ov019_0217f128(unisys);
}

void func_ov019_0217f098(UnionSystem *unisys, Field *field)
{
    void *fldmsg_bg;

    if (unisys->fld_msgwin_stream == NULL) {
        GFL_ASSERT(unisys->msgdata == NULL);
        fldmsg_bg = Field_GetMsgBGSys(field);
        unisys->msgdata = func_ov021_021904b0(fldmsg_bg, TEXT_BANK_0399);
        unisys->msgbuf = GFL_StrBufCreate(0x100, UNION_HEAPID);
        unisys->fmtbuf = GFL_StrBufCreate(0x100, UNION_HEAPID);
        func_ov019_0217f020(unisys);
        unisys->fld_msgwin_stream = func_ov021_02191558(fldmsg_bg, unisys->msgdata, 0, 1, 0x13, 0x1e, 4);
    } else {
        func_ov021_021915ec(unisys->fld_msgwin_stream);
    }
}


void func_ov019_0217f128(UnionSystem *unisys)
{
    if (unisys->fld_msgwin_stream != NULL) {
        func_ov021_0219160c(unisys->fld_msgwin_stream);
        func_ov021_021915b4(unisys->fld_msgwin_stream);
        unisys->fld_msgwin_stream = NULL;
        unisys->msgPrinting = 0;
        GFL_StrBufFree(unisys->msgbuf);
        GFL_StrBufFree(unisys->fmtbuf);
        unisys->msgbuf = NULL;
        unisys->fmtbuf = NULL;
    }
    if (unisys->msgdata != NULL) {
        func_ov019_0217f044(unisys);
        func_ov021_021904c8(unisys->msgdata);
        unisys->msgdata = NULL;
    }
}

void func_ov019_0217f188(UnionSystem *unisys, u32 messageId)
{
    GFL_ASSERT(unisys->fld_msgwin_stream != NULL);
    GFL_MsgDataLoadStrbuf(unisys->msgdata, messageId, unisys->msgbuf);
    GFL_WordSetFormatStrbuf(unisys->wordset, unisys->fmtbuf, unisys->msgbuf);
    func_ov021_021916a8(unisys->fld_msgwin_stream, 0, 0, unisys->fmtbuf);
    unisys->msgPrinting = 1;
}


BOOL func_ov019_0217f1ec(UnionSystem *unisys)
{
    if (unisys->fld_msgwin_stream == NULL || unisys->msgPrinting == 0) {
        return TRUE;
    }
    return func_ov021_021916e0(unisys->fld_msgwin_stream);
}

void func_ov019_0217f210(UnionSystem *unisys, Field *field, u32 messageId)
{
    func_ov019_0217f098(unisys, field);
    func_ov019_0217ee90(unisys);
    func_ov019_0217f188(unisys, messageId);
}

void func_ov019_0217f22c(UnionSystem *unisys, Field *field)
{
    if (unisys->fldmenu_yesno_func == NULL) {
        unisys->fldmenu_yesno_func = func_ov021_02190be4(Field_GetMsgBGSys(field), 0);
    }
}

void func_ov019_0217f24c(UnionSystem *unisys)
{
    if (unisys->fldmenu_yesno_func != NULL) {
        func_ov021_021909bc(unisys->fldmenu_yesno_func);
        unisys->fldmenu_yesno_func = NULL;
    }
}

BOOL func_ov019_0217f268(UnionSystem *unisys, BOOL *yes)
{
    u32 answer;

    GFL_ASSERT(unisys->fldmenu_yesno_func != NULL);
    answer = func_ov021_02190c78(unisys->fldmenu_yesno_func);
    if (answer == 2) {
        return FALSE;
    }
    *yes = answer == 0;
    return TRUE;
}


static void func_ov019_0217f2a8(UnionSystem *unisys, Field *field, const ListMenuRow *rows, u32 count,
                                const ListMenuRequest *request)
{
    void *msgBGSys = Field_GetMsgBGSys(field);

    if (unisys->fldmenu_func == NULL) {
        ListMenuOption *options = func_ov021_02190a28(rows, count, unisys->msgdata, UNION_HEAPID);

        unisys->fldmenu_func = ListMenuUI_Create(msgBGSys, request, options, NULL, NULL, 0, 0, TRUE);
    }
}

static void func_ov019_0217f2f4(UnionSystem *unisys)
{
    if (unisys->fldmenu_func != NULL) {
        func_ov021_021909bc(unisys->fldmenu_func);
        unisys->fldmenu_func = NULL;
    }
}

intptr_t func_ov019_0217f310(UnionSystem *unisys, Field *field)
{
    void *fldmsg_bg = Field_GetMsgBGSys(field);

    GFL_ASSERT(fldmsg_bg != NULL);
    if (fldmsg_bg != NULL && !func_ov021_0219049c(fldmsg_bg)) {
        return BMPMENULIST_NULL;
    }
    GFL_ASSERT(unisys->fldmenu_func != NULL);
    return ListMenuUI_Update(unisys->fldmenu_func);
}


void func_ov019_0217f360(UnionSystem *unisys, Field *field)
{
    func_ov019_0217f2a8(unisys, field, data_ov019_021804dc, 6, &data_ov019_0218042c);
}

void func_ov019_0217f378(UnionSystem *unisys)
{
    func_ov019_0217f2f4(unisys);
}

intptr_t func_ov019_0217f380(UnionSystem *unisys, Field *field)
{
    return func_ov019_0217f310(unisys, field);
}

// Opens the menu of the battle rules: the kind says which of data_ov019_021805a4's, and which choices the rows show
void func_ov019_0217f388(UnionSystem *unisys, Field *field, int kind, u8 *ruleSel)
{
    ListMenuRow *rows;
    u8 count;

    ruleSel[3] = kind;
    ListMenuRequest request = data_ov019_0218046c;
    count = data_ov019_021805a4[kind].count;
    request.count = count;
    request.unk02 = count;
    request.height = request.count * 2;
    if (kind == 1 || kind == 5) {
        request.width = 30;
    }
    if (kind >= 5) {
        request.y = 5;
    }

    if (kind == 1) {
        rows = GFL_HeapAllocate(UNION_HEAPID, sizeof(ListMenuRow) * 6, TRUE, "union_msg.c", 0x647);
        MI_CpuCopy8(data_ov019_0218050c, rows, sizeof(ListMenuRow) * 6);
        rows[0].messageId = Bank0399_Text_255 + ruleSel[0];
        rows[1].messageId = Bank0399_Text_260 + ruleSel[1];
        rows[2].messageId = Bank0399_Text_262 + ruleSel[2];
        func_ov019_0217f2a8(unisys, field, rows, count, &request);
        GFL_HeapFree(rows);
    } else if (kind == 5) {
        rows = GFL_HeapAllocate(UNION_HEAPID, sizeof(ListMenuRow) * 5, TRUE, "union_msg.c", 0x657);
        MI_CpuCopy8(data_ov019_021804b4, rows, sizeof(ListMenuRow) * 5);
        rows[0].messageId = Bank0399_Text_260 + ruleSel[1];
        rows[1].messageId = Bank0399_Text_262 + ruleSel[2];
        func_ov019_0217f2a8(unisys, field, rows, count, &request);
        GFL_HeapFree(rows);
    } else {
        func_ov019_0217f2a8(unisys, field, data_ov019_021805a4[kind].rows, count, &request);
    }
}

void func_ov019_0217f4ac(UnionSystem *unisys)
{
    func_ov019_0217f2f4(unisys);
}

// Runs the menu of the battle rules and takes the choice: the value is a submenu or a rule. picked says the choice
// changed the rules or opened a menu, isFinal that the rules are confirmed
static inline const UnionMenuInfo *UnionMsg_GetMenuInfo(int kind) {
    return &data_ov019_021805a4[kind];
}

u32 func_ov019_0217f4b4(UnionSystem *unisys, BOOL *picked, u8 *ruleSel, BOOL *isFinal, Field *field)
{
    uintptr_t ret;
    u32 i;

    *picked = FALSE;
    *isFinal = FALSE;
    ret = func_ov019_0217f310(unisys, field);

    switch (ret) {
    case BMPMENULIST_NULL:
        return (u32)ret;
    case BMPMENULIST_CANCEL:
        if (ruleSel[3] == 0) {
            return (u32)ret;
        }
        ret = data_ov019_021805a4[ruleSel[3]].rows[UnionMsg_GetMenuInfo(ruleSel[3])->count - 1].value;
        break;
    case 0x64:
        return data_ov019_021803a0[ruleSel[0]][ruleSel[1]][ruleSel[2]];
    case 0x96:
        *isFinal = TRUE;
        return data_ov019_021803a0[ruleSel[0]][ruleSel[1]][ruleSel[2]];
    }

    if (ruleSel[3] == 0) {
        if (ret == (uintptr_t)data_ov019_0218050c) {
            if (ruleSel[0] == 4) {
                ruleSel[0] = 0;
            }
        } else if (ret == (uintptr_t)data_ov019_021804b4) {
            ruleSel[0] = 4;
        }
    }

    for (i = 0; i < 8 ? TRUE : FALSE; i++) {
        uintptr_t rows = (uintptr_t)data_ov019_021805a4[i].rows;

        if (rows == ret) {
            *picked = TRUE;
            return i;
        }
        switch (ruleSel[3]) {
        case 2:
            if (ret < 5) {
                ruleSel[0] = ret;
                *picked = TRUE;
                return TRUE;
            }
            break;
        case 3:
        case 6:
            if (ret < 2) {
                ruleSel[1] = ret;
                *picked = TRUE;
                if (ruleSel[3] == 6) {
                    return 5;
                }
                return TRUE;
            }
            break;
        case 4:
        case 7:
            if (ret < 2) {
                ruleSel[2] = ret;
                *picked = TRUE;
                if (ruleSel[3] == 7) {
                    return 5;
                }
                return TRUE;
            }
            break;
        }
    }
    GFL_ASSERT(FALSE);
    return 0;
}

void func_ov019_0217f5e0(UnionSystem *unisys, Field *field)
{
    if (unisys->fldmsgwin == NULL) {
        void *msgBGSys = Field_GetMsgBGSys(field);

        unisys->fldmsgwin = func_ov021_02191558(msgBGSys, unisys->msgdata, 0, 1, 1, 0x1e, 2);
        func_ov021_0219161c(unisys->fldmsgwin, 0, 0, Bank0399_Text_259);
    }
}

void func_ov019_0217f624(UnionSystem *unisys)
{
    if (unisys->fldmsgwin != NULL) {
        func_ov021_021915b4(unisys->fldmsgwin);
        unisys->fldmsgwin = NULL;
    }
}

void func_ov019_0217f640(UnionSystem *unisys, Field *field, u32 okMask, u32 battleType, u32 mode)
{
    void *msgBGSys = Field_GetMsgBGSys(field);
    BOOL isMulti;
    int i;

    GFL_ASSERT(unisys->wordset != NULL);
    GFL_ASSERT(unisys->fldmsgwin == NULL);
    GFL_ASSERT(unisys->alloc.rpm == NULL);

    isMulti = TRUE;
    if (battleType != 1) {
        isMulti = FALSE;
    }
    unisys->fldmsgwin = func_ov021_02191558(msgBGSys, unisys->msgdata, 0, 1, 1, 0x1a, 0x10);
    unisys->alloc.rpm = func_0201a4d4(unisys->alloc.regulation, unisys->wordset, UNION_HEAPID, isMulti);
    func_ov019_0217ee90(unisys);

    if (mode == 0) {
        StrBuf *title = makeStrbufLoadBattleTypeText(unisys->alloc.regulation, UNION_HEAPID);

        func_ov021_02191648(unisys->fldmsgwin, 0, 0, title);
        GFL_StrBufFree(title);
    } else {
        func_ov021_0219161c(unisys->fldmsgwin, 0, 0, mode == 1 ? Bank0399_Text_335 : Bank0399_Text_336);
    }

    for (i = 0; i < 6; i++) {
        RegulationRulesText *rpm;
        u16 color;

        if (okMask & (1 << i)) {
            color = PRINT_COLOR(4, 2, 0);
        } else {
            color = PRINT_COLOR(1, 2, 0);
        }

        rpm = unisys->alloc.rpm;
        func_ov021_02191674(unisys->fldmsgwin, 0, (i * 2 + 4) * 8, rpm->rule[i], color);
        rpm = unisys->alloc.rpm;
        func_ov021_02191674(unisys->fldmsgwin, 0x70, (i * 2 + 4) * 8, rpm->value[i], color);
    }
}


BOOL func_ov019_0217f794(UnionSystem *unisys, Field *field)
{
    void *queue = func_ov021_021904a8(Field_GetMsgBGSys(field));

    if (unisys->alloc.rpm == NULL) {
        return TRUE;
    }
    return func_0201c6b4(queue);
}

void func_ov019_0217f7b8(UnionSystem *unisys)
{
    if (unisys->alloc.rpm != NULL) {
        func_ov021_021915b4(unisys->fldmsgwin);
        unisys->fldmsgwin = NULL;
        func_0201a570(unisys->alloc.rpm);
        unisys->alloc.rpm = NULL;
    }
}
