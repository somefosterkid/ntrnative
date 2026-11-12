// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/nhttp_rap.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_NHTTP_RAP_H
#define BWPORT_GFL_NHTTP_RAP_H

#include "types.h"
#include "gfl/heap.h"

typedef struct NhttpRapWork NhttpRapWork;

typedef struct NHttpRap NHttpRap;

// pokebw2's NHttpRap_Create
// Starts a check of Pokemon with the player's ID and the login buffer
NHttpRap *func_ov114_021b9800(HeapID heapId, s32 playerId, void *loginBuffer);
// pokebw2's NHttpRap_BeginPost
// Starts the request's data: its size and the type of check
void func_ov114_021b98a0(NHttpRap *rap, HeapID heapId, u32 size, u32 type);
// pokebw2's NHttpRap_CreateRequest
// Makes the request of the index, its URL built from the player's ID and the login token
BOOL func_ov114_021b95d4(u32 index, NHttpRap *rap);
// pokebw2's NHttpRap_SendValidate
BOOL func_ov114_021b98f8(NHttpRap *rap);
// pokebw2's NHttpRap_GetAnswer
// The answer of the finished request
void *func_ov114_021b97f4(NHttpRap *rap);
// pokebw2's NHttpRap_GetCheckStatus
// The answer's status, 0 when every Pokemon passed
u8 func_ov114_021b99f0(const void *answer);
// pokebw2's NHttpRap_GetCheckSignature
// The signature in the answer, after the results of count Pokemon
const void *func_ov114_021b9a14(const void *answer, int count);

// pokebw2's NHttpRap_FreePostData
// Frees the request's data
void func_ov114_021b99b8(NHttpRap *rap);
// pokebw2's NHttpRap_Destroy
void func_ov114_021b9838(NHttpRap *rap);
// pokebw2's NHttpRap_AddPostData
// Adds data to the request
void func_ov114_021b98d8(NHttpRap *rap, const void *data, u32 size);
// pokebw2's NHttpRap_StartRequest
// The request's error, 0 when it started
int func_ov114_021b9748(NHttpRap *rap);
// pokebw2's NHttpRap_GetStatus
// The HTTP status of the answer
int func_ov114_021b99dc(NHttpRap *rap);
// pokebw2's NHttpRap_Poll
// 0 once the request ended, 15 while it runs, or an error
int func_ov114_021b9790(NHttpRap *rap);
// pokebw2's NHttpRap_EndRequest
// Ends the request
void func_ov114_021b9774(NHttpRap *rap);
// pokebw2's NHttpRap_GetCheckResult
u32 func_ov114_021b99f4(const void *body, int index);

// pokebw2's NHttpRap_GetConnection
// The NHTTP request of the last one sent
int func_ov114_021b973c(NHttpRap *rap);

#endif // BWPORT_GFL_NHTTP_RAP_H
