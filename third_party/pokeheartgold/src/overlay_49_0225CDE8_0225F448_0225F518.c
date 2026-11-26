#include "global.h"

#include "constants/sndseq.h"

#include "unk_02005D10.h"
#include "pm_string.h"
#include "sys_task.h"
#include "screen_fade_internal.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct String;

struct StringList;

struct SysTask;

struct SysTaskQueue;

struct System;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov45_0222A4D0(const WiFiPlazaSystem *param0);

void ov45_0222A72C(WiFiPlazaSystem *param0, u32 param1);

void ov45_0222A770(WiFiPlazaSystem *param0, int type1, int type2);

BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

u32 ov66_0222F0DC(const WiFiPlazaSystem *param0);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov70_0225CCAC(UnkStruct_ov70_0225CA20 *param0);

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02258E7C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

typedef struct Window Window;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const StringList *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

WiFiPlazaSystem *ov70_0225DEE8(UnkStruct_ov70_0225DEE8 *param0, UnkStruct_ov70_0225DEE8 *);

UnkStruct_ov70_0225CA20 *ov49_02259FF0(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_02260AD4 *ov49_0225A008(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_022630A4 *ov49_0225A010(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A018(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

u32 ov70_0225DF2C(const UnkStruct_ov70_0225DEE8 *param0);

BOOL ov70_0225DF30(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

void ov70_0225DF9C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

BOOL ov49_0225A0AC(const UnkStruct_ov70_0225DEE8 *param0);

void ov70_0225DFEC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A174(UnkStruct_ov70_0225DEE8 *param0, const ListMenuTemplate *param1, s32 param2, s32 param3);

u32 ov70_0225E0D4(UnkStruct_ov70_0225DEE8 *param0);

void ov70_0225E0E4(UnkStruct_ov70_0225DEE8 *param0, u16 *param1, u16 *param2);

void ov70_0225E0F4(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

void ov49_0225A264(UnkStruct_ov70_0225DEE8 *param0);

int ov49_0225A2C4(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A2F8(UnkStruct_ov70_0225DEE8 *param0);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov70_0225E29C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov49_0225A40C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, int param2);

void ov70_0225E328(UnkStruct_ov70_0225DEE8 *param0, u32 param1, BOOL param2);

void ov49_0225A490(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A530(UnkStruct_ov70_0225DEE8 *param0);

void ov70_02260B38(UnkStruct_ov70_02260AD4 *param0, const UnkStruct_ov70_0225CC54 *param1);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF68(UnkStruct_ov70_02263344 *param0);


u32 ov70_02262E84(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF8C(UnkStruct_ov70_02263344 *param0, u32 param1);

void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);

void ov70_02262E94(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;

extern System gSystem;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    s32 unk_04;
    ListMenuTemplate unk_08;
    u16 unk_28;
    u16 unk_2A;
} UnkStruct_ov70_022653DC;

UnkStruct_ov70_0225CC54 *ov70_022631F8(UnkStruct_ov70_0225DEE8 *param0, u32 param1);

BOOL ov49_0225FDCC(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_0226412C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

void ov49_022614CC(UnkStruct_ov70_022653DC *param0, UnkStruct_ov70_0225DEE8 *param1);

void ov70_02265450(UnkStruct_ov70_022653DC *param0, UnkStruct_ov70_0225DEE8 *param1);

extern const UnkStruct_ov70_0226DA18 ov49_02269B38;

BOOL ov49_0225F518(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

WiFiPlazaSystem *ov49_02259FE8(UnkStruct_ov70_0225DEE8 *param0, UnkStruct_ov70_0225DEE8 *);

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

u32 ov45_0222B1B4(const WiFiPlazaSystem *param0);

UnkStruct_ov70_0225CC54 *ov49_0225F2FC(UnkStruct_ov70_0225DEE8 *param0, u32 param1);

BOOL ov49_0225A030(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225CC40(UnkStruct_ov70_02260AD4 *param0, const UnkStruct_ov70_0225CC54 *param1);

void ov49_0225A09C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

void ov49_0225A1F4(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

u32 ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, u16 *param1, u16 *param2);

void ov49_0225A39C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *param0, u32 param1, BOOL param2);

void ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *param0);

void ov49_02261540(UnkStruct_ov70_022653DC *param0, UnkStruct_ov70_0225DEE8 *param1);

u8 ov49_0225A02C(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225EF98(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

BOOL ov49_0225F518(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_022653DC *v0;
    UnkStruct_ov70_0225CC54 *v1;
    UnkStruct_ov70_0225CA20 *v2;
    WiFiPlazaSystem *v3;
    String *v4;

    v3 = ov49_02259FE8(param1, param1);
    v2 = ov49_02259FF0(param1);
    v1 = ov49_02258DAC(v2);
    v0 = ov49_0225EF84(param0);

    switch (ov49_0225EF88(param0)) {
    case 0:
        v0 = ov49_0225EF40(param0, sizeof(UnkStruct_ov70_022653DC));

        ov49_022614CC(v0, param1);
        ov45_0222A4D0(v3);

        {
            v0->unk_02 = ov45_0222B1B4(v3);
            ov45_0222A72C(v3, v0->unk_02);
        }

        v1 = ov49_0225F2FC(param1, param2);

        ov49_02258EEC(v2, v1, 4);
        ov49_0225EF90(param0);
        break;
    case 1:
        if ((ov49_0225A030(param1) == 0) && (ov49_02258F38(v1) == 1)) {
            ov49_02258EEC(v2, v1, 0);

            {
                UnkStruct_ov70_02260AD4 *v5;

                v5 = ov49_0225A008(param1);
                ov49_0225CC40(v5, v1);
            }

            v0->unk_04 = 16;
            ov49_0225EF90(param0);
        }
        break;
    case 2:
        v0->unk_04--;

        if (v0->unk_04 == 0) {
            ov49_0225A018(param1, 1);
            ov49_0225EF90(param0);
        }
        break;
    case 3:
        ov49_02258E7C(v2, v1, 2, 0);
        v0->unk_00 = 4;
        ov49_0225EF8C(param0, 7);
        break;
    case 4:
        ov49_02258E7C(v2, v1, 2, 0);
        v0->unk_00 = 5;
        ov49_0225EF8C(param0, 7);
        break;
    case 5:
        ov49_02258E7C(v2, v1, 1, 3);
        v0->unk_00 = 6;
        ov49_0225EF8C(param0, 7);
        break;
    case 6:
        ov49_02258E7C(v2, v1, 2, 3);
        v0->unk_00 = 8;
        ov49_0225EF8C(param0, 7);
        break;
    case 7:
        if (ov49_02258E60(v1, 5) == 0) {
            ov49_0225EF8C(param0, v0->unk_00);
        }
        break;
    case 8:
        v4 = ov49_0225A30C(param1, 1, 0);
        ov49_0225A08C(param1, v4);
        v0->unk_00 = 16;
        ov49_0225EF8C(param0, 21);
        break;
    case 9:
        v4 = ov49_0225A30C(param1, 1, 5);
        ov49_0225A09C(param1, v4);
        ov49_0225A174(param1, &v0->unk_08, 0, 0);
        ov49_0225A1F4(param1, 1);
        ov49_0225EF8C(param0, 10);
        break;
    case 10: {
        u32 v6;
        u32 v7;
        BOOL v8 = 0;

        v6 = ov49_0225A1D4(param1);

        switch (v6) {
        case 0xffffffff:
            break;
        case 0xfffffffe:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            v0->unk_28 = 0;
            v8 = 1;
            break;
        default:
            v0->unk_28 = v6;
            v8 = 1;
            ov49_0225A40C(param1, 0, v6);
            v7 = 8;
            break;
        }

        if (v8 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);

            if (v0->unk_28 != 0) {
                v4 = ov49_0225A30C(param1, 1, v7);
                ov49_0225A09C(param1, v4);
                ov49_0225A264(param1);
                ov49_0225EF8C(param0, 11);
            } else {
                ov49_0225EF8C(param0, 17);
            }
        }
    } break;
    case 11: {
        int v9;

        v9 = ov49_0225A2C4(param1);

        switch (v9) {
        case 0:
            ov49_0225A2F8(param1);
            ov49_0225EF8C(param0, 12);
            ov45_0222A770(v3, v0->unk_28, v0->unk_2A);
            ov49_0225A490(param1);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case 1:
            ov49_0225A2F8(param1);
            ov49_0225EF8C(param0, 9);
            break;
        default:
            break;
        }
    } break;
    case 12:
        v4 = ov49_0225A30C(param1, 1, 10);
        ov49_0225A09C(param1, v4);
        ov49_0225A174(param1, &v0->unk_08, 0, 0);
        ov49_0225A1F4(param1, 1);
        ov49_0225EF8C(param0, 13);
        break;
    case 13: {
        u32 v10;
        u32 v11;
        BOOL v12 = 0;

        v10 = ov49_0225A1D4(param1);

        switch (v10) {
        case 0xffffffff:
            break;
        case 0xfffffffe:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            v0->unk_2A = 0;
            v12 = 1;
            break;
        default:
            v0->unk_2A = v10;
            v12 = 1;
            ov49_0225A40C(param1, 0, v10);
            v11 = 8;
            break;
        }

        if (v12 == 1) {
            ov49_0225A1E4(param1, 0L, 0L);

            if (v0->unk_2A != 0) {
                v4 = ov49_0225A30C(param1, 1, v11);
                ov49_0225A09C(param1, v4);
                ov49_0225A264(param1);
                ov49_0225EF8C(param0, 15);
            } else {
                ov49_0225EF8C(param0, 19);
            }
        }
    } break;
    case 15: {
        int v13;

        v13 = ov49_0225A2C4(param1);

        switch (v13) {
        case 0:
            ov49_0225A2F8(param1);
            ov49_0225EF8C(param0, 20);
            ov45_0222A770(v3, v0->unk_28, v0->unk_2A);
            ov49_0225A490(param1);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case 1:
            ov49_0225A2F8(param1);
            ov49_0225EF8C(param0, 12);
            break;
        default:
            break;
        }
    } break;
    case 16:
        PlaySE(SEQ_SE_PL_EFF03_4);
        ov49_0225A530(param1);
        ov49_0225A334(param1, param2, 0);
        ov49_0225A39C(param1, v0->unk_02, 1);
        {
            BOOL v14;

            v14 = ov45_0222AB28(v3, param2);

            if (v14 == 0) {
                v4 = ov49_0225A30C(param1, 1, 1);
            } else {
                v4 = ov49_0225A30C(param1, 1, 124);
            }
        }
        ov49_0225A08C(param1, v4);
        v0->unk_00 = 18;
        ov49_0225EF8C(param0, 21);
        break;
    case 17:
        v4 = ov49_0225A30C(param1, 1, 2);
        ov49_0225A08C(param1, v4);
        v0->unk_00 = 22;
        ov49_0225EF8C(param0, 21);
        break;
    case 18:
        if (!((IsSEPlaying(SEQ_SE_PL_EFF03_4) == 0) && (gSystem.newKeys & (0x0001 | 0x0002)))) {
            break;
        }

        PlaySE(SEQ_SE_DP_SELECT);

        v4 = ov49_0225A30C(param1, 1, 4);
        ov49_0225A08C(param1, v4);

        v0->unk_00 = 9;
        ov49_0225EF8C(param0, 21);

        ov45_0222A770(v3, v0->unk_28, v0->unk_2A);
        ov49_0225A428(param1, param2, 0);
        break;
    case 19:
        ov49_0225A40C(param1, 0, v0->unk_28);
        v4 = ov49_0225A30C(param1, 1, 12);

        ov49_0225A08C(param1, v4);
        v0->unk_00 = 17;

        ov49_0225EF8C(param0, 21);
        break;
    case 20:
        ov49_0225A40C(param1, 0, v0->unk_28);
        ov49_0225A40C(param1, 1, v0->unk_2A);

        v4 = ov49_0225A30C(param1, 1, 11);
        ov49_0225A08C(param1, v4);

        v0->unk_00 = 17;
        ov49_0225EF8C(param0, 21);
        break;

    case 21:
        if (ov49_0225A0AC(param1) == 1) {
            ov49_0225EF8C(param0, v0->unk_00);
        }
        break;
    case 22:
        ov49_02258EEC(v2, v1, 1);
        ov49_0225A0EC(param1);
        ov49_02261540(v0, param1);
        ov49_0225EF68(param0);

        {
            UnkStruct_ov70_022630A4 *v15;
            u32 v16;

            v15 = ov49_0225A010(param1);
            v16 = ov49_0225A02C(param1);

            ov49_0225EF98(v15, v16, &ov49_02269B38, 0L);
        }
        break;
    }

    return 0;
}
