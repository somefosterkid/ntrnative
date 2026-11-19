#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "sprite.h"
#include "bg_window.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "screen_fade.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"

struct BgConfig;

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

struct TouchScreenHitTableActionStateInternal;

struct PlayerProfile;

struct UnkStruct_ov114_0225E854_t;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

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

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

void ov00_021E6A4C(void);

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

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

void ov90_02259170(UnkStruct_ov114_0225CFCC *param0);

void ov90_02259200(UnkStruct_ov114_0225D084 *param0, const PlayerProfile *param1);

void ov90_0225926C(UnkStruct_ov114_0225D084 *param0, u32 param1);

void ov90_02259464(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225D084 *param1, u32 param2, u32 param3);

void ov90_022594FC(UnkStruct_ov114_0225D338 *param0, u32 param1);

BOOL ov90_02259538(const UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_0225A088(UnkStruct_ov114_0225DFFC *param0, UnkStruct_ov114_0225CEF0 *param1, u32 heapID);

void ov90_0225A108(UnkStruct_ov114_0225DFFC *param0);

u32 ov90_0225A28C(u32 param0);

void ov90_0225B274(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B2A8(UnkStruct_ov114_0225E854 *param0);

BOOL ov90_0225B38C(UnkStruct_ov114_0225F270 *param0, UnkStruct_ov114_0225D338 *param1, UnkStruct_ov114_0225D084 *param2, u32 param3, u32 heapID);

u8 ov90_0225B538(const UnkStruct_ov114_0225F270 *param0);

void ov90_0225B7FC(UnkStruct_ov114_0225E854 *param0, u32 param1, u32 param2);

u32 ov90_0225BA14(const UnkStruct_ov114_0225E854 *param0);

void ov90_0225BA38(UnkStruct_ov114_0225E854 *param0);

BOOL ov90_0225B8F0(UnkStruct_ov114_0225E854 *param0);

BOOL ov90_0225B978(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B9A8(UnkStruct_ov114_0225E854 *param0);

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_0225A980(SysTask *param0, void *param1);

void ov90_0225A980(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225E854 *v0 = param1;
    BOOL v1;

    switch (v0->unk_04) {
    case 0:

        v0->unk_6B0 = v0->unk_0C.unk_0C->unk_00;

        if (v0->unk_0C.unk_0B == 1) {
            v0->unk_09 = 0;
        } else {
            u32 v2;

            v2 = ov90_0225BA14(v0);

            if ((v0->unk_1C.unk_10[v0->unk_0C.unk_09] == 0) && (v0->unk_1C.unk_00[v0->unk_0C.unk_09] > v2)) {
                v0->unk_0C.unk_0C->unk_00 = ov90_0225A28C(v0->unk_0C.unk_0C->unk_00);
                v0->unk_09 = 1;
            } else {
                v0->unk_09 = 0;
            }

            if (v0->unk_0C.unk_09 == 0) {
                ov90_0225BA38(v0);
            }
        }

        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)(1), (enum FadeType)(1), 0xffff, 6, 1, (enum HeapID)(v0->heapID));
        v0->unk_04++;
        break;
    case 1:
        v1 = IsPaletteFadeFinished();

        if (v1) {
            v0->unk_04++;
        }
        break;
    case 2:
        ov90_02259464(&v0->unk_68, &v0->unk_58, 1, 0);
        v0->unk_04++;
        break;
    case 3:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1) {
            v0->unk_00 = 64;
            v0->unk_04++;
        }
        break;
    case 4:
        if (v0->unk_00 > 0) {
            v0->unk_00--;

            if (v0->unk_00 == 0) {
                int v3;
                BOOL v4 = 1;
                u32 v5;

                ov90_022594FC(&v0->unk_68, 0);
                ov90_0225A088(&v0->unk_3A0, &v0->unk_4C, v0->heapID);

                v5 = ov90_0225BA14(v0);

                for (v3 = 0; v3 < v0->unk_0C.unk_08; v3++) {
                    if (v0->unk_1C.unk_00[v3] > v5) {
                        v4 = 0;
                    }
                }

                if (v4 == 1) {
                    v0->unk_6B6 = 16;
                    v0->unk_04 = 22;
                } else {
                    v0->unk_04++;
                    v0->unk_6B4 = 4;
                }
            }
        }
        break;
    case 5: {
        int v6;
        BOOL v7;

        v7 = 0;

        do {
            v0->unk_6B4--;

            for (v6 = 0; v6 < v0->unk_0C.unk_08; v6++) {
                if (v0->unk_1C.unk_10[v6] == v0->unk_6B4) {
                    v7 = 1;
                }
            }
        } while (v7 == 0);

        switch (v0->unk_6B4) {
        case 3:
        case 2:
            v0->unk_6B6 = 16;
            break;
        case 1:
        case 0:
            v0->unk_6B6 = 32;
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }

        ov90_0225B8F0(v0);
        v0->unk_04++;
    } break;
    case 6:
        v0->unk_6B6--;
        ov90_0225B8F0(v0);

        if (v0->unk_6B6 > 0) {
            break;
        }

        {
            int v8;
            BOOL v9;

            for (v8 = 0; v8 < v0->unk_0C.unk_08; v8++) {
                v9 = 0;

                if (v0->unk_1C.unk_10[v8] == v0->unk_6B4) {
                    v9 = 1;
                } else {
                    if ((v0->unk_6B4 == 1) && (v0->unk_1C.unk_10[v8] == 0)) {
                        v9 = 1;
                    }
                }

                if (v9) {
                    ov90_0225B7FC(v0, v8, v0->unk_1C.unk_10[v8]);
                }
            }
        }

        if (v0->unk_6B4 > 1) {
            v0->unk_04 = 5;
        } else {
            v0->unk_04++;
        }
        break;
    case 7:
        if (ov90_0225B8F0(v0)) {
            v0->unk_04++;
        }
        break;
    case 8:
        v0->unk_04++;
        ov90_0225B274(v0);
        v0->unk_00 = 30;
        break;
    case 9:
        if (v0->unk_00 > 0) {
            v0->unk_00--;
        }

        if (v0->unk_00 == 0) {
            v0->unk_04 = 10;
            v0->unk_00 = 0;
        }
        break;
    case 10: {
        u32 v10;

        if (v0->unk_1C.unk_10[v0->unk_0C.unk_09] == 0) {
            v10 = 17;
        } else {
            v10 = 18;
        }

        ov90_02259200(&v0->unk_58, v0->unk_34.unk_08[v0->unk_0C.unk_09]);
        ov90_02259464(&v0->unk_68, &v0->unk_58, v10, 0);
    }
        v0->unk_04++;
        break;
    case 11:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1) {
            if (ov90_0225B978(v0) == 1) {
                if (v0->unk_0C.unk_0B == 1) {
                    v0->unk_00 = 102;
                    v0->unk_04 = 14;
                } else {
                    v0->unk_00 = 102;
                    v0->unk_04++;
                }
            }
        }
        break;
    case 12:
        if (v0->unk_00 > 0) {
            v0->unk_00--;
            break;
        }

        {
            u32 v11;

            if (v0->unk_09) {
                ov90_0225926C(&v0->unk_58, v0->unk_0C.unk_0C->unk_00);

                if (v0->unk_6B0 == v0->unk_0C.unk_0C->unk_00) {
                    v11 = 19;
                } else {
                    v11 = 2;
                }
            } else {
                v11 = 9;
            }

            ov90_02259200(&v0->unk_58, v0->unk_34.unk_08[v0->unk_0C.unk_09]);
            ov90_02259464(&v0->unk_68, &v0->unk_58, v11, 0);
        }
        v0->unk_04 = 13;
        break;
    case 13:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1) {
            v0->unk_00 = 102;
            v0->unk_04++;
        }
        break;
    case 14:
        if (v0->unk_00 > 0) {
            v0->unk_00--;

            if (v0->unk_00 == 0) {
                ov90_022594FC(&v0->unk_68, 0);
                v0->unk_00 = 30;
                v0->unk_04++;
            }
        }
        break;
    case 15:
        if (v0->unk_00 > 0) {
            v0->unk_00--;
        } else {
            sub_02037AC0(130);

            if (v0->unk_0C.unk_0A) {
                ov00_021E6A4C();
            }

            v0->unk_04++;
        }
        break;
    case 16:
        if (sub_02037B38(130)) {
            if (v0->unk_0C.unk_0B) {
                v0->unk_04 = 19;
            } else {
                v0->unk_04++;
            }
        }
        break;
    case 17:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, (enum HeapID)(v0->heapID));
        v0->unk_04++;
        break;
    case 18:
        v1 = IsPaletteFadeFinished();

        if (v1) {
            v0->unk_04 = 21;
        }
        break;
    case 19:
        ov90_0225B9A8(v0);
        v0->unk_04++;
        break;
    case 20:
        v1 = ov90_0225B38C(&v0->unk_24C, &v0->unk_68, &v0->unk_58, v0->unk_07, v0->heapID);

        if (v1) {
            v0->unk_08 = ov90_0225B538(&v0->unk_24C);
            v0->unk_04++;
        }
        break;
    case 21:
        ov90_0225A108(&v0->unk_3A0);
        v0->unk_06 = 1;
        break;
    case 22:
        v0->unk_6B6--;

        if (v0->unk_6B6 <= 0) {
            v0->unk_04++;
        }
        break;
    case 23: {
        int v12;

        for (v12 = 0; v12 < v0->unk_0C.unk_08; v12++) {
            v0->unk_1C.unk_10[v12] = v0->unk_0C.unk_08 - 1;
            ov90_0225B7FC(v0, v12, v0->unk_1C.unk_10[v12]);
        }

        v0->unk_04 = 7;
    } break;
    }

    ov90_0225B2A8(v0);
    ov90_02259170(&v0->unk_A0);

    ScheduleSetBgPosText(v0->unk_4C.unk_00, 3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(v0->unk_4C.unk_00, 5, BG_POS_OP_ADD_Y, 2);
}

