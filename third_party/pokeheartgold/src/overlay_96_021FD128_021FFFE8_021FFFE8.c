#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_96_ov96_mon_icon_work.h"

void ov96_021FFFE8(Ov96MonIconWork *work) {
    s32 i;

    i = 0;
    do {
        Heap_Free(work->pics[i].charBuf);
        Heap_Free(work->pics[i].plttBuf);
        i += 1;
    } while (i < 3);
    i = 0;
    do {
        Heap_Free(work->charBuf[i]);
        i += 1;
    } while (i < 12);
    Heap_Free(work->scrnBuf);
    i = 0;
    do {
        String_Delete(work->names[i]);
        i += 1;
    } while (i < 3);
    MessageFormat_Delete(work->msgFormat);
    DestroyMsgData(work->msgData);
    RemoveWindow(&work->windows[0]);
    RemoveWindow(&work->windows[1]);
    Heap_Free(work);
}
