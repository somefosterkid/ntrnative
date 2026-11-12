// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_b_gauge.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_B_GAUGE_H
#define BWPORT_BATTLE_BTLV_B_GAUGE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black's: 0x1c bytes (btlv_scu.c's func_ov093_021ee104 fills it with MI_CpuFill8(param, 0, 0x1c)); Black 2 adds
// a u32 mode at 0x1c
struct BtlvBGaugeParam {
    u32 side;     // 0x00  0: the player's, slides in from the right; 1: the opponent's, from the left
    u32 balls[6]; // 0x04  each ball's state, the column of its animations
};

// pokebw2's BtlvBGauge_IsBusy
BOOL func_ov094_022090a4(BtlvBGauge *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvBGauge_Create
BtlvBGauge *func_ov094_02208dac(const BtlvBGaugeParam *param, HeapID heapId);
// pokebw2's BtlvBGauge_Delete
void func_ov094_02209068(BtlvBGauge *work);

#endif // BWPORT_BATTLE_BTLV_B_GAUGE_H
