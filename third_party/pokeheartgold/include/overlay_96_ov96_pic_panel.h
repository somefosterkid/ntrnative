#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_PIC_PANEL_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_PIC_PANEL_H

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_96_ov96_sprite_mgr.h"

struct NNSG2dScreenData;
struct PokeathlonCourseData;

// One Pokemon of the panel (the 0x10 byte entries of the caller, three of them).
typedef struct Ov96PicPanelMon {
    u16 species;      // 0x0
    u16 form;         // 0x2
    u8 unk_4[2];      // 0x4
    u8 shiny;         // 0x6
    u8 gender;        // 0x7
    u8 unk_8[4];      // 0x8
    u32 personality;  // 0xC
} Ov96PicPanelMon;    // size 0x10

typedef struct Ov96PicPanelPic {
    void *charData;   // 0x0 sub_0201457C
    void *plttData;   // 0x4 sub_02014450
} Ov96PicPanelPic;    // size 0x8

// Ov96PicPanel: the 0x74 Pokemon picture panel of the Pokeathlon (overlay 96) result screen, allocated by
// ov96_02203A00 and freed by ov96_02203A30; stored at Ov96_02203468Work.unk_5E0.
typedef struct Ov96PicPanel {
    enum HeapID heapID;                    // 0x00
    struct PokeathlonCourseData *course;   // 0x04
    BgConfig *bgConfig;                    // 0x08
    Window window;                         // 0x0C player name window
    void *scrnBuf;                         // 0x1C
    struct NNSG2dScreenData *scrnData;     // 0x20 points into scrnBuf
    Ov96PicPanelPic pics[3];               // 0x24
    u32 charVramOffs[3];                   // 0x3C
    u32 plttVramOffs[3];                   // 0x48
    struct Ov96Obj *picObjs[3];                   // 0x54
    struct Ov96Obj *frameObjs[3];                 // 0x60
    u8 unk_6C[8];                          // 0x6C
} Ov96PicPanel; // size 0x74 on the DS

Ov96PicPanel *ov96_02203A00(enum HeapID heapID, BgConfig *bgConfig, struct PokeathlonCourseData *course);
void ov96_02203A30(Ov96PicPanel *panel);
void ov96_02203A64(Ov96PicPanel *panel, s32 arg1);
void ov96_02203B44(u32 param_1, Ov96SpriteMgr *mgr);
void ov96_02203B8C(Ov96PicPanel *panel, Ov96SpriteMgr *mgr, Ov96PicPanelMon *mons);
void ov96_02203BC0(Ov96PicPanel *panel, s32 index, s32 arg2);
void ov96_02203D74(Ov96PicPanel *panel, s32 arg1, s32 arg2);
void ov96_02203DCC(Ov96PicPanel *panel, Ov96PicPanelMon *mons);
void ov96_02203E30(Ov96SpriteMgr *mgr, Ov96PicPanel *panel, Ov96PicPanelMon *mons);
void ov96_02203F0C(Ov96PicPanel *panel);
void ov96_02203F50(Ov96SpriteMgr *mgr, Ov96PicPanel *panel);
void ov96_02203FBC(Ov96PicPanel *panel);
void ov96_02203FFC(Ov96PicPanel *panel);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_PIC_PANEL_H
