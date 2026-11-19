#include "global.h"

#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"

extern const u8 ov32_0225E178[4][4];

void ov32_0225DC68(NamePickerData *data, NARC *narc) {
    u32 i;
    const u8 *spec;
    SpriteResourcesHeader header;

    ov32_0225DB1C(data->resObjs, data->resMans, narc, 4, 7, 5, 6, 1, 0x781, 0x781, 0x781, 0x781);
    ov32_0225DBAC(data->resObjs, data->resMans, &header, 1);
    spec = ov32_0225E178[0];
    i = 0;
    do {
        ov32_0225DC0C(data, i, &header, spec);
        i = i + 1;
        spec = spec + 4;
    } while (i < 4);
}

void ov32_0225DCD4(NamePickerData *data) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_Delete(data->sprites[v0]);
        v0 += 1;
    } while (v0 < 4U);
    SpriteTransfer_DeleteCharTransferTask(data->resObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(data->resObjs[1]);
}

void ov32_0225DD04(NamePickerData *data) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_UpdateAnim(data->sprites[v0], 0x1000);
        v0 += 1;
    } while (v0 < 4U);
}
