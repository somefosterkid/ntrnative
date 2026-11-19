#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"

#include "overlay_96_ov96_rd_work.h"

typedef struct BgTemplateSet {
    BgTemplate templates[7];
} BgTemplateSet;

extern const GraphicsModes ov96_0221D6C4;

extern const BgTemplateSet ov96_0221D834;

extern const u32 ov96_0221D738[];

void ov96_0221654C(Ov96RdWork *arg);

void ov96_0221654C(Ov96RdWork *arg) {
    GraphicsModes graphicsModes;
    BgTemplateSet templates;
    BgConfig *bgConfig = arg->bgConfig;
    enum HeapID heapId = arg->heapId;
    int i;

    graphicsModes = ov96_0221D6C4;
    SetBothScreensModesAndDisable(&graphicsModes);

    templates = ov96_0221D834;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, ov96_0221D738[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, ov96_0221D738[i]);
        BG_ClearCharDataRange(ov96_0221D738[i], 0x20, 0, heapId);
    }

    arg->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, arg->heapId);
    arg->msgFormat = MessageFormat_New(arg->heapId);
    FontID_Alloc(FONT_UNK_4, arg->heapId);
    GfGfx_EngineATogglePlanes(8, 0);
}
