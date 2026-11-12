// The keys and touches of the Pokédex's list, besides those of the list itself: pokebw2's src/ov299/zknlist_ui.c
// (GPL-3.0, https://github.com/fuddlesworth/pokebw2), the file after zknlist_obj.c. Black's code is 0x021be658-0x021be794
// and its touch table 0x021beae0

#include "types.h"
#include "app/zknlist_ui.h"
#include "app/ui/frame_list.h"
#include "app/zknlist_main.h"
#include "app/zukanlist.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "save/pokedex.h"

// The picture, the icons of the rows and the buttons of the touch bar
static const TouchRect data_ov209_021beae0[] = {
#include "asset/zknlist_ui_data_ov209_021beae0.inc"
};


int func_ov209_021be658(ZknListWork *wk) {
    int ret = func_02035c08(data_ov209_021beae0);

    if (ret != -1) {
        if (ret == ZKNLIST_UI_START) {
            if (func_0200ca54(wk->param->pokedex) == 0) {
                return -1;
            }
        }
        if (ret == ZKNLIST_UI_SELECT) {
            if (PokeDex_IsNationalObtained(wk->param->pokedex) == FALSE) {
                return -1;
            }
            if (wk->param->searched == TRUE) {
                return -1;
            }
        }
        if (ret == ZKNLIST_UI_SHORTCUT) {
            if (wk->param->searched == TRUE) {
                return -1;
            }
            if (func_020355c4() == FALSE) {
                func_020355d8(TRUE);
                func_ov067_021b941c(wk->frameList);
            }
        }
        func_020355d8(TRUE);
        return ret;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        return ZKNLIST_UI_RETURN;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        return ZKNLIST_UI_EXIT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        if (func_0200ca54(wk->param->pokedex) == 1) {
            func_020355d8(FALSE);
            return ZKNLIST_UI_START;
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        if (PokeDex_IsNationalObtained(wk->param->pokedex) == FALSE) {
            return -1;
        }
        if (wk->param->searched == TRUE) {
            return -1;
        }
        func_020355d8(FALSE);
        return ZKNLIST_UI_SELECT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
        if (wk->param->searched == TRUE) {
            return -1;
        }
        if (func_020355c4() == TRUE) {
            func_020355d8(FALSE);
            func_ov067_021b940c(wk->frameList);
        } else {
            func_020355d8(FALSE);
        }
        return ZKNLIST_UI_SHORTCUT;
    }
    return ret;
}

