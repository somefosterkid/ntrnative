#include "global.h"

#include "constants/heap.h"

#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "msgdata.h"

struct MsgData;

struct MessageFormat;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov61_0222B138;

typedef struct {
    u8 unk_00;
    u8 unk_01[3];
    u32 unk_04;
} UnkStruct_ov61_0222B2D8;

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_0223CAA4;

typedef void (*UnkFuncPtr_ov61_0222B920)(void *, const UnkStruct_ov62_0223CAA4 *);

typedef BOOL (*UnkFuncPtr_ov61_0222B920_1)(void *, void *);

typedef struct {
    UnkFuncPtr_ov61_0222B920 unk_00;
    u16 unk_04;
    u16 unk_06;
    u8 unk_08;
    UnkFuncPtr_ov61_0222B920_1 unk_0C;
} UnkStruct_ov61_0222B920;

typedef void (*UnkFuncPtr_ov62_022349A8)(void *, String *);

typedef struct {
    void *unk_00;
    UnkFuncPtr_ov61_0222B920 unk_04;
    UnkFuncPtr_ov61_0222B920 unk_08;
    UnkFuncPtr_ov61_0222B920 unk_0C;
    UnkFuncPtr_ov61_0222B920 unk_10;
    UnkFuncPtr_ov61_0222B920 unk_14;
    UnkFuncPtr_ov61_0222B920 unk_18;
    UnkFuncPtr_ov61_0222B920 unk_1C;
    UnkFuncPtr_ov61_0222B920 unk_20;
    UnkFuncPtr_ov61_0222B920 unk_24;
    UnkFuncPtr_ov61_0222B920 unk_28;
} UnkStruct_ov62_022349A8_sub2;

typedef struct {
    u32 personality;
    u32 otID;
    u16 species;
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
} UnkStruct_ov61_0222AE80_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov61_0222AE80_sub2;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov62_02239DA4_sub1;

typedef struct {
    u32 integrity;
    UnkStruct_ov61_0222AE80_sub1 unk_04;
    u32 unk_24;
    MailMessage title;
    UnkStruct_ov61_0222AE80_sub2 unk_30[10];
    u8 unk_58;
    u8 language;
    u8 unk_5A[2];
    UnkStruct_ov62_02239DA4_sub1 unk_5C;
} UnkStruct_ov61_0222AE80;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B_0 : 1;
    u8 unk_1B_1 : 7;
    u16 unk_1C;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_ov62_02239DA4_sub1 unk_7C;
} UnkStruct_ov61_0222AFC0;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222AE80 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub1;

typedef struct {
    u16 unk_00[20];
    u16 unk_28[30];
    u32 unk_64[30];
    u32 unk_DC[30];
    u32 unk_154;
    u8 unk_158[30];
    u8 unk_176;
    u8 unk_177;
    u8 unk_178[32];
    UnkStruct_ov62_02239DA4_sub1 unk_198;
} UnkStruct_ov62_02239DA4;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov62_02239DA4 unk_80;
} UnkStruct_ov62_022349A8_sub3_sub2;

typedef struct {
    u16 unk_00[12];
    u16 unk_18;
    u8 unk_1A;
    u8 unk_1B;
    s8 unk_1C;
    s8 unk_1D;
    u8 unk_1E_0 : 1;
    u8 unk_1E_1 : 1;
    u8 unk_1E_2 : 1;
    u8 unk_1E_3 : 1;
    u8 unk_1E_4 : 1;
    u8 : 3;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub1_sub1;

typedef struct {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    UnkStruct_ov61_0222BED8_sub1_sub1 unk_28;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_ov62_02239DA4_sub1 unk_60;
} UnkStruct_ov61_0222BED8_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04[4];
    u32 unk_0C;
    u32 unk_10;
    u16 unk_14[8];
    MailMessage unk_24;
    MailMessage unk_2C;
} UnkStruct_ov61_0222BED8_sub2_sub1_sub1;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08[4];
    UnkStruct_ov61_0222BED8_sub2_sub1_sub1 unk_18[4];
    int unk_E8;
    int unk_EC;
    int unk_F0;
    int unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
    int unk_108;
    int unk_10C;
    int unk_110;
    u32 unk_114[4];
    u32 unk_124;
    int unk_128;
    u32 unk_12C;
    u32 unk_130;
    int unk_134[4];
    u16 unk_144;
    u16 unk_146;
    int unk_148;
    u8 unk_14C[4];
} UnkStruct_ov61_0222BED8_sub2_sub1;

