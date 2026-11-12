// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pdc_msg.h: the lines in use so far.
#ifndef BWPORT_APP_PDC_MSG_H
#define BWPORT_APP_PDC_MSG_H

#include "types.h"
#include "app/pdc.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct PdcMsg PdcMsg;

// pokebw2's PdcMsg_Free
void func_ov098_021b68dc(PdcMsg *msg);
// pokebw2's PdcMsg_StartSawDream
// The messages: the dream Pokémon's appearance, the question what to do, the call to it, the joining of the party and the player's leaving.
// Each starts the window's fade in
void func_ov098_021b6910(PdcMsg *msg, PdcWork *pdc);
// pokebw2's PdcMsg_Step
// Runs a frame of the message, which returns whether it is done
BOOL func_ov098_021b6a3c(PdcMsg *msg);
// pokebw2's PdcMsg_StartWhatWill
void func_ov098_021b694c(PdcMsg *msg, PdcWork *pdc);
// pokebw2's PdcMsg_StartComeHere
void func_ov098_021b6988(PdcMsg *msg, PdcWork *pdc);
// pokebw2's PdcMsg_StartJoinedParty
void func_ov098_021b69c4(PdcMsg *msg, PdcWork *pdc);
// pokebw2's PdcMsg_ConsumeFinished
// Whether the message has finished printing, once
BOOL func_ov098_021b6b74(PdcMsg *msg);
// pokebw2's PdcMsg_StartLeft
void func_ov098_021b6a00(PdcMsg *msg, PdcWork *pdc);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PdcMsg_Create
PdcMsg *func_ov098_021b67d4(Font *font, const Config *config, TCBExManager *tcbManager, HeapID heapId);
// pokebw2's PdcMsg_StartHide
// Starts fading the window out, and whether that is done
void func_ov098_021b6b5c(PdcMsg *msg);
// pokebw2's PdcMsg_IsHideDone
BOOL func_ov098_021b6b68(PdcMsg *msg);

#endif // BWPORT_APP_PDC_MSG_H
