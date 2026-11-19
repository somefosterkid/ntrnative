#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

extern u32 ov96_0221CF58[];

extern const GraphicsModes ov96_0221CF0C;

typedef struct {
    BgTemplate a[7];
} ov96_0221D088_Array;

extern const ov96_0221D088_Array ov96_0221D088;

void ov96_0220ED9C(UnkStruct_ov96_0220E3F0 *arg0);

void ov96_0220ED9C(UnkStruct_ov96_0220E3F0 *arg0) {
    BgConfig *bgConfig = arg0->bgConfig;
    enum HeapID heapId = arg0->heapId;
    GraphicsModes graphicsModes = ov96_0221CF0C;
    u32 i;

    SetBothScreensModesAndDisable(&graphicsModes);

    ov96_0221D088_Array bgTemplates = ov96_0221D088;

    for (i = 0; (s32)i < 7; i++) {
        InitBgFromTemplate(bgConfig, (u8)ov96_0221CF58[i], &bgTemplates.a[i], 0);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)ov96_0221CF58[i]);
        BG_ClearCharDataRange((u8)ov96_0221CF58[i], 0x20, 0, heapId);
    }

    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, arg0->heapId);
    arg0->msgFormat = MessageFormat_New(arg0->heapId);
    FontID_Alloc(FONT_UNK_4, arg0->heapId);
    GfGfx_EngineATogglePlanes(4, 0);
}
