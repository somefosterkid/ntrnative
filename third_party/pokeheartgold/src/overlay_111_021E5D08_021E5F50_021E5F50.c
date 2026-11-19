#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_111.h"

typedef struct {
    BgTemplate a[5];
} ov111_021E6CA4_Array;

extern const GraphicsModes ov111_021E6B7C;

extern const ov111_021E6CA4_Array ov111_021E6CA4;

extern const u8 _021E6B74[5];

void ov111_021E5F50(BugContestSwapMon *work);

void ov111_021E5F50(BugContestSwapMon *work) {
    int i;
    work->bgConfig = BgConfig_Alloc(work->heapID);
    {
        GraphicsModes graphicsModes = ov111_021E6B7C;
        SetBothScreensModesAndDisable(&graphicsModes);
    }
    {
        ov111_021E6CA4_Array templates = ov111_021E6CA4;
        for (i = 0; i < 5; i++) {
            InitBgFromTemplate(work->bgConfig, _021E6B74[i], &templates.a[i], 0);
            BgClearTilemapBufferAndCommit(work->bgConfig, _021E6B74[i]);
            BG_FillCharDataRange(work->bgConfig, (enum GFBgLayer)_021E6B74[i], 0, 1, 0);
        }
    }
    work->narc = NARC_New(NARC_a_1_7_3, work->heapID);
}
