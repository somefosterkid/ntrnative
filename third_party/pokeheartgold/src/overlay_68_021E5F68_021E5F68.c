#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "move_relearner.h"
#include "overlay_68_move_relearner.h"

void ov68_021E7898(MoveRelearner *param0, u32 param1);

void ov68_021E7A18(MoveRelearner *param0, u32 param1);

u32 ov68_021E7A90(MoveRelearner *param0, u32 param1);

u32 ov68_021E7AB4(MoveRelearner *param0, u32 param1);

u32 ov68_021E7B94(MoveRelearner *param0);

u32 ov68_021E5F68(MoveRelearner *param0);

u32 ov68_021E5F68(MoveRelearner *param0) {
    u32 input;
    u32 result;
    BOOL touch;

    param0->lastGridInput = GridInputHandler_GetNextInput(param0->gridInput);
    input = GridInputHandler_HandleInput_AllowHold(param0->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        touch = System_GetTouchNew();
        if (touch == 0) {
            if (param0->args->scrollOffset + input < param0->numMoves) {
                PlaySE(0x5dd);
                ov68_021E7A18(param0, input);
                result = ov68_021E7B94(param0);
                return result;
            }
        } else if (param0->args->scrollOffset + input < param0->numMoves) {
            PlaySE(0x5dd);
            ov68_021E7A18(param0, input);
        } else {
            ov68_021E7A18(param0, 5);
        }
        break;
    case 4:
    case 5:
        break;
    case 0xFFFFFFFC:
    case 0xFFFFFFFD:
    case 0xFFFFFFFF:
        break;
    case 6:
    case 0xFFFFFFFE:
        PlaySE(0x5dd);
        ov68_021E7A18(param0, 5);
        ov68_021E7898(param0, 0);
        result = ov68_021E7A90(param0, 0xe);
        return result;
    case 7:
        PlaySE(0x5dd);
        result = ov68_021E7AB4(param0, 0xf);
        return result;
    }

    return 1;
}
