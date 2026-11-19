#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "map_header.h"
#include "message_format.h"

s32 ov18_021F04C0(PokedexAppData *arg0, u32 arg1) {
    if (*(s32 *)(arg0->unk_18DC.unk_20.maps + (arg1)) == -2) {
        if (arg0->unk_18C8 == 0) {
            return 0x86;
        }
        return 0x87;
    }
    BufferLandmarkName(arg0->msgFormat, 0U, MapHeader_GetMapSec(ov18_021E8AE0(arg0, arg1)));
    return 0x85;
}
