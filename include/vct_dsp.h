#ifndef VCT_DSP_H
#define VCT_DSP_H

#include "nitro/os.h"

struct VCTPacket;

// lib/dwc/src/vct/vad.c: clears the microphone state
void func_ov013_021b3018(void);
// lib/dwc/src/vct/udp_buffer.c: puts a packet back on the free list, with interrupts off
OSIntrMode func_ov013_021b2fe4(struct VCTPacket *packet);
// lib/dwc/src/vct/udp_buffer.c: takes a packet off the free list, or NULL
struct VCTPacket *func_ov013_021b2fb0(void);
// lib/dwc/src/vct/vad.c: sets up the capture state
void func_ov013_021b3250(void);
// lib/dwc/src/vct/fir.c: sets up the echo state
void func_ov013_021b3364(void);
// the sequence number source in the main program (no header yet)
u16 OS_GetTickLo(void);

// lib/dwc/src/vct/mixer.c: mixes two frames into out
void func_ov013_021b3624(s16 *a, s16 *b, s16 *out, u32 bytes);
// lib/dwc/src/vct/fir.c: keeps the far end's frame for the echo canceller
void func_ov013_021b33e0(const s16 *samples, int bytes);

// lib/dwc/src/vct/fir.c: filters a frame, returns its energy
u32 func_ov013_021b3278(s16 *samples, u32 bytes);
// lib/dwc/src/vct/fir.c: cancels the echo of a frame
void func_ov013_021b3604(s16 *samples, int bytes, u32 rms);
// lib/dwc/src/vct/vad.c: updates the voice activity level
int func_ov013_021b3084(u32 energy, u32 rms, u8 *level);

#endif // VCT_DSP_H
