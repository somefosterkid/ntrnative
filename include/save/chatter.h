// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/chatter.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SAVE_CHATTER_H
#define BWPORT_SAVE_CHATTER_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
#define PERAP_VOICE_DECODED_SIZE 8180

// pokebw2's PerapVoice_Encode
// Stores a recording of PERAP_VOICE_DECODED_SAMPLES signed 8-bit samples as the cry
void func_02007bd0(PerapVoice *chatter, const s8 *src);

// The Chatot's recorded cry, save block 0x24: 2000 samples of four bits
#define PERAP_VOICE_SAMPLES 2000

// The decoded cry: every sample four times, a second of sound, with silence after it
#define PERAP_VOICE_DECODED_SAMPLES (PERAP_VOICE_SAMPLES * 4)

struct PerapVoice {
    BOOL exists;
    u8 voice[PERAP_VOICE_SAMPLES / 2];
};

void initChatotChatter(PerapVoice *chatter);
PerapVoice *allocChatotChatterBlk(HeapID heapId);
void moveChatter(PerapVoice *dest, const PerapVoice *src);
// pokebw2's PerapVoice_GetSize
u32 func_02007b00(void);
// pokebw2's PerapVoice_GetChatterEffect
// What a Chatter with the cry does, drawn from the cry: 0 for nothing, or 1 or 2
u8 func_02007c70(PerapVoice *chatter);

// No twin in pokebw2's C: these prototypes are Black's, checked against its assembly
PerapVoice *getChatterBlockAddress(SaveControl *save);
BOOL doesChatotExist(PerapVoice *chatter);
void setChatotNotPresent(PerapVoice *chatter);
void setChatotPresent(PerapVoice *chatter);
// Forgets the recorded cry once no Chatot is in the party
void checkChatotInParty(PerapVoice *chatter, PokeParty *party);
void *getChatterVoice(PerapVoice *chatter);
// pokebw2's PerapVoice_Decode
// Decodes a recorded cry into PERAP_VOICE_DECODED_SIZE signed 8-bit samples
void func_02007b68(s8 *dest, const s8 *src);

#endif // BWPORT_SAVE_CHATTER_H
