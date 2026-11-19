#include "global.h"

#include "bg_window.h"

extern BgTemplate ov112_021FF134;

extern BgTemplate ov112_021FF150;

void ov112_021F0668(BgConfig *arg0);

void ov112_021F0668(BgConfig *arg0) {
    BgTemplate sp1C;
    BgTemplate sp0;

    sp1C = ov112_021FF134;
    InitBgFromTemplate(arg0, 5U, &sp1C, 0U);
    BgClearTilemapBufferAndCommit(arg0, 5U);
    sp0 = ov112_021FF150;
    InitBgFromTemplate(arg0, 7U, &sp0, 0U);
    BgClearTilemapBufferAndCommit(arg0, 7U);
}
