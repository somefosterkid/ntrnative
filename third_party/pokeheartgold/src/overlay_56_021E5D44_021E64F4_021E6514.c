#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "msgdata.h"
#include "overlay_56_mail_viewer_app.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct ListMenu2D;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct ListMenu2D ListMenu2D;

void MailViewer_SetVRAMBanks(void);

void ov56_021E64F4(void);

void ov56_021E6514(MailViewerApp *mailViewerApp);

extern const GraphicsModes _021E6DD4;

typedef struct {
    BgTemplate a[5];
} ov56_021E6E80_Array;

extern const ov56_021E6E80_Array ov56_021E6E80;

void ov56_021E6514(MailViewerApp *mailViewerApp) {
    ov56_021E64F4();

    mailViewerApp->bgConfig = BgConfig_Alloc(mailViewerApp->heapID);

    GraphicsModes graphicsModes = _021E6DD4;

    SetBothScreensModesAndDisable(&graphicsModes);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    ov56_021E6E80_Array bgTemplates = ov56_021E6E80;

    InitBgFromTemplate(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_0, &bgTemplates.a[0], BG_TYPE_STATIC);
    InitBgFromTemplate(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_1, &bgTemplates.a[1], BG_TYPE_STATIC);
    InitBgFromTemplate(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_2, &bgTemplates.a[2], BG_TYPE_STATIC);
    InitBgFromTemplate(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_3, &bgTemplates.a[3], BG_TYPE_STATIC);
    InitBgFromTemplate(mailViewerApp->bgConfig, GF_BG_LYR_SUB_0, &bgTemplates.a[4], BG_TYPE_STATIC);

    BgClearTilemapBufferAndCommit(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(mailViewerApp->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(mailViewerApp->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, (8 * (8 / 2)), 0, mailViewerApp->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, (8 * (8 / 2)), 0, mailViewerApp->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, (8 * (8 / 2)), 0, mailViewerApp->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, (8 * (8 / 2)), 0, mailViewerApp->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, (8 * (8 / 2)), 0, mailViewerApp->heapID);
}
