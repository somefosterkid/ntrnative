// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_audience.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_AUDIENCE_H
#define BWPORT_APP_MUSICAL_STA_ACT_AUDIENCE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

#define STA_ACT_AUDIENCE_COUNT 60

// What a spectator looks at when no Pokémon or light is picked
#define STA_ACT_AUDIENCE_NONE 0xff

// Black 2's; twinned code confirms the offsets of target, timer, unk4 and x in Black
struct StaActAudienceMember {
    // The position of the Pokémon looked at, or STA_ACT_AUDIENCE_NONE
    u8 target;
    // Frames until the spectator turns again
    u8 timer;
    // Which Pokémon's fan it is, which picks its look
    u8 type;
    u16 unk4;
    // Where it sits, in 4 by 4 tile blocks
    u8 x;
    u8 y;
};

// Black 2's; twinned code confirms the offsets of heapId, stage, cheer, lookTarget, refresh and members, and its size
// in Black
struct StaActAudience {
    HeapID heapId;
    u8 unk2[0x1e6];
    StaActing *stage;
    // Whether each Pokémon is being cheered for
    BOOL cheer[4];
    // The light everyone looks at, or STA_ACT_AUDIENCE_NONE
    u8 lookTarget;
    // Whether the spectators pick what to look at again
    BOOL refresh;
    StaActAudienceMember members[STA_ACT_AUDIENCE_COUNT];
};

// pokebw2's StaActAudience_InitSystem
StaActAudience *func_ov132_021f9fec(HeapID heapId, StaActing *stage, MusicalStageParam *param);
// pokebw2's StaActAudience_UpdateSystem
void func_ov132_021fa064(StaActAudience *sys);
// pokebw2's StaActAudience_SetCheerPoke
void func_ov132_021fa558(StaActAudience *sys, u8 pos, BOOL cheer);
// pokebw2's StaActAudience_SetLookLight
void func_ov132_021fa56c(StaActAudience *sys, u8 light);
// pokebw2's StaActAudience_SetScrollOffset
void func_ov132_021fa57c(StaActAudience *sys, int scroll);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActAudience_TermSystem
void func_ov132_021fa04c(StaActAudience *sys);

#endif // BWPORT_APP_MUSICAL_STA_ACT_AUDIENCE_H
