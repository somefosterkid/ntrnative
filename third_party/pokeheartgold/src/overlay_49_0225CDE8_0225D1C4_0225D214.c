#include "global.h"
#include "battle/battle.h"
#include "overlay_08_battle_party.h"

struct ActionOrderEntry;

struct ArcTool;

struct BattleCondition;

struct BattleEventHandlerEntry;

struct BattleEventItem;

struct BattleHandlerString;

struct BattleMon;

struct BattleMonCore;

struct BattleMonDamageRecord;

struct BattleMonLevelUp;

struct BattleMonStatStages;

struct BattleMoveCore;

struct BattleMoveEffectState;

struct BattleMoveWork;

struct BattleParty;

struct BoxPkm;

struct BtlActionState;

struct BtlClient;

struct BtlClientIDList;

struct BtlFaintRec;

struct BtlFaintRecTurn;

struct BtlField;

struct BtlFlowClientList;

struct BtlFlowDamageEntry;

struct BtlFlowDamageList;

struct BtlFlowExpEntry;

struct BtlFlowHitWork;

struct BtlFlowMoveParam;

struct BtlFlowReactionList;

struct BtlFlowUnk1B50;

struct BtlFlowWorkFrame;

struct BtlMainModule;

struct BtlMainSeq;

struct BtlMainSyncData;

struct BtlMonSet;

struct BtlPokeCon;

struct BtlPosPoke;

struct BtlPosPokeEntry;

struct BtlRecReader;

struct BtlServer;

struct BtlServerCmdQueue;

struct BtlServerFlow;

struct BtlSetup;

struct BtlTrainerData;

struct BtlWazaRec;

struct BtlWazaRecord;

struct BtlvCore;

struct MailData;

struct PMSData;

struct PartyPkm;

struct PlayerInfo;

struct PokeParty;

struct StrBuf;

typedef union BattleAction BattleAction;

typedef struct BattleCondition BattleCondition;

typedef struct ArcTool ArcTool;

typedef struct BoxPkm BoxPkm;

typedef struct BtlSetup BtlSetup;

typedef struct PartyPkm PartyPkm;

typedef struct PlayerInfo PlayerInfo;

typedef struct PMSData PMSData;

typedef struct PokeParty PokeParty;

typedef struct StrBuf StrBuf;

typedef struct BattleMonDamageRecord BattleMonDamageRecord;

typedef struct BtlFlowMoveParam BtlFlowMoveParam;

typedef struct MailData MailData;

typedef struct ActionOrderEntry ActionOrderEntry;

typedef struct BattleEventHandlerEntry BattleEventHandlerEntry;

typedef struct BattleEventItem BattleEventItem;

typedef struct BattleHandlerString BattleHandlerString;

typedef struct BattleMoveCore BattleMoveCore;

typedef struct BattleMoveWork BattleMoveWork;

typedef struct BtlMainModule BtlMainModule;

typedef struct BtlClient BtlClient;

typedef struct BtlField BtlField;

typedef struct BtlPokeCon BtlPokeCon;

typedef struct BtlServer BtlServer;

typedef struct BtlServerFlow BtlServerFlow;

typedef struct BtlvCore BtlvCore;

typedef struct BtlActionState BtlActionState;

typedef struct BtlRecReader BtlRecReader;

typedef struct BattleMonStatStages BattleMonStatStages;

typedef struct BattleMonCore BattleMonCore;

typedef struct BattleMonLevelUp BattleMonLevelUp;

typedef struct BtlMainSeq BtlMainSeq;

typedef struct BtlTrainerData BtlTrainerData;

typedef struct BtlClientIDList BtlClientIDList;

typedef struct BtlMainSyncData BtlMainSyncData;

typedef struct BtlWazaRec BtlWazaRec;

typedef struct BtlFaintRec BtlFaintRec;

typedef struct BtlServerCmdQueue BtlServerCmdQueue;

