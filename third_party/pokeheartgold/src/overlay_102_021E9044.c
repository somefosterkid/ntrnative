#include "global.h"

#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_app.h"

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
u32 ov102_021E87A8(const UnkStruct_ov20_021D18BC *param0);

u32 ov102_021E9044(const EasyChatApp *easyChatApp);

u32 ov102_021E9044(const EasyChatApp *easyChatApp) {
    return ov102_021E87A8(&(easyChatApp->unk_54));
}
