#include "global.h"

extern void *sPmAgbCartridgeSpec;

void PmAgbCartridgeUnlinkSpec(void);

void PmAgbCartridgeUnlinkSpec(void) {
    sPmAgbCartridgeSpec = 0;
}

u8 PmAgbCartridge_GetVersionInternal(void);

u8 PmAgbCartridge_GetVersionInternal(void) {
    return *(u8 *)((u8 *)sPmAgbCartridgeSpec + 5);
}

u16 PmAgbCartridge_GetLanguage(void);

u16 PmAgbCartridge_GetLanguage(void) {
    return *(u16 *)((u8 *)sPmAgbCartridgeSpec + 6);
}
