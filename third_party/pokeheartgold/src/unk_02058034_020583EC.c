#include "global.h"
#include "field_communication_manager.h"
#include "constants/heap.h"

enum DSScreen {
    DS_SCREEN_MAIN = 0,
    DS_SCREEN_SUB,
    DS_SCREEN_MAX,
};
enum CommSync {
    SYNC_INIT_VS_LINK_BATTLE = 3,
    SYNC_START_VS_LINK_BATTLE,
    SYNC_5,

    SYNC_BATTLE_ROOM_RETURN = 30,

    SYNC_BATTLE_END_CONNECTION = 91,
    SYNC_PAUSE_BATTLE,

    SYNC_94 = 94,
    SYNC_SEND_TRAINER_CASE,

    SYNC_SWITCH_TRANSITION = 97,
    SYNC_CHANGE_TO_BATTLE_ROOM,
};
u16 sub_0203769C(void);
void sub_02056E60(void);
void sub_0205701C(BOOL param0);
BOOL sub_02034638(void);
enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};
enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};
void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);
void ResetVisibleHardwareWindows(enum DSScreen screen);
BOOL sub_02037B38(u8 syncState);
void sub_020582F4(FieldCommTask task, u16 time);
void sub_02058478(void);

void sub_020583EC(void);

void sub_020583EC(void) {
    if (sub_0203769C() == 0) {
        sub_02034638();
    }

    if (sub_02037B38(SYNC_PAUSE_BATTLE)) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_FIELD1);
        ResetVisibleHardwareWindows(DS_SCREEN_MAIN);
        ResetVisibleHardwareWindows(DS_SCREEN_SUB);
        sub_02056E60();
        sub_0205701C(0);
        sub_020582F4(sub_02058478, 1);
    }
}
