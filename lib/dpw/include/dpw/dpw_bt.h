// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dpw/include/dpw/dpw_bt.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DPW_DPW_BT_H
#define BWPORT_DPW_DPW_BT_H

#include "types.h"
#include "dpw/dpw_tr.h"

// pokebw2's DpwBt_Init
// Starts and ends the library, for the player's ID and friend key, and runs it every frame
void func_ov114_021c8f2c(s32 pid, u64 friendKey, int server);
// pokebw2's DpwBt_Process
void func_ov114_021c8fa8(void);
// pokebw2's DpwBt_End
void func_ov114_021c93bc(void);
// pokebw2's DpwBt_IsFinished
// Whether the last request has ended, and its result
BOOL func_ov114_021c93d0(void);
// pokebw2's DpwBt_GetResult
s32 func_ov114_021c940c(void);
// pokebw2's DpwBt_GetRoomNumber
// Asks for the number of the room that the given kind of battle has
void func_ov114_021c941c(int kind);
// pokebw2's DpwBt_Download
// Asks for the Pokémon of a room: the kind of battle and the room's number; they go to the result
void func_ov114_021c94a0(int kind, int room, void *result);
// pokebw2's DpwBt_Upload
// Uploads a team of 0xf0 bytes for the kind of battle and the room, with a byte of the result, a signature and a word
void func_ov114_021c9530(int kind, int room, u8 win, const void *team, const void *sign, u32 id);
// pokebw2's DpwBt_Cancel
// Cancels the request
void func_ov114_021c9634(void);
// pokebw2's DpwBt_GetServerState
// Asks the server's state
void func_ov114_021c96cc(void);
// pokebw2's DpwBt_SetProfile
// Sends the player's profile
void func_ov114_021c9744(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result);

#endif // BWPORT_DPW_DPW_BT_H
