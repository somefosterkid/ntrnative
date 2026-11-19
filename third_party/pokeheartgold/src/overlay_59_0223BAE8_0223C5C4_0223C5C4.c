#include "global.h"

#include "camera.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_59_poffin_cutscene_mon_sprite.h"
#include "pokepic.h"

struct Camera;

struct CameraAngle;

struct PoffinCutsceneMonSprite;

struct PoffinCutscenePokemon;

struct Pokepic;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct SpriteAnimFrame;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct PoffinCutscenePokemon PoffinCutscenePokemon;

void PoffinCutsceneMonSprite_Init(PoffinCutsceneMonSprite *monSprite, PoffinCutscenePokemon *mon, enum HeapID heapID);

void SetupSprite(PoffinCutsceneMonSprite *monSprite, PoffinCutscenePokemon *mon, enum HeapID heapID);

void ov59_0223C5C4(PoffinCutsceneMonSprite *monSprite, enum HeapID heapID);

void ov59_0223C5C4(PoffinCutsceneMonSprite *monSprite, enum HeapID heapID) {
    VecFx32 pos = { 0, 0, 0x10000 };
    CameraAngle cameraAngle = { 0, 0, 0 };
    fx32 distance = 0x10000;
    u16 fovY = 0x5c1;

    monSprite->camera = Camera_New(heapID);

    Camera_Init_FromPosDistanceAndAngle(&pos, distance, &cameraAngle, fovY, 1, monSprite->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), monSprite->camera);
    Camera_ClearFixedTarget(monSprite->camera);
    Camera_SetStaticPtr(monSprite->camera);
}
