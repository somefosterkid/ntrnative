#ifndef POKEHEARTGOLD_OVERLAY_43_PALPAD_FNS_H
#define POKEHEARTGOLD_OVERLAY_43_PALPAD_FNS_H

#include "overlay_43_palpad.h"

BOOL ov43_0222D508(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 heapID);
u32 ov43_0222D24C(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 action);
s32 ov43_0222D2FC(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
u32 ov43_0222D3B8(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 *touched);
int ov43_0222D47C(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222DAE8(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 slot, s32 page, u32 heapID);
void ov43_0222DB28(PalPadDetails *details, PalPadGfx *gfx, s32 page, u32 unused);
void ov43_0222DB94(PalPadDetails *details, PalPadGfx *gfx, s16 page);
void ov43_0222DF4C(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, s32 page, PalPadDetailsScreen *screens, BOOL hasFrontierPages, u32 heapID);
void ov43_0222ED18(PalPadDetails *details, s32 delta);

BOOL ov43_0222C358(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222C378(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s16 page, s32 cursor, u32 slot, enum HeapID heapID);
s32 ov43_0222C228(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
BOOL ov43_0222C2B0(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
BOOL ov43_0222C5D8(const PalPadRoster *roster, PalPadShared *shared);
void ov43_0222ADB8(PalPadShared *shared, PalPadGfx *gfx, u32 cursor);

void ov43_0222B944(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222B5A8(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222BAB8(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
BOOL ov43_0222C714(PalPadRoster *roster, PalPadGfx *gfx);
void ov43_0222C7EC(PalPadRosterPage *page, PalPadGfx *gfx);

void ov43_0222B93C(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);

#endif
