// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_game_clear.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_GAME_CLEAR_H
#define BWPORT_FIELD_EVENT_GAME_CLEAR_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "app/unova_link.h"
#include "system/game_event.h"

// Loads overlay 20 for the event it creates
GameEvent *EventMapChangeBlackout_CreateExternal(GameSystem *gsys);

// Black 2's with 16 states, not 31 (Black allocates 0x7c: the counter is at 0x78); twinned code confirms the offsets
// of gameData and ov265Param in Black
struct GameClearWork {
    GameSystem *gameSystem;
    GameData *gameData;
    u32 unk08;
    u8 unk0C[4];
    PlayerInfo *unk10;
    struct {
        GameSystem *gsys;
        u32 param;
    } ov295Param;
    struct {
        PokeParty *party;
        PlayerInfo *playerInfo;
        PlayTime *playTime;
    } ov265Param;
    // The staff roll's (overlay 181): Black's has no player info (Black 2's StaffRollParam has it at 0x30)
    struct {
        BOOL unk00;
        u32 unk04;
    } ov266Param;
    // The end's (overlay 182), pokebw2's ov267Param
    struct {
        u32 unk00;
    } ov267Param;
    // The state that runs, from the sequence that SetGameClearStatusSequence builds
    u32 current;
    u32 states[16];
    // The frames the event waits before the end (state 12)
    s32 counter;
}; // size: 0x7c

void SetGameClearGameData(GameClearWork *work);
void func_ov010_0215f9ec(GameClearWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventGameClear_Create(GameSystem *gsys, u32 param);
BOOL LoadFieldGlueOverlay(void);
void UnloadFieldGlueOverlay(void);
GameEventReturnCode EventGameClear_Callback(GameEvent *event, u32 *state, void *data);
void SetGameClearStatusSequence(GameClearWork *work);
GameEvent *EventGameClear_GiveMonotypeMedals(GameSystem *gsys, GameSystemProcData *procData);
u32 EventGameClear_Get3DDemoID(void);
void EventGameClear_NextState(GameClearWork *work, u32 *state);
void func_ov010_0215f920(GameClearWork *work);
// pokebw2's UnloadFieldGlueOverlay
GameEventReturnCode func_ov010_0215fa54(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_EVENT_GAME_CLEAR_H
