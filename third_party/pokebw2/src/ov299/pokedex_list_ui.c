#include "types.h"
#include "app/pokedex_list_main.h"
#include "app/pokedex_list_ui.h"
#include "app/ui/frame_list.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "save/pokedex_savedata.h"

// The keys and touches of the Pokédex's list, besides those of the list itself. The ROM doesn't name this file;
// pokedex_list_ui.c is a guess after the search screen's pokedex_search_ui.c

// The picture, the icons of the rows, the buttons of the touch bar and the buttons of the Pokédexes, with the
// SELECT button
static const TouchRect sPokedexListTouchHabitat[] = {
#include "asset/pokedex_list_ui_sPokedexListTouchHabitat.inc"
};

// The same before the habitat list, when the START button is where the SELECT button goes
static const TouchRect sPokedexListTouchNoHabitat[] = {
#include "asset/pokedex_list_ui_sPokedexListTouchNoHabitat.inc"
};

int PokedexListUI_Main(PokedexListWork *wk) {
    int ret;

    if (PokeDex_IsHabitatListEnabled(wk->param->pokedex) == TRUE) {
        ret = GFL_HIDGetTouchPressedRect(sPokedexListTouchHabitat);
    } else {
        ret = GFL_HIDGetTouchPressedRect(sPokedexListTouchNoHabitat);
    }
    if (ret != -1) {
        if (ret == POKEDEX_LIST_UI_START) {
            if (PokeDex_IsGiven(wk->param->pokedex) == 0) {
                return -1;
            }
        }
        if (ret == POKEDEX_LIST_UI_SELECT) {
            if (PokeDex_IsHabitatListEnabled(wk->param->pokedex) == FALSE) {
                return -1;
            }
            if (wk->param->searched == TRUE) {
                return -1;
            }
        }
        if (ret == POKEDEX_LIST_UI_SHORTCUT) {
            if (wk->param->searched == TRUE) {
                return -1;
            }
            if (GCTX_HIDIsUsingTouch() == FALSE) {
                GCTX_HIDSetUsingTouch(TRUE);
                FrameList_RedrawCursor(wk->frameList);
            }
        }
        if (ret == POKEDEX_LIST_UI_UNOVA) {
            if (PokeDex_IsNationalObtained(wk->param->pokedex) != TRUE ||
                PokeDex_IsHabitatListEnabled(wk->param->pokedex) != TRUE) {
                return -1;
            }
            if (wk->param->searched == TRUE) {
                return -1;
            }
            if (PokeDex_GetMode(wk->param->pokedex) != 1) {
                return -1;
            }
            GCTX_HIDSetUsingTouch(TRUE);
        }
        if (ret == POKEDEX_LIST_UI_NATIONAL) {
            if (PokeDex_IsNationalObtained(wk->param->pokedex) != TRUE ||
                PokeDex_IsHabitatListEnabled(wk->param->pokedex) != TRUE) {
                return -1;
            }
            if (wk->param->searched == TRUE) {
                return -1;
            }
            // The button of the Pokédex that the list is of does nothing
            if (PokeDex_IsNationalObtained(wk->param->pokedex) == TRUE) {
                if (PokeDex_GetMode(wk->param->pokedex) == 1) {
                    return -1;
                }
            } else {
                if (PokeDex_GetMode(wk->param->pokedex) == 1) {
                    return -1;
                }
            }
            GCTX_HIDSetUsingTouch(TRUE);
        }
        if (ret == POKEDEX_LIST_UI_HABITAT) {
            if (PokeDex_IsHabitatListEnabled(wk->param->pokedex) == FALSE) {
                return -1;
            }
            if (wk->param->searched == TRUE) {
                return -1;
            }
            GCTX_HIDSetUsingTouch(TRUE);
        }
        GCTX_HIDSetUsingTouch(TRUE);
        return ret;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        GCTX_HIDSetUsingTouch(FALSE);
        return POKEDEX_LIST_UI_RETURN;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        GCTX_HIDSetUsingTouch(FALSE);
        return POKEDEX_LIST_UI_EXIT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        if (PokeDex_IsGiven(wk->param->pokedex) == 1) {
            GCTX_HIDSetUsingTouch(FALSE);
            return POKEDEX_LIST_UI_START;
        }
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        if (PokeDex_IsHabitatListEnabled(wk->param->pokedex) == FALSE) {
            return -1;
        }
        if (wk->param->searched == TRUE) {
            return -1;
        }
        GCTX_HIDSetUsingTouch(FALSE);
        return POKEDEX_LIST_UI_SELECT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
        if (wk->param->searched == TRUE) {
            return -1;
        }
        if (GCTX_HIDIsUsingTouch() == TRUE) {
            GCTX_HIDSetUsingTouch(FALSE);
            FrameList_ShowCursor(wk->frameList);
        } else {
            GCTX_HIDSetUsingTouch(FALSE);
        }
        return POKEDEX_LIST_UI_SHORTCUT;
    }
    return ret;
}
