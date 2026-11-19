#include "global.h"

#include "overlay_29.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225DB38(NumberEntryData *data);

void ov29_0225DB38(NumberEntryData *data) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_Delete(data->sprites[v0]);
        v0 += 1;
    } while (v0 < 0xBU);
    SpriteTransfer_DeleteCharTransferTask(data->digitResObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(data->digitResObjs[1]);
    SpriteTransfer_DeleteCharTransferTask(data->padResObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(data->padResObjs[1]);
}

void ov29_0225DB7C(NumberEntryData *arg0) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_UpdateAnim(arg0->sprites[v0], 0x1000);
        v0 += 1;
    } while (v0 < 0xBU);
}
