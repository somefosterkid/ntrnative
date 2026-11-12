// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/livebattlematch_irc.h: the lines in use
// so far.
#ifndef BWPORT_APP_LIVEBATTLEMATCH_IRC_H
#define BWPORT_APP_LIVEBATTLEMATCH_IRC_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The data a player sends first is followed by the team: the size of its part before the team
#define LIVEBATTLEMATCH_PLAYER_HEADER_SIZE 0xd4

typedef struct LiveBattleMatchIrc LiveBattleMatchIrc;

// pokebw2's LiveBattleMatchIrc_Create
LiveBattleMatchIrc *func_ov176_0217e00c(GameData *gameData, HeapID heapId);
// pokebw2's LiveBattleMatchIrc_Delete
void func_ov176_0217e058(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_ResetLink
// Resets the connection's steps, then TRUE
BOOL func_ov176_0217e0a8(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_StepLink
// Starts the infrared network and waits for the other machine: 0 while it goes on, 1 once connected, 2 if the other
// machine left first
int func_ov176_0217e0b0(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_CancelLink
// Deletes the commands and the callback
void func_ov176_0217e158(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_StepCancel
// Ends the network after the commands were deleted by LiveBattleMatchIrc_CancelLink, then TRUE once it ended
BOOL func_ov176_0217e178(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_StepEnd
// Deletes the commands and ends the network, then TRUE once it ended
BOOL func_ov176_0217e1c0(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_SendPlayerData
// Sends the player's data (LIVEBATTLEMATCH_PLAYER_HEADER_SIZE bytes and a team) to the other machine
BOOL func_ov176_0217e1fc(LiveBattleMatchIrc *irc, void *data);
// pokebw2's LiveBattleMatchIrc_TakePlayerData
// If the other machine's data came, sets *data to it and returns TRUE
BOOL func_ov176_0217e23c(LiveBattleMatchIrc *irc, void **data);
// pokebw2's LiveBattleMatchIrc_SendParty
// Sends the team picked, and then is TRUE once it is sent
BOOL func_ov176_0217e258(LiveBattleMatchIrc *irc, PokeParty *party);
// pokebw2's LiveBattleMatchIrc_TakeParty
// Copies the other player's team into party, and then is TRUE, once it has arrived
BOOL func_ov176_0217e298(LiveBattleMatchIrc *irc, PokeParty *party);
// pokebw2's LiveBattleMatchIrc_SendReady
// Sends 4 bytes, the ready flag
BOOL func_ov176_0217e2c0(LiveBattleMatchIrc *irc, const u32 *flag);
// pokebw2's LiveBattleMatchIrc_TakeReady
// If the other machine's 4 bytes came, copies them to *flag and returns TRUE
BOOL func_ov176_0217e2f8(LiveBattleMatchIrc *irc, u32 *flag);
// pokebw2's LiveBattleMatchIrc_StepDelivery
// Steps the delivery: 0 while it goes on, then 1 if the data came, 3 if there was none, 2 otherwise
int func_ov176_0217e380(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_ResetDeliveryEnd
// Resets the steps of LiveBattleMatchIrc_StepDeliveryEnd
void func_ov176_0217e40c(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_StepDeliveryEnd
// Ends the delivery, then TRUE once the network is free
BOOL func_ov176_0217e414(LiveBattleMatchIrc *irc);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's LiveBattleMatchIrc_Main
// Called every frame; does nothing
void func_ov176_0217e074(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_CheckError
// Checks for a network error, and if there is one, ends the delivery and the network's error screen and returns TRUE
BOOL func_ov176_0217e078(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_ResetEnd
// Resets the steps of LiveBattleMatchIrc_StepEnd, then TRUE
BOOL func_ov176_0217e1b8(LiveBattleMatchIrc *irc);
// pokebw2's LiveBattleMatchIrc_StartDelivery
// Starts receiving the competition data of a Download Station into buffer (0x160 bytes)
void func_ov176_0217e318(LiveBattleMatchIrc *irc, void *buffer);

#endif // BWPORT_APP_LIVEBATTLEMATCH_IRC_H
