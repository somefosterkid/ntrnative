#include "global.h"
#include "save.h"
#include "save_frontier.h"
#include "player_data.h"
#include "message_format.h"

u32 ov83_0224776C(u32 param_1, u32 param_2);

struct FrontierSave;
struct SaveData;
struct MessageFormat;
struct PlayerProfile;

enum BattleFrontierStatsIndex sub_0205C174(u8 challengeType, u8 rankType);
int sub_0205C268(u32 statIndex);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
BOOL PlaySE(u16 seqID);
PlayerProfile *sub_02034818(int netId);
u16 sub_0203769C(void);
u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 rankType);
void ov83_022477B0(u32 input, u16 seqID);
void ov83_022477C4(MessageFormat *template, u32 idx);

void ov83_022477E4(s32 *arg0);

void ov83_022477EC(s32 arg0, s32 arg1, s32 *arg2);

u32 ov83_0224776C(u32 param_1, u32 param_2) {
    if (param_2 >= param_1) {
        param_2 = (param_2 - param_1) & 0xff;
    }
    return param_2;
}

u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 rankType) {
    return FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), sub_0205C174(challengeType, rankType), sub_0205C268(sub_0205C174(challengeType, rankType)));
}

void ov83_022477B0(u32 input, u16 seqID) {
    if (input != -1) {
        PlaySE(seqID);
    }
}

void ov83_022477C4(MessageFormat *template, u32 idx) {
    PlayerProfile *partner = sub_02034818(sub_0203769C() ^ 1);
    BufferPlayersName(template, idx, partner);
}

void ov83_022477E4(s32 *arg0) {
    *arg0 = 0;
}

void ov83_022477EC(s32 arg0, s32 arg1, s32 *arg2) {
    s32 v0;

    v0 = arg0 * 4;
    *arg2 = ((arg1 + 1) << v0) | (*arg2 & (-1 ^ (0xF << v0)));
}
