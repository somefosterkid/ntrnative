#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

typedef struct HMCutInWithPlayer1 {
    HMCutIn *cutIn;
    Sprite *playerSprite;
} HMCutInWithPlayer1;

extern const OverworldAnimManagerFuncs ov02_022534A4;

OverworldAnimManager *ov02_0224B298(OverworldAnimManagerList *param0, Sprite *sprite);

typedef struct PlayerAnimEnv {
    int state;
    BOOL animationPartFinished;
    BOOL _;
    int positionDeltaIdx;
    VecFx32 position;
    HMCutIn *cutIn;
    Sprite *playerSprite;
} PlayerAnimEnv;
void ov02_0224B2C0(OverworldAnimManager *param0);

OverworldAnimManager *ov02_0224B298(OverworldAnimManagerList *param0, Sprite *sprite) {
    OverworldAnimManager *v0;
    HMCutInWithPlayer1 cutInWithPlayer;
    VecFx32 initialPos = { 0, 0, 0 };

    cutInWithPlayer.playerSprite = sprite;
    v0 = sub_02068B0C(param0, &ov02_022534A4, &initialPos, 0, &cutInWithPlayer, 129);

    return v0;
}

void ov02_0224B2C0(OverworldAnimManager *param0) {
    PlayerAnimEnv *v0 = sub_02068D74(param0);
    v0->state = 0;
}
