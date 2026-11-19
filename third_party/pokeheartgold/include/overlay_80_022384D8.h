#ifndef POKEHEARTGOLD_OVERLAY_80_022384D8_H
#define POKEHEARTGOLD_OVERLAY_80_022384D8_H

#include "global.h"

struct BgConfig;
struct SysTask;

typedef struct WFCFacilitySelectorMonitorFlicker {
    struct SysTask *task;
    struct BgConfig *bgConfig;
    u16 monitorState;
    u16 delay;
} WFCFacilitySelectorMonitorFlicker;

typedef struct WFCFacilitySelectorEffects {
    u32 unk_00;
    WFCFacilitySelectorMonitorFlicker *flicker;
} WFCFacilitySelectorEffects;

WFCFacilitySelectorEffects *ov80_022384D8(struct BgConfig *bgConfig);
WFCFacilitySelectorMonitorFlicker *ov80_022384FC(struct BgConfig *bgConfig);

void ov80_0223857C(u16 *tiles, u8 tileset);
void ov80_022385B0(WFCFacilitySelectorEffects *effects);

u16 ov80_022385D8(u8 facility);
u16 ov80_02238610(u8 facility);

#endif // POKEHEARTGOLD_OVERLAY_80_022384D8_H
