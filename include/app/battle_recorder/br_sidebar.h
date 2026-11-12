// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_sidebar.h, the lines
// in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_SIDEBAR_H
#define BWPORT_APP_BATTLE_RECORDER_BR_SIDEBAR_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

// The Battle Recorder's sidebars (br_sidebar.c), the strips along the edges of the screens that move between its
// screens. pokebw2's guessed names are in the comments

// pokebw2's BrSidebar_SetVisible: shows or hides the sidebars of a screen (CLACT_SURFACE_MAIN or CLACT_SURFACE_SUB)
void func_ov186_021feec0(BrSidebar *p_wk, u32 display, BOOL isVisible);

// pokebw2's BrSidebar_StartOpen
// Slide the sidebars in and sway them, or slide them back out
void func_ov186_021fee24(BrSidebar *p_wk);
// pokebw2's BrSidebar_StartClose
void func_ov186_021fee74(BrSidebar *p_wk);
// pokebw2's BrSidebar_StartBound
void func_ov186_021fee4c(BrSidebar *p_wk);
void func_ov186_021fee9c(BrSidebar *p_wk);

// pokebw2's BrSidebar_Init
BrSidebar *func_ov186_021fed50(ClActUnit *unit, BrFade *fade, BrRes *res, HeapID heapId);
// pokebw2's BrSidebar_Exit
void func_ov186_021fedc4(BrSidebar *p_wk, BrRes *res);
// pokebw2's BrSidebar_Main
void func_ov186_021fee00(BrSidebar *p_wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrSidebar_SetEndPos
// Puts the sidebars where the slide in ends

#endif // BWPORT_APP_BATTLE_RECORDER_BR_SIDEBAR_H
