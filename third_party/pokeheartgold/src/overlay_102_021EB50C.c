#include "global.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"
#include "overlay_87_scratch_off_card_app.h"

struct CoordinatesS16;
struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct UnkStruct_ov20_021D3980_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;
typedef struct UnkStruct_ov20_021D3980_t UnkStruct_ov20_021D3980;
struct UnkStruct_ov20_021D3980_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    Sprite *unk_0C;
};
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

void ov102_021EB50C(UnkStruct_ov20_021D3980 *param0);
void ov102_021EB518(UnkStruct_ov20_021D3980 *param0);
void ov102_021EB524(BattleFactoryAppCursor *cursor, BOOL draw);

void ov102_021EB50C(UnkStruct_ov20_021D3980 *param0) {
    Sprite_SetAnimCtrlSeq(param0->unk_0C, 3);
}

void ov102_021EB518(UnkStruct_ov20_021D3980 *param0) {
    Sprite_SetAnimCtrlSeq(param0->unk_0C, 2);
}

void ov102_021EB524(BattleFactoryAppCursor *cursor, BOOL draw) {
    Sprite_SetDrawFlag(cursor->sprite, draw);
}
