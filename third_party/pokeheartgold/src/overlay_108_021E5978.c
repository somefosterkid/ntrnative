#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "safari_zone.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"
#include "overlay_108_safari_area_customizer.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct SafariAreaCustomizer;

enum SafariAreaCustomizerState {
    SAC_STATE_SETUP,
    SAC_STATE_TEARDOWN,
    SAC_STATE_SELECT_SLOT,
    SAC_STATE_SELECT_AREA,
    SAC_STATE_MOVE_AREA,
    SAC_STATE_FADE_IN,
    SAC_STATE_FADE_OUT,
    SAC_STATE_EXIT,
};

enum SafariAreaCustomizerInputState {
    SAC_INPUT_STATE_WAIT,
    SAC_INPUT_STATE_MENU,
    SAC_INPUT_STATE_SCROLL,
    SAC_INPUT_STATE_CHANGE_PAGE,
    SAC_INPUT_STATE_SWAP_AREAS = SAC_INPUT_STATE_CHANGE_PAGE,
    SAC_INPUT_STATE_EXIT,
};

s32 ov108_021E5BFC(SafariAreaCustomizer *arg0);

enum SafariAreaCustomizerInputState SafariAreaCustomizer_HandleConfirmArea(SafariAreaCustomizer *customizer);

enum SafariAreaCustomizerInputState SafariAreaCustomizer_SwapAreas(SafariAreaCustomizer *customizer);

s32 ov108_021E5A78(SafariAreaCustomizer *arg0);

s32 ov108_021E5A8C(SafariAreaCustomizer *arg0);

u32 ov108_021E5AA0(SafariAreaCustomizer *param_1);

u32 ov108_021E5AF0(SafariAreaCustomizer *param_1);

u32 ov108_021E5B48(SafariAreaCustomizer *param_1);

s32 ov108_021E5B98(SafariAreaCustomizer *arg0);

BOOL SafariAreaCustomizer_Main(OverlayManager *man, int *state);

BOOL SafariAreaCustomizer_Main(OverlayManager *man, int *state) {
    SafariAreaCustomizer *customizer = OverlayManager_GetData(man);

    switch (*state) {
    case SAC_STATE_SETUP:
        *state = ov108_021E5A78(customizer);
        break;
    case SAC_STATE_TEARDOWN:
        *state = ov108_021E5A8C(customizer);
        break;
    case SAC_STATE_SELECT_SLOT:
        *state = ov108_021E5AA0(customizer);
        break;
    case SAC_STATE_SELECT_AREA:
        *state = ov108_021E5AF0(customizer);
        break;
    case SAC_STATE_MOVE_AREA:
        *state = ov108_021E5B48(customizer);
        break;
    case SAC_STATE_FADE_IN:
        *state = ov108_021E5B98(customizer);
        break;
    case SAC_STATE_FADE_OUT:
        *state = ov108_021E5BFC(customizer);
        break;
    case SAC_STATE_EXIT:
        return TRUE;
    }
    return FALSE;
}
