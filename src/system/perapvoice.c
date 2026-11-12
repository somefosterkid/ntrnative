// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/perapvoice.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/chatter.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/mi.h"
#include "pml/poke_party.h"
#include "save/save_control.h"

// The Chatot's recorded cry (Perap is Chatot's Japanese name). The name is the ROM's string. Block 0x24.

#define SAVE_BLOCK_PERAP_VOICE 0x24

// pokebw2's PerapVoice_GetSize
u32 func_02007b00(void) {
    return sizeof(PerapVoice);
}

void initChatotChatter(PerapVoice *chatter) {
    MIi_CpuClear32(0, chatter, sizeof(PerapVoice));
    chatter->exists = FALSE;
}

PerapVoice *allocChatotChatterBlk(HeapID heapId) {
    PerapVoice *chatter = GFL_HeapAllocate(heapId, sizeof(PerapVoice), TRUE, "perapvoice.c", 87);

    initChatotChatter(chatter);
    return chatter;
}

PerapVoice *getChatterBlockAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_PERAP_VOICE);
}

BOOL doesChatotExist(PerapVoice *chatter) {
    return chatter->exists;
}

void setChatotNotPresent(PerapVoice *chatter) {
    chatter->exists = FALSE;
}

void setChatotPresent(PerapVoice *chatter) {
    chatter->exists = TRUE;
}

void *getChatterVoice(PerapVoice *chatter) {
    return chatter->voice;
}

// pokebw2's PerapVoice_Decode
void func_02007b68(s8 *dest, const s8 *src) {
    int i;
    int pos = 0;
    int j;
    u8 nibble;
    s8 value;
    s8 sample;

    for (i = 0; i < PERAP_VOICE_SAMPLES / 2; i++) {
        nibble = src[i] & 0xf;
        value = nibble - 8;
        sample = value << 4;
        for (j = 0; j < 4; j++) {
            dest[pos + j] = sample;
        }
        nibble = src[i] >> 4;
        value = nibble - 8;
        sample = value << 4;
        for (j = 4; j < 8; j++) {
            dest[pos + j] = sample;
        }
        pos += 8;
    }
    for (i = PERAP_VOICE_DECODED_SAMPLES; i < PERAP_VOICE_DECODED_SIZE; i++) {
        dest[i] = 0;
    }
}

// pokebw2's PerapVoice_Encode
void func_02007bd0(PerapVoice *chatter, const s8 *src) {
    int i;
    int pos = 0;
    s8 sample;
    u8 nibble;

    chatter->exists = TRUE;
    for (i = 0; i < PERAP_VOICE_DECODED_SAMPLES; i += 8) {
        sample = src[i] / 16;
        chatter->voice[pos] = sample + 8;
        sample = src[i + 4] / 16;
        nibble = sample + 8;
        chatter->voice[pos] |= nibble << 4;
        pos++;
    }
}

void moveChatter(PerapVoice *dest, const PerapVoice *src) {
    MI_CpuCopy8(src, dest, sizeof(PerapVoice));
}

// Forgets the recorded cry once no Chatot is in the party
void checkChatotInParty(PerapVoice *chatter, PokeParty *party) {
    int i;
    int count = PokeParty_GetPkmCount(party);

    for (i = 0; i < count; i++) {
        if (PokeParty_GetParam(PokeParty_GetPkm(party, i), PKM_PARAM_SPECIES, NULL) == SPECIES_CHATOT) {
            return;
        }
    }
    setChatotNotPresent(chatter);
}

// pokebw2's PerapVoice_GetChatterEffect
u8 func_02007c70(PerapVoice *chatter) {
    u8 value;

    if (doesChatotExist(chatter) == TRUE) {
        value = chatter->voice[100] ^ chatter->voice[500] ^ chatter->voice[700];
        if (value < 100) {
            return 1;
        }
        if (value >= 150 && value < 256) {
            return 2;
        }
    }
    return 0;
}
