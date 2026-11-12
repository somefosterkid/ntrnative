// GameSpy's NAT negotiation (natneg.h): the functions after lib/dwc/src/gs/natneg/natneg.c's range that it and the
// match code call (not written yet)
#ifndef BWPORT_GS_NATNEG_H
#define BWPORT_GS_NATNEG_H

#include "types.h"

typedef enum {
    nr_success,
    nr_deadbeatpartner,
    nr_inittimeout,
    nr_unknownerror
} NegotiateResult;

typedef enum {
    ne_noerror,
    ne_allocerror,
    ne_socketerror,
    ne_dnserror
} NegotiateError;

// the callbacks of a negotiation: how far it is, and how it ended (the peer's address, a sockaddr_in, is by byte)
typedef void (*NegotiateProgressFunc)(u32 state, void *userdata);
typedef void (*NegotiateCompletedFunc)(s32 result, u32 gamesocket, u8 *remoteaddr, void *userdata);

s32 NNBeginNegotiationWithSocket(s32 socket, u32 cookie, u32 type, NegotiateProgressFunc progresscallback,
                                 NegotiateCompletedFunc completedcallback, void *userdata);
void NNCancel(int cookie);
void NNThink(void);

#endif // BWPORT_GS_NATNEG_H
