// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/nvram.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_NVRAM_H
#define BWPORT_NITRO_NVRAM_H

#include "types.h"

// The result of a call, as the callback gets it and the synchronous functions return it. The names are by where each
// is produced: ILLEGAL_STATUS also when the library is not initialized, WRITE_ERROR when the flash's status register
// shows its error bit after a page was written, and VERIFY_ERROR when the data read back differs
typedef enum NVRAMResult {
    NVRAM_RESULT_SUCCESS,
    NVRAM_RESULT_BUSY,
    NVRAM_RESULT_ILLEGAL_PARAMETER,
    NVRAM_RESULT_SEND_ERROR,
    NVRAM_RESULT_ILLEGAL_STATUS,
    NVRAM_RESULT_WRITE_ERROR,
    NVRAM_RESULT_VERIFY_ERROR,
    NVRAM_RESULT_FATAL_ERROR,
} NVRAMResult;

typedef void (*NVRAMCallback)(NVRAMResult result, void *arg);

#endif // BWPORT_NITRO_NVRAM_H
