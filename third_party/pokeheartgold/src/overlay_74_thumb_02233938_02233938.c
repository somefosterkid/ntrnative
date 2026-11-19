#include "global.h"

#include "pm_agb_save.h"

typedef struct PmAgbCartridgeSpec {
    u32 gameCode;
    u8 gameType;
    u8 version;
    u16 language;
} PmAgbCartridgeSpec;

extern PmAgbCartridgeSpec *sPmAgbCartridgeSpec;

extern PmAgbCartridgeSpec sPmAgbRomCodeMappings[];

BOOL PmAgbCartridgeHasFlash(void);

int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count);

int PmAgbCartridgeGetOffsets(void *buffer);

int PmAgbCartridgeGetOffsets(void *buffer) {
    int result;
    ov74_0223D33C.buffer = buffer;
    if (sPmAgbCartridgeSpec != NULL) {
        return 12;
    }
    result = IdentifyPmAgbCartridge(sPmAgbRomCodeMappings, 30);
    if (result != 0) {
        return result;
    }
    if (sPmAgbCartridgeSpec->gameType == 0) {
        ov74_0223D33C.header.saveSizes.trainerInfoSize = 0x890;
        ov74_0223D33C.header.saveSizes.gameStateSize = 0x3AC0;
        ov74_0223D33C.header.unk_50 = 0x1220;
        ov74_0223D33C.header.saveSizes.unk_30 = 9;
    } else {
        CTRDG_CpuCopy32((const void *)0x08000100, &ov74_0223D33C.header, sizeof(PmAgbRomHeader));
    }
    if (!PmAgbCartridgeHasFlash()) {
        return 3;
    }
    return 0;
}
