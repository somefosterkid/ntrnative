// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsiv_tool.h, the lines in use so far.
#ifndef BWPORT_APP_PMSIV_TOOL_H
#define BWPORT_APP_PMSIV_TOOL_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/clact.h"

// The phrase input's BG effects that the categories and the word window share: a BG scrolled over some frames, and
// an alpha blend or a brightness changed over some frames, each run by a V-blank task. The ROM doesn't name this file.
// The names are Black's dsd names; pokebw2's guesses are in the comments

// The direction of a scroll
#define PMSIV_TOOL_SCROLL_X 0
#define PMSIV_TOOL_SCROLL_Y 1

// The largest blend
#define PMSIV_BLEND_MAX 124

typedef struct {
    int plane1;
    int plane2;
    int ev;
    int evEnd;
    int evStep;
    int wait;
    int seq;
    TCB *tcb;
} PMSIVToolBlendWork;

typedef struct {
    u32 bg;
    u32 op;
    fx32 pos;
    fx32 endPos;
    fx32 step;
    u16 timer;
    u16 seq;
    TCB *tcb;
} PMSIVToolScrollWork;

// Scrolls a BG by vector pixels over wait frames, once func_ov110_021bc7dc is first called (PMSIVTool_SetupScrollWork)
void func_ov110_021bc78c(PMSIVToolScrollWork *wk, u32 bg, int direction, int vector, int wait);
BOOL func_ov110_021bc7dc(PMSIVToolScrollWork *wk); // PMSIVTool_WaitScroll
// PMSIVTool_SetupBlendWork
void func_ov110_021bc868(PMSIVToolBlendWork *wk, int plane1, int plane2, int start, int end, int wait);
BOOL func_ov110_021bc898(PMSIVToolBlendWork *wk); // PMSIVTool_WaitBlend

// pokebw2's PMSIVTool_SetupBrightWork
void func_ov110_021bc8fc(PMSIVToolBlendWork *wk, int plane, int start, int end, int wait);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIV_CURSOR_HIDE_POS
// Where a cursor goes to be out of sight. It sits between pmsiv_category.c's data and pmsiv_menu.c's, so it is
// placed here
extern const ClActorPos data_ov110_021c38d0;
// pokebw2's PMSIVTool_WaitBright
BOOL func_ov110_021bc93c(PMSIVToolBlendWork *wk);

#endif // BWPORT_APP_PMSIV_TOOL_H
