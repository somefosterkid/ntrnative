#include "global.h"
struct FrontierBrainEncounterEffect;

#include "filesystem.h"
#include "heap.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "bg_window.h"
#include "overlay_80_0223A00C.h"
#include "palette.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

void ov80_0223A0C0(struct FrontierBrainEncounterEffect *arg0, SysTask *arg1);

struct FrontierBrainEncounterEffectAnimations;
struct UnkStruct_02012744_t;
typedef struct FrontierBrainBannerAnimation {
    s32 top;
    s32 bottom;
    u8 isFinished;
    u8 state;
    u8 left0;
    u8 top0;
    u8 right0;
    u8 bottom0;
    u8 left1;
    u8 top1;
    u8 right1;
    u8 bottom1;
} FrontierBrainBannerAnimation;
typedef struct FrontierBrainMugshotSlideInManager {
    int xOffset;
    int state;
} FrontierBrainMugshotSlideInManager;
typedef struct FrontierBrainEncounterEffectAnimations FrontierBrainEncounterEffectAnimations;
typedef struct FrontierBrainEncounterEffect {
    int state;
    int unused[2];
    FrontierBrainEncounterEffectAnimations *anims;
    BgConfig *bgConfig;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *currentBannerPalette;
    u16 *effectIsFinished;
    NARC *narc;
    s16 xAnchor;
    s16 yAnchor;
    u8 facility;
    UnkStruct_02012744 *unk_30;
    SysTask *vBlankCallback;
    int bannerPaletteTimer;
    int bannerPaletteIdx;
    u16 allPalettes[8 * (16)];
    SysTask *bannerPaletteTask;
    FrontierBrainBannerAnimation bannerAnim;
    FrontierBrainMugshotSlideInManager mugshotSlideIn;
    int mugshotPlttOffset;
    u32 plttOffsets;
} FrontierBrainEncounterEffect;

void ov80_0223A0C0(FrontierBrainEncounterEffect *arg0, SysTask *task) {
    SysTask_Destroy(arg0->bannerPaletteTask);
    SysTask_Destroy(arg0->vBlankCallback);
    NARC_Delete(arg0->narc);
    Heap_FreeExplicit(HEAP_ID_101, *((void * *)&arg0->anims));
    DestroySysTaskAndEnvironment(task);
}

void ov80_0223A0EC(SysTask *task, void *data) {
    FrontierBrainEncounterEffect *encEffect = data;

    G2_SetWnd0Position(encEffect->bannerAnim.left0, encEffect->bannerAnim.top0, encEffect->bannerAnim.right0, encEffect->bannerAnim.bottom0);
    G2_SetWnd1Position(encEffect->bannerAnim.left1, encEffect->bannerAnim.top1, encEffect->bannerAnim.right1, encEffect->bannerAnim.bottom1);
}
