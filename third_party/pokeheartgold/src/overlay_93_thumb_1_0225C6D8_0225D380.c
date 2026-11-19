#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "font_types_def.h"
#include "obj_char_transfer.h"
#include "overlay_93_thumb_1.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

void ov93_0225D380(UnkStruct_ov117_02261280_t *arg0) {
    AddWindowParameterized(arg0->unk_2C, arg0->unk_30, 1U, 0xBU, 0x13U, 0x14U, 4U, 0xEU, 0x300U);
    AddWindowParameterized(arg0->unk_2C, &arg0->unk_30[1], 4U, 0x12U, 1U, 0xAU, 2U, 5U, 0x314U);
    AddWindowParameterized(arg0->unk_2C, &arg0->unk_30[2], 4U, 0x15U, 0xEU, 0xAU, 2U, 5U, 0x328U);
    AddWindowParameterized(arg0->unk_2C, &arg0->unk_30[3], 4U, 1U, 0xEU, 0xAU, 2U, 5U, 0x33CU);
    FillWindowPixelBuffer(&arg0->unk_30[1], 0xFU);
    FillWindowPixelBuffer(&arg0->unk_30[2], 0xFU);
    FillWindowPixelBuffer(&arg0->unk_30[3], 0xFU);
    AddWindowParameterized(arg0->unk_2C, &arg0->unk_30[4], 4U, 2U, 0x13U, 0x1CU, 4U, 5U, 0x359U);
    FillWindowPixelBuffer(&arg0->unk_30[4], 0xFU);
}

struct OverlayManager;

struct TextOBJ;

struct GF3DVramMan;

struct MsgData;

struct ParticleSystem;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef u32 TextColor;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void ov93_0225D468(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D468(UnkStruct_ov117_02261280_t *param0) {
    String *v0;

    DrawFrameAndWindow1(&param0->unk_30[4], 1, (((((0x8000 - 0x2000) / 32) + (10 * 2)) + (10 * 2)) + (10 * 2)) + (10 * 2), 6);

    v0 = NewString_ReadMsgData(param0->unk_80, 3);
    AddTextPrinterParameterizedWithColor(&param0->unk_30[4], FONT_SYSTEM, v0, 0, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0))), 0L);
    String_Delete(v0);

    ScheduleWindowCopyToVram(&param0->unk_30[4]);
}

void ov93_0225D4B8(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D4B8(UnkStruct_ov117_02261280_t *param0) {
    FillBgTilemapRect(param0->unk_2C, 4, 1023, 2 - 1, 0x13 - 1, 0x1c + 2, 4 + 2, 17);
    ScheduleBgTilemapBufferTransfer(param0->unk_2C, 4);
}
