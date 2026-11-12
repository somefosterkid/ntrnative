// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_BATTLE_BTLV_H
#define BWPORT_BATTLE_BTLV_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "battle/btl_action.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"

// Black has 8 args (Black 2 has 9); twinned code confirms the offsets of mode
struct BtlvStringParam {
    u16 message;
    u8 mode;
    u8 type : 4;
    u8 count : 4;
    u32 args[8]; // Black's: 0x24 bytes
};

// Black 2's, not checked against Black
// Called while a message prints
typedef BOOL (*BtlvMsgCallback)(u32 arg);

// Black 2's, not checked against Black
// The mons a target is chosen among, which the client fills, and the action the display fills in
struct BtlvSelectTargetParam {
    struct {
        BattleMon *mon;
        u8 selectable[4];
    } mons[3];
    BattleAction rotateAction;
    BattleAction action;
};

void BattleClientCmd_QuitPokeSelect(BtlvCore *core);
void func_ov093_021e90a0(BtlvCore *core, u32 cmd);
BOOL func_ov093_021e90d0(BtlvCore *core);
void func_ov093_021eaf78(BtlvStringParam *param, u8 mode);
void func_ov093_021ea614(BtlvCore *core, const BtlvStringParam *param);
void func_ov093_021ea65c(BtlvCore *core, const BtlvStringParam *param);
u32 func_ov093_021e97a4(BtlvCore *core);
void func_ov093_021e98bc(BtlvCore *core, BattleMon *mon, void *arg2);
void func_ov093_021e9e70(BtlvCore *core);
u16 func_ov093_021ea01c(BtlvCore *core);
u8 func_ov093_021ea030(BtlvCore *core);
void func_ov093_021ea058(BtlvCore *core);
void func_ov093_021ea600(BtlvCore *core);
void func_ov093_021ea5e4(BtlvCore *core, const StrBuf *src);
void func_ov093_021ea5f0(BtlvCore *core, const BtlvStringParam *param);
void func_ov093_021ea488(BtlvCore *core, u8 pos, u8 arg2, u8 arg3);
void func_ov093_021ea6f4(BtlvCore *core, u8 monId, u16 move);
void func_ov093_021ea354(BtlvCore *core, u8 pos, u32 visible);
void func_ov093_021ea0f4(BtlvCore *core, u16 arg1, u8 pos, u8 type);
BOOL func_ov093_021ea120(BtlvCore *core);
void func_ov093_021ea280(BtlvCore *core, u16 count, u32 type, const u8 *monIds, u16 arg4);
void func_ov093_021ea7c8(BtlvCore *core, u32 arg1);
void func_ov093_021ea7d8(BtlvCore *core, u32 arg1);
void func_ov093_021ea904(BtlvCore *core, u8 clientId, u8 arg2, u8 arg3, u8 slot1, u8 slot2);
void func_ov093_021ea1b8(BtlvCore *core, u16 arg1, u8 pos, u32 type);
void func_ov093_021ea3b8(BtlvCore *core, u32 arg1, u16 arg2);
void func_ov093_021ea638(BtlvCore *core, const BtlvStringParam *param, BtlvMsgCallback callback);
void func_ov093_021eaa6c(BtlvCore *core, u8 clientId, u8 arg2);
void func_ov093_021ea3e8(BtlvCore *core, u32 arg1, u32 arg2, u16 arg3);

// Black 2's, not checked against Black
typedef BOOL (*BtlvMainProc)(BtlvCore *core, s32 *seq, void *work);
// A procedure of the battle display, which btlv_core.c and btlv_scu.c run the same way: init until it returns TRUE,
// then main
typedef BOOL (*BtlvSubProcFn)(s32 *seq, void *arg);

// Black 2's, not checked against Black
struct BtlvSubProc {
    BtlvSubProcFn init;
    BtlvSubProcFn main;
    void *arg;
    s32 seq;
};

