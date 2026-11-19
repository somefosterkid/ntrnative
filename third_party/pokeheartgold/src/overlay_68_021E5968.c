#include "global.h"

#include "overlay_manager.h"
#include "sprite_system.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E61A0(MoveRelearner *);

s32 ov68_021E5F50(MoveRelearner *);

s32 ov68_021E6058(MoveRelearner *);

s32 ov68_021E7568(MoveRelearner *);

u32 ov68_021E61B8(MoveRelearner *);

int ov68_021E614C(MoveRelearner *);

s32 ov68_021E6078(MoveRelearner *arg0);

s32 ov68_021E7AD8(MoveRelearner *);

s32 ov68_021E7B8C(MoveRelearner *);

u32 ov68_021E5F68(MoveRelearner *);

s32 ov68_021E74C0(MoveRelearner *);

void ov68_021E734C(MoveRelearnerMonPic *monPic);

u32 ov68_021E60D8(MoveRelearner *);

s32 ov68_021E61EC(MoveRelearner *);

s32 ov68_021E74D8(MoveRelearner *);

s32 ov68_021E7B6C(MoveRelearner *);

BOOL MoveRelearner_Main(OverlayManager *man, int *state);

BOOL MoveRelearner_Main(OverlayManager *man, int *state) {
    MoveRelearner *work;
    s32 next;

    work = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        next = ov68_021E5F50(work);
        *state = next;
        break;
    case 1:
        next = ov68_021E5F68(work);
        *state = next;
        break;
    case 2:
        next = ov68_021E6058(work);
        *state = next;
        break;
    case 3:
        next = ov68_021E6078(work);
        *state = next;
        break;
    case 4:
        next = ov68_021E60D8(work);
        *state = next;
        break;
    case 5:
        next = ov68_021E614C(work);
        *state = next;
        break;
    case 6:
        next = ov68_021E61A0(work);
        *state = next;
        break;
    case 7:
        next = ov68_021E61B8(work);
        *state = next;
        break;
    case 8:
        next = ov68_021E61EC(work);
        *state = next;
        break;
    case 9:
        return 1;
    case 10:
        next = ov68_021E74C0(work);
        *state = next;
        break;
    case 0xb:
        next = ov68_021E74D8(work);
        *state = next;
        return 0;
    case 0xc:
        next = ov68_021E7568(work);
        *state = next;
        return 0;
    case 0xd:
        next = ov68_021E7AD8(work);
        *state = next;
        break;
    case 0xe:
        next = ov68_021E7B6C(work);
        *state = next;
        break;
    case 0xf:
        next = ov68_021E7B8C(work);
        *state = next;
    }
    ov68_021E734C(&work->monPic);
    SpriteSystem_DrawSprites(work->spriteManager);
    return 0;
}
