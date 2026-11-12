// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_comm.h: the lines in use so far.
#ifndef BWPORT_FIELD_UNION_COMM_H
#define BWPORT_FIELD_UNION_COMM_H

#include "types.h"
#include "dwc/dwc.h"
#include "field/field.h"
#include "field/trcard_sys.h"
#include "system/pms_data.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of mac and used in Black
// A player met, in a group's list and in the beacon
struct UnionMember {
    u8 mac[6];
    u8 look; // func_02008bf4's appearance
    u8 gender : 1;
    u8 used : 1;
};

#define UNION_MEMBER_MAX 5

// Black 2's, not checked against Black
// All the players of a list at once, to copy it
struct UnionMemberList {
    UnionMember list[UNION_MEMBER_MAX];
};

// Black 2's; twinned code confirms the offsets of look, gender, name, members and trainerId in Black
// The beacon this machine sends
struct UnionBeacon {
    u8 mac[6]; // its own, or the parent's when a child
    u8 memberCount : 3;
    u8 entryClosed : 1;
    u8 look : 4;
    u8 state : 3;
    u8 gender : 1;
    u8 valid : 1;
    u8 unk07_5 : 3;
    u8 version;
    u8 language;
    u8 unk0A;
    u8 activity;
    u16 name[8];
    u16 look2[4];
    u16 counter;
    u8 country;
    u8 province;
    union {
        UnionMember members[UNION_MEMBER_MAX];
        UnionMemberList memberList;
    };
    // The friend data token of the player, for the Union Room
    DWCFriendData unk50;
    u32 trainerId;
    u8 unk60[4];
};

// Black 2's; twinned code confirms the offsets of playerInfo, gameData and gsys in Black
// What UnionMain_Boot boots the communication with
struct UnionBootParam {
    PlayerInfo *playerInfo;
    GameCommSys *comm;
    GameData *gameData;
    GameSystem *gsys;
};

// Black 2's; twinned code confirms the offsets of entry, member, step, nextStep, phase and seq in Black
struct UnionActorSlot {
    struct UnionBeaconEntry *entry;
    u8 look;
    u8 gender;
    u8 member; // in entry->beacon.members
    u8 used;
    u8 step;
    u8 nextStep;
    u8 phase;
    u8 seq;
};

// Black 2's, not checked against Black
#define UNION_ACTOR_MAX 50

// Black 2's; twinned code confirms the offsets of beacon, mac, index, timeout and actor in Black
// A beacon heard from another machine
struct UnionBeaconEntry {
    UnionBeacon beacon;
    u8 mac[6];
    u8 status; // 1 when new, 2 when heard again
    u8 index;
    u16 timeout; // frames until it is forgotten, reset on each beacon
    UnionActorSlot *actor[UNION_MEMBER_MAX];
    u8 unk84[8];
};

// Black 2's; twinned code confirms the offsets of target, unk4, answer_pc, parent, unk10, unk19, unk1A, members, unk44,
// unk46_0, unk46_2, unk47, doneMask, step and active in Black
// The players this machine is grouped with
struct UnionGroup {
    // The player spoken to, the one who answered, and the parent joined (the names of the third are the ROM's)
    UnionBeaconEntry *target;
    UnionBeaconEntry *unk4;
    UnionBeaconEntry *answer_pc;
    UnionBeaconEntry *parent;
    u8 unk10[8];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    union {
        UnionMember members[UNION_MEMBER_MAX];
        UnionMemberList memberList;
    };
    u16 unk44;
    u8 unk46_0 : 1;
    u8 unk46_1 : 1;
    u8 unk46_2 : 2;
    u8 unk46_4 : 4;
    u8 unk47;
    u8 unk48;
    u8 recvFlag;
    u8 doneMask; // a bit per net ID
    u8 step : 7;
    u8 active : 1;
};

// Black's layout, 0x7c bytes: union_oneself.c's matching steps confirm the offsets of every field but unk10
// This machine's state, which its beacon is built from
struct UnionSelf {
    u8 activity;
    u8 step; // 0x2a when idle; the beacon sends it
    u8 unk02;
    u16 look[4];
    u16 counter;
    u8 changed;
    u8 state;
    u8 unk10[4];
    UnionGroup mycomm; // the ROM's name
    u8 ruleSel[4]; // the battle format, two options and the menu kind
    u32 partyCheck; // the regulation's checks of the party and of the Battle Box
    u32 boxCheck;
    s16 wait; // frames
    s16 result;
    u8 prevStep;
    u8 nextStep;
    u8 phase; // of the step: 0 entering, 1 running, 2 leaving
    u8 seq;
    u8 mac78[6]; // the player to show the focus effect on
};