u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1);

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);

void ov90_02259250(UnkStruct_ov114_0225D084 *param0, u32 param1);

void ov90_02259BCC(UnkStruct_ov114_0225DAC0 *param0, u32 param1, u32 param2, NNSG2dScreenData *param3, UnkStruct_ov114_0225CEF0 *param4, u32 param5, u32 param6, UnkStruct_ov114_0225CFCC *param7, UnkStruct_ov114_0225CDB4 *param8, u32 heapID);

void ov90_02259D50(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, const PlayerProfile *param2, s16 param3, s16 param4, u32 param5);

void ov90_02259DAC(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225D084 *param1, BOOL param2, u32 param3, u32 param4, s16 param5);

void ov90_02259E8C(UnkStruct_ov114_0225DAC0 *param0, UnkStruct_ov114_0225CEF0 *param1);

void ov90_02259EA0(UnkStruct_ov114_0225DAC0 *param0);

void ov90_02259EE0(UnkStruct_ov114_0225DAC0 *param0, s16 param1, s16 param2);

void ov90_0225B6B0(UnkStruct_ov114_0225F578 *param0, u32 param1);

BOOL ov90_0225B6C4(UnkStruct_ov114_0225F578 *param0, UnkStruct_ov114_0225CEF0 *param1);

void ov90_0225BAA0(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B954(UnkStruct_ov114_0225E854 *param0);

void ov90_0225AE4C(SysTask *param0, void *param1);

void ov90_0225AE4C(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225E854 *v0 = param1;
    BOOL v1;

    switch (v0->unk_04) {
    case 0:
        if (v0->unk_0C.unk_0B == 1) {
            v0->unk_09 = 0;
        } else {
            if (v0->unk_1C.unk_14 >= 10) {
                v0->unk_0C.unk_0C->unk_00 = ov90_0225A28C(v0->unk_0C.unk_0C->unk_00);
                v0->unk_09 = 1;
            } else {
                v0->unk_09 = 0;
            }

            if (v0->unk_0C.unk_09 == 0) {
                ov90_0225BAA0(v0);
            }
        }

        BeginNormalPaletteFade(FADE_MAIN_ONLY, (enum FadeType)(1), (enum FadeType)(1), 0xffff, 6, 1, (enum HeapID)(v0->heapID));
        v0->unk_04++;
        break;
    case 1:
        v1 = IsPaletteFadeFinished();

        if (v1) {
            v0->unk_04++;

            v0->unk_6B4 = 4;
        }
        break;
    case 2:
        ov90_02259464(&v0->unk_68, &v0->unk_58, 1, 0);
        v0->unk_04++;
        break;
    case 3:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1) {
            v0->unk_00 = 102;
            v0->unk_04++;
        }
        break;
    case 4:
        if (v0->unk_00 > 0) {
            v0->unk_00--;

            if (v0->unk_00 == 0) {
                ov90_022594FC(&v0->unk_68, 0);
                v0->unk_04++;
            }
        }
        break;
    case 5: {
        int v2;
        BOOL v3;
        u32 v4;

        ov90_0225A088(&v0->unk_3A0, &v0->unk_4C, v0->heapID);

        for (v2 = 0; v2 < v0->unk_0C.unk_08; v2++) {
            ov90_02259BCC(&v0->unk_2C0[v2], v0->unk_0C.unk_08, v0->unk_07, v0->unk_2BC[0], &v0->unk_4C, v2, v0->unk_0C.unk_09, &v0->unk_A0, v0->unk_248, v0->heapID);

            v4 = ov90_0225888C(&v0->unk_0C, v2);
            v3 = ov90_022588A4(&v0->unk_0C, v4);

            ov90_02259D50(&v0->unk_2C0[v2], &v0->unk_58, v0->unk_34.unk_08[v2], 8, 0, v3);
            ov90_02259DAC(&v0->unk_2C0[v2], &v0->unk_58, v0->unk_05, v0->unk_34.unk_00[v2], v0->unk_34.unk_04[v2], 8);
            ov90_02259E8C(&v0->unk_2C0[v2], &v0->unk_4C);
            ov90_02259EA0(&v0->unk_2C0[v2]);
            ov90_02259EE0(&v0->unk_2C0[v2], -8, 3 * v2);
        }
    }
        v0->unk_04++;
        v0->unk_00 = 16;
        break;
    case 6:
        v1 = ov90_0225B8F0(v0);

        if (v1 == 1) {
            v0->unk_00--;

            if (v0->unk_00 <= 0) {
                ov90_0225B6B0(&v0->unk_26C, v0->unk_1C.unk_14);
                v0->unk_04++;
            }
        }
        break;
    case 7:
        v1 = ov90_0225B6C4(&v0->unk_26C, &v0->unk_4C);

        if (v1 == 1) {
            if (v0->unk_09 == 1) {
                ov90_0225B954(v0);
            }

            v0->unk_04++;
        }
        break;
    case 8:
        ov90_02259250(&v0->unk_58, v0->unk_1C.unk_14);
        ov90_02259464(&v0->unk_68, &v0->unk_58, 7, 0);
        v0->unk_00 = 102;
        v0->unk_04++;
        break;
    case 9:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1 == 1) {
            if (v0->unk_00 <= 0) {
                if (ov90_0225B978(v0) == 1) {
                    if (v0->unk_0C.unk_0B == 1) {
                        v0->unk_00 = 102;
                        v0->unk_04 = 12;
                    } else {
                        v0->unk_04++;
                    }
                }
            } else {
                v0->unk_00--;
            }
        }
        break;
    case 10: {
        u32 v5;

        if (v0->unk_09) {
            v5 = 8;
        } else {
            v5 = 9;
        }

        ov90_02259464(&v0->unk_68, &v0->unk_58, v5, 0);
    }
        v0->unk_04++;
        break;
    case 11:
        v1 = ov90_02259538(&v0->unk_68, 0);

        if (v1) {
            v0->unk_00 = 102;

            if (v0->unk_09) {
                v0->unk_04 = 13;
            } else {
                v0->unk_04++;
            }
        }
        break;
    case 12:
        if (v0->unk_00 > 0) {
            v0->unk_00--;

            if (v0->unk_00 == 0) {
                v0->unk_04 = 14;
            }
        }
        break;
    case 13:
        if (v0->unk_00 > 0) {
            v0->unk_00--;
        }

        if (v0->unk_00 == 0) {
            v0->unk_04 = 14;
        }
        break;
    case 14:
        if (v0->unk_0C.unk_0A) {
            ov00_021E6A4C();
        }

        sub_02037AC0(130);
        v0->unk_04++;
        break;
    case 15:
        if (sub_02037B38(130)) {
            if (v0->unk_0C.unk_0B) {
                v0->unk_04 = 18;
            } else {
                v0->unk_04++;
            }
        }
        break;
    case 16:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, (enum HeapID)(v0->heapID));
        v0->unk_04++;
        break;
    case 17:
        v1 = IsPaletteFadeFinished();

        if (v1) {
            v0->unk_04 = 20;
        }
        break;
    case 18:
        ov90_0225B9A8(v0);
        v0->unk_04++;
        break;
    case 19:
        v1 = ov90_0225B38C(&v0->unk_24C, &v0->unk_68, &v0->unk_58, v0->unk_07, v0->heapID);

        if (v1) {
            v0->unk_08 = ov90_0225B538(&v0->unk_24C);
            v0->unk_04++;
        }
        break;
    case 20:
        ov90_0225A108(&v0->unk_3A0);
        v0->unk_06 = 1;
        break;
    }

    ov90_0225B2A8(v0);
    ov90_02259170(&v0->unk_A0);

    ScheduleSetBgPosText(v0->unk_4C.unk_00, 3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(v0->unk_4C.unk_00, 5, BG_POS_OP_ADD_Y, 2);
}
