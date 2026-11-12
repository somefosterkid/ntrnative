// GameSpy's server browser (sb_internal.h's struct _ServerBrowser): the query engine, the server list and the callback to
// the game, which sb_serverbrowsing.c makes and runs. Black's is 0x858 bytes: the engine at 0, the list at 0x60.
#ifndef BWPORT_GS_SB_INTERNAL_H
#define BWPORT_GS_SB_INTERNAL_H

#include "types.h"
#include "gs/serverbrowsing.h"
#include "gs/sb_queryengine.h"

struct ServerBrowser_ {
    SBQueryEngine engine;
    SBServerList list; // 0x60
    BOOL disconnectFlag; // 0x83c
    BOOL dontUpdate; // 0x840
    u32 triggerIP; // 0x844
    u16 triggerPort; // 0x848
    ServerBrowserCallback BrowserCallback; // 0x84c
    u32 unk_850; // 0x850, cleared when it is made; the free runs func_ov009_0219922c when it is set
    void *instance; // 0x854
};

#endif // BWPORT_GS_SB_INTERNAL_H
