#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "screen_fade.h"
#include "struct_defs/resdat_narc_structs.h"

struct GraphicsModes;

struct MessageBank;

struct MessageBankEntry;

struct SaveBlockInfo;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct PlayerProfile;

struct UnkStruct_ov114_0225D678_t;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
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

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1);

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);

void ov00_021E6A4C(void);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

BOOL sub_02037030(int cmd, const void *data, int size);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

struct UnkStruct_ov114_0225D678_t {
    u8 unk_00;
    u8 unk_01;
    u16 heapID;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225E0F8 unk_18;
    UnkStruct_ov114_0225CEF0 unk_30;
    UnkStruct_ov114_0225D084 unk_3C;
    UnkStruct_ov114_0225D338 unk_4C;
    UnkStruct_ov114_0225CFCC unk_84;
    UnkStruct_ov114_0225CCD0 unk_214;
    UnkStruct_ov114_0225CDB4 *unk_22C;
    UnkStruct_ov114_0225E1A4 unk_230;
    void *unk_260;
    NNSG2dScreenData *unk_264;
    UnkStruct_ov114_0225DAC0 unk_268[4];
    UnkStruct_ov114_0225DFFC unk_348;
    SysTask *unk_650;
    SysTask *unk_654;
};

void ov90_02259170(UnkStruct_ov114_0225CFCC *param0);

void ov90_02259464(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, u32 param3);

void ov90_022594FC(UnkStruct_ov114_0225D338 *param0, u32 param1);

