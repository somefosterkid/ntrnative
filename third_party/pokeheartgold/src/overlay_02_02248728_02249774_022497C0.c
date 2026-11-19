#include "global.h"

#include "overlay_02_hmcut_in.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "field_roamer.h"
#include "field_system.h"
#include "field_warp_tasks.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "roamer.h"
#include "save_local_field_data.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "unk_02062108.h"

struct BoxPokemon;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct PartyPokemon;

struct PokedexData;

struct SEAL;

struct Sprite;

struct String;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

int ov02_022497C0(HMCutIn *work);

int ov02_022497C0(HMCutIn *work) {
    VecFx32 vec;
    fx32 v = work->pokemonSpriteDeltaX / 2;
    work->pokemonSpriteDeltaX = v;
    if (v > (fx32)0xFFFFE000) {
        work->pokemonSpriteDeltaX = (fx32)0xFFFFE000;
        work->state++;
        {
            int species = GetMonData(work->mon, 5, 0L);
            int form = GetMonData(work->mon, 112, 0L);
            PlayCry((u16)species, (u8)form);
        }
    }
    vec = *Sprite_GetMatrixPtr(work->pokemonSprite);
    vec.x = vec.x + work->pokemonSpriteDeltaX;
    Sprite_SetMatrix(work->pokemonSprite, &vec);
    return 0;
}
