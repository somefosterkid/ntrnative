#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "overlay_49_getters.h"
#include "sys_task.h"
#include "screen_fade_internal.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct SysTask;

struct SysTaskManager;

struct System;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

UnkStruct_ov63_0222CC3C ov42_02228270(UnkStruct_ov63_0222CC3C param0, int param1);

BOOL ov45_0222A2F8(const WiFiPlazaSystem *param0);


BOOL ov45_0222A3A0(const WiFiPlazaSystem *param0);

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

BOOL ov45_0222B00C(const WiFiPlazaSystem *param0);

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

u32 ov49_022589C4(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);

BOOL ov49_02258A30(u32 param0);

BOOL ov49_02258A50(u32 param0);

BOOL ov49_02258A70(u32 param0);

BOOL ov49_02258A90(u32 param0);

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

UnkStruct_ov70_0225CC54 *ov49_02258DB0(UnkStruct_ov70_0225CA20 *param0);

UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

int ov49_02258F3C(const UnkStruct_ov70_0225CC54 *param0);

UnkStruct_ov70_0225CC54 *ov49_02258F40(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1);

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;



UnkStruct_ov70_02261E10 *ov49_02259FF8(UnkStruct_ov70_0225DEE8 *param0);

UnkStruct_ov70_0225C894 *ov49_0225A000(UnkStruct_ov70_0225DEE8 *param0);


void ov49_0225A03C(UnkStruct_ov70_0225DEE8 *param0, u8 param1);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_49_0225A064_0225A064.c (tools/pokehg_abi.py)
u8 ov49_0225A064(const UnkStruct_ov70_0225DEE8 *param0, int param1);
#else
u8 ov49_0225A064(const UnkStruct_ov70_0225DEE8 *param0, u8 param1);
#endif

BOOL ov49_0225A084(const UnkStruct_ov70_0225DEE8 *param0, u8 param1);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *param0, u32 param1, BOOL param2);

void ov49_0225A4D0(UnkStruct_ov70_0225DEE8 *param0);