typedef struct BtlFlowClientList BtlFlowClientList;

typedef struct BtlFlowDamageEntry BtlFlowDamageEntry;

typedef struct BtlFlowDamageList BtlFlowDamageList;

typedef struct BtlFlowExpEntry BtlFlowExpEntry;

typedef struct BtlFlowHitWork BtlFlowHitWork;

typedef struct BtlFlowReactionList BtlFlowReactionList;

typedef struct BtlFlowUnk1B50 BtlFlowUnk1B50;

typedef struct BtlFlowWorkFrame BtlFlowWorkFrame;

typedef struct BtlWazaRecord BtlWazaRecord;

typedef struct BtlFaintRecTurn BtlFaintRecTurn;

typedef struct BtlPosPoke BtlPosPoke;

typedef struct BtlPosPokeEntry BtlPosPokeEntry;

typedef struct BtlMonSet BtlMonSet;

typedef struct BattleMoveEffectState BattleMoveEffectState;

union BattleAction {
    u32 raw;
    struct {
        u32 action : 4;
        u32 target : 3;
        u32 move : 16;
        u32 unk23 : 3;
        u32 unk26 : 1;
        u32 unk27 : 5;
    } bits;

    struct {
        u32 action : 4;
        u32 target : 3;
        u32 item : 16;
        u32 param : 8;
        u32 unk31 : 1;
    } item;

    struct {
        u32 action : 4;
        u32 unk4 : 3;
        u32 slot : 3;
        u32 unk10 : 1;
        u32 unk11 : 21;
    } change;
};

typedef u16 HeapID;

struct BattleCondition {
    union {
        u32 raw;
        struct {
            u32 type : 3;
            u32 turns : 6;
            u32 unk09 : 23;
        } common;
        struct {
            u32 type : 3;
            u32 turns : 6;
            u32 param : 16;
            u32 unk25 : 7;
        } timed;
        struct {
            u32 unk00 : 9;
            u32 monId : 6;
            u32 unk15 : 17;
        } mon;
        struct {
            u32 unk00 : 25;
            u32 flag : 1;
            u32 unk26 : 6;
        } one;
        struct {
            u32 unk00 : 31;
            u32 flag : 1;
        } four;
    };
};

typedef u16 PokeTypePair;

struct BattleMoveCore {
    u16 id;
    union {
        struct {
            u8 pp;
            u8 maxPP;
        };
        u16 ppPair;
    };
    union {
        struct {
            u8 ppUp;
            u8 flagsLow : 4;
            u8 flagsHigh : 4;
        };
        u16 flagsPair;
    };
};

struct BattleMoveWork {
    BattleMoveCore truth;
    BattleMoveCore surface;

    u8 linked;
};

struct BattleMonStatStages {
    s8 attack;
    s8 defense;
    s8 spAttack;
    s8 spDefense;
    s8 speed;
    s8 accuracy;
    s8 evasion;
};

struct BattleMonDamageRecord {
    u16 move;
    u16 damage;
    u8 type;
    u8 attackerId;
    u8 attackerPos;
    u8 unk7;
};

typedef enum {
    CONDITION_ID_COUNT = 36,
} BattleConditionID;

struct BattleMonCore {
    PartyPkm *src;
    PartyPkm *illusionDisguise;
    u32 exp;
    u16 species;
    u16 maxHP;
    u16 hp;
    u16 heldItem;
    u16 consumedItem;
    u16 baseAbility;
    u8 level;
    u8 monId;
    u8 unk1a;
    u8 baseForm : 5;
    u8 transformed : 1;
    u8 illusion : 1;
    u8 unk1b_7 : 1;
    BattleCondition conditions[CONDITION_ID_COUNT];
    u8 conditionCounters[CONDITION_ID_COUNT];

    u8 unkD0Count;
    u8 unkD1[24];
};

struct BattleMonLevelUp {
    u8 level;
    u16 hp;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
};

