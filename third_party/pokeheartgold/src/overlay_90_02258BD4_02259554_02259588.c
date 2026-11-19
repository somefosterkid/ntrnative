#include "global.h"

#include "game_stats.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_00_thumb.h"
#include "sound_02004A44.h"
#include "sys_task_api.h"

s32 ov90_0225888C(void *, u32);

s32 ov90_022588A4(void *, s32);

s32 ov90_02258DD0(void *, s32, enum HeapID);

s32 ov90_02258FF0(void *, s32 *, s32 *, s32, enum HeapID);

s32 ov90_022590CC(void *, s32, s32, s32, enum HeapID);

s32 ov90_02259184(void *, enum HeapID);

s32 ov90_0225938C(void *, void *, SaveData *, s32, enum HeapID);

s32 ov90_022596C8(void *, enum HeapID);

void ov90_02259794(SysTask *task, void *data);

void ov90_02259B18(SysTask *task, void *data);

s32 ov90_0225A204(void *, void *);

s32 ov90_0225C178(void *);

extern s32 ov90_0225C2A4;

extern GraphicsBanks ov90_0225C2CC;

extern s32 ov90_0225C39C;

typedef struct Ov90Args {
    SaveData *saveData;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C[4];
} Ov90Args;

typedef struct Ov90Work {
    u16 unk_00;
    u16 heapID;
    s16 unk_04;
    u8 unk_06[2];
    Ov90Args args;
    u8 unk_18[0x18];
    u8 unk_30[0xC];
    u8 unk_3C[0x10];
    u8 unk_4C[0x38];
    u8 unk_84[0x12C];
    u8 unk_1B0[0x43C];
    SysTask *unk_5EC;
    SysTask *unk_5F0;
} Ov90Work;

Ov90Work *ov90_02259588(const Ov90Args *args, s16 arg1, enum HeapID heapID);

Ov90Work *ov90_02259588(const Ov90Args *args, s16 arg1, enum HeapID heapID) {
    Ov90Work *work;

    Sound_SetSceneAndPlayBGM(19, 0x480, 0);

    work = Heap_Alloc(heapID, sizeof(Ov90Work));
    memset(work, 0, sizeof(Ov90Work));
    memcpy(&work->args, args, sizeof(Ov90Args));

    work->unk_04 = arg1;

    G2_BlendNone();
    G2S_BlendNone();
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);

    GfGfx_SetBanks(&ov90_0225C2CC);

    work->heapID = heapID;

    ov90_0225C178(work);
    ov90_0225A204(work->unk_18, &work->args);
    ov90_02258FF0(work->unk_30, &ov90_0225C2A4, &ov90_0225C39C, 5, heapID);
    ov90_022590CC(work->unk_84, 16, 1, 1, heapID);
    ov90_02258DD0(work->unk_1B0, 1, heapID);
    ov90_02259184(work->unk_3C, heapID);
    ov90_022596C8(work, heapID);

    {
        s32 v2;
        s32 v3;

        v3 = ov90_0225888C(&work->args, work->args.unk_09);
        v2 = ov90_022588A4(&work->args, v3);

        ov90_0225938C(work->unk_4C, work->unk_30, work->args.saveData, v2, heapID);
    }

    work->unk_5EC = SysTask_CreateOnMainQueue(ov90_02259794, work, 0);
    work->unk_5F0 = SysTask_CreateOnVWaitQueue(ov90_02259B18, work, 0);

    if (work->args.unk_0A) {
        ov00_021E69A8(heapID);
    }

    {
        GameStats *stats = Save_GameStats_Get(work->args.saveData);
        if (work->args.unk_0B) {
            GameStats_AddScore(stats, 0x27);
        }
    }

    return work;
}