u32 ov49_0225A4E0(const UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_0225A4F0(const UnkStruct_ov70_0225DEE8 *param0);

BOOL ov49_0225A500(const UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A53C(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

BOOL ov49_0225A55C(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225E58C(UnkStruct_ov70_02261E10 *param0, u8 param1, u8 param2);

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

void *ov49_0225EF40(UnkStruct_ov70_02263344 *param0, u32 param1);

void *ov49_0225EF84(UnkStruct_ov70_02263344 *param0);

u32 ov49_0225EF88(const UnkStruct_ov70_02263344 *param0);

void ov49_0225EF90(UnkStruct_ov70_02263344 *param0);

void ov49_0225EFC4(UnkStruct_ov70_022630A4 *param0, u32 param1, const UnkStruct_ov70_0226DA18 *param2, void *param3);

BOOL ov70_022669FC(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02266E9C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02267A44(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02268390(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_0226889C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_0226CC74(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

BOOL PlaySE(u16 seqID);

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
    SysTaskManager *mainTaskQueue;
    SysTaskManager *vblankTaskQueue;
    SysTaskManager *vwaitTaskQueue;
    SysTaskManager *printTaskQueue;
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
    u32 unk_00;
} UnkStruct_ov70_0226315C;

typedef void (*UnkFuncPtr_ov70_0226DBB8)(UnkStruct_ov70_0226315C *, UnkStruct_ov70_0225DEE8 *, u32, u32);

typedef struct {
    u8 unk_00[8];
    u32 unk_08;
    UnkStruct_ov70_0226DA18 unk_0C;
    UnkFuncPtr_ov70_0226DBB8 unk_14;
} UnkStruct_ov70_0226DBB8;

BOOL ov49_0225F224(int param0);

BOOL ov70_02263344(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

void ov49_0225F260(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, const UnkStruct_ov70_0226DBB8 *param3, u32 param4);

BOOL ov70_02264150(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02264324(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_022646C8(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02264974(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02264C9C(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov70_02264D38(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

extern const UnkStruct_ov70_0226DA18 ov49_02269B80;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov49_0225F448(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

// asm/overlay_49_0225AC24_0225B244_02269724.s keeps the DS copy (a 4-byte callback pointer and a literal 0), which the 64-bit
// port emits with 4-byte pointers (wordmix.py): the PC build uses this C definition instead.
const UnkStruct_ov70_0226DA18 ov49_02269B80 = { ov49_0225F448, NULL };
#endif

extern const UnkStruct_ov70_0226DBB8 ov49_02269D20[9];

extern const UnkStruct_ov70_0226DBB8 ov49_02269C90[6];

extern const UnkStruct_ov70_0226DBB8 ov49_02269C60[2];

extern const UnkStruct_ov70_0226DA18 ov49_02269B60;

extern const UnkStruct_ov70_0226DA18 ov49_02269B70;

BOOL ov49_0225FDCC(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2);

BOOL ov49_0225FDCC(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2) {
    UnkStruct_ov70_0226315C *v0 = ov49_0225EF84(param0);

    switch (ov49_0225EF88(param0)) {
    case 0:
        ov49_0225EF40(param0, sizeof(UnkStruct_ov70_0226315C));
        ov49_0225EF90(param0);
        break;
    case 1:
        ov49_0225A53C(param1, 0);

        {
            u32 v1;
            u32 v2;
            UnkStruct_ov70_0225CC54 *v3;
            UnkStruct_ov63_0222CC3C v4, v5;
            u32 v6, v7;
            UnkStruct_ov70_0225CA20 *v8;
            UnkStruct_ov70_02261E10 *v9;
            UnkStruct_ov70_0225C894 *v10;
            WiFiPlazaSystem *v11;
            UnkStruct_ov70_022630A4 *v12;
            UnkStruct_ov70_0225CC54 *v13;

            v12 = ov49_0225A010(param1);
            v11 = ov49_02259FE8(param1);
            v8 = ov49_02259FF0(param1);
            v10 = ov49_0225A000(param1);
            v9 = ov49_02259FF8(param1);
            v13 = ov49_02258DB0(v8);

            if (v13 != 0L) {
                if ((ov45_0222A330(v11) == 0) && (ov45_0222A3A0(v11) == 1) && (ov45_0222A2F8(v11) == 0)) {
                    if (ov49_02258F3C(v13) != 9) {
                        ov49_02258EEC(v8, v13, 9);
                    }
                } else {
                    if (ov49_02258F3C(v13) != 0) {
                        ov49_02258EEC(v8, v13, 0);
                    }
                }
            }

            v3 = ov49_02258DAC(v8);
            v1 = ov49_02258E60(v3, 5);
            v2 = ov49_02258E60(v3, 6);
            v4 = ov49_02258E34(v3);
            v5 = ov42_02228270(v4, v2);
            v6 = ov49_022589C4(v10, (v4.unk_00) / 16, (v4.unk_02) / 16);
            v7 = ov49_022589C4(v10, (v5.unk_00) / 16, (v5.unk_02) / 16);

            {
                u32 v14;

                v14 = ov49_0225A4E0(param1);

                if (v14 != param2) {
                    if (ov49_0225A084(param1, v14)) {
                        ov49_0225A4D0(param1);
                    }
                }
            }

            if (v1 != 0) {
                if (ov49_0225A500(param1) == 1) {
                    UnkStruct_ov70_0225CC54 *v15;
                    u32 v16;

                    if ((v1 == 2) || (v1 == 3) || (v1 == 1)) {
                        v16 = ov49_0225A4E0(param1);

                        if (v16 != param2) {
                            v15 = ov49_02258D70(v8, v16);

                            if (v15) {
                                if (ov49_02258F3C(v15) == 0) {
                                    ov49_02258EEC(v8, v15, v0->unk_00);
                                }
                            }
                        }

                        ov49_0225A4D0(param1);
                    }
                }
            } else {
                ov49_0225A53C(param1, 0);

                if (ov49_02258A30(v6) == 1) {
                    ov49_0225F260(param0, param1, param2, ov49_02269D20, v6);
                    ov49_02258EEC(v8, v3, 0);
                    ov49_0225A03C(param1, v6);
                    ov49_0225A53C(param1, 1);
                    return 0;
                }

                if (ov49_0225A55C(param1) == 1) {
                    ov49_02258EEC(v8, v3, 0);
                    ov49_0225EFC4(v12, param2, &ov49_02269B80, 0L);
                    ov49_0225A53C(param1, 1);
                    return 0;
                }

                if (ov45_0222B00C(v11)) {
                    ov49_0225EFC4(v12, param2, &ov49_02269B70, 0L);
                    ov49_02258EEC(v8, v3, 0);
                    ov49_0225A53C(param1, 1);
                    return 0;
                }

                if (gSystem.newKeys & 0x0001) {
                    {
                        UnkStruct_ov70_0225CC54 *v17;
                        u32 v18;
                        BOOL v19;
                        u32 v20;

                        v17 = ov49_02258F40(v8, v3);

                        if (v17) {
                            v18 = ov49_02258E60(v17, 5);
                            v20 = ov49_02258E60(v17, 4);

                            if (v20 != 0xfe) {
                                v19 = ov49_0225A064(param1, v20);

                                if ((v18 == 0) && (v19 == 0)) {
                                    ov49_0225EFC4(v12, param2, &ov49_02269B60, 0L);
                                    ov49_02258EEC(v8, v3, 0);
                                    ov49_02258EEC(v8, v17, 0);
                                    ov49_0225A53C(param1, 1);
                                    return 0;
                                }
                            }
                        }
                    }

                    if (ov49_02258A90(v7) == 1) {
                        u32 v21;

                        v21 = ov49_02258E60(v3, 6);

                        if (v21 == 0) {
                            ov49_0225E58C(v9, (v5.unk_00) / 16, (v5.unk_02) / 16);
                            ov49_0225A03C(param1, v7);
                            ov49_0225A53C(param1, 1);

                            return 0;
                        }
                    }

                    if (ov49_02258A70(v7) == 1) {
                        ov49_0225F260(param0, param1, param2, ov49_02269C90, v7);
                        ov49_02258EEC(v8, v3, 0);
                        ov49_0225A03C(param1, v7);
                        ov49_0225A53C(param1, 1);

                        return 0;
                    }
                }

                if (ov49_0225F224(v2) == 1) {
                    if (v2 == 0) {
                        if (ov49_02258A50(v7) == 1) {
                            ov49_0225F260(param0, param1, param2, ov49_02269C60, v7);
                            ov49_02258EEC(v8, v3, 0);
                            ov49_0225A03C(param1, v7);
                            ov49_0225A53C(param1, 1);

                            return 0;
                        }
                    }

                    {
                        UnkStruct_ov70_0225CC54 *v22;
                        u32 v23;

                        v22 = ov49_02258F40(v8, v3);

                        if (v22) {
                            v23 = ov49_02258E60(v22, 4);

                            if (v23 != 0xfe) {
                                if (ov49_0225A4F0(param1) == 0) {
                                    if (ov49_0225A084(param1, v23) == 0) {
                                        ov49_0225A428(param1, v23, 1);
                                        v0->unk_00 = ov49_02258F3C(v22);
                                        ov49_02258EEC(v8, v22, 0);

                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }

                if (gSystem.newKeys & 0x0400) {
                    if (ov49_0225A4F0(param1) == 0) {
                        ov49_0225A428(param1, ov45_0222A53C(v11), 0);
                        PlaySE(1500);
                    } else {
                        u32 v25;

                        v25 = ov49_0225A4E0(param1);

                        if (v25 == param2) {
                            ov49_0225A4D0(param1);
                            PlaySE(1500);
                        }
                    }

                    return 0;
                }
            }
        }
        break;
    }

    return 0;
}