// Black 2's; twinned code confirms the offsets of mac in Black
// A line of the Union Room's chat: what a player said, as the touch screen shows it, in a ring of lines
struct UnionVisitor {
    u16 name[8];
    // What the player said, when kind is UNION_CHAT_KIND_SENTENCE
    PMSData sentence;
    u16 unk18;
    u8 mac[6]; // of the player, who is looked up by it
    u8 gender;
    u8 kind; // UNION_CHAT_KIND_*
    u8 subKind; // 0, or 1 for a friend and 2 for another player
    u8 unk23;
};

#define UNION_VISITOR_MAX 30

// Black 2's; twinned code confirms the offsets of visitors, count, viewEnd and drawnEnd in Black
struct UnionVisitorList {
    UnionVisitor visitors[UNION_VISITOR_MAX];
    u8 unk438[4];
    // How many lines were added, the newest line the touch screen shows at its bottom, and the one it showed before
    s32 count;
    s32 viewEnd;
    s32 drawnEnd;
};

// Black 2's, not checked against Black
// The command the system is running: a row of union_main.c's sUnionCommands, or UNION_SUBPROC_ID_NULL
struct UnionSubproc {
    BOOL active;
    u32 id;
    void *arg;
    u8 step;
};

// Black 2's, not checked against Black
#define UNION_BEACON_ENTRY_MAX 10

// Black 2's; twinned code confirms the offsets of param, wordset, msgdata, fld_msgwin_stream, fldmenu_func,
// fldmenu_yesno_func, fldmsgwin, msgbuf, fmtbuf, msgPrinting, fieldActive, entryReply, entryAcceptedTo, entryRepliedTo,
// unk282E and situ in Black
struct UnionSystem {
    UnionBootParam *param;
    u8 recvBuf[5][0x800]; // a buffer per net ID
    // The names of these six are the ROM's, from overlay 34's asserts
    WordSet *wordset;
    MsgData *msgdata;
    void *fld_msgwin_stream;
    ListMenuUI *fldmenu_func;
    void *fldmenu_yesno_func;
    void *fldmsgwin;
    u32 unk281C;
    StrBuf *msgbuf; // the message, and with its words set
    StrBuf *fmtbuf;
    u8 msgPrinting;
    u8 beaconReady;
    u8 fieldActive;
    u8 entryReply; // 1 when this machine's entry request was accepted, 2 when refused
    u8 entryAcceptedTo; // a bit per net ID
    u8 entryRepliedTo;
    u8 unk282E;
    u8 unk282F;
    UnionSelf situ; // the ROM's name for it
    UnionBeacon beacon;
    UnionBeaconEntry found[UNION_BEACON_ENTRY_MAX];
    UnionActorSlot actors[UNION_ACTOR_MAX];
    UnionVisitorList visitorList;
    // What the system allocates as players meet (the names are the ROM's, from its asserts). The trainer cards are sent
    // whole
    struct {
        UnionApp *uniapp;
        Regulation *regulation;
        void *rpm; // the regulation's rules printed, from PokeRegulation_CreatePrintMsg (the name is the ROM's)
        void *partySelect;
        PokeParty *boxParty; // the Battle Box as a party
        TrainerCardParam *cardParam;
        TrainerCardData *my_card;
        TrainerCardData *target_card;
    } alloc;
    UnionSubproc subproc;
    void *cmdArg; // what overlay 34 allocates for the command it starts
    UnionColosseum *colosseum_sys;
    u8 commState;
    u8 linkState; // 0 when idle, then the steps of UnionComm_UpdateLink's restart of the communication
    u8 linkMode;
    u8 updateDisabled;
    u32 unk3564;
    u32 unk3568;
    u32 unk356C;
};

// Black 2's, not checked against Black
typedef BOOL (*UnionSituFunc)(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);

