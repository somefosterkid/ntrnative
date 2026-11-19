#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "overlay_68_move_relearner.h"
struct MsgData;

u32 ov68_021E6678(MoveRelearner *work);

s32 ov68_021E67E0(MoveRelearner *);

void ov68_021E6820(MoveRelearner *arg0);

void ov68_021E6820(MoveRelearner *arg0) {
    MsgData *temp_r7;
    u16 temp_r2;
    u32 var_r4;

    arg0->numMoves = ov68_021E6678(arg0);
    arg0->moveList = ListMenuItems_New((u32) arg0->numMoves, HEAP_ID_66);
    temp_r7 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2EE, HEAP_ID_66);
    var_r4 = 0;
    if (var_r4 < (u32) arg0->numMoves) {
        do {
            temp_r2 = arg0->args->eligibleMoves[var_r4];
            if (temp_r2 != 0xFFFF) {
                ListMenuItems_AppendFromMsgData(arg0->moveList, temp_r7, (s32)temp_r2, (s32)temp_r2);
            } else {
                ListMenuItems_AppendFromMsgData(arg0->moveList, arg0->msgData, 0x20, -2);
                break;
            }
            var_r4 += 1;
        } while (var_r4 < (u32) arg0->numMoves);
    }
    DestroyMsgData(temp_r7);
    arg0->unk_1BB = 0;
    arg0->unk_1BC = (s8)arg0->args->cursorPos;
    ov68_021E67E0(arg0);
}
