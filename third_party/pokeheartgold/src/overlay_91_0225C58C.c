#include "global.h"

#include "constants/heap.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "sys_task.h"
#include "overlay_manager.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "screen_fade.h"
#include "screen_fade_internal.h"
#include "save_wifi_history.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c
typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    struct PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct SysTask;

struct SysTaskQueue;

struct System;

struct PlayerProfile;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov115_0226095C_t;

struct UnkStruct_ov115_02260BE4_t;

struct UnkStruct_ov115_02260D78_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;

typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);


enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

void NintendoWFC_StartVoiceChat(enum HeapID heapID);

void ov00_021E6A4C(void);

typedef u16 charcode_t;



typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

u32 ov90_0225886C(const UnkStruct_ov114_0225C76C *param0, u32 param1);

UnkStruct_ov114_0225D678 *ov90_02258914(const UnkStruct_ov114_0225C76C *param0, u32 heapID);

void ov114_0225C838(UnkStruct_ov114_0225D678 *param0);

BOOL ov90_022589BC(const UnkStruct_ov114_0225D678 *param0);

int ov90_022589CC(const UnkStruct_ov114_0225D678 *param0);

UnkStruct_ov114_0225E854 *ov90_022589E0(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225C9A8 *param1, u32 heapID);

void ov114_0225C904(UnkStruct_ov114_0225E854 *param0);


BOOL ov08_02224B8C(const UnkStruct_ov114_0225E854 *param0);

void ov90_02258AA8(UnkStruct_ov114_0225C9A8 *param0, u32 param1);

BOOL ov90_02258B54(UnkStruct_ov114_0225C76C *param0);

BOOL ov90_02258B98(const UnkStruct_ov114_0225C76C *param0);

typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

UnkStruct_ov115_02260BE4 *ov91_0225CCC4(enum HeapID heapID, u32 param1, u32 param2, const UnkStruct_ov115_02262F50 *param3);

void ov115_02260BE4(UnkStruct_ov115_02260BE4 *param0);

BOOL ov115_02260BEC(UnkStruct_ov115_02260BE4 *param0);

BOOL ov115_02260C54(const UnkStruct_ov115_02260BE4 *param0);

void ov91_0225CD64(UnkStruct_ov115_02260BE4 *param0);

u32 ov115_02260C64(const UnkStruct_ov115_02260BE4 *param0);

BOOL ov115_02260CA4(const UnkStruct_ov115_02260BE4 *param0);

void ov91_0225CDC4(UnkStruct_ov115_02260BE4 *param0, UnkStruct_ov115_02265AD4 *param1);

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov115_022612BC;

UnkStruct_ov115_02260D78 *ov91_0225CDF4(u32 heapID, u32 param1, u32 param2, u32 param3, UnkStruct_ov115_02262F50 *param4);

void ov115_02260D78(UnkStruct_ov115_02260D78 *param0);

BOOL ov115_02260DAC(UnkStruct_ov115_02260D78 *param0, u32 param1);

BOOL ov115_02260F70(UnkStruct_ov115_02260D78 *param0, u32 param1);

BOOL ov115_022610D4(UnkStruct_ov115_02260D78 *param0);

u32 ov115_02261274(const UnkStruct_ov115_02260D78 *param0);

void ov115_022612BC(UnkStruct_ov115_02260D78 *param0, const UnkStruct_ov115_022612BC *param1);

void sub_020971F8(UnkStruct_ov115_0226095C *param0);

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov115_02260440;

int sub_020347A0(void);

BOOL sub_02037030(int cmd, const void *data, int size);

u16 sub_0203769C(void);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void sub_0203A880(void);

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

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

void FinishScreenFade(void);

void SetColorBrightness(u16 color);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

void GF_DestroyVramTransferManager(void);

struct UnkStruct_ov115_0226095C_t {
    UnkStruct_ov114_0225D678 *unk_00;
    UnkStruct_ov114_0225E854 *unk_04;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225C9A8 unk_18;
    UnkStruct_ov115_02260BE4 *unk_30;
    UnkStruct_ov115_02260D78 *unk_34;
    BOOL unk_38;
    BOOL unk_3C;
    BOOL unk_40;
    u16 unk_44;
    u16 unk_46;
    UnkStruct_ov115_02262F50 unk_48;
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u32 unk_7C;
    BOOL unk_80;
    u8 unk_84[4];
};

void ov91_0225CB58(void *param0);

void ov115_02260A5C(UnkStruct_ov115_0226095C *param0);

void ov115_02260A90(UnkStruct_ov115_0226095C *param0);

void ov115_02260B44(UnkStruct_ov115_0226095C *param0, UnkStruct_ov115_02260440 *param1);

BOOL ov115_02260BA0(UnkStruct_ov115_0226095C *param0, int param1, const void *param2, int param3);

