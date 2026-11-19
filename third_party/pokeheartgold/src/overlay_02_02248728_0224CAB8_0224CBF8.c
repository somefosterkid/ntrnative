#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "easy_chat.h"
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

struct PlayerProfile;

struct PokedexData;

struct SEAL;

struct String;

struct WallpaperPasswordBank;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

extern void ov02_0224CA58(u8 arr[], u8 length, u8 iterations);

int ov02_0224CBF8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d);

int ov02_0224CBF8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d) {
    s16 idx[4];
    u8 out[4];
    int count;
    int k;
    int val;
    int combined;

    count = WallpaperPasswordBank_GetCount(bank);
    idx[0] = WallpaperPasswordBank_GetIndexOfWord(bank, a);
    idx[1] = WallpaperPasswordBank_GetIndexOfWord(bank, b);
    idx[2] = WallpaperPasswordBank_GetIndexOfWord(bank, c);
    idx[3] = WallpaperPasswordBank_GetIndexOfWord(bank, d);

    for (k = 0; k < 4; k++) {
        if (idx[k] < 0) {
            return -1;
        }
        if (k > 0) {
            if (idx[k] >= idx[k - 1]) {
                int v = idx[k] - idx[k - 1];
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            } else {
                int v = count - (idx[k - 1] - idx[k]);
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            }
        } else {
            if (idx[0] > 0xff) {
                return -1;
            }
            out[0] = idx[0];
        }
    }

    ov02_0224CA58(out, 4, 5);

    for (k = 0; k < 3; k++) {
        out[k] ^= (out[3] >> 4) | (out[3] & 0xf0);
    }

    ov02_0224CA58(out, 3, out[3] & 0xf);

    val = out[0] & 0xf;
    if ((u8)val < 8 || (u8)val >= 0xb) {
        return -1;
    }
    out[1] ^= out[0];
    out[2] ^= out[0];
    combined = (out[1] << 8) | out[2];
    if (trainerId == combined && ((out[0] & 0xf0) >> 4) == 6 && out[3] == (u8)((out[0] + out[1]) * out[2])) {
        return val;
    }
    return -1;
}
