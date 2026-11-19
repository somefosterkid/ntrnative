#include "global.h"

#include "overlay_49.h"
#include "sound_chatot.h"
#include "unk_02005D10.h"

BOOL sub_0200606C(u32 seqID, int playerID);

void ov49_022693A4(BOOL param0, BOOL param1, BOOL param2, u32 seqID) {
    if (param0) {
        if (param1) {
            if (param2) {
                sub_0200606C((u16)seqID, 5);
            } else {
                PlaySE(seqID);
            }
        }
    } else {
        PlaySE(seqID);
    }
}

void ov49_022693D4(BOOL param0, BOOL param1, BOOL param2, u32 species) {
    if (param0) {
        if (param1) {
            PlayCry(species, 0);
        }
    } else {
        PlayCry(species, 0);
    }
}