typedef struct {
    u8 unk_00[4][1024];
} UnkStruct_ov61_0222BED8_sub2_sub2;

typedef struct {
    u32 unk_00;
    u16 unk_04_0 : 1;
    u16 unk_04_1 : 1;
    u16 unk_04_2 : 1;
    u16 unk_04_3 : 13;
    u16 unk_06;
    u16 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u16 unk_1C[4];
    u8 unk_24[4];
    u8 unk_28[4];
    u32 unk_2C_0 : 5;
    u32 unk_2C_5 : 5;
    u32 unk_2C_10 : 5;
    u32 unk_2C_15 : 5;
    u32 unk_2C_20 : 5;
    u32 unk_2C_25 : 5;
    u32 unk_2C_30 : 1;
    u32 unk_2C_31 : 1;
    u8 unk_30_0 : 1;
    u8 unk_30_1 : 2;
    u8 unk_30_3 : 5;
    u16 unk_32[11];
    u16 unk_48[8];
    u8 unk_58;
    u8 unk_59;
    u32 unk_5C;
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u16 unk_6A;
    u16 unk_6C;
    u16 unk_6E;
} UnkStruct_ov61_0222BED8_sub2_sub3_sub1;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov61_0222BED8_sub2_sub3_sub1 unk_04[6];
} UnkStruct_ov61_0222BED8_sub2_sub3;

typedef struct {
    u16 unk_00[8];
    u32 unk_10;
    u32 unk_14;
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D_0 : 1;
    u8 unk_1D_1 : 1;
    u8 unk_1D_2 : 6;
    u8 unk_1E;
    u8 unk_1F;
} UnkStruct_ov61_0222BED8_sub2_sub4;

typedef struct {
    u16 unk_00_0 : 4;
    u16 unk_00_4 : 2;
    u16 unk_00_6 : 1;
    u16 unk_00_7 : 1;
    u16 unk_00_8 : 2;
    u16 unk_00_10 : 5;
    u16 unk_00_15 : 1;
} UnkStruct_ov61_0222BED8_sub2_sub5;

typedef struct {
    UnkStruct_ov61_0222BED8_sub2_sub1 unk_00;
    UnkStruct_ov61_0222BED8_sub2_sub2 unk_150;
    UnkStruct_ov61_0222BED8_sub2_sub3 unk_1150[4];
    UnkStruct_ov61_0222BED8_sub2_sub4 unk_1B68[4];
    UnkStruct_ov61_0222BED8_sub2_sub5 unk_1BE8;
    u16 unk_1BEA;
    UnkStruct_ov62_02239DA4_sub1 unk_1BEC;
} UnkStruct_ov61_0222BED8_sub2;

typedef struct {
    UnkStruct_ov61_0222AFC0 unk_00;
    UnkStruct_ov61_0222BED8_sub1 unk_80;
    UnkStruct_ov61_0222BED8_sub2 unk_E4;
} UnkStruct_ov62_022349A8_sub3_sub3;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05[3];
    u32 unk_08;
    u8 unk_0C[4];
} UnkStruct_ov62_022349A8_sub3_sub4;

typedef struct {
    u32 unk_00;
} UnkStruct_ov62_022349A8_sub3_sub5;

typedef struct {
    u8 version;
    u8 language;
    u8 month;
    u8 appearance;
    u16 species;
    u8 isEgg : 1;
    u8 form : 7;
    u8 unk_07;
} UnkStruct_ov61_0222AFCC;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov62_0223D518_sub1_sub1;

typedef struct {
    UnkStruct_ov61_0222AFCC unk_00;
    UnkStruct_ov62_0223D518_sub1_sub1 unk_08;
    UnkStruct_ov61_0222B2D8 unk_0C[3];
} UnkStruct_ov62_0223D518_sub1;

typedef struct {
    union {
        UnkStruct_ov62_022349A8_sub3_sub1 unk_00_val1;
        UnkStruct_ov62_022349A8_sub3_sub2 unk_00_val2;
        UnkStruct_ov62_0223D518_sub1 unk_00_val3;
        UnkStruct_ov62_022349A8_sub3_sub3 *unk_00_val4;
        UnkStruct_ov62_022349A8_sub3_sub4 unk_00_val5;
        UnkStruct_ov62_022349A8_sub3_sub5 unk_00_val6;
    };
    union {
        struct {
            u8 unk_00;
            u8 unk_01;
            u8 unk_02[2];
        } val1;
        struct {
            u16 unk_00;
            u16 unk_02;
        } val2;
        struct {
            u64 unk_00;
        } val3;
    } unk_21C;
} UnkStruct_ov62_022349A8_sub3;

