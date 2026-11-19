#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "error_handling.h"
#include "field_system.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "overlay_34.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_palpad.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

extern const TouchscreenHitbox ov34_0225E6AC[5];

int ov34_0225E020(Ov34Work *work) {
    u32 x, y;
    int hit = TouchscreenHitbox_FindRectAtTouchHeld(ov34_0225E6AC);

    if (hit != -1) {
        switch (hit) {
        case 0:
            ov34_0225E5DC(work, FALSE);
            System_GetTouchHeldCoords(&x, &y);
            ov34_0225DDB8(work->sprites[2], y);

            if (work->count > 3) {
                int step, i;

                step = (192 - 8 * 8 - 16 * 2) / (work->count - 2);

                for (i = 0; i < work->count - 2; i++) {
                    if ((y >= (8 * 4 + 16) + step * i) && (y < (8 * 4 + 16) + step * (i + 1))) {
                        work->scroll = i;
                        break;
                    }
                }
            }
            break;
        default:
            if (work->chatLog->count >= hit) {
                if (work->selectedRow == (hit - 1)) {
                    if (work->rows[hit - 1].unk34 < 2 * 2 + 1) {
                        work->rows[hit - 1].unk34++;
                    }
                }
            }
            break;
        }
    } else {
        ov34_0225E5DC(work, TRUE);
    }

    return hit;
}

struct System;

void ov34_0225E0E4(Ov34Work *work) {
    if (gSystem.heldKeys & PAD_BUTTON_L) {
        ov34_0225E5EC(work, 0);

        if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
            if (work->scroll != 0) {
                work->scroll--;
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
        }
    } else if (gSystem.heldKeys & PAD_BUTTON_R) {
        ov34_0225E5EC(work, 1);

        if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
            if (work->scroll < work->count - 3) {
                work->scroll++;
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
        }
    }
}
