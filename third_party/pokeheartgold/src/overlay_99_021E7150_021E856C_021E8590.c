#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_99_pokeathlon_event_record.h"

extern GraphicsModes ov99_021EA374;

typedef struct {
    BgTemplate t[6];
} Tmpl6;

extern Tmpl6 ov99_021EA4F4;

extern u8 ov99_021EA59C[];

void ov99_021E8590(u8 *param_1);

void ov99_021E8590(u8 *param_1) {
    BgConfig *pBVar1;
    NARC *pNVar3;
    int i;
    BgTemplate *pT;
    u8 *puVar8;
    Tmpl6 tt;
    GraphicsModes modes;
    pBVar1 = BgConfig_Alloc(*((enum HeapID *)&((PokeathlonEventRecord *)param_1)->heapId));
    ((PokeathlonEventRecord *)param_1)->bgConfig = pBVar1;
    modes = ov99_021EA374;
    SetBothScreensModesAndDisable(&modes);
    tt = ov99_021EA4F4;
    puVar8 = ov99_021EA59C;
    i = 0;
    pT = &tt.t[0];
    for (; i < 6; i++) {
        InitBgFromTemplate(((PokeathlonEventRecord *)param_1)->bgConfig, *puVar8, pT, 0);
        BgClearTilemapBufferAndCommit(((PokeathlonEventRecord *)param_1)->bgConfig, *puVar8);
        BG_ClearCharDataRange(*puVar8, 0x20, 0, *((enum HeapID *)&((PokeathlonEventRecord *)param_1)->heapId));
        pT++;
        puVar8++;
    }
    pNVar3 = NARC_New(NARC_a_1_7_7, *((enum HeapID *)&((PokeathlonEventRecord *)param_1)->heapId));
    ((PokeathlonEventRecord *)param_1)->narc = pNVar3;
}
