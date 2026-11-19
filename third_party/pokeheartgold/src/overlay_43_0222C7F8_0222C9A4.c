#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    void *unk_00[2];
    NNSG2dScreenData *unk_08[2];
} UnkStruct_ov64_022300E4;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    Window unk_08;
    UnkStruct_ov63_0222BB38 *unk_18[8];
} UnkStruct_ov64_02230074;

void ov43_0222CA30(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1);

void ov43_0222C9A4(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, UnkStruct_ov64_022300E4 *param2);

void ov43_0222C9A4(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, UnkStruct_ov64_022300E4 *param2) {
    s32 iVar1;
    s32 iVar2;

    ov43_0222CA30(param0, param1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_08);
    iVar1 = (s32)(param0->unk_00).unk_02;
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    iVar2 = (s32)(param0->unk_00).unk_00;
    if (iVar2 < 0) {
        iVar2 = -iVar2;
    }
    FillBgTilemapRect(param1->bgConfig, 2, 0, (u8)iVar2, (u8)iVar1, (param0->unk_00).unk_04, (param0->unk_00).unk_05, 0);
    ScheduleBgTilemapBufferTransfer(param1->bgConfig, 2);
    iVar1 = (s32)(param0->unk_00).unk_02;
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    iVar2 = (s32)(param0->unk_00).unk_00;
    if (iVar2 < 0) {
        iVar2 = -iVar2;
    }
    FillBgTilemapRect(param1->bgConfig, 3, 0, (u8)iVar2, (u8)iVar1, (param0->unk_00).unk_04, (param0->unk_00).unk_05, 0);
    ScheduleBgTilemapBufferTransfer(param1->bgConfig, 3);
    return;
}