// Black 2's, not checked against Black
#define UNION_SUBPROC_ID_NULL 0

// pokebw2's UnionComm_SetSelf
// Sets a part of this machine's state: a group's target 0 to 3 (a UnionBeaconEntry *), or the activity for 4
void func_ov012_021734a8(UnionSystem *sys, u32 which, uintptr_t value);
// pokebw2's UnionComm_SetLook
// Sets this machine's look, and its state; both make the beacon change
void func_ov012_021736a0(UnionSystem *sys, const u16 *look);
// pokebw2's UnionGroup_ResetTargets
// Resets a group, with this machine as its first member
void func_ov012_02173518(UnionGroup *group);
// pokebw2's UnionGroup_AddBeaconMember
void func_ov012_021735fc(UnionGroup *group, const UnionBeaconEntry *entry);
// pokebw2's UnionComm_SetCommandBase
// Sets the net game command base for an activity
void func_ov012_021736e0(u32 activity);
// pokebw2's UnionComm_ActivateGroup
void func_ov012_02172edc(UnionSystem *sys);
// pokebw2's UnionGroup_RemoveBeaconMember
void func_ov012_02173650(UnionGroup *group, const UnionBeaconEntry *entry);
// pokebw2's UnionGroup_RemoveMember
void func_ov012_02173614(UnionGroup *group, const u8 *mac);

// Black 2's, not checked against Black
// What a line of the chat says: the player's sentence, or that it entered the Union Room, or what the player wants to do
#define UNION_CHAT_KIND_SENTENCE 0

// Black 2's, not checked against Black
#define UNION_CHAT_KIND_ENTERED 1
#define UNION_CHAT_KIND_BATTLE 2
#define UNION_CHAT_KIND_TRADE 3
#define UNION_CHAT_KIND_DRAW 4
#define UNION_CHAT_KIND_SPIN_TRADE 5

// pokebw2's UnionComm_SetState
void func_ov012_021736c8(UnionSystem *sys, u8 state);

// pokebw2's UnionChat_GetLine
// The line of the ring that the index calls for, or NULL if it is empty
UnionVisitor *func_ov021_021bbc2c(UnionVisitorList *list, s32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UnionGroup_Init
void func_ov012_02173524(UnionSystem *sys, UnionGroup *group);
// pokebw2's UnionComm_IsLinkRequested
BOOL func_ov012_02172d68(UnionSystem *sys);
// pokebw2's UnionComm_RequestLink0
// Asks for the communication to restart in a mode, and whether it was asked to
void func_ov012_02172d20(UnionSystem *sys);
// pokebw2's UnionComm_RequestLink1
void func_ov012_02172d38(UnionSystem *sys);
// Clears the beacons heard (pokebw2's UnionComm_ClearBeacons)
void func_ov012_02173408(UnionSystem *sys);
// pokebw2's UnionComm_FreeResources
void func_ov012_02173420(UnionSystem *sys);
// pokebw2's UnionComm_RequestLink2
void func_ov012_02172d50(UnionSystem *sys);
// pokebw2's UnionGroup_AddMember
// Adds a player to the group's list, and removes one; the entry forms take the player from a beacon heard
int func_ov012_0217357c(UnionGroup *group, const u8 *mac, u8 look, u8 gender);
// pokebw2's UnionComm_Create
// Allocates the system for the boot parameters
UnionSystem *func_ov012_02172bc0(UnionBootParam *param);
// pokebw2's UnionComm_Init
// The GameCommSys callbacks for GAME_COMM_NO_UNION
void *func_ov012_02172c4c(u32 *seq, void *param);
// pokebw2's UnionComm_InitWait
BOOL func_ov012_02172c78(u32 *seq, void *param, void *work);
// pokebw2's UnionComm_OnNetReady
// Called once the communication is up, and once it has ended
void func_ov012_02172c98(void *work);
// pokebw2's UnionComm_Exit
BOOL func_ov012_02172cac(u32 *seq, void *param, void *work);
// pokebw2's UnionComm_ExitWait
BOOL func_ov012_02172ce8(u32 *seq, void *param, void *work);
// pokebw2's UnionComm_Main
void func_ov012_02172e48(u32 *seq, void *param, void *work);

#endif // BWPORT_FIELD_UNION_COMM_H
