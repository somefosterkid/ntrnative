#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
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
#include "overlay_02_0224D0C8.h"
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

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BoxPokemon;

struct BugContest;

struct CAPSULE;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierFieldSystem;

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct Mail;

struct MailMessage;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct NARC;

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SEAL;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

void ov02_0224D044(PlayerAvatar *const playerAvatar, Field3dObject *object);

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

void ov02_0224D9C0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D9C0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Field3dCameraEffectData *effect = data;
    VecFx32 offset;
    VecFx32 target;
    VecFx32 pos;
    memset(effect, 0, sizeof(Field3dCameraEffectData));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(&effect->model, 0x13, 0x11, 2, &effect->allocator);
    ov02_0224D044(fieldSystem->playerAvatar, &effect->model.object);
    target = Camera_GetLookAtCamTarget(fieldSystem->camera);
    effect->cameraTarget = target;
    pos = Camera_GetLookAtCamPos(fieldSystem->camera);
    effect->cameraPos = pos;
    {
        int *p = (int *)&offset;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    switch (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar)) {
    case 0:
        offset.z -= 0x8000;
        break;
    case 1:
        offset.z += 0x8000;
        break;
    case 2:
        offset.x -= 0x8000;
        break;
    case 3:
        offset.x += 0x8000;
        break;
    }
    effect->cameraOffset = offset;
    PlaySE(2302);
    effect->state = 0;
}

void ov02_0224CFD8(MapObjectManager *mapObjectManager, u32 a1, Field3dObject *object);

void ov02_0224DAA4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DAA4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Field3dCameraEffectData *effect = data;
    VecFx32 offset;
    VecFx32 target;
    VecFx32 pos;
    memset(effect, 0, sizeof(Field3dCameraEffectData));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(&effect->model, 0x13, 0x11, 2, &effect->allocator);
    ov02_0224CFD8(fieldSystem->mapObjectManager, 0xfd, &effect->model.object);
    target = Camera_GetLookAtCamTarget(fieldSystem->camera);
    effect->cameraTarget = target;
    pos = Camera_GetLookAtCamPos(fieldSystem->camera);
    effect->cameraPos = pos;
    {
        int *p = (int *)&offset;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    switch (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar)) {
    case 0:
        offset.z -= 0x8000;
        break;
    case 1:
        offset.z += 0x8000;
        break;
    case 2:
        offset.x -= 0x8000;
        break;
    case 3:
        offset.x += 0x8000;
        break;
    }
    effect->cameraOffset = offset;
    PlaySE(2302);
    effect->state = 0;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the Field3dObjectTask templates of the Field3dEffectData / Field3dEffectData16 / Field3dCameraEffectData effects.
// The DS keeps them in asm/overlay_02_02248728_0224D9A4_02253974.s and
// asm/overlay_02_02248728_0224B6E4_0224BAE4_0224BE24_022536F0.s (same labels), with the data size as a literal (0xF0 / 0x114)
// that is too small once the model holds pointers: here it comes from the type.
void ov02_0224D1E4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D22C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D278(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D288(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D2BC(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D310(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D358(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D3A4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D3B4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D3E8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D648(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D658(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224D670(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DB8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DB9C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);
void ov02_0224DC58(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

const Field3dObjectTaskTemplate ov02_022538FC = { 0x400, sizeof(Field3dEffectData), ov02_0224D310, ov02_0224D3A4, ov02_0224D3B4, ov02_0224D3E8 };
const Field3dObjectTaskTemplate ov02_02253914 = { 0x400, sizeof(Field3dEffectData16), ov02_0224D5B4, ov02_0224D648, ov02_0224D658, ov02_0224D670 };
const Field3dObjectTaskTemplate ov02_0225392C = { 0x400, sizeof(Field3dCameraEffectData), ov02_0224DAA4, ov02_0224DB8C, ov02_0224DB9C, ov02_0224DC58 };
const Field3dObjectTaskTemplate ov02_02253974 = { 0x400, sizeof(Field3dEffectData), ov02_0224D1E4, ov02_0224D278, ov02_0224D288, ov02_0224D2BC };
const Field3dObjectTaskTemplate ov02_0225398C = { 0x400, sizeof(Field3dEffectData), ov02_0224D358, ov02_0224D3A4, ov02_0224D3B4, ov02_0224D3E8 };
const Field3dObjectTaskTemplate ov02_022539A4 = { 0x400, sizeof(Field3dCameraEffectData), ov02_0224D9C0, ov02_0224DB8C, ov02_0224DB9C, ov02_0224DC58 };
const Field3dObjectTaskTemplate ov02_022539BC = { 0x400, sizeof(Field3dEffectData), ov02_0224D22C, ov02_0224D278, ov02_0224D288, ov02_0224D2BC };
#endif
