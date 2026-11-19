#include "global.h"

#include "unk_0202B614.h"
#include "constants/heap.h"
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sys_task.h"
#include "mail_misc.h"
#include "overlay_87_scratch_off_card_app.h"


struct CoordinatesS16;
struct UnlockedEasyChatWords;
SysTask *sub_0202B730(SysTaskFunc callback, void *param, u32 priority);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct UnlockedEasyChatWords UnlockedEasyChatWords;
typedef struct BattleFactoryAppCursorSprite {
    u8 numSlots;
    u8 cursorMode;
    u8 currentSlot;
    u8 selectionActive;
    const CoordinatesS16 *positions;
    const u8 *animIDs;
    Sprite *sprite;
    u8 numBallSlots;
} BattleFactoryAppCursor;
u32 sub_0202BEFC(const UnkStruct_ov20_021D1F34 *param0);
u32 sub_0202BF00(const EasyChatArgs *args);
u8 sub_0202BF04(BattleFactoryAppCursor *cursor);

s8 sub_0202BEDC(void *arg0) {
    return *(s8 *)((u8 *)arg0 + 0x30);
}

u8 sub_0202BEE4(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x31);
}

u8 sub_0202BEEC(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x32);
}

SysTask *sub_0202BEF4(SysTaskFunc callback, void *param, int priority) {
    return sub_0202B730(callback, param, priority);
}

u32 sub_0202BEFC(const UnkStruct_ov20_021D1F34 *param0) {
    return param0->unk_00;
}

u32 sub_0202BF00(const EasyChatArgs *args) {
    return args->unk1;
}

u8 sub_0202BF04(BattleFactoryAppCursor *cursor) {
    return cursor->currentSlot;
}

s8 sub_0202BF08(void *arg0) {
    return *(s8 *)((u8 *)arg0 + 3);
}
