#include "global.h"

#include "constants/sndseq.h"

#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_02013FDC.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_0205BFF0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

struct Sprite;

void ov83_0224759C(BattleCastleAppSprite *obj, int x, int y);

void ov83_0224759C(BattleCastleAppSprite *obj, int x, int y) {
    VecFx32 vec = *Sprite_GetMatrixPtr(obj->sprite);
    vec.x = x * FX32_ONE;
    vec.y = y * FX32_ONE + GX_LCD_SIZE_Y * FX32_ONE;
    Sprite_SetMatrix(obj->sprite, &vec);
}
