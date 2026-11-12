#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_SIDEBAR_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_SIDEBAR_H

#include "types.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_res.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's sidebars (br_sidebar.c), the strips along the edges of the screens that move between its
// screens

BrSidebar *BrSidebar_Init(ClActUnit *unit, BrFade *fade, BrRes *res, HeapID heapId);
void BrSidebar_Exit(BrSidebar *p_wk, BrRes *res);
void BrSidebar_Main(BrSidebar *p_wk);
// Slide the sidebars in and sway them, or slide them back out
void BrSidebar_StartOpen(BrSidebar *p_wk);
void BrSidebar_StartBound(BrSidebar *p_wk);
void BrSidebar_StartClose(BrSidebar *p_wk);
// Puts the sidebars where the slide in ends
void BrSidebar_SetEndPos(BrSidebar *p_wk);
// Shows or hides the sidebars of a screen (CLACT_SURFACE_MAIN or CLACT_SURFACE_SUB)
void BrSidebar_SetVisible(BrSidebar *p_wk, u32 display, BOOL isVisible);

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_SIDEBAR_H
