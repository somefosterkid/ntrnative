#include "global.h"

#include "constants/gx.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "pokemon_summary_app.h"
#include "touchscreen.h"

struct Camera;

struct CameraAngle;

struct Rodata0208DE40;

struct StatPosTemplate;

typedef struct StatPosTemplate {
    u16 full[3];
    u16 base[3];
    u16 delta[3];
} StatPosTemplate;

typedef struct Rodata0208DE40 {
    VecFx32 cameraPos;
    StatPosTemplate statPosTemplates[16];
    u8 touchResponseTable[11][4];
    TouchscreenHitbox touchHitboxes[11];
} Rodata0208DE40;

extern const Rodata0208DE40 _02104EA4;

void sub_0208DF2C(PokemonSummary *work);

void sub_0208DF2C(PokemonSummary *work) {
    VecFx32 pos = _02104EA4.cameraPos;
    CameraAngle angle = { 0, 0, 0, 0 };

    work->camera = Camera_New(HEAP_ID_19);
    Camera_Init_FromPosDistanceAndAngle(&pos, 0x10000, &angle, 0x5C1, 1, work->camera);
    Camera_SetPerspectiveClippingPlane(0, 0x64000, work->camera);
    Camera_ClearFixedTarget(work->camera);
    Camera_SetStaticPtr(work->camera);
}
