#include "global.h"

#include "bg_window.h"

typedef struct {
    BgTemplate t[3];
} BgTemplate3;

extern const BgTemplate3 ov95_021E7860;

void ov95_021E6FC4(BgConfig *param_1);

void ov95_021E6FC4(BgConfig *param_1) {
    BgTemplate3 templates = ov95_021E7860;

    InitBgFromTemplate(param_1, 4, &templates.t[0], 0);
    BgClearTilemapBufferAndCommit(param_1, 4);
    InitBgFromTemplate(param_1, 5, &templates.t[1], 0);
    BgClearTilemapBufferAndCommit(param_1, 5);
    InitBgFromTemplate(param_1, 6, &templates.t[2], 0);
    BgClearTilemapBufferAndCommit(param_1, 6);
}
