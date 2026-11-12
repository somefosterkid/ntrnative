// The PC build (port/meson.build's nnsys option) compiles lib/nnsys/src/snd/player.c through this file
// (tools/port_sources.py --nnsys). libntr's sound driver keeps the music players' tempo with the real clock when the
// Speed setting runs the game faster (libntr-sp77.patch, snd_interface.c): it needs to know which NitroSystem player
// (0 and 6 are the music's) a sequence starts on in each driver slot, before SND_PrepareSeq, which looks it up. That
// was libntrsystem-sp77.patch's line in libntrsystem's player.c; here the call wraps the one SND_PrepareSeq of
// NNSi_SndPlayerStartSeq (seqPlayer is its argument, sPlayer the file's players). The DS build never sees this file.
#include <nitro.h>

extern void SIM_Snd_SeqPlayerAssign(int slot, int nnsPlayer);   // libntr-sp77.patch

#pragma push_macro("SND_PrepareSeq")
#define SND_PrepareSeq(playerNo, ...)                                                                                  \
    (SIM_Snd_SeqPlayerAssign((playerNo), (int)(seqPlayer->player - sPlayer)), SND_PrepareSeq((playerNo), __VA_ARGS__))
#include "../../../../../../lib/nnsys/src/snd/player.c"
#undef SND_PrepareSeq
#pragma pop_macro("SND_PrepareSeq")
