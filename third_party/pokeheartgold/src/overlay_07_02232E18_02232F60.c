#include "global.h"

#include "overlay_07.h"

extern BOOL (*const ov07_022371B8[2])(UnkBallData *data);

BOOL ov07_02232F60(UnkBallData *data, s32 ballAnim_Unused) {
    return ov07_022371B8[data->unk0](data);
}
