#include "global.h"

#include "constants/sndseq.h"

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

struct ObjCharTransferTemplate;

struct PokedexData;

extern const ObjCharTransferTemplate ov83_0224817C;

void ov83_022472DC(void);

void ov83_022472DC(void) {
    ObjCharTransferTemplate template = ov83_0224817C;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(32, HEAP_ID_107);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
