#include "global.h"

#include "bg_window.h"
#include "overlay_87_scratch_off_card_app.h"

extern const WindowTemplate ov87_021E842C[NUM_SCRATCH_WINDOWS];

void ov87_021E80C0(BgConfig *bgConfig, Window *windows);

extern const WindowTemplate sWinTemplates[NUM_SCRATCH_WINDOWS];
void ov87_021E80F0(Window *windows);

void ov87_021E80C0(BgConfig *bgConfig, Window *windows) {
    const WindowTemplate *templates = ov87_021E842C;

    for (u8 i = 0; i < NELEMS(ov87_021E842C); i++) {
        AddWindow(bgConfig, &windows[i], &templates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void ov87_021E80F0(Window *windows) {
    for (u16 i = 0; i < (sizeof(sWinTemplates) / sizeof(*(sWinTemplates))); i++) {
        RemoveWindow(&windows[i]);
    }
}
