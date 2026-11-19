#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02005D10.h"

typedef struct {
    u8 filler0[0x708];
    Sprite *sprite708;
    u8 timer;
    u8 state;
    u16 posY;
    Sprite *sprite710;
    Sprite *sprite714;
    u32 planes;
    u8 filler71C[0xD6C - 0x71C];
    void (*callback)(void *work);
} Ov96Work_021E667C;

BOOL ov96_021E839C(Ov96Work_021E667C *work);

#define SEQ_SE_GS_UNK_899 0x899

#define SEQ_SE_GS_UNK_898 0x898

BOOL ov96_021E667C(Ov96Work_021E667C *work);

BOOL ov96_021E667C(Ov96Work_021E667C *work) {
    VecFx32 pos;
    switch (work->state) {
    case 0:
        Sprite_SetDrawFlag(work->sprite714, FALSE);
        PlaySE(SEQ_SE_GS_UNK_899);
        pos.z = 0;
        pos.x = FX32_CONST(128);
        pos.y = (work->posY + 0x48) * FX32_ONE;
        Sprite_SetMatrix(work->sprite708, &pos);
        Sprite_SetAnimCtrlSeq(work->sprite708, 3);
        Sprite_SetDrawFlag(work->sprite708, TRUE);
        work->state = 1;
        break;
    case 1:
        work->timer++;
        if (work->timer > 60) {
            work->timer = 0;
            work->state = 2;
        }
        break;
    case 2:
        if (work->callback != NULL) {
            work->callback(work);
        }
        if (ov96_021E839C(work)) {
            PlaySE(SEQ_SE_GS_UNK_898);
            Sprite_SetAnimCtrlSeq(work->sprite710, 0);
            Sprite_SetDrawFlag(work->sprite710, TRUE);
        }
        GfGfx_EngineATogglePlanes(work->planes, GF_PLANE_TOGGLE_ON);
        work->callback = NULL;
        work->state = 3;
        break;
    case 3:
        work->timer++;
        if (work->timer > 120) {
            work->timer = 0;
            work->state = 4;
            return TRUE;
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}