typedef struct {
    s32 unk_00;
    u8 unk_04;
    u8 language;
    char unk_06[301];
    u8 padding[5];
} UnkStruct_ov62_022349A8_sub1;

int ov39_0222A13C(void);

typedef struct {
    SaveData *saveData;
    UnkFuncPtr_ov62_022349A8 unk_04;
    void *unk_08;
    UnkStruct_ov62_022349A8_sub1 unk_0C;
    enum HeapID heapID;
    int unk_148;
    int unk_14C;
    UnkStruct_ov62_022349A8_sub2 unk_150;
    UnkStruct_ov62_0223CAA4 unk_17C;
    void *unk_18C;
    UnkStruct_ov62_022349A8_sub3 unk_190;
    void *unk_3B4;
    int unk_3B8;
    int unk_3BC;
    int unk_3C0;
    BOOL unk_3C4;
    BOOL unk_3C8;
    UnkStruct_ov61_0222B138 unk_3CC;
    int unk_3D8;
    int unk_3DC;
    int unk_3E0;
    int unk_3E4;
    int unk_3E8;
    int unk_3EC;
    int unk_3F0;
    MsgData *unk_3F4;
    MessageFormat *unk_3F8;
    String *unk_3FC;
    UnkStruct_ov61_0222B920 unk_400;
    u8 unk_40D;
    u8 unk_40E;
} UnkStruct_ov62_022349A8;

typedef int (*UnkFuncPtr_ov61_0222E48C)(UnkStruct_ov62_022349A8 *, UnkStruct_ov61_0222B138 *);

int ov39_022278D4(UnkStruct_ov62_022349A8 *param0);

int ov39_02227A5C(UnkStruct_ov62_022349A8 *param0);

extern const UnkFuncPtr_ov61_0222E48C *const ov39_0222A8B4[3];

int ov39_02227778(UnkStruct_ov62_022349A8 *param0);

BOOL ov39_0222A164(UnkStruct_ov62_022349A8_sub3_sub1 *param0, void *param1);
BOOL ov39_0222A1C0(s32 param0, void *param1);
BOOL ov39_0222A200(s32 param0, UnkStruct_ov62_022349A8_sub3_sub2 *param1, void *param2);
BOOL ov39_0222A268(s32 param0, void *param1);
BOOL ov39_0222A2CC(void *param0);
BOOL ov39_0222A2EC(UnkStruct_ov62_0223D518_sub1 *param0, void *param1);
BOOL ov39_0222A33C(UnkStruct_ov62_022349A8_sub3_sub3 *param0, void *param1);
BOOL ov39_0222A394(UnkStruct_ov62_022349A8_sub3_sub4 *param0, void *param1);
BOOL ov39_0222A3DC(UnkStruct_ov62_022349A8_sub3_sub5 *param0, void *param1);
BOOL ov39_0222A434(UnkStruct_ov62_022349A8_sub3_sub5 *param0, void *param1);
BOOL ov39_0222A48C(u64 param0, u32 param1, void *param2);
BOOL ov39_0222A4C0(u64 param0, void *param1);
int ov39_0222A158(void);
int ov39_02227B5C(UnkStruct_ov62_022349A8 *param0);
BOOL ov39_02227B1C(void *param0, void *param1);
BOOL ov39_02227B58(void *param0, void *param1);

