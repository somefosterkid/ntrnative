#include "global.h"

#include "sprite.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

struct EasyChatApp;

struct EasyChatArgs;

struct MailMessage;

struct EasyChatWordList;

struct Sprite;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

struct UnkStruct_ov20_021D3980_t;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

typedef struct UnkStruct_ov20_021D3980_t UnkStruct_ov20_021D3980;

struct UnkStruct_ov20_021D3980_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    Sprite *unk_0C;
};

void ov102_021EB530(UnkStruct_ov20_021D3980 *param0, int param1);

void ov102_021EB530(UnkStruct_ov20_021D3980 *param0, int param1) {
    VecFx32 v0;

    v0.z = 0;

    switch (param1) {
    case 1:
    default:
        v0.x = 220 * FX32_ONE;
        v0.y = 120 * FX32_ONE;
        break;
    case 2:
        v0.x = 220 * FX32_ONE;
        v0.y = 144 * FX32_ONE;
        break;
    }

    Sprite_SetMatrix(param0->unk_0C, &v0);
    Sprite_SetAnimCtrlSeq(param0->unk_0C, 2);
}
