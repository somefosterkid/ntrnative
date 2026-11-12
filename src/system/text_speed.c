// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/text_speed.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/text_speed.h"
#include "types.h"
#include "save/config.h"
#include "save/trainer_data_savedata.h"

// The waits between characters that the text speeds give. The file has no name in the ROM: text_speed.c is a guess
// from what its functions do. It follows game_data.c and keeps its own pointer to the save control

#define TEXT_SPEED_MAX 5

// The original has this in .data, as an explicit zero
#pragma explicit_zero_data on
// pokebw2's sSaveControl
static SaveControl *data_020a7298 = NULL;
#pragma explicit_zero_data reset

// A negative wait prints that many characters a frame
// pokebw2's sTextSpeedWaits
static const s8 data_0209dfb0[TEXT_SPEED_MAX] = { 3, 1, -2, -4, 6 };

void saveMainSaveAreaAddress(SaveControl *save) {
    data_020a7298 = save;
}

s32 func_02012ffc(void) {
    u32 speed = func_02008374(&getTrainerDataBlkAddress(data_020a7298)->config);

    if (speed >= TEXT_SPEED_MAX) {
        speed = TEXT_SPEED_MAX - 1;
    }
    return data_0209dfb0[speed];
}

s32 func_02013020(void) {
    u32 speed = func_02008374(&getTrainerDataBlkAddress(data_020a7298)->config);

    switch (speed) {
    case 0:
        speed = 1;
        break;
    case 1:
        speed = 2;
        break;
    case 2:
        speed = 3;
        break;
    }
    return func_02013080(speed);
}

s32 func_02013050(void) {
    u32 speed = func_02008374(&getTrainerDataBlkAddress(data_020a7298)->config);

    switch (speed) {
    case 0:
        speed = 4;
        break;
    case 1:
        speed = 0;
        break;
    case 2:
        speed = 1;
        break;
    }
    return func_02013080(speed);
}

s32 func_02013080(u32 speed) {
    if (speed >= TEXT_SPEED_MAX) {
        speed = TEXT_SPEED_MAX - 1;
    }
    return data_0209dfb0[speed];
}