BOOL ov90_02259538(const UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_02259BCC(UnkStruct_ov114_0225DAC0 *param0, u32 param1, u32 param2, NNSG2dScreenData *param3, UnkStruct_ov114_0225CEF0 *param4, u32 param5, u32 param6, UnkStruct_ov114_0225CFCC *param7, UnkStruct_ov114_0225CDB4 *param8, u32 heapID);

void ov90_02259D50(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, const PlayerProfile *param2, s16 param3, s16 param4, u32 param5);

void ov90_02259DAC(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, BOOL param2, u32 param3, u32 param4, s16 param5);

void ov90_02259E8C(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225CEF0 *param1);

void ov90_02259EA0(UnkStruct_ov114_0225DAC0 *param0);

void ov90_02259EE0(UnkStruct_ov114_0225DAC0 *param0, s16 param1, s16 param2);

void ov90_02259F44(UnkStruct_ov114_0225DAC0 *param0, s16 param1);

void ov90_0225A088(UnkStruct_ov114_0225DFFC *param0, UnkStruct_ov114_0225CEF0 *param1, u32 heapID);

void ov90_0225A108(UnkStruct_ov114_0225DFFC *param0);

BOOL ov90_0225A258(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225E0F8 *param1);

void ov90_0225A350(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, u32 heapID);

void ov90_0225A3E0(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

BOOL ov90_0225A428(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

BOOL ov90_0225A544(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

int ov90_02259B38(const u8 *param0, u32 param1);

BOOL ov90_02259B68(UnkStruct_ov114_0225D678 *param0);

extern const u8 ov90_0225C1E8[3];

extern const u8 _0225C1E0[2];

extern const u8 ov90_0225C1E4[3];

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_02259794(SysTask *param0, void *param1);

void ov90_02259794(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225D678 *v0 = param1;
    BOOL v1;

    switch (v0->unk_00) {
    case 0:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)(1), (enum FadeType)(1), 0xffff, 6, 1, (enum HeapID)(v0->heapID));
        ov90_0225A350(&v0->unk_230, &v0->unk_30, v0->heapID);
        v0->unk_00++;
        break;
    case 1:
        ov90_0225A428(&v0->unk_230, &v0->unk_30);
        v1 = IsPaletteFadeFinished();

        if (v1 == 1) {
            v0->unk_00++;
        }
        break;
    case 2:
        v1 = ov90_0225A428(&v0->unk_230, &v0->unk_30);

        if (v1 == 1) {
            v0->unk_00++;
        }
        break;
    case 3:
        ov90_02259464(&v0->unk_4C, &v0->unk_3C, 0, 0);
        v0->unk_00++;
        break;
    case 4:
        v1 = ov90_02259538(&v0->unk_4C, 0);

        if (v1) {
            v0->unk_00++;
            v0->unk_01 = 64;
        }
        break;
    case 5:
        if (v0->unk_01 > 0) {
            v0->unk_01--;

            if (v0->unk_01 == 0) {
                ov90_022594FC(&v0->unk_4C, 0);
                ov90_0225A3E0(&v0->unk_230, &v0->unk_30);
                v0->unk_00++;
            }
        }
        break;
    case 6:
        v1 = ov90_0225A544(&v0->unk_230, &v0->unk_30);

        if (v1 == 1) {
            v0->unk_00++;
        }
        break;
    case 7: {
        int v2;
        BOOL v3;
        BOOL v4;
        u32 v5;

        ov90_0225A088(&v0->unk_348, &v0->unk_30, v0->heapID);

        v3 = ov90_0225A258(&v0->unk_08, &v0->unk_18);

        for (v2 = 0; v2 < v0->unk_08.unk_08; v2++) {
            ov90_02259BCC(&v0->unk_268[v2], v0->unk_08.unk_08, v0->unk_04, v0->unk_264, &v0->unk_30, v2, v0->unk_08.unk_09, &v0->unk_84, v0->unk_22C, v0->heapID);

            v5 = ov90_0225888C(&v0->unk_08, v2);
            v4 = ov90_022588A4(&v0->unk_08, v5);

            ov90_02259D50(&v0->unk_268[v2], &v0->unk_3C, v0->unk_18.unk_08[v2], 8, 0, v4);
            ov90_02259DAC(&v0->unk_268[v2], &v0->unk_3C, v3, v0->unk_18.unk_00[v2], v0->unk_18.unk_04[v2], 8);
            ov90_02259EE0(&v0->unk_268[v2], -8, 3 * v2);
        }
    }

        {
            int v6;

            for (v6 = 0; v6 < v0->unk_08.unk_08; v6++) {
                ov90_02259E8C(&v0->unk_268[v6], &v0->unk_30);
                ov90_02259EA0(&v0->unk_268[v6]);
            }
        }

        v0->unk_01 = 92;
        v0->unk_00++;
        break;
    case 8:
        if (ov90_02259B68(v0) == 1) {
            v0->unk_00++;
        }
        break;
    case 9:
        v0->unk_01--;

        if (v0->unk_01 == 0) {
            v0->unk_00++;
            sub_02037AC0(129);

            if (v0->unk_08.unk_0A) {
                ov00_021E6A4C();
            }
        }
        break;
    case 10:
        if (sub_02037B38(129)) {
            v0->unk_00++;
        }
        break;
    case 11: {
        int v7;

        for (v7 = 0; v7 < v0->unk_08.unk_08; v7++) {
            ov90_02259F44(&v0->unk_268[v7], v7 * 3);
        }

        v0->unk_00++;
        v0->unk_01 = 8;
    } break;
    case 12:
        v0->unk_01--;
        ov90_02259B68(v0);

        if (v0->unk_01 <= 0) {
            v0->unk_00++;
        }
        break;
    case 13:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, (enum HeapID)(v0->heapID));
        ov90_02259B68(v0);

        if (v0->unk_08.unk_09 == 0) {
            u32 v8;

            switch (v0->unk_04) {
            case UnkEnum_ov66_022324D0_00:
                v8 = ov90_02259B38(ov90_0225C1E8, 3);
                break;
            case UnkEnum_ov66_022324D0_01:
                v8 = ov90_02259B38(_0225C1E0, 2);
                break;
            case UnkEnum_ov66_022324D0_02:
                v8 = ov90_02259B38(ov90_0225C1E4, 3);
                break;
            }

            sub_02037030(26, &v8, sizeof(u32));
        }

        v0->unk_00++;
        break;
    case 14:
        ov90_02259B68(v0);
        v1 = IsPaletteFadeFinished();

        if (v1 == 1) {
            ov90_0225A108(&v0->unk_348);
            v0->unk_00++;
        }
        break;
    case 15:
        if (v0->unk_07 == 1) {
            v0->unk_00++;
        }
        break;
    case 16:
        break;
    }

    ov90_02259170(&v0->unk_84);

    ScheduleSetBgPosText(v0->unk_30.unk_00, 3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(v0->unk_30.unk_00, 5, BG_POS_OP_ADD_Y, 2);
}