void func_ov093_021eb234(BtlvScu *scu);
void func_ov093_021ed118(BtlvScu *scu, u8 attackerPos, u8 targetPos, u16 move, s32 arg4, u32 arg5, u8 arg6);
BOOL func_ov093_021ed250(BtlvScu *scu);
void func_ov093_021ed208(BtlvScu *scu, u8 pos, u16 move, BOOL arg3);
void func_ov093_021ed268(BtlvScu *scu, u8 pos, u16 move);
void func_ov093_021ed494(BtlvScu *scu, u8 pos, BOOL arg2);
void func_ov093_021ed294(BtlvScu *scu, u8 pos, BOOL arg2);
void func_ov093_021ed2d0(BtlvScu *scu, u8 pos);
void func_ov093_021ed318(BtlvScu *scu, u8 pos, u16 effect, BOOL immediate);
BOOL func_ov093_021ed374(BtlvScu *scu);
void func_ov093_021ed3d4(BtlvScu *scu, u8 pos, u8 clientId, u8 monId, BOOL noEffect);
BOOL func_ov093_021ed450(BtlvScu *scu);
BOOL func_ov093_021ecf50(BtlvScu *scu);
void func_ov093_021eda5c(BtlvScu *scu, u8 pos, BOOL flash);
BOOL func_ov093_021eda94(BtlvScu *scu, u8 pos);
void func_ov093_021edab0(BtlvScu *scu, u8 pos);
BOOL func_ov093_021edae0(BtlvScu *scu, u8 pos);
void func_ov093_021edb00(BtlvScu *scu, u8 pos);
void func_ov093_021ee200(BtlvScu *scu);
void func_ov093_021ed56c(BtlvScu *scu, u8 viewPos);
void func_ov093_021ed590(BtlvScu *scu, u8 pos, u32 arg2);
BOOL func_ov093_021ed5d8(BtlvScu *scu);
void func_ov093_021ed6bc(BtlvScu *scu, u32 index, BOOL flag);
void func_ov093_021ed660(BtlvScu *scu, u32 arg1, u32 index, BOOL flag);
void func_ov093_021ed8ac(BtlvScu *scu);
void func_ov093_021ed4e4(BtlvScu *scu, u8 pos1, u8 pos2);
BOOL func_ov093_021ed510(BtlvScu *scu, u8 pos1, u8 pos2);
void func_ov093_021ed53c(BtlvScu *scu, u8 pos);
BOOL func_ov093_021ed554(BtlvScu *scu, u8 pos);
void func_ov093_021ee264(BtlvScu *scu, BattleMon *mon, const BattleMonLevelUp *levelUp);
void func_ov093_021ee2f8(BtlvScu *scu);
void func_ov093_021ee228(BtlvScu *scu);
void func_ov093_021ee23c(BtlvScu *scu);
void func_ov093_021e8f08(void);
void *func_ov093_021e9100(BtlvCore *core, u32 size);
BattleMon *func_ov093_021e9634(BtlvCore *core, u32 index);
void func_ov093_021e9728(BtlvCore *core, BtlvMainProc proc);
void func_ov093_021e9730(BtlvCore *core);
BOOL func_ov093_021e9738(BtlvCore *core);
void func_ov093_021ea32c(BtlvCore *core, u32 type);
void func_ov093_021eaf00(BtlvCore *core, u32 se);

BOOL func_ov093_021eb52c(BtlvScu *scu);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void Btlv_StringParam_Setup(BtlvStringParam *param, u32 type, u16 message);
void Btlv_StringParam_AddArg(BtlvStringParam *param, u32 arg);
BtlvCore *BtlvCore_Create(BtlMainModule *mainModule, BtlClient *client, BtlPokeCon *pokeCon, u32 arg3, HeapID heapId);
void func_ov093_021e9034(BtlvCore *viewCore);
void func_ov093_021e908c(BtlvCore *viewCore);
void func_ov093_021e8e28(HeapID heapId);
void BattleClientCmd_StartPokeList(BtlvCore *core, const BtlvPokeListCmd *cmd, s32 partyIndex, u16 move,
                                   BtlvPokeSelectParam *select);
