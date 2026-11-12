// GameSpy's server browsing query engine (sb_internal.h's SBQueryEngine), which sb_queryengine.c runs: the queue of the
// servers it asks and the socket it asks them on. Black's is a later GameSpy's: a key list of 40 and the QR2 query version.
#ifndef BWPORT_GS_SB_QUERYENGINE_H
#define BWPORT_GS_SB_QUERYENGINE_H

#include "types.h"
#include "gs/serverbrowsing.h"

// max number of keys for the basic key list
#define MAX_QUERY_KEYS 40

typedef struct SBQueryEngine SBQueryEngine;

// callback types for the query engine
typedef enum {
    qe_updatesuccess,
    qe_updatefailed,
    qe_engineidle,
    qe_challengereceived
} SBQueryEngineCallbackReason;

typedef void (*SBEngineCallbackFn)(SBQueryEngine *engine, SBQueryEngineCallbackReason reason, SBServer server,
                                   void *instance);

typedef struct SBServerFIFO {
    SBServer first;
    SBServer last;
    int count;
} SBServerFIFO;

struct SBQueryEngine {
    int queryversion;
    int maxupdates;
    SBServerFIFO querylist;
    SBServerFIFO pendinglist;
    int querysock;
    int icmpsock;
    u32 mypublicip;
    u8 serverkeys[MAX_QUERY_KEYS];
    int numserverkeys;
    SBEngineCallbackFn ListCallback;
    void *instance;
};

void SBQueryEngineInit(SBQueryEngine *engine, int maxupdates, int queryversion, BOOL lanBrowse,
                       SBEngineCallbackFn callback, void *instance);
void SBEngineCleanup(SBQueryEngine *engine);
void SBQueryEngineSetPublicIP(SBQueryEngine *engine, u32 mypublicip);
void SBQueryEngineUpdateServer(SBQueryEngine *engine, SBServer server, BOOL addFront, int queryType, BOOL challenge);
void SBQueryEngineAddQueryKey(SBQueryEngine *engine, u8 keyid);
void SBQueryEngineRemoveServerFromFIFOs(SBQueryEngine *engine, SBServer server);

#endif // BWPORT_GS_SB_QUERYENGINE_H
