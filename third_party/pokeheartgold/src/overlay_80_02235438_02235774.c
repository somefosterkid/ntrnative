#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier_script_context.h"

#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "unk_02035900.h"

struct BattleTower;
typedef struct BattleTower BattleTower;
u16 ov80_02236AD8(BattleTower *battleTower, const u16 *payload);
u16 *ov80_0222BE24(FrontierScriptContext *ctx, u16 varID);
const void *sub_02037C44(int param0);

BOOL ov80_02235774(FrontierScriptContext *ctx, BattleTower *battleTower, SaveData *saveData, u16 cmd, u16 destVar);

BOOL ov80_02235774(FrontierScriptContext *ctx, BattleTower *battleTower, SaveData *saveData, u16 cmd, u16 destVar) {
    const void *payload = sub_02037C44(1 - sub_0203769C());

    if (payload == 0L) {
        return 0;
    }

    u16 *isRetiring = ov80_0222BE24(ctx, destVar);

    switch (cmd) {
    case 2:
        *isRetiring = ov80_02236AD8(battleTower, payload);
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 1;
}
