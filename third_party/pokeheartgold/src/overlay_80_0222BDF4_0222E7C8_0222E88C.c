#include "global.h"

#include "frontier/frontier_menu_manager.h"
#include "frontier/frontier_script_manager.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

void ov80_0222E88C(FrontierMenuManager *menuManager);

void ov80_0222E88C(FrontierMenuManager *menuManager) {
    s32 i;
    FrontierScriptManager *scriptMan;

    scriptMan = menuManager->scriptMan;
    PlaySE(0x5DCU);
    Delete2dMenu(menuManager->menu, NULL);
    ClearWindowPixelBuffer(menuManager->menuTemplate.window, 0);
    RemoveWindow(menuManager->menuTemplate.window);
    for (i = 0; i < 0x1C; i++) {
        String_Delete(menuManager->choiceStringBuffers[i]);
    }
    if (menuManager->freeMsgLoaderOnDelete == 1) {
        DestroyMsgData(menuManager->msgLoader);
    }
    SysTask_Destroy(menuManager->sysTask);
    Heap_Free(menuManager);
    scriptMan->menuMan = NULL;
}
