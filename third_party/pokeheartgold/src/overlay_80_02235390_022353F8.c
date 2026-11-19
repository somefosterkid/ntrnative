#include "global.h"

struct ItemFieldUseContext;
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_02235408(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);

BOOL FrtCmd_089(ItemFieldUseContext *usageContext);

BOOL FrtCmd_089(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_02235408);
    return 1;
}
