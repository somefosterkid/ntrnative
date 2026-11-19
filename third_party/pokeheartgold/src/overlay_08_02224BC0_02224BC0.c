#include "global.h"
#include "overlay_08_battle_sub_menu_cursor.h"
struct BattleSubMenuCursor;

void ov08_02224BC0(struct BattleSubMenuCursor *param_1);

void ov08_02224BC0(BattleSubMenuCursor *param_1) {
    param_1->currentPositionIndex = 0;
    param_1->previousPositionIndex = 0xff;
    return;
}