int ov91_0225C58C(OverlayManager *appMan, int *param1);

void ov90_02258938(UnkStruct_ov114_0225D678 *param0);

void sub_0200FB70(void);

void sub_0200FC20(u16 arg0);

void ov91_0225CC4C(UnkStruct_ov115_0226095C *param0, UnkStruct_ov115_02260440 *param1);

void ov91_02261B2C(void *arg0);

void ov00_021E69A8(s32 arg0);

void ov91_0225D3C4(UnkStruct_ov115_02260D78 *param0, const UnkStruct_ov115_022612BC *param1);

BOOL ov91_0225CEB4(UnkStruct_ov115_02260D78 *param0, u32 param1);

u32 ov91_0225D37C(const UnkStruct_ov115_02260D78 *param0);

BOOL ov91_0225CCF4(UnkStruct_ov115_02260BE4 *param0);

u8 ov91_0225CD5C(void *arg0);

u8 ov91_0225CD6C(void *arg0);

BOOL ov91_0225CCA8(UnkStruct_ov115_0226095C *param0, int param1, const void *param2, int param3);

BOOL ov91_0225D1DC(UnkStruct_ov115_02260D78 *param0);

void ov91_0225CB64(UnkStruct_ov115_0226095C *param0);

void ov91_0225CB98(UnkStruct_ov115_0226095C *param0);

BOOL ov91_0225D078(UnkStruct_ov115_02260D78 *param0, u32 param1);

s32 ov91_0225CDAC(void *arg0);

void ov91_0225CCEC(UnkStruct_ov115_02260BE4 *param0);

void ov91_0225CE80(UnkStruct_ov115_02260D78 *param0);

BOOL ov90_02258AA0(const UnkStruct_ov114_0225E854 *param0);

BOOL ov90_02258AA4(const UnkStruct_ov114_0225E854 *param0);

void ov90_02258A04(UnkStruct_ov114_0225E854 *param0);

