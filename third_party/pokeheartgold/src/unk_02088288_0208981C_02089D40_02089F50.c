#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "pokemon_summary_app.h"

void sub_02089F50(PokemonSummary *param_1, BgConfig *param_2, s32 param_3, u32 param_4);

void sub_02089F50(PokemonSummary *param_1, BgConfig *param_2, s32 param_3, u32 param_4) {
    void *pNscrFile;
    NNSG2dScreenData *pNStack_1c;

    pNscrFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_6_2, param_3, HEAP_ID_19);
    NNS_G2dGetUnpackedScreenData(pNscrFile, &pNStack_1c);
    LoadRectToBgTilemapRect(param_1->bgConfig, (u8)param_4, pNStack_1c->rawData, 0, 0, 0x20, 0x20);
    ScheduleBgTilemapBufferTransfer(param_2, (u8)param_4);
    Heap_Free(pNscrFile);
    return;
}
