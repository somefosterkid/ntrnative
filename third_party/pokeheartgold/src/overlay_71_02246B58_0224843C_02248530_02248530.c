#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "overlay_71_trade_tube_phase.h"
#include "overlay_71_ball_path_state.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_sprite_anim_resources.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"

typedef struct BgScaleAnimation BgScaleAnimation;

typedef struct Trade3DScene Trade3DScene;

typedef struct AxisRotAnimator AxisRotAnimator;

typedef u16 charcode_t;

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
};

struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
};

struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
};

void ov71_02246D40(void *arg0);

s32 ov71_02249254(SysTask *arg0);

void ov71_02248D0C(TradeTubePhase *, SysTask **);

void ov71_0224926C(SysTask *);

s32 ov71_02248530(TradeTubePhase *ttPhase, s32 *subStepCounter);

s32 ov71_02248530(TradeTubePhase *ttPhase, s32 *subStepCounter) {
    s32 iVar1;
    s32 sVar2;
    BOOL BVar3;

    switch (*subStepCounter) {
    case 0:
        ov71_0224926C(ttPhase->task0);
        ttPhase->timer = 0;
        *subStepCounter = *subStepCounter + 1;
        break;
    case 1:
        iVar1 = ttPhase->timer + 1;
        ttPhase->timer = iVar1;
        if (2 < iVar1) {
            ttPhase->timer = 0;
            ov71_02246D40(ttPhase->shimmerEffect);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 2:
        if (ttPhase->shimmerActive == 0) {
            ov71_02248D0C(ttPhase, &ttPhase->task1);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 3:
        if ((ttPhase->timer < 0x28) && (iVar1 = ttPhase->timer + 1, ttPhase->timer = iVar1, iVar1 == 0x28)) {
            PlaySE(0x6af);
        }
        sVar2 = ov71_02249254(ttPhase->task0);
        if (sVar2 != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 4, 1, HEAP_ID_57);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 4:
        BVar3 = IsPaletteFadeFinished();
        if (BVar3 != 0) {
            return 1;
        }
    }
    return 0;
}
