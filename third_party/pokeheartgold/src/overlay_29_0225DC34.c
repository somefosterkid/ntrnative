#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_29.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225DC34(NumberEntryData *data);

s32 ov29_0225DB9C();

void ov29_0225DC34(NumberEntryData *data) {
    GridInputHandler_Free(data->gridInput);
}

void ov29_0225DC44(void) {
}

void ov29_0225DC48(void) {
    ov29_0225DB9C();
}
