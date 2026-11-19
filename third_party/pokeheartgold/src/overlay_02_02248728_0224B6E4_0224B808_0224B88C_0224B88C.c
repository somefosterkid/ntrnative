#include "global.h"

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
#include "overlay_02_hmcut_in.h"
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

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct Mail;

struct MailMessage;

struct PartyPokemon;

struct PlayerProfile;

struct PokedexData;

struct SEAL;

struct String;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

extern void ov01_021FBD38(Field3dModel *model, void *narcData);

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

void ov02_0224B88C(HMCutIn *work);

void ov02_0224B88C(HMCutIn *work) {
    HMCutInFieldModel *d = &work->fieldModel;
    HeapExp_FndInitAllocator(&d->allocator, HEAP_ID_FIELD1, 0x20);
    d->modelData = AllocAtEndAndReadWholeNarcMemberByIdPair((NarcId)0x67, 0x83, HEAP_ID_FIELD1);
    ov01_021FBD38(&d->model, d->modelData);
    Field3dObject_InitFromModel(&d->object, &d->model);
    Field3dModelAnimation_LoadFromFilesystem(&d->anims[0], &d->model, (NarcId)0x67, 0xa7, HEAP_ID_FIELD1, &d->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&d->anims[1], &d->model, (NarcId)0x67, 0xa5, HEAP_ID_FIELD1, &d->allocator);
    Field3dObject_AddAnimation(&d->object, &d->anims[0]);
    Field3dObject_AddAnimation(&d->object, &d->anims[1]);
}