BOOL BattleClientCmd_WaitPokeSelect(BtlvCore *core);
BOOL func_ov093_021ea710(BtlvCore *core);
void func_ov093_021e9964(BtlvCore *core);
BOOL func_ov093_021e9974(BtlvCore *core);
BOOL func_ov093_021ea720(BtlvCore *core);
void func_ov093_021eabc4(BtlvCore *core, const BtlvStringParam *param1, const BtlvStringParam *param2, u32 arg3);
BOOL func_ov093_021eac54(BtlvCore *core, u32 *answer);
void func_ov093_021ea7fc(BtlvCore *core);
BOOL func_ov093_021ea814(BtlvCore *core);
void func_ov093_021ea824(BtlvCore *core);
void func_ov093_021ea458(BtlvCore *core, u8 pos, u16 arg2);
BOOL func_ov093_021ea478(BtlvCore *core);
void func_ov093_021ea678(BtlvCore *core, u16 message, const u32 *args);
void func_ov093_021ea694(BtlvCore *core, u16 message, const u32 *args);
void func_ov093_021ea0a0(BtlvCore *core, u8 attackerPos, u8 targetPos, u16 move, s32 arg4, u32 arg5, u8 arg6);
BOOL func_ov093_021ea0e4(BtlvCore *core);
BOOL func_ov093_021ea2bc(BtlvCore *core);
void func_ov093_021ea384(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea3a0(BtlvCore *core);
u32 func_ov093_021e98e8(BtlvCore *core);
void func_ov093_021e9920(BtlvCore *core);
void func_ov093_021e9954(BtlvCore *core);
void func_ov093_021ea438(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea448(BtlvCore *core);
BOOL func_ov093_021ea7e8(BtlvCore *core, u32 arg1);
void func_ov093_021ea838(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea850(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea950(BtlvCore *core);
void func_ov093_021ea8b4(BtlvCore *core, u32 arg1, u32 arg2);
BOOL func_ov093_021ea8d4(BtlvCore *core);
BOOL func_ov093_021ea1f0(BtlvCore *core);
void func_ov093_021ea8e4(BtlvCore *core);
BOOL func_ov093_021ea8f4(BtlvCore *core);
BOOL func_ov093_021ea3d4(BtlvCore *core);
void func_ov093_021eac64(BtlvCore *core, u8 partyIndex, u16 move);
BOOL func_ov093_021eac80(BtlvCore *core, u8 *slot);
BOOL func_ov093_021eaad8(BtlvCore *core);
void func_ov093_021ea768(BtlvCore *core, u8 pos, BOOL flash);
BOOL func_ov093_021ea778(BtlvCore *core, u8 pos);
void func_ov093_021ea7a8(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea7b8(BtlvCore *core, u8 pos);
void func_ov093_021ea868(BtlvCore *core, u8 pos, u32 arg2);
BOOL func_ov093_021ea878(BtlvCore *core);
BOOL func_ov093_021ea3f8(BtlvCore *core);
void func_ov093_021ea888(BtlvCore *core, u32 arg1);
BOOL func_ov093_021ea8a4(BtlvCore *core);
void func_ov093_021ea788(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea798(BtlvCore *core, u8 pos);
void BattleClientCmd_StartMoveInfoView(BtlvCore *core, u8 partyIndex, u8 slot);
void BattleClientCmd_StartItemSelect(BtlvCore *core, u32 mode, u8 arg2, u8 arg3, u8 canUseItems, u8 arg5);
void BattleClientCmd_ForceQuitInputNotify(BtlvCore *core);
BOOL BattleClientCmd_ForceQuitInputWait(BtlvCore *core);
extern const BGSysVRAMConfig data_ov093_021f387c;
// The battle's display: the BG, 2D and 3D systems, and the battle view and AI overlays
extern const BGSysLCDConfig data_ov093_021f3850;
extern const ClActSysSetup data_ov093_021f3860;
BOOL func_ov093_021ed280(BtlvScu *scu);
BOOL func_ov093_021ecf68(BtlvScu *scu);
BOOL func_ov093_021ed4d0(BtlvScu *scu);
BOOL func_ov093_021ed2bc(BtlvScu *scu);
BOOL func_ov093_021ed304(BtlvScu *scu);
void func_ov093_021eceb8(BtlvScu *scu, const StrBuf *strbuf, u16 wait, BtlvMsgCallback callback);
void func_ov093_021ece60(BtlvScu *scu, const StrBuf *strbuf);
BOOL func_ov093_021edb20(BtlvScu *scu, u8 pos);
BOOL func_ov093_021ee21c(BtlvScu *scu);
void func_ov093_021ee224(BtlvScu *scu);
BOOL func_ov093_021ed57c(BtlvScu *scu, u8 viewPos);
BOOL func_ov093_021ed714(BtlvScu *scu);
BOOL func_ov093_021ed8d8(BtlvScu *scu);
BOOL func_ov093_021ee2a0(BtlvScu *scu);
BOOL func_ov093_021ee318(BtlvScu *scu);
void func_ov093_021ee370(BtlvScu *scu);
BOOL func_ov093_021ee390(BtlvScu *scu);
BOOL func_ov093_021ee25c(BtlvScu *scu);
void func_ov093_021e8dc4(HeapID heapId);
void func_ov093_021e8df8(void);
void func_ov093_021e8e38(HeapID heapId);
BOOL func_ov093_021e964c(BtlvCore *core, s32 *seq, void *work);
BOOL func_ov093_021e96c4(BtlvCore *core, s32 *seq, void *work);
void func_ov093_021e9760(BtlvCore *core, BattleMon *mon, u32 arg2, void *arg3);
void func_ov093_021e979c(BtlvCore *core);
void func_ov093_021e97bc(BtlvCore *core, BattleMon *mon, void *arg2);
void func_ov093_021e9808(BtlvCore *core, BtlvSelectTargetParam *param, void *arg2);
void func_ov093_021e9858(BtlvCore *core);
BOOL func_ov093_021e9884(BtlvCore *core, s32 *seq, void *work);
void func_ov093_021e98ac(BtlvCore *core);
void func_ov093_021e9928(BtlvCore *core);
BOOL func_ov093_021e98b4(BtlvCore *core);
BOOL func_ov093_021e9e80(BtlvCore *core);
BOOL func_ov093_021ea160(s32 *seq, void *arg);
BOOL func_ov093_021ea230(s32 *seq, void *arg);
void func_ov093_021ea40c(BtlvCore *core, u8 pos);
BOOL func_ov093_021ea428(BtlvCore *core);
u8 func_ov093_021ea044(BtlvCore *core);
u8 func_ov093_021ea04c(BtlvCore *core);
BOOL func_ov093_021ea4c4(BtlvCore *core);
BOOL func_ov093_021ea504(s32 *seq, void *arg);
void func_ov093_021ea55c(const BtlvStringParam *param, StrBuf *strbuf);
BOOL func_ov093_021ea6b0(BtlvCore *core, u16 trainerId, u16 msgId);
void func_ov093_021ea730(BtlvCore *core, const StrBuf *strbuf, u16 wait, BtlvMsgCallback callback);
void func_ov093_021ea758(BtlvCore *core, const StrBuf *strbuf);
BOOL func_ov093_021ea990(s32 *seq, void *arg);
void func_ov093_021eaa00(BtlvCore *core, u8 arg1);
BOOL func_ov093_021eaa34(BtlvCore *core, u8 *out);
void func_ov093_021eaa5c(BtlvCore *core);
BOOL func_ov093_021eab18(s32 *seq, void *arg);
void func_ov093_021eada4(BtlvCore *core, BattleMon *mon, const BattleMonLevelUp *levelUp);
BOOL func_ov093_021eadb4(BtlvCore *core);
void func_ov093_021eadc4(BtlvCore *core);
BOOL func_ov093_021eadd4(BtlvCore *core);
void func_ov093_021eade4(BtlvCore *core);
BOOL func_ov093_021eadf4(BtlvCore *core);
void func_ov093_021eae04(BtlvCore *core);
BOOL func_ov093_021eae14(BtlvCore *core);
void func_ov093_021eae24(BtlvCore *core);
BOOL func_ov093_021eae34(BtlvCore *core);
u32 func_ov093_021eae64(BtlvCore *core);
void func_ov093_021eae74(BtlvCore *core, u16 arg1, u16 arg2);
void func_ov093_021eaea8(BtlvCore *core, u16 arg1);
void func_ov093_021eaecc(BtlvCore *core, u32 arg1, u32 arg2);
// btlv_scu.c, the message and command unit of the battle display
BtlvScu *func_ov093_021eaf7c(BtlvCore *core, BtlMainModule *mainModule, BtlPokeCon *pokeCon, TCBExManager *tcbManager,
                             Font *font, Font *smallFont, u8 clientId, HeapID heapId);
void func_ov093_021eb028(BtlvScu *scu);
void func_ov093_021eb2a8(const BtlvScu *scu);
void func_ov093_021eb32c(BtlvScu *scu, BOOL recPlay);
BOOL func_ov093_021ed1b0(BtlvScu *scu);

BtlvScu *func_ov093_021eaf7c(BtlvCore *core, BtlMainModule *mainModule, BtlPokeCon *pokeCon, TCBExManager *tcbManager,
                             Font *font, Font *font2, u8 clientId, HeapID heapId);

void func_ov093_021eb32c(BtlvScu *scu, BOOL arg1);

#endif // BWPORT_BATTLE_BTLV_H
