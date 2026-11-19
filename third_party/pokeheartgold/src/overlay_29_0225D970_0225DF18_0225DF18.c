#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_29.h"
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
#include "overlay_29_number_entry_data.h"

struct BgConfig;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct GridInputHandler;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SysTask;

struct SysTaskQueue;

struct Window;

void ov29_0225DF18(NumberEntryData *data, u8 seq);

void ov29_0225DF18(NumberEntryData *data, u8 seq) {
    if (!data->blink.isBg) {
        Sprite_SetAnimCtrlSeq(data->sprites[data->blink.target], seq);
    } else {
        BgTilemapRectChangePalette(data->bgConfig, data->blink.target, data->blink.x, data->blink.y, data->blink.width, data->blink.height, seq);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, data->blink.target);
    }
}

BOOL ov29_0225DF74(NumberEntryData *data);

BOOL ov29_0225DF74(NumberEntryData *data) {
    NumberEntryBlink *blink = &data->blink;

    switch (blink->phase) {
    case 0:
        ov29_0225DF18(data, blink->seqA);
        blink->phase++;
        break;
    case 1:
        blink->timer++;
        if (blink->timer == 4) {
            ov29_0225DF18(data, blink->seqB);
            blink->timer = 0;
            blink->phase++;
        }
        break;
    case 2:
        blink->timer++;
        if (blink->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}
