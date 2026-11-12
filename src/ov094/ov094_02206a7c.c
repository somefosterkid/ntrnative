// Black's code at ov094 0x02206a7c-0x02206b58. No name is known: the module and address.

#include "types.h"
#include "battle/btlv_input.h"
#include "gfl/sound.h"
#include "struct_decls.h"
#include "battle/btlv_effect.h"


u32 func_ov094_02206a7c(BtlvInput *work) {
    if (work->laterPoke == 0) {
        if (work->rule == 2) {
            if (func_ov094_021f88a8() == 1) {
                if (work->pokePos == 4) {
                    return 0x65;
                }
                return 0x5F;
            }
            if (work->pokePos == 4) {
                return 0x5D;
            }
            return 0x5E;
        }
        if (func_ov094_021f88a8() == 1) {
            return 0x65;
        }
        return 0x5D;
    }
    if (work->rule == 2 && work->pokePos != 4) {
        if (func_ov094_021f88a8() == 1) {
            return 0x67;
        }
        return 0x61;
    }
    if (func_ov094_021f88a8() == 1) {
        return 0x66;
    }
    return 0x60;
}

void func_ov094_02206af8(BtlvInput *work) {
    if ((work->unk54 != 3) || (work->screen == 6)) {
        GFL_SndSEPlay(0x555);
    }
}

void func_ov094_02206b14(BtlvInput *work) {
    if ((work->unk54 != 3) || (work->screen == 6)) {
        GFL_SndSEPlay(0x548);
    }
}

void func_ov094_02206b30(BtlvInput *work) {
    if (work->screen != 6) {
        if (work->unk54 != 3) {
            GFL_SndSEPlay(0x54DU);
        }
    } else {
        GFL_SndSEPlay(0x54CU);
    }
}
