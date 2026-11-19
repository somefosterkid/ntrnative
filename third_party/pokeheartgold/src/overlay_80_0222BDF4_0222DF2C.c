#include "global.h"

struct ItemFieldUseContext;
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_0222DF3C(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);

BOOL FrtCmd_079(ItemFieldUseContext *usageContext);

BOOL FrtCmd_079(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_0222DF3C);
    return 1;
}
