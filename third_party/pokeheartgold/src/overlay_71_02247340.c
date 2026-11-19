#include "global.h"
struct TradeSequenceData;

#include "constants/heap.h"

#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_71_trade_sequence_data.h"

Sprite *ov71_02247340(u8 *param_1, SpriteResourcesHeader *param_2, s32 param_3, s32 param_4, u32 param_5, NNS_G2D_VRAM_TYPE param_6);

struct BgConfig *ov71_02247384(TradeSequenceData *arg0);
void *ov71_02247388(struct TradeSequenceData *arg0);
s32 ov71_0224738C(struct TradeSequenceData *arg0);
s32 ov71_02247390(s32 **arg0);
s32 ov71_02247398(void **arg0);
s32 ov71_022473A0(void **arg0);
u16 ov71_022473A8(struct TradeSequenceData *arg0);
u16 ov71_022473B0(struct TradeSequenceData *arg0);
u16 ov71_022473BC(struct TradeSequenceData *arg0);
u16 ov71_022473C4(void *arg0);

Sprite *ov71_02247340(u8 *param_1, SpriteResourcesHeader *param_2, s32 param_3, s32 param_4, u32 param_5, NNS_G2D_VRAM_TYPE param_6) {
    Sprite *sprite;
    SimpleSpriteTemplate SStack_28;

    SStack_28.spriteList = *(SpriteList **)(param_1 + 0x18);
    SStack_28.position.x = param_3 << 0xc;
    SStack_28.position.y = param_4 << 0xc;
    SStack_28.position.z = 0;
    SStack_28.priority = param_5;
    SStack_28.whichScreen = param_6;
    SStack_28.heapID = HEAP_ID_56;
    SStack_28.header = param_2;
    sprite = Sprite_Create(&SStack_28);
    if (sprite != NULL) {
        Sprite_SetAnimActiveFlag(sprite, 1);
        Sprite_SetAnimSpeed(sprite, 0x1000);
    }
    return sprite;
}

struct BgConfig *ov71_02247384(TradeSequenceData *arg0) {
    return arg0->bgConfig;
}

void *ov71_02247388(TradeSequenceData *arg0) {
    return *((void **)&arg0->strTemplate);
}

s32 ov71_0224738C(TradeSequenceData *arg0) {
    return *((s32 *)&arg0->msgLoader);
}

s32 ov71_02247390(s32 **arg0) {
    return **arg0;
}

s32 ov71_02247398(void **arg0) {
    return *(s32 *)((u8 *)(*arg0) + 4);
}

s32 ov71_022473A0(void **arg0) {
    return *(s32 *)((u8 *)(*arg0) + 0xC);
}

u16 ov71_022473A8(TradeSequenceData *arg0) {
    return arg0->sendingSpecies;
}

u16 ov71_022473B0(TradeSequenceData *arg0) {
    return arg0->sendingForm;
}

u16 ov71_022473BC(TradeSequenceData *arg0) {
    return arg0->receivingSpecies;
}

u16 ov71_022473C4(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x152);
}
