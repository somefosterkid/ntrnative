#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "font_types_def.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "text.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

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

int ov93_0225E3C4(UnkStruct_ov117_02261280_t *param0, int param1);

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);

PlayerProfile *ov90_022588CC(const UnkStruct_ov114_0225C76C *param0, u32 param1);

typedef u32 TextColor;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

u16 sub_0203769C(void);

extern __attribute__((aligned(4))) const u8 ov93_02262A68[5][4];

void ov93_0225D4EC(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D5AC(UnkStruct_ov117_02261280_t *param0, int param1);

void ov93_0225D4EC(UnkStruct_ov117_02261280_t *param0) {
    int v0, v1;
    PlayerProfile *v2;
    String *v3;
    int v4;
    TextColor v5;
    int v6, v7;

    v4 = sub_0203769C();

    for (v0 = 0; v0 < param0->unk_00->unk_30; v0++) {
        if (v4 != param0->unk_00->unk_2C[v0]) {
            v2 = ov90_022588CC(&param0->unk_00->unk_00, param0->unk_00->unk_2C[v0]);
            v3 = PlayerProfile_GetPlayerName_NewString(v2, HEAP_ID_117);
            v1 = ov93_0225E3C4(param0, param0->unk_00->unk_2C[v0]);

            if (ov90_022588A4(&param0->unk_00->unk_00, param0->unk_00->unk_2C[v0]) == 1) {
                v5 = ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
            } else {
                v5 = ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((15 & 0xFF) << 0)));
            }

            v6 = FontID_String_GetWidth(FONT_SYSTEM, v3, 0);
            v7 = 8 * 10 / 2 - v6 / 2;

            if (v6 & 1) {
                v7--;
            }

            AddTextPrinterParameterizedWithColor(&param0->unk_30[ov93_02262A68[param0->unk_00->unk_30][v1]], FONT_SYSTEM, v3, v7, 0, 0, v5, 0L);
            Heap_Free(v3);
        }
    }
}

void ov93_0225D5AC(UnkStruct_ov117_02261280_t *param0, int param1) {
    if (param1 == 1) {
        FillBgTilemapRect(param0->unk_2C, 4, 1023, 0, 13, 12, 4, 17);
        FillBgTilemapRect(param0->unk_2C, 4, 1023, 0x11, 0, 12, 4, 17);
        FillBgTilemapRect(param0->unk_2C, 4, 1023, 0x14, 13, 12, 4, 17);
    } else {
        switch (param0->unk_00->unk_30) {
        case 2:
            FillBgTilemapRect(param0->unk_2C, 4, 1023, 0, 13, 12, 4, 17);
            FillBgTilemapRect(param0->unk_2C, 4, 1023, 0x14, 13, 12, 4, 17);
            break;
        case 3:
            FillBgTilemapRect(param0->unk_2C, 4, 1023, 0x11, 0, 12, 4, 17);
            break;
        }
    }

    ScheduleBgTilemapBufferTransfer(param0->unk_2C, 4);
}
