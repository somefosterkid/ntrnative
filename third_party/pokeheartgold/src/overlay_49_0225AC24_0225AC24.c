#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_app_0225ac24.h"

void ov49_0225AC24(void *arg0);

void ov49_0225AC24(void *arg0) {
    WaitingIcon *waitingIcon;

    waitingIcon = *(WaitingIcon **)((u8 *)arg0 + 0x1C);
    if (waitingIcon != NULL) {
        sub_0200F450(waitingIcon);
        *(WaitingIcon **)((u8 *)arg0 + 0x1C) = NULL;
    }
}

void ov49_0225AC38(void *arg0);

void ov49_0225AC38(void *arg0) {
    if (*((s32 *)&((UnkStruct_ov70_0225E9C8 *)arg0)->unk_1C) != 0) {
        sub_0200F478();
        *((s32 *)&((UnkStruct_ov70_0225E9C8 *)arg0)->unk_1C) = 0;
    }
}

s32 ov49_0225AC4C(void *arg0);

s32 ov49_0225AC4C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x1C) != 0) {
        return 1;
    }
    return 0;
}

s32 ov49_0225AC5C(void *arg0);

s32 ov49_0225AC5C(void *arg0) {
    if (TextPrinterCheckActive((u8) *((s32 *)&((UnkStruct_ov70_0225E9C8 *)arg0)->unk_10)) == 0) {
        return 1;
    }
    return 0;
}

void ov49_0225AC74(Window *arg0);

void ov49_0225AC74(Window *window) {
    if (TextPrinterCheckActive((u8) * (s32 *)((u8 *)window + 0x10)) != 0) {
        RemoveTextPrinter((u8) * (s32 *)((u8 *)window + 0x10));
    }
    ov49_0225AC38(window);
    ClearFrameAndWindow2(window, 1);
    ClearWindowTilemapAndScheduleTransfer(window);
}

s32 ov49_0225AAC8();

void ov49_0225ACA8(Window *arg0);

void ov49_0225ACA8(Window *window) {
    ov49_0225AAC8();
    SetWindowPaletteNum(window, 2U);
}

s32 ov49_0225AB14();

void ov49_0225ACBC(void);

void ov49_0225ACBC(void) {
    ov49_0225AB14();
}

void ov49_0225ACC4(Window *arg0, String *arg1);

void ov49_0225ACC4(Window *window, String *src) {
    if (TextPrinterCheckActive((u8) ((Ov49App_0225AC24 *)window)->unk_10) != 0) {
        RemoveTextPrinter((u8) ((Ov49App_0225AC24 *)window)->unk_10);
    }
    FillWindowPixelBuffer(window, 0xFU);
    String_Copy(((Ov49App_0225AC24 *)window)->unk_18, src);
    ((Ov49App_0225AC24 *)window)->unk_10 = (s32)AddTextPrinterParameterized(window, FONT_MESSAGE, ((Ov49App_0225AC24 *)window)->unk_18, 0U, 0U, ((Ov49App_0225AC24 *)window)->unk_14, NULL);
    DrawFrameAndWindow3(window, 1, 0x1FU, 2U, 3U);
}
