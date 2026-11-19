#include "global.h"

#include "bg_window.h"

#include "overlay_96_ov96_rd_work.h"

extern WindowTemplate ov96_0221D6A4;

void ov96_022193E4(Ov96RdWork *work);

void ov96_022193E4(Ov96RdWork *work) {
    AddWindow(work->bgConfig, &work->window, &ov96_0221D6A4);
}
