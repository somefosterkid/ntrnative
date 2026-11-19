#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

void sub_02023694(BillboardList *list);
void sub_020236BC(Billboard *billboard);

void sub_02023694(BillboardList *list) {
    list->active = 0;
    list->draw = 0;
    list->billboards = 0L;
    list->capacity = 0;
    list->freeBillboards = 0L;
    list->freeBillboardHead = 0;
    list->allocator = 0L;
    list->vramTransfer = 0L;
    list->redraw = BILLBOARD_REDRAW_NONE;
}

void sub_020236BC(Billboard *billboard) {
    billboard->list = 0L;
    billboard->anims = 0L;
    billboard->modelSet = 0L;
    billboard->model = 0L;
    billboard->texture = 0L;
    billboard->animTexture = 0L;

    memset(&billboard->gfxSequence, 0, sizeof(BillboardGfxSequence));

    billboard->texKey = (u32)0x0;
    billboard->tex4x4Key = (u32)0x0;
    billboard->plttKey = (u32)0x0;
    billboard->vramTransfer = 0L;

    VEC_Set(&billboard->pos, 0, 0, 0);
    VEC_Set(&billboard->scale, ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L));

    billboard->rotMatrix = 0L;
    billboard->animNum = 0;
    billboard->state = BILLBOARD_STATE_INACTIVE;
    billboard->next = 0L;
    billboard->prev = 0L;
    billboard->draw = 0;
    billboard->callback = 0L;
}
