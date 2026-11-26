#include "global.h"

#include "error_handling.h"
#include "message_format.h"
#include "overlay_111.h"
#include "pokemon.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov111_021E5D2C(Window *, u8, u8);
#else
void ov111_021E5D2C(Window *, u16, u16);
#endif

void ov111_021E64C8(SwapMonPanel *panel, SwapMonText *text) {
    u32 v0;
    u32 v2;
    s32 v1;
    MessageFormat *messageFormat;
    Pokemon *mon;
    s32 v4;
    u8 v3;

    messageFormat = ov111_021E6A2C(text);
    mon = panel->mon;
    if (panel == NULL) {
        GF_AssertFail();
    }
    if (text == NULL) {
        GF_AssertFail();
    }
    v0 = GetMonData(mon, 0xA3, NULL);
    v2 = GetMonData(mon, 0xA4, NULL);
    BufferBoxMonSpeciesName(messageFormat, 0U, Mon_GetBoxMon(mon));
    ov111_021E6934(text, &panel->windows[0], 4, 4, 1);
    v4 = 0x70800;
    v3 = GetMonGender(mon);
    switch (v3) {
    case 0:
        v1 = 5;
        break;
    case 1:
        v1 = 6;
        v4 = 0x30400;
        break;
    case 2:
        v1 = 7;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov111_021E696C(text, &panel->windows[1], v1, 0, 1, v4);
    ov111_021E69A0(text, &panel->windows[2], GetMonData(mon, 0xA1, NULL), 3, 1, 1);
    ov111_021E69A0(text, &panel->windows[3], v0, 3, 0, 0);
    ov111_021E69A0(text, &panel->windows[4], v2, 3, 1, 0);
    ov111_021E5D2C(&panel->windows[5], (u16)v0, (u16)v2);
}

void ov111_021E65CC(SwapMonPanel *panel, s32 sliding, s32 targetY) {
    panel->sliding = sliding;
    panel->targetY = targetY;
}
