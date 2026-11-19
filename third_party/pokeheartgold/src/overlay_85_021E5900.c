#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "unk_struct_0209_bdf8.h"
#include "unk_struct_0209_c194.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "overlay_85_ov109_021D0F70.h"
#include "overlay_manager.h"
#include "options.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_85_ov109_021D5140.h"
#include "msgdata.h"
#include "screen_fade.h"
#include "screen_fade_internal.h"
#include "unk_02037C94.h"

struct OverlayManager;

struct JournalEntry;

struct NARC;

struct Party;

struct SaveData;

void *OverlayManager_CreateAndGetData(OverlayManager *appMan, u32 size, enum HeapID heapID);

void *OverlayManager_GetArgs(OverlayManager *appMan);

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

Party *SaveArray_Party_Get(SaveData *saveData);

typedef struct JournalEntry JournalEntry;


u16 sub_0203769C(void);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

u16 GF_GetCurrentPlayingBGM(void);

static inline u16 Sound_GetCurrentBGM(void) {
    return GF_GetCurrentPlayingBGM();
}

void EnableTouchPad(void);

BOOL InitializeTouchPad(u32 autoSamplingFrequency);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

void ov85_021E6764(void *param0);

void ov109_021D1C28(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E752C(UnkStruct_ov109_021D0F70 *param0);

void ov109_021D2AF0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7D08(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7E3C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7F74(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E80E0(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E82F8(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E83E0(UnkStruct_ov109_021D0F70 *param0);

extern const u16 ov85_021EA788[5 + 1][5];

int ov85_021E5900(OverlayManager *appMan, int *param1);

void ov85_021E678C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7650(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5900(OverlayManager *appMan, int *param1) {
    UnkStruct_ov109_021D0F70 *v0;
    UnkStruct_0209C194 *v1 = OverlayManager_GetArgs(appMan);

    sub_020398D4(1, 1);
    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    sub_0201A728(1 << 1);
    Heap_Create(HEAP_ID_3, HEAP_ID_102, 0x80000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov109_021D0F70), HEAP_ID_102);
    memset(v0, 0, sizeof(UnkStruct_ov109_021D0F70));

    v1->unk_34 = v0;
    v0->unk_CC = v1;
    v0->unk_D0 = v1->unk_30;
    v0->unk_24 = SaveArray_Party_Get(v0->unk_CC->unk_14.saveData);
    v0->unk_D80 = NARC_New((enum NarcId)187, HEAP_ID_102);

    GF_CreateVramTransferManager(8, HEAP_ID_102);
    EnableTouchPad();
    InitializeTouchPad(4);
    ov85_021E678C(v0);
    Main_SetVBlankIntrCB(ov85_021E6764, v0);
    ov85_021E752C(v0);
    ov85_021E7650(v0);

    {
        int v2 = 0, v3 = 0;
        int v4 = sub_0203769C();
        UnkStruct_ov109_021D2AE4 *v5 = &v0->unk_D4;

        do {
            if (v0->unk_CC->unk_0C & (1 << v2)) {
                if (v2 == v4) {
                    break;
                }

                v3++;
            }

            v2++;
        } while (v2 < 5);

        v5->unk_40 = (((fx32)0x0000000000001000L) * (ov85_021EA788[v0->unk_CC->unk_08][v3]));
    }

    v0->unk_1C = Sound_GetCurrentBGM();

    ov85_021E7D08(v0);
    ov85_021E7E3C(v0);
    ov85_021E7F74(v0);
    ov85_021E80E0(v0);
    ov85_021E82F8(v0);
    ov85_021E83E0(v0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 8, 1, HEAP_ID_102);

    return 1;
}