int ov39_02227778(UnkStruct_ov62_022349A8 *param0) {
    int v0;

    if (param0->unk_3C4 == 1) {
        if (param0->unk_3E8 != 23004) {
            if (param0->unk_40E > 0) {
                param0->unk_40E--;
            } else {
                switch (ov39_0222A13C()) {
                case 1:
                case 7:
                case 8:
                case 9:
                    v0 = ov39_022278D4(param0);

                    if (v0 == 1) {
                        param0->unk_3E8 = 23004;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        ov39_02227A5C(param0);

        if (param0->unk_400.unk_0C != NULL) {
            if (param0->unk_400.unk_0C(param0, &param0->unk_400) == 1) {
                param0->unk_400.unk_0C = NULL;

                if (param0->unk_400.unk_00 != NULL) {
                    param0->unk_400.unk_00(param0->unk_150.unk_00, &param0->unk_17C);
                    param0->unk_400.unk_00 = NULL;
                }

                MI_CpuClear8(&param0->unk_400, sizeof(UnkStruct_ov61_0222B920));
                param0->unk_3EC = 23004;
            }
        }
    }

    if (ov39_0222A8B4[param0->unk_3BC] != NULL) {
        int v1;

        v1 = ov39_0222A8B4[param0->unk_3BC][param0->unk_3C0](param0, &param0->unk_3CC);

        switch (v1) {
        case 0:
            break;
        case 1:
            MI_CpuClear8(&param0->unk_3CC, sizeof(UnkStruct_ov61_0222B138));
            param0->unk_3C0++;
            MI_CpuClear8(&param0->unk_3CC, sizeof(UnkStruct_ov61_0222B138));

            if (ov39_0222A8B4[param0->unk_3BC][param0->unk_3C0] == NULL) {
                param0->unk_3C0 = 0;
                param0->unk_3BC = 0;
            }
            break;
        case 2:
            MI_CpuClear8(&param0->unk_3CC, sizeof(UnkStruct_ov61_0222B138));
            param0->unk_3C0 = 0;
            break;
        }
    }

    return 1;
}

int ov39_022278D4(UnkStruct_ov62_022349A8 *param0) {
    int v0 = 0;

    switch (param0->unk_3E8) {
    case 20000:
        v0 = ov39_0222A164(&param0->unk_190.unk_00_val1, param0->unk_3B4);
        break;
    case 20001:
        v0 = ov39_0222A1C0(param0->unk_190.unk_21C.val2.unk_00, param0->unk_3B4);
        break;
    case 21000:
        v0 = ov39_0222A200(param0->unk_190.unk_21C.val1.unk_00, &param0->unk_190.unk_00_val2, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 21001:
        v0 = ov39_0222A268(param0->unk_190.unk_21C.val1.unk_00, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 22000:
        v0 = ov39_0222A2CC(param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 22001:
        v0 = ov39_0222A2EC(&param0->unk_190.unk_00_val3, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 23000:
        v0 = ov39_0222A33C(param0->unk_190.unk_00_val4, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 23001:
        switch (param0->unk_3F0) {
        case 0:
            v0 = ov39_0222A394(&param0->unk_190.unk_00_val5, param0->unk_3B4);
            break;
        case 1:
            v0 = ov39_0222A3DC(&param0->unk_190.unk_00_val6, param0->unk_3B4);
            break;
        case 2:
            v0 = ov39_0222A434(
                &param0->unk_190.unk_00_val6, param0->unk_3B4);
            break;
        }

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 23002:
        v0 = ov39_0222A48C(param0->unk_190.unk_21C.val3.unk_00, 0x140, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    case 23003:
        v0 = ov39_0222A4C0(param0->unk_190.unk_21C.val3.unk_00, param0->unk_3B4);

        if (v0 == 1) {
            (void)0;
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    if (v0 == 1) {
        param0->unk_3EC = param0->unk_3E8;
    } else {
        (void)0;
    }

    return v0;
}

int ov39_02227A5C(UnkStruct_ov62_022349A8 *param0) {
    if (param0->unk_3EC == 23004) {
        return 1;
    }

    param0->unk_148 = ov39_0222A13C();

    if (param0->unk_148 != param0->unk_14C) {
        switch (param0->unk_148) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            param0->unk_17C.unk_04 = 1;
            param0->unk_17C.unk_08 = 0;
            param0->unk_17C.unk_0C = 7;
            param0->unk_17C.unk_00 = 1;
            param0->unk_400.unk_0C = ov39_02227B58;
            break;
        case 8:
            param0->unk_400.unk_0C = ov39_02227B1C;
            ov39_02227B5C(param0);
            break;
        case 9:
            param0->unk_17C.unk_04 = 0;
            param0->unk_17C.unk_08 = 0;
            param0->unk_17C.unk_0C = ov39_0222A158();
            param0->unk_17C.unk_00 = 1;
            param0->unk_400.unk_0C = ov39_02227B58;
            break;
        }

        param0->unk_14C = param0->unk_148;
    }

    return 1;
}
