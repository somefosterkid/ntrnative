#include "global.h"
struct MoveRelearner;

#include "list_menu_items.h"
#include "overlay_68_move_relearner.h"

void ov68_021E68C4(struct MoveRelearner *arg0);

void ov68_021E68C4(MoveRelearner *arg0) {
    ListMenuItems_Delete(*((ListMenuItem * *)&arg0->moveList));
}
