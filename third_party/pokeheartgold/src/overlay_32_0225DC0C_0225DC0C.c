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
#include "sprite_resources_header.h"

void ov32_0225DC0C(NamePickerData *data, int index, SpriteResourcesHeader *header, const u8 *spec) {
    SpriteTemplate template;

    template.spriteList = data->spriteList;
    template.header = header;
    template.position.x = spec[0] << FX32_SHIFT;
    template.position.y = (spec[1] << FX32_SHIFT) + FX32_CONST(256);
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = spec[2];
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_8;
    data->sprites[index] = Sprite_CreateAffine(&template);
    Sprite_SetAnimCtrlSeq(data->sprites[index], spec[3]);
}
