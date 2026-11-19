#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_3d_loader.h"
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

struct Background;

struct BgConfig;

struct BgTemplate;

struct DpadMenuBox;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct GridCallbacks;

struct GridInputHandler;

struct NumberEntryRodata;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SysTask;

struct SysTaskQueue;

struct System;

struct Window;

struct WindowTemplate;

enum NumberEntryState {
    NUMBER_ENTRY_STATE_INPUT = 0,
    NUMBER_ENTRY_STATE_BLINK,
    NUMBER_ENTRY_STATE_DONE,
};

void ov29_0225DC50(NumberEntryData *data);

BOOL ov29_0225DF74(NumberEntryData *data);

typedef struct NumberEntryRodata {
    u8 xOffsets1[1];
    TouchscreenHitbox hitboxes1[2];
    TouchscreenHitbox hitboxes2[3];
    TouchscreenHitbox hitboxes3[4];
    GridCallbacks gridCallbacks;
    TouchscreenHitbox hitboxes4[5];
    TouchscreenHitbox hitboxes5[6];
    WindowTemplate windowTemplates[3];
    TouchscreenHitbox hitboxes6[7];
    const TouchscreenHitbox *hitboxes[7];
    BgTemplate bgTemplate4;
    const u8 *xOffsets[7];
    BgTemplate bgTemplate5;
    u8 spriteSpecs[11][4];
    TouchscreenHitbox padHitboxes[14];
    DpadMenuBox padDpadBoxes[13];
    u8 xOffsets2[4];
    u8 xOffsets3[4];
    u8 xOffsets4[4];
    u8 xOffsets5[8];
    u8 xOffsets6[8];
} NumberEntryRodata;

extern const NumberEntryRodata ov29_0225E114;

void ov29_0225DB9C(void *param_1, u32 param_2);

void ov29_0225DCD0(SysTask *task, void *taskData);

void ov29_0225DCD0(SysTask *task, void *taskData) {
    NumberEntryData *data = taskData;
    u32 input;

    switch (data->state) {
    case NUMBER_ENTRY_STATE_INPUT:
        input = TouchscreenHitbox_FindRectAtTouchNew(ov29_0225E114.hitboxes[data->numDigits]);
        if (input != -1) {
            data->cursor = input;
            ov29_0225DC50(data);
            ov29_0225E028(data, input + 5, data->digits[data->cursor] + 12, data->digits[data->cursor] + 1, 0);
            PlaySE(1501);
        } else if (gSystem.newKeys & 0x0008) {
            GridInputHandler_SetNextInput(data->gridInput, 11);
            ov29_0225DB9C((u8 *)(data), 11);
            PlaySE(1501);
        } else if (gSystem.newKeys & 0x0002) {
            if (ov29_0225DEB8(data, -1) == 1) {
                PlaySE(1501);
            }
        } else {
            input = GridInputHandler_HandleInput_AllowHold(data->gridInput);
            switch (input) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                ov29_0225DEF4(data, input);
                ov29_0225DEB8(data, 1);
                ov29_0225E078(data, input % 5 * 6 + 1, input / 5 * 6 + 7, NUMBER_ENTRY_STATE_INPUT);
                PlaySE(1501);
                break;
            case 10:
                ov29_0225E028(data, 2, 4, 3, 0);
                ov29_0225DEB8(data, -1);
                PlaySE(1501);
                break;
            case 11:
                ov29_0225E0E0(data);
                if (data->value > data->args->max) {
                    ov29_0225E028(data, 3, 4, 3, 0);
                    PlaySE(1501);
                } else {
                    data->args->result = data->value;
                    ov29_0225E028(data, 3, 4, 3, 2);
                    PlaySE(1501);
                }
                break;
            case 12:
                data->args->result = 0;
                ov29_0225E028(data, 4, 4, 3, 2);
                PlaySE(1501);
                break;
            }
        }
        break;
    case NUMBER_ENTRY_STATE_BLINK:
        if (!ov29_0225DF74(data)) {
            data->state = data->nextState;
        }
        break;
    case NUMBER_ENTRY_STATE_DONE:
        break;
    }

    ov29_0225DB7C(data);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}
