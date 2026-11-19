#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "options.h"
#include "overlay_manager.h"
#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "pokemon_types_def.h"
#include "mystery_gift.h"
#include "overlay_74_main_menu_window.h"
#include "pokedex.h"
#include "overlay_74_ranger_link_manager.h"
#include "player_data.h"
#include "message_format.h"
#include "render_window.h"
#include "gf_gfx_planes.h"

void ov74_022295C8(RangerLinkManager *param0, int param1);

void ov74_022295C8(RangerLinkManager *param0, int param1) {
    param0->timeout = param1;
}
