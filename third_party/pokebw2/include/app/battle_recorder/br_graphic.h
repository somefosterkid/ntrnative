#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_GRAPHIC_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_GRAPHIC_H

#include "types.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's graphics system (br_graphic.c): the BG system, the cell actors and the VBlank task

// The VRAM setup: the Battle Recorder's, or the musical photos', which draw in 3D
enum {
    BR_GRAPHIC_TYPE_NORMAL,
    BR_GRAPHIC_TYPE_MUSICAL,
};

// The display mode with BG0 in 3D, which nothing reads
extern const BGSysLCDConfig BR_GRAPHIC_LCD_CONFIG_3D;

BrGraphic *BrGraphic_Init(u32 type, u32 displayLayout, HeapID heapId);
void BrGraphic_Exit(BrGraphic *p_wk);
void BrGraphic_Main(BrGraphic *p_wk);
ClActUnit *BrGraphic_GetClunit(const BrGraphic *p_wk);
// Releases the main screen's BGs to draw BG0 in 3D, and sets them up again
void BrGraphic_StartMain3D(BrGraphic *p_wk, HeapID heapId);
void BrGraphic_EndMain3D(BrGraphic *p_wk, HeapID heapId);

// Reconstructed, not known from the ROM. MWCC puts BR_GRAPHIC_LCD_CONFIG_3D in the section of br_graphic.c's other
// data, as the ROM has it, only when some code reads it before its definition. Nothing calls this
static inline void BrGraphic_SetLCDConfig3D(void) {
    GFL_BGSysSetLCDConfig(&BR_GRAPHIC_LCD_CONFIG_3D);
}

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_GRAPHIC_H
