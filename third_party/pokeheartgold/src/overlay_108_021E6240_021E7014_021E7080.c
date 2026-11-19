#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_108_safari_area_customizer.h"

extern const u32 ov108_021EA724[2];

extern const WindowTemplate ov108_021EA7A8[5];

void ov108_021E7080(SafariAreaCustomizer *work);

void ov108_021E7080(SafariAreaCustomizer *work) {
    int i;
    u8 t;
    u8 h;
    int baseTile;
    u8 y8;
    int idx;

    for (i = 0; i <= 4; i++) {
        AddWindow(work->bgConfig, &work->windows[i], &ov108_021EA7A8[i]);
        FillWindowPixelBuffer(&work->windows[i], 0);
    }
    baseTile = 0x3A6;
    for (i = 0; i < 5; i++) {
        idx = i * 2 + 5;
        y8 = i * 4 + 1;
        t = 1 ^ (i / 4);
        h = t * 2 + 2;
        baseTile -= 22 * h;
        AddWindowParameterized(work->bgConfig, &work->windows[idx], 7, 1, y8, 22, h, 5, baseTile);
        FillWindowPixelBuffer(&work->windows[idx], ov108_021EA724[i % 2] & 0xFF);
        baseTile -= 22 * h;
        AddWindowParameterized(work->bgConfig, &work->windows[idx + 1], 7, 26, y8 + t, 5, 2, 5, baseTile);
        FillWindowPixelBuffer(&work->windows[idx + 1], ov108_021EA724[i % 2] & 0xFF);
        baseTile -= 10;
    }
    InitWindow(&work->textWindow);
    AddTextWindowTopLeftCorner(work->bgConfig, &work->textWindow, 6, 2, 0x2E1, 0);
    FillWindowPixelBufferText_AssumeTileSize32(&work->textWindow, 0);
    work->yesNoPrompt = YesNoPrompt_Create(work->heapID);
}
