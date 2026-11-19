#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_32.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "unk_0202C034.h"
#include "overlay_32_name_picker_data.h"

void ov32_0225DFE8(NamePickerData *data, u8 seq) {
    if (!data->blink.isBg) {
        Sprite_SetAnimCtrlSeq(data->sprites[data->blink.target], seq);
    } else {
        BgTilemapRectChangePalette(data->bgConfig, data->blink.target, data->blink.x, data->blink.y, data->blink.width, data->blink.height, seq);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, data->blink.target);
    }
}