int ov91_0225C58C(OverlayManager *appMan, int *param1) {
    UnkStruct_ov115_0226095C *v0 = OverlayManager_GetData(appMan);
    UnkStruct_ov115_02260440 *v1 = OverlayManager_GetArgs(appMan);
    BOOL v2;

    if (ov90_02258B54(&v0->unk_08) == 1) {
        switch (v0->unk_78) {
        case 0:

            if (IsPaletteFadeFinished() == 1) {
                sub_0200FB70();
            }

            sub_0200FC20(((0) << 10) | ((0) << 5) | (0));
            v0->unk_78++;
            break;
        case 1:
            if (ov90_02258B98(&v0->unk_08) == 1) {
                v0->unk_78++;
            }
            break;
        case 2:
            ov91_0225CC4C(v0, v1);
            return 1;
        default:
            GF_AssertFail();
            return 1;
        }

        return 0;
    }

    switch (*param1) {
    case 0:

        v0->unk_00 = ov90_02258914(&v0->unk_08, HEAP_ID_106);

        (*param1)++;
        break;

    case 1:
        if (ov90_022589BC(v0->unk_00) == 1) {
            v0->unk_70 = ov90_022589CC(v0->unk_00);
            ov90_02258938(v0->unk_00);
            v0->unk_00 = 0L;
            (*param1)++;
        }
        break;
    case 2:
        if (gSystem.heldKeys & 0x0001) {
            break;
        }

        Main_SetVBlankIntrCB(ov91_0225CB58, v0);
        HBlankInterruptDisable();

        v0->unk_38 = 0;
        v0->unk_3C = 0;
        v0->unk_40 = 0;
        memset(v0->unk_84, 0, sizeof(u8) * 4);

        GF_CreateVramTransferManager(32, HEAP_ID_106);
        ov91_02261B2C(v0);

        v0->unk_80 = 1;
        v0->unk_74 = sub_020347A0();

        ((v0->unk_74 > 1) ? (void)0 : GF_AssertFail());

        v0->unk_44 = sub_0203769C();
        v0->unk_46 = ov90_0225886C(&v0->unk_08, v0->unk_44);

        if (v0->unk_44 == 0) {
            v0->unk_30 = ov91_0225CCC4(HEAP_ID_106, (30 * 40), v0->unk_74, &v0->unk_48);
            v0->unk_7C = 1;
        }

        v0->unk_34 = ov91_0225CDF4(HEAP_ID_106, (30 * 40), v0->unk_74, v0->unk_46, &v0->unk_48);
        sub_0203A880();

        if (v1->unk_38) {
            ov00_021E69A8(HEAP_ID_106);
        }

        {
            UnkStruct_ov115_022612BC v4;

            switch (v0->unk_70) {
            case 0:
                v4.unk_00 = 0;
                v4.unk_01 = 0;
                break;
            case 1:
                v4.unk_00 = 0;
                v4.unk_01 = 1;
                break;
            case 2:
                v4.unk_00 = 1;
                v4.unk_01 = 0;
                break;
            default:
                ((0) ? (void)0 : GF_AssertFail());
                break;
            }

            ov91_0225D3C4(v0->unk_34, &v4);
        }

        sub_02037AC0(1);

        (*param1)++;
        break;
    case 3:
        if (!sub_02037B38(1)) {
            break;
        }

        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)(27), (enum FadeType)(27), 0xffff, 6, 1, HEAP_ID_106);
        (*param1)++;
        break;
    case 4:
        ov91_0225CEB4(v0->unk_34, 0);

        if (IsPaletteFadeFinished()) {
            if (v0->unk_44 == 0) {
                v2 = sub_02037030(22, 0L, 0);

                if (v2) {
                    (*param1)++;
                }
            } else {
                (*param1)++;
            }
        }
        break;
    case 5:
        ov91_0225CEB4(v0->unk_34, 0);

        if (v0->unk_38) {
            (*param1)++;
        }
        break;
    case 6:
        v2 = ov91_0225CEB4(v0->unk_34, 1);

        if (v2 == 0) {
            (*param1)++;
        }
        break;
    case 7:
        if (v0->unk_3C) {
            u32 v5;

            v5 = ov91_0225D37C(v0->unk_34);
            v2 = sub_02037030(25, &v5, sizeof(u32));

            if (v2) {
                (*param1)++;
            }
            break;
        }

        if (v0->unk_44 == 0) {
            v2 = ov91_0225CCF4(v0->unk_30);

            if (ov91_0225CD5C(v0->unk_30)) {
                u32 v6;
                BOOL v7;

                v6 = ov91_0225CD6C(v0->unk_30);
                v7 = ov91_0225CCA8(v0, 27, &v6, sizeof(u32));

                if (v7 == 1) {
                    ov91_0225CD64(v0->unk_30);
                }
            }

            if (v2 == 0) {
                ov91_0225CCA8(v0, 23, 0L, 0);
            }
        }

        ov91_0225D1DC(v0->unk_34);
        ov91_0225CB64(v0);
        ov91_0225CB98(v0);
        break;
    case 8:
        ov91_0225D078(v0->unk_34, 0);

        if (v0->unk_44 == 0) {
            v2 = ov91_0225CDAC(v0->unk_30);

            if (v2 == 1) {
                UnkStruct_ov115_02265AD4 v8;

                ov91_0225CDC4(v0->unk_30, &v8);
                v2 = sub_02037030(26, &v8, sizeof(UnkStruct_ov115_02265AD4));

                if (v2) {
                    (*param1)++;
                }
            }
        } else {
            (*param1)++;
        }
        break;
    case 9:
        ov91_0225D078(v0->unk_34, 0);

        if (v0->unk_40 == 1) {
            (*param1)++;
        }
        break;
    case 10:
        v2 = ov91_0225D078(v0->unk_34, 1);

        if (v2 == 0) {
            (*param1)++;
        }
        break;
    case 11:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)(26), (enum FadeType)(26), 0xffff, 6, 1, HEAP_ID_106);
        ov91_0225D078(v0->unk_34, 1);
        (*param1)++;
        break;
    case 12:
        ov91_0225D078(v0->unk_34, 1);

        if (IsPaletteFadeFinished()) {
            sub_02037AC0(3);
            (*param1)++;
        }
        break;
    case 13:
        if (!sub_02037B38(3)) {
            return 0;
        }

        if (v1->unk_38) {
            ov00_021E6A4C();
        }

        if (v0->unk_44 == 0) {
            ov91_0225CCEC(v0->unk_30);
            v0->unk_30 = 0L;
        }

        ov91_0225CE80(v0->unk_34);
        v0->unk_34 = 0L;
        Main_SetVBlankIntrCB(0L, 0L);
        HBlankInterruptDisable();
        GF_DestroyVramTransferManager();
        (*param1)++;
        break;
    case 14: {
        u32 v9;

        ov90_02258AA8(&v0->unk_18, v0->unk_08.unk_08);
        v9 = v0->unk_18.unk_10[v0->unk_46];
    }
        v0->unk_04 = ov90_022589E0(&v0->unk_08, &v0->unk_18, HEAP_ID_106);
        (*param1)++;
        break;
    case 15: {
        BOOL v10;

        if (ov90_02258AA0(v0->unk_04) == 1) {
            v10 = ov90_02258AA4(v0->unk_04);

            ov90_02258A04(v0->unk_04);
            v0->unk_04 = 0L;

            if (v10 == 0) {
                return 1;
            } else {
                (*param1) = 0;
            }
        }
    } break;
    }

    return 0;
}
