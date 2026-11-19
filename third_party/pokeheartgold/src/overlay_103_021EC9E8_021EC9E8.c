#include "global.h"

#include "bg_window.h"
#include "constants/heap.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EC9E8(MailBoxWork *param0);

extern const GraphicsModes ov103_021EEB40;

extern const BgTemplate ov103_021EEBC0;

extern const BgTemplate ov103_021EEBA4;

extern const BgTemplate ov103_021EEB88;

extern const BgTemplate ov103_021EEC14;

extern const BgTemplate ov103_021EEBDC;

extern const BgTemplate ov103_021EEB50;

extern const BgTemplate ov103_021EEB6C;

extern const BgTemplate ov103_021EEBF8;

void ov103_021EC9E8(MailBoxWork *param0) {
    param0->view->bgConfig = BgConfig_Alloc(HEAP_ID_157);

    {
        GraphicsModes v0 = ov103_021EEB40;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov103_021EEBC0;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_SUB_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_SUB_0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_157);
    }

    {
        BgTemplate v2 = ov103_021EEBA4;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_SUB_1, &v2, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_SUB_1);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_157);
    }

    {
        BgTemplate v3 = ov103_021EEB88;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_SUB_2, &v3, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_SUB_2);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_157);
    }

    {
        BgTemplate v4 = ov103_021EEC14;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_SUB_3, &v4, 0);
    }

    {
        BgTemplate v5 = ov103_021EEBDC;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_MAIN_0, &v5, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_MAIN_0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_157);
    }

    {
        BgTemplate v6 = ov103_021EEB50;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_MAIN_1, &v6, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_MAIN_1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_157);
    }

    {
        BgTemplate v7 = ov103_021EEB6C;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_MAIN_2, &v7, 0);
        BgClearTilemapBufferAndCommit(param0->view->bgConfig, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v8 = ov103_021EEBF8;

        InitBgFromTemplate(param0->view->bgConfig, GF_BG_LYR_MAIN_3, &v8, 0);
    }
}
