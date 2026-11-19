#include "global.h"

#include "overlay_manager.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

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

void ov102_021E7A24(EasyChatApp *easyChatApp, OverlayManager *appMan);

BOOL EasyChat_Exit(OverlayManager *appMan, int *state);

BOOL EasyChat_Exit(OverlayManager *appMan, int *state) {
    ov102_021E7A24(OverlayManager_GetData(appMan), appMan);

    Heap_Destroy(HEAP_ID_34);
    Heap_Destroy(HEAP_ID_35);

    return 1;
}
