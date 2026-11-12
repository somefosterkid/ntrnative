// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gamestats.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_GAMESTATS_H
#define BWPORT_DWC_GAMESTATS_H

#include "types.h"

// The servers of the game statistics: the live one, and two for development
#define GAMESTATS_SERVER_LIVE 0

#define GAMESTATS_SERVER_DEVELOPMENT 1
#define GAMESTATS_SERVER_TEST 2

// The states that gamestats_Poll gives: a call failed, or ended with its answer kept
#define GAMESTATS_STATE_FAILED 1

#define GAMESTATS_STATE_DONE 8

// Called with the answer before it is kept; FALSE refuses it
typedef BOOL (*GamestatsCheckFunc)(const void *data, int size);

// pokebw2's Gamestats_Poll
// Runs the call and returns its state
int func_ov114_021ca0a0(void);
// pokebw2's Gamestats_Close
// Ends the library's use of the HTTP client and frees the call's memory
void func_ov114_021ca1b4(void);
// pokebw2's Gamestats_GetBody
// The answer of the last call and its size
void *func_ov114_021ca228(int *size);
// pokebw2's Gamestats_Start
// Sets up the server (one of the GAMESTATS_SERVER_ constants) and the game's key: "key" is the game's secret of 20
// characters, four numbers of 8 hexadecimal digits that give the generator of the payload's scrambling, and the name
// of the game
void func_ov114_021c9ee4(int server, const char *key);
// pokebw2's Gamestats_Send
// Sends the page with the player's ID and the payload; 0 when it started, 2 when it ran out of memory, 3 when the
// game's key was not set
int func_ov114_021ca470(const char *page, int pid, const void *payload, int size, GamestatsCheckFunc check);
// pokebw2's Gamestats_Abort
// Ends the call that is running
void DWCi_RankingSessionCancel(void);

#endif // BWPORT_DWC_GAMESTATS_H