struct BtlClientIDList {
    u32 count;
    u8 clientIds[4];
};

typedef BOOL (*BattleEventSkipCheckFn)(BattleEventItem *item, BtlServerFlow *flow, u32 factorType, u32 event, u16 subId, u8 monId);

typedef void (*BattleEventHandlerFn)(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

struct BattleEventHandlerEntry {
    u32 event;
    BattleEventHandlerFn handler;
};

struct BattleEventItem {
    BattleEventItem *prev;
    BattleEventItem *next;
    const BattleEventHandlerEntry *handlers;
    BattleEventSkipCheckFn skipCheck;
    u32 factorType;

    u32 priority;

    u32 callDepth : 16;
    u32 numHandlers : 8;

    u32 running : 1;

    u32 unk25 : 1;
    u32 tempItem : 1;

    u32 removeReserved : 1;

    u32 recallEnable : 1;
    u32 active : 1;

    u32 sleeping : 1;
    u32 unk31 : 1;
    u32 work[7];
    u16 subId;
    u8 monId;
    u8 dependMonId;
};

struct ActionOrderEntry {
    BattleMon *mon;
    BattleAction action;

    u32 key;
    u8 clientId;
    u8 done;

    u8 interrupting;
    u8 unk0F;
};

struct BtlFaintRecTurn {
    u8 count;
    u8 handled[24];
    u8 monIds[24];
};

struct BtlFaintRec {
    BtlFaintRecTurn turns[4];
};

struct BtlActionState {
    union {
        u32 raw;
        struct {
            u32 useItemNo : 10;

            u32 workPos : 9;
            u32 savedPos : 9;
            u32 prevResult : 1;
            u32 result : 1;
            u32 used : 1;
            u32 unk31 : 1;
        };
    };

    u8 work[500];
};

struct BtlMonSet {
    BattleMon *mons[6];
    u16 damage[6];
    u16 substituteDamage[6];
    u8 damageKind[6];
    u16 speed[6];
    u8 count;
    u8 countMax;
    u8 getIndex;
    u8 numTargets;
};

struct BtlPosPokeEntry {
    u8 exists;
    u8 clientId;
    u8 monId;
};

struct BtlPosPoke {
    BtlPosPokeEntry entries[6];
    u8 posOfMon[6];
};

struct BtlWazaRecord {
    u32 turn;
    u16 move;
    u8 monId;
    u8 succeeded;
};

struct BtlWazaRec {
    u32 head;
    BtlWazaRecord records[120];
};

struct BattleHandlerString {
    u16 message;
    union {
        u16 flags;
        struct {
            u16 enabled : 8;
            u16 count : 7;
            u16 hasSound : 1;
        };
    };
    union {
        u32 args[8];

        struct {
            u32 unused_args[7];
            u32 soundEffect;
        };
    };
};

struct BtlFlowMoveParam {
    u16 move;
    u16 originalMove;
    PokeTypePair userType;
    u8 type;
    u8 unk07;
    u32 category;

    u32 targetType;
    union {
        u32 raw;
        struct {
            u32 unk0 : 1;
            u32 unk1 : 31;
        };
    } flags;
};

struct BtlFlowDamageEntry {
    u16 damage;
    u16 monId : 5;
    u16 effectiveness : 4;
    u16 unk9 : 4;
    u16 critical : 1;
    u16 fixedDamage : 1;
    u16 substitute : 1;
};

struct BtlFlowDamageList {
    u8 count;
    u8 substituteCount;
    u8 total;
    u8 unk03;
    BtlFlowDamageEntry entries[6];
};

struct BattleMoveEffectState {
    u16 unk00;
    u8 pos1;
    u8 pos2;
    u8 index;
    u8 enabled : 1;
    u8 unk05_1 : 1;
    u8 unk05_2 : 6;
};

struct BtlFlowHitWork {
    u8 count;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    u8 unk04;
    u8 unk05;
};

struct BtlFlowExpEntry {
    u32 exp;

    u8 boosted;

    u8 hp;
    u8 attack;
    u8 defense;
    u8 speed;
    u8 spAttack;
    u8 spDefense;
    u8 unk0B;
};

struct BtlFlowUnk1B50 {
    u8 unk000[0x220];
    u32 unk220;
};

struct BtlFlowReactionList {
    u8 count;
    u8 monIds[6];
    u8 targets[6];
    u8 unk0D[6];
};

struct BtlFlowClientList {
    u8 count;
    u8 clientIds[4];
    u8 unk05;
};

struct BtlFlowWorkFrame {

    BtlMonSet monSets[7];
    BattleMoveEffectState moveEffect;
    BtlFlowMoveParam moveParams[2];
    BtlFlowHitWork hitWork;
    BtlFlowReactionList reactionLists[2];
    BtlFlowDamageList damageLists[2];
    u8 unk28C;
    u8 unk28D;
    u8 unk28E;
};

struct BtlServerFlow {
    BtlServer *server;
    BtlMainModule *mainModule;
    BtlPokeCon *pokeCon;
    BtlServerCmdQueue *queue;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    BtlWazaRec wazaRec;
    BtlFaintRec faintRec;
    ArcTool *unk4A4;
    BattleMoveEffectState *moveEffect;
    BtlFlowReactionList *unk4AC;
    BtlFlowReactionList *unk4B0;
    BtlFlowHitWork *unk4B4;
    BtlClientIDList clientIdList;
    BattleMonLevelUp levelUp;
    BtlFlowClientList unk4CE;
    u8 unk4D4[0x2a0];
    u32 unk774;
    u32 unk778;
    u8 unk77C;
    u8 unk77D;
    u8 unk77E;
    u8 unk77F;
    HeapID heapId;
    u8 actionOrderCount;
    u8 unk783;
    u8 unk784;
    u8 unk785;
    u8 unk786;
    u8 unk787;

    u8 interruptCount;
    u8 unk789;
    u8 unk78A_0 : 1;
    u8 unk78A_1 : 1;
    u8 unk78A_2 : 1;
    u8 unk78A_3 : 1;
    u8 unk78A_4 : 1;
    u8 unk78A_5 : 1;
    u8 unk78A_6 : 1;
    u8 unk78A_7 : 1;
    u8 interruptMonIds[6];
    u8 unk791[0x18];

    u8 unk7A9[24];
    u8 unk7C1[24];
    u8 unk7D9[4];
    u8 unk7DD[3];
    ActionOrderEntry actionOrder[6];

    ActionOrderEntry tempEntry;

    void *unk850;
    void *unk854;
    void *unk858;
    void *unk85C;
    void *unk860;
    void *unk864;
    void *unk868;
    BtlFlowDamageList *unk86C;
    BtlFlowDamageList *unk870;
    BtlFlowWorkFrame frames[7];
    u32 frameDepth;

    BtlMonSet unk1A68;
    BtlFlowMoveParam *unk1AB0;
    BtlFlowMoveParam *unk1AB4;
    BtlPosPoke posPoke;
    u8 unk1AD0[0x14];
    BattleHandlerString message;
    BtlFlowExpEntry expEntries[6];
    BtlFlowUnk1B50 unk1B50;
    BtlActionState actionState;
    u8 unk1F6C[8];
    u16 unk1F74;
    u8 unk1F76[2];
    u32 unk1F78;
    u16 unk1F7C;
    u16 unk1F7E;

    u16 unk1F80[4];
    u32 unk1F88[24];
    u8 unk1FE8[4];
    u8 unk1FEC[0x140];
};

struct BtlRecReader {
    const u8 *data;
    u32 size : 31;

    u32 error : 1;

    u32 pos[4];

    BattleAction actions[4][16];
};

struct PMSData {

    u16 type;
    u16 id;
    u16 words[2];
};

struct BtlPokeCon {
    BtlMainModule *mainModule;
    BattleParty parties[4];

    PokeParty *srcParties[4];
    BattleMon *mons[24];
    u32 unkE4;
};

typedef BOOL (*BtlMainSeqFunc)(u32 *state, BtlMainModule *mainModule);

struct BtlMainSeq {
    BtlMainSeqFunc func;
    BtlMainSeqFunc nextFunc;
    BtlMainModule *mainModule;
    u32 state;
};

struct BtlMainSyncData {
    MATHRandContext32 rand;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F_0 : 4;
    u8 unk1F_4 : 4;
};

struct BtlTrainerData {
    PlayerInfo *playerInfo;
    StrBuf *name;
    u16 unk08;
    u16 unk0A;
    u32 unk0C;
    u16 unk10[4];
    PMSData unk18;
    PMSData unk20;
};

struct BtlMainModule {
    BtlSetup *setup;
    BtlvCore *viewCore;
    BtlServer *server;
    BtlServer *unk0C;
    BtlClient *clients[4];
    BtlTrainerData trainers[4];
    PlayerInfo *unkC0;
    u8 unkC4[4];
    BtlPokeCon pokeCons[2];
    PokeParty *unk298[4];
    PokeParty *unk2A8[4];
    PokeParty *unk2B8;
    BtlField *field;
    PartyPkm *unk2C0;
    void *unk2C4;
    BtlRecReader recReader;
    void *unk3E0[4];
    MATHRandContext32 rand;

    BtlMainSyncData syncData;
    u8 posClientIds[6];
    u8 unk42e[2];

    u32 prizeMoney;
    u32 money;
    u32 unk438;
    u32 unk43C;
    u16 unk440;
    u16 unk442;
    u32 result;
    BtlClientIDList unk448;
    BtlMainSeq seq;
    s32 unk460;
    BOOL(*mainFunc)
    (BtlMainModule *mainModule);
    u16 heapId;
    u8 unk46A;
    u8 clientCount;
    u8 playerClientId;
    u8 unk46D;
    u8 unk46E;
    u8 unk46F;
    u8 unk470;
    u8 unk471;
    u8 unk472;
    u8 unk473_0 : 1;
    u8 unk473_1 : 1;
    u8 unk473_2 : 1;
    u8 unk473_3 : 1;
    u8 unk473_4 : 1;
    u8 unk473_5 : 1;
    u8 unk473_6 : 1;
    u8 unk473_7 : 1;
};

struct MailData {
    u32 trainerId;
    u8 trainerGender;
    u8 region;
    u8 gameVersion;
    u8 unk07;
    u16 trainerName[8];
    u16 unk18[3];

    u16 unk1E;
    PMSData message[3];
};

typedef struct {
    u8 rawData[32];
} PkmBufferChunk;

typedef struct {
    PkmBufferChunk chunks[4];
} PkmBuffer;

struct BoxPkm {
    u32 pid;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 badEgg : 1;
    u16 unk4_3 : 13;
    u16 checksum;
    PkmBuffer contentBuffer;
};

struct PartyPkm {
    BoxPkm base;
    u32 statusCond;
    u8 level;
    u8 unk8D;
    u16 nowHP;
    u16 maxHP;
    u16 atk;
    u16 def;
    u16 spe;
    u16 spa;
    u16 spd;
    MailData mail;
    u32 unkD4;
    u32 unkD8;
};

struct PokeParty {
    s32 capacity;
    s32 count;
    PartyPkm pkms[6];
    u8 slotExists;
};

void ov49_0225D224(BattleEventItem *item, BtlServerFlow *flow, int monId, s32 *work, u8 type);

void ov49_0225D214(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

void ov49_0225D214(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    ov49_0225D224(item, flow, monId, work, 0);
}
