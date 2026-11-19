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

void ov34_0225E5EC(Ov34Work *work, int index) {
    u32 frame;
    u16 anim;

    frame = Sprite_GetAnimationFrame(work->sprites[index]);
    anim = (int)Sprite_GetAnimationNumber(work->sprites[index]);

    if ((frame != 0) || (anim != index + 4)) {
        Sprite_SetAnimCtrlSeq(work->sprites[index], index + 4);
    }

    work->animFlags[index] = 1;
}
