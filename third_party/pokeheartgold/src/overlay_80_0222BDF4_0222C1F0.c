#include "global.h"

struct ItemFieldUseContext;
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_0222C200(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);

BOOL FrtCmd_020(ItemFieldUseContext *usageContext);

BOOL FrtCmd_020(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_0222C200);
    return 1;
}
