#include "global.h"

#include "bg_window.h"
#include "overlay_112_sprite_app.h"

typedef struct {
    BgTemplate a[5];
} ov112_021FF434_Array;

extern const GraphicsModes ov112_021FF2F4;

extern const ov112_021FF434_Array ov112_021FF434;

extern const u8 ov112_021FF4C0[5];

void ov112_021F179C(Ov112SpriteApp *work);

void ov112_021F179C(Ov112SpriteApp *work) {
    int i;
    work->bgConfig = BgConfig_Alloc(work->heapID);
    {
        GraphicsModes graphicsModes = ov112_021FF2F4;
        SetBothScreensModesAndDisable(&graphicsModes);
    }
    {
        ov112_021FF434_Array templates = ov112_021FF434;
        for (i = 0; i < 5; i++) {
            InitBgFromTemplate(work->bgConfig, ov112_021FF4C0[i], &templates.a[i], 0);
            BgClearTilemapBufferAndCommit(work->bgConfig, ov112_021FF4C0[i]);
            BG_FillCharDataRange(work->bgConfig, (GFBgLayer)ov112_021FF4C0[i], 0, 1, 0);
        }
    }
}
