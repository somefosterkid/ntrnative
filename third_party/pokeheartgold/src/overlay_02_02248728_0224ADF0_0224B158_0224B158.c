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
#include "math_util.h"
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

struct Sprite;

int ov02_0224B158(FlyTaskEnv *work);

int ov02_0224B158(FlyTaskEnv *work) {
    Sprite *spriteA = work->birdSprite;
    Sprite *spriteB;
    VecFx32 *scalePtr;
    VecFx32 mtx;
    VecFx32 mtx2;
    VecFx32 scale;
    work->pathParam = work->pathParam + work->pathDelta;
    work->pathDelta += 0x1000;
    if (work->pathDelta > 0x10000) {
        work->pathDelta = 0x10000;
    }
    work->pathOffset.x = GF_CosDeg((u16)(work->angleParam / 0x1000)) * (work->pathParam / 0x1000);
    work->pathOffset.y = GF_SinDeg(0x80) * (work->pathParam / 0x1000);
    if (work->angleParam < 0x87000) {
        work->angleParam += 0x1000;
    }
    work->birdScale.x += work->scaleDelta;
    if (work->birdScale.x > 0x2000) {
        work->birdScale.x = 0x2000;
    }
    work->birdScale.y += work->scaleDelta;
    if (work->birdScale.y > 0x2000) {
        work->birdScale.y = 0x2000;
    }
    mtx.x = work->birdStartPos.x + work->pathOffset.x;
    mtx.y = work->birdStartPos.y + work->pathOffset.y;
    if (mtx.y / 0x1000 >= 0xe6) {
        work->subState = 2;
        (work->state)++;
    }
    spriteB = work->cutIn->playerSprite;
    scalePtr = Sprite_GetScalePtr(spriteB);
    mtx2 = mtx;
    scale = *scalePtr;
    work->playerOffsetY -= 0x1000;
    mtx2.y += work->playerOffsetY;
    Sprite_SetMatrix(spriteB, &mtx2);
    scale.x += 0x100;
    if (scale.x > 0x2000) {
        scale.x = 0x2000;
    }
    scale.y += 0x100;
    if (scale.y > 0x2000) {
        scale.y = 0x2000;
    }
    Sprite_SetAffineScale(spriteB, &scale);
    Sprite_SetAffineScale(spriteA, &work->birdScale);
    Sprite_SetMatrix(spriteA, &mtx);
    return 0;
}
