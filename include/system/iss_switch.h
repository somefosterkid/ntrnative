// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/iss_switch.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_ISS_SWITCH_H
#define BWPORT_SYSTEM_ISS_SWITCH_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ISSSwitch *ISSSwitch_Create(HeapID heapId);
void ISSSwitch_Free(ISSSwitch *sw);
// Sets the length of the switch's fades, in frames, and its tracks
void ISSSwitch_Set(ISSSwitch *sw, u16 fadeFrames, u16 trackMask);
BOOL ISSSwitch_IsOn(ISSSwitch *sw);
void ISSSwitch_ReqFadeIn(ISSSwitch *sw);
void ISSSwitch_ReqFadeOut(ISSSwitch *sw);
void ISSSwitch_Unmute(ISSSwitch *sw);
void ISSSwitch_Mute(ISSSwitch *sw);
void ISSSwitch_Update(ISSSwitch *sw);
// Sets the volume of the switch's tracks, scaled by the switch set's master volume
void ISSSwitch_CommitVolume(ISSSwitch *sw, int masterVolume);

#endif // BWPORT_SYSTEM_ISS_SWITCH_H
