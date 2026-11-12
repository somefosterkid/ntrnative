// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/net_bgm_volume.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/dwc_rap.h"
#include "gfl/net.h"
#include "gfl/sound.h"

// The BGM's volume while a Wi-Fi connection may be using the sound, as during voice chat. One function between the
// friend checks and net_err.c that belongs to neither by what it uses; the ROM has no string for the file, so the
// name is a guess.

void func_0200f698(u16 trackMask, s32 volume) {
    if (!func_0203d29c() || func_ov009_02156b70() == 0) {
        GFL_SndBGMSetVolume(trackMask, volume);
    }
}
