// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/box2_seq.h"
#include "app/box2_bgwfrm.h"
#include "app/box2_bmp.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "app/box2_ui.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "constants/pokemon.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/box.h"
#include "system/app_taskmenu.h"
#include "system/bgwinfrm.h"
#include "system/cursor_move.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wipe.h"

// The rodata of this file: pairs of a message ID and a flag, read by func_ov170_021eb988
static const u16 data_ov170_021ef8ec[] = { 0x53, 0x00, 0x50, 0x01 };
static const u16 data_ov170_021ef8f4[] = { 0x5d, 0x00, 0x50, 0x01 };
static const u16 data_ov170_021ef8fc[] = { 0x5e, 0x00, 0x5c, 0x00, 0x50, 0x01 };
static const u16 data_ov170_021ef908[] = { 0x1f, 0x00, 0x20, 0x00, 0x21, 0x00, 0x22, 0x01 };
static const u16 data_ov170_021ef9a0[] = {
#include "asset/box2_seq_data_ov170_021ef9a0.inc"
};

static const u16 data_ov170_021ef918[] = {
#include "asset/box2_seq_data_ov170_021ef918.inc"
};
static const u16 data_ov170_021ef928[] = {
#include "asset/box2_seq_data_ov170_021ef928.inc"
};
static const u16 data_ov170_021ef93c[] = {
#include "asset/box2_seq_data_ov170_021ef93c.inc"
};
static const u16 data_ov170_021ef950[] = {
#include "asset/box2_seq_data_ov170_021ef950.inc"
};
static const u16 data_ov170_021ef964[] = {
#include "asset/box2_seq_data_ov170_021ef964.inc"
};
static const u16 data_ov170_021ef978[] = {
#include "asset/box2_seq_data_ov170_021ef978.inc"
};
static const u16 data_ov170_021ef98c[] = {
#include "asset/box2_seq_data_ov170_021ef98c.inc"
};
static const u16 data_ov170_021ef9b4[] = { 0x4e, 0x00, 0x4a, 0x00, 0x4c, 0x00, 0x4d, 0x00, 0x50, 0x01 };
static const u16 data_ov170_021ef9c8[] = {
#include "asset/box2_seq_data_ov170_021ef9c8.inc"
};
static const u16 data_ov170_021ef9e0[] = {
#include "asset/box2_seq_data_ov170_021ef9e0.inc"
};
static const u16 data_ov170_021ef9f8[] = {
#include "asset/box2_seq_data_ov170_021ef9f8.inc"
};
static const u16 data_ov170_021efa10[] = {
#include "asset/box2_seq_data_ov170_021efa10.inc"
};


// The PC box's sequences: one function per state of the box, run each frame by Box2Seq_Main, each returning the
// next state. Names are ours

typedef int (*Box2SeqFunc)(Box2SysWork *syswk);

// What the yes/no menu's answers run, by the question asked
// Black 2's, not checked against Black
typedef struct {
    Box2SeqFunc yes;
    Box2SeqFunc no;
} Box2YesNoFuncs;

// The sub procs, with the state after each
// Black 2's, not checked against Black
typedef struct {
    Box2SeqFunc call;
    Box2SeqFunc exit;
    int nextSeq;
} Box2SubProc;

int func_ov170_021e2094(Box2SysWork *syswk);
int func_ov170_021e20d4(Box2SysWork *syswk);
int func_ov170_021e2140(Box2SysWork *syswk);
int func_ov170_021e2154(Box2SysWork *syswk);
int func_ov170_021e216c(Box2SysWork *syswk);
int func_ov170_021e9010(Box2SysWork *syswk);
int func_ov170_021e90d4(Box2SysWork *syswk);
int func_ov170_021e912c(Box2SysWork *syswk);
int func_ov170_021e91a0(Box2SysWork *syswk);
int func_ov170_021e9284(Box2SysWork *syswk);
int func_ov170_021e92a8(Box2SysWork *syswk);
int func_ov170_021e93f4(Box2SysWork *syswk);
int func_ov170_021e941c(Box2SysWork *syswk);
int func_ov170_021e946c(Box2SysWork *syswk);


int func_ov170_021e8fe0(Box2SysWork *syswk, int seq);
int func_ov170_021e8fe8(Box2SysWork *syswk, s16 flag);
int func_ov170_021e949c(Box2SysWork *syswk, u32 frame, int seq);
int func_ov170_021ea83c(Box2SysWork *syswk, u32 seq);

int func_ov170_021ea858(Box2SysWork *syswk, u32 seq);
u32 func_ov170_021ea8cc(Box2SysWork *syswk);

int func_ov170_021df348(Box2SysWork *syswk);
int func_ov170_021df378(Box2SysWork *syswk);
BOOL func_ov170_021df38c(Box2SysWork *syswk);
int func_ov170_021df404(Box2SysWork *syswk);

static const Box2SubProc data_ov170_021efa28[] = {
    { func_ov170_021df288, func_ov170_021df324, 81 },
    { func_ov170_021df348, func_ov170_021df378, 83 },
    { func_ov170_021df38c, func_ov170_021df404, 82 },
};

int func_ov170_021e1e80(Box2SysWork *syswk);
int func_ov170_021e1f90(Box2SysWork *syswk);
int func_ov170_021e2020(Box2SysWork *syswk);
int func_ov170_021e203c(Box2SysWork *syswk);
int func_ov170_021e2054(Box2SysWork *syswk);
int func_ov170_021e2070(Box2SysWork *syswk);
int func_ov170_021e21a0(Box2SysWork *syswk);
int func_ov170_021e22b8(Box2SysWork *syswk);
int func_ov170_021e22e0(Box2SysWork *syswk);
int func_ov170_021e2334(Box2SysWork *syswk);
int func_ov170_021e24ec(Box2SysWork *syswk);
int func_ov170_021e2648(Box2SysWork *syswk);
int func_ov170_021e2650(Box2SysWork *syswk);
int func_ov170_021e2b04(Box2SysWork *syswk);
int func_ov170_021e2b48(Box2SysWork *syswk);
int func_ov170_021e2b88(Box2SysWork *syswk);
int func_ov170_021e2cbc(Box2SysWork *syswk);
int func_ov170_021e3258(Box2SysWork *syswk);
int func_ov170_021e3340(Box2SysWork *syswk);
int func_ov170_021e3568(Box2SysWork *syswk);
int func_ov170_021e35d8(Box2SysWork *syswk);
int func_ov170_021e36a8(Box2SysWork *syswk);
int func_ov170_021e376c(Box2SysWork *syswk);
int func_ov170_021e3b50(Box2SysWork *syswk);
int func_ov170_021e3bf8(Box2SysWork *syswk);
int func_ov170_021e3c38(Box2SysWork *syswk);
int func_ov170_021e3c88(Box2SysWork *syswk);
int func_ov170_021e3e84(Box2SysWork *syswk);
int func_ov170_021e432c(Box2SysWork *syswk);
int func_ov170_021e4470(Box2SysWork *syswk);
int func_ov170_021e4504(Box2SysWork *syswk);
int func_ov170_021e4944(Box2SysWork *syswk);
int func_ov170_021e498c(Box2SysWork *syswk);
int func_ov170_021e49cc(Box2SysWork *syswk);
int func_ov170_021e4a64(Box2SysWork *syswk);
int func_ov170_021e4de8(Box2SysWork *syswk);
int func_ov170_021e4e6c(Box2SysWork *syswk);
int func_ov170_021e4eac(Box2SysWork *syswk);
int func_ov170_021e4ee8(Box2SysWork *syswk);
int func_ov170_021e4f60(Box2SysWork *syswk);
int func_ov170_021e5374(Box2SysWork *syswk);
int func_ov170_021e53b0(Box2SysWork *syswk);
int func_ov170_021e543c(Box2SysWork *syswk);
int func_ov170_021e5524(Box2SysWork *syswk);
int func_ov170_021e557c(Box2SysWork *syswk);
int func_ov170_021e58d8(Box2SysWork *syswk);
int func_ov170_021e5914(Box2SysWork *syswk);
int func_ov170_021e594c(Box2SysWork *syswk);
int func_ov170_021e5ad4(Box2SysWork *syswk);
int func_ov170_021e5c5c(Box2SysWork *syswk);
int func_ov170_021e5dd8(Box2SysWork *syswk);
int func_ov170_021e5e08(Box2SysWork *syswk);
int func_ov170_021e5e68(Box2SysWork *syswk);
int func_ov170_021e62bc(Box2SysWork *syswk);
int func_ov170_021e62fc(Box2SysWork *syswk);
int func_ov170_021e6374(Box2SysWork *syswk);
int func_ov170_021e63b0(Box2SysWork *syswk);
int func_ov170_021e6418(Box2SysWork *syswk);
int func_ov170_021e644c(Box2SysWork *syswk);
int func_ov170_021e65e4(Box2SysWork *syswk);
int func_ov170_021e695c(Box2SysWork *syswk);
int func_ov170_021e6a60(Box2SysWork *syswk);
int func_ov170_021e6c3c(Box2SysWork *syswk);
int func_ov170_021e6cd4(Box2SysWork *syswk);
int func_ov170_021e6d00(Box2SysWork *syswk);
int func_ov170_021e6ec0(Box2SysWork *syswk);
int func_ov170_021e7258(Box2SysWork *syswk);
int func_ov170_021e7298(Box2SysWork *syswk);
int func_ov170_021e73d8(Box2SysWork *syswk);
int func_ov170_021e754c(Box2SysWork *syswk);
int func_ov170_021e75e0(Box2SysWork * syswk);
int func_ov170_021e78f4(Box2SysWork *syswk);
int func_ov170_021e7904(Box2SysWork *syswk);
int func_ov170_021e79c8(Box2SysWork *syswk);
int func_ov170_021e7a04(Box2SysWork *syswk);
int func_ov170_021e7a10(Box2SysWork *syswk);
int func_ov170_021e7bd0(Box2SysWork *syswk);
int func_ov170_021e7c58(Box2SysWork *syswk);
int func_ov170_021e7d04(Box2SysWork *syswk);
int func_ov170_021e7d3c(Box2SysWork *syswk);
int func_ov170_021e7d64(Box2SysWork *syswk);
int func_ov170_021e7e54(Box2SysWork *syswk);
int func_ov170_021e7f6c(Box2SysWork *syswk);
int func_ov170_021e8014(Box2SysWork *syswk);
int func_ov170_021e8024(Box2SysWork *syswk);
int func_ov170_021e8188(Box2SysWork *syswk);
int func_ov170_021e81d0(Box2SysWork *syswk);
int func_ov170_021e8340(Box2SysWork *syswk);
int func_ov170_021e83c0(Box2SysWork *syswk);
int func_ov170_021e8508(Box2SysWork *syswk);
int func_ov170_021e85f0(Box2SysWork *syswk);
int func_ov170_021e86ec(Box2SysWork *syswk);
int func_ov170_021e87a0(Box2SysWork *syswk);
int func_ov170_021e8950(Box2SysWork *syswk);
int func_ov170_021e8a58(Box2SysWork *syswk);
int func_ov170_021e8a74(Box2SysWork *syswk);
int func_ov170_021e8bf8(Box2SysWork *syswk);
int func_ov170_021e8e24(Box2SysWork *syswk);
int func_ov170_021e8f14(Box2SysWork *syswk);
int func_ov170_021e8f20(Box2SysWork *syswk);

const Box2SeqFunc data_ov170_021efa7c[] = {
    func_ov170_021e1e80, func_ov170_021e1f90, func_ov170_021e2020, func_ov170_021e203c, func_ov170_021e2054,
    func_ov170_021e2070, func_ov170_021e2094, func_ov170_021e20d4, func_ov170_021e2140, func_ov170_021e2154,
    func_ov170_021e216c, func_ov170_021e21a0, func_ov170_021e22b8, func_ov170_021e22e0, func_ov170_021e2334,
    func_ov170_021e24ec, func_ov170_021e2648, func_ov170_021e2650, func_ov170_021e2b04, func_ov170_021e2b48,
    func_ov170_021e2b88, func_ov170_021e2cbc, func_ov170_021e3258, func_ov170_021e3340, func_ov170_021e3568,
    func_ov170_021e35d8, func_ov170_021e36a8, func_ov170_021e376c, func_ov170_021e3b50, func_ov170_021e3bf8,
    func_ov170_021e3c38, func_ov170_021e3c88, func_ov170_021e3e84, func_ov170_021e432c, func_ov170_021e4470,
    func_ov170_021e4504, func_ov170_021e4944, func_ov170_021e498c, func_ov170_021e49cc, func_ov170_021e4a64,
    func_ov170_021e4de8, func_ov170_021e4e6c, func_ov170_021e4eac, func_ov170_021e4ee8, func_ov170_021e4f60,
    func_ov170_021e5374, func_ov170_021e53b0, func_ov170_021e543c, func_ov170_021e5524, func_ov170_021e557c,
    func_ov170_021e58d8, func_ov170_021e5914, func_ov170_021e594c, func_ov170_021e5ad4, func_ov170_021e5c5c,
    func_ov170_021e5dd8, func_ov170_021e5e08, func_ov170_021e5e68, func_ov170_021e62bc, func_ov170_021e62fc,
    func_ov170_021e6374, func_ov170_021e63b0, func_ov170_021e6418, func_ov170_021e644c, func_ov170_021e65e4,
    func_ov170_021e695c, func_ov170_021e6a60, func_ov170_021e6c3c, func_ov170_021e6cd4, func_ov170_021e6d00,
    func_ov170_021e6ec0, func_ov170_021e7258, func_ov170_021e7298, func_ov170_021e73d8, func_ov170_021e754c,
    func_ov170_021e75e0, func_ov170_021e78f4, func_ov170_021e7904, func_ov170_021e79c8, func_ov170_021e7a04,
    func_ov170_021e7a10, func_ov170_021e7bd0, func_ov170_021e7c58, func_ov170_021e7d04, func_ov170_021e7d3c,
    func_ov170_021e7d64, func_ov170_021e7e54, func_ov170_021e7f6c, func_ov170_021e8014, func_ov170_021e8024,
    func_ov170_021e8188, func_ov170_021e81d0, func_ov170_021e8340, func_ov170_021e83c0, func_ov170_021e8508,
    func_ov170_021e85f0, func_ov170_021e86ec, func_ov170_021e87a0, func_ov170_021e8950, func_ov170_021e8a58,
    func_ov170_021e8a74, func_ov170_021e8bf8, func_ov170_021e8e24, func_ov170_021e8f14, func_ov170_021e8f20
};

// pokebw2's sYesNoFuncs
static const Box2YesNoFuncs data_ov170_021efa4c[] = {
    { func_ov170_021e9010, func_ov170_021e946c },
    { func_ov170_021e90d4, func_ov170_021e912c },
    { func_ov170_021e91a0, func_ov170_021e946c },
    { func_ov170_021e9284, func_ov170_021e92a8 },
    { func_ov170_021e92a8, func_ov170_021e9284 },
    { func_ov170_021e93f4, func_ov170_021e941c },
};

int func_ov170_021e95cc(Box2SysWork *syswk, u8 type);
void func_ov170_021ea728(Box2SysWork *syswk);
s32 func_ov170_021e152c(Box2SysWork *);
int func_ov170_021e8f60(Box2SysWork *syswk, u32 seq);
u32 func_ov170_021e8f50(Box2SysWork *syswk, u32 seq);
void func_ov170_021ea984(Box2SysWork *syswk);
int func_ov170_021e8fc0(Box2SysWork *syswk, Box2VFunc func, u32 val);
BOOL func_ov170_021e070c(Box2SysWork *syswk);
s32 func_ov170_021e0f4c(Box2SysWork *);
void func_ov170_021ea1e8(Box2SysWork *syswk);
int func_ov170_021e95f8(Box2SysWork *syswk);
int func_ov170_021e96e8(Box2SysWork *syswk);
int func_ov170_021e9848(Box2SysWork *syswk);
s32 func_ov170_021e0e50(Box2SysWork *);
BOOL func_ov170_021e01b0(Box2SysWork *syswk);
void func_ov170_021ee364(Box2SysWork *syswk, u32 id, u32 pos);
void func_ov170_021ea1c0(Box2SysWork *syswk);
s32 func_ov170_021dfa30(Box2SysWork *);
s32 func_ov170_021e1b1c(Box2SysWork *);
s32 func_ov170_021e0778(Box2SysWork *);
s32 func_ov170_021df484(Box2SysWork *);
s32 func_ov170_021e0200(Box2SysWork *);
s32 func_ov170_021e0470(Box2SysWork *);
int func_ov170_021e9968(Box2SysWork *syswk);
int func_ov170_021e9938(Box2SysWork *syswk);
int func_ov170_021e99b0(Box2SysWork *syswk);
int func_ov170_021e9b74(Box2SysWork *syswk);
void func_ov170_021ea750(Box2SysWork *syswk);
void func_ov170_021ee754(Box2SysWork *syswk, u32 pos);
void func_ov170_021ea75c(Box2SysWork *syswk);
s32 func_ov170_021e1324(Box2SysWork *);
s32 func_ov170_021e0a40(Box2SysWork *);
int func_ov170_021e8fd0(Box2SysWork *syswk, u32 val);
void func_ov170_021ea770(Box2SysWork *syswk);
void func_ov170_021e072c(Box2SysWork *);
void func_ov170_021ea908(Box2SysWork *syswk, u32 bit);
int func_ov170_021e94ac(Box2SysWork *syswk, u32 button, int seq);
s32 func_ov170_021e002c(Box2SysWork *);
s32 func_ov170_021e087c(Box2SysWork *);
s32 func_ov170_021e1458(Box2SysWork *);
void func_ov170_021eaac4(Box2SysWork *);
s32 func_ov170_021e0fd8(Box2SysWork *);
s32 func_ov170_021e0abc(Box2SysWork *);
s32 func_ov170_021e0068(Box2SysWork *);
s32 func_ov170_021e1628(Box2SysWork *);
void func_ov170_021ee520(Box2SysWork *syswk, u32 pos);
int func_ov170_021e9c94(Box2SysWork *syswk, u32 a, u32 b);
int func_ov170_021e9ce0(Box2SysWork *syswk, u32 a, u32 b);
int func_ov170_021e954c(Box2SysWork *syswk, u32 id, u32 pal, int seq);
s32 func_ov170_021e1988(Box2SysWork *);
s32 func_ov170_021e122c(Box2SysWork *);
s32 func_ov170_021e09e0(Box2SysWork *);
s32 func_ov170_021e0ed4(Box2SysWork *);
int func_ov170_021ea458(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ea9f4(Box2SysWork *syswk, u32 seq);
int func_ov170_021ea1f4(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021eea48(Box2SysWork *syswk);
u32 func_ov170_021eaa3c(Box2SysWork *syswk, u32 seq);
int func_ov170_021ea2dc(Box2SysWork *syswk, u32 pos);
void func_ov170_021eaa80(Box2SysWork *syswk);
void func_ov170_021eaca8(Box2SysWork *syswk);
void func_ov170_021eacfc(Box2SysWork *syswk);
void func_ov170_021ee2c4(Box2SysWork *syswk);
void func_ov170_021ea784(Box2SysWork *syswk);
int func_ov170_021ea114(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ef1fc(u32 *param_1, u32 *param_2);
s32 func_ov170_021e1a78(Box2SysWork *);
s32 func_ov170_021e1acc(Box2SysWork *);
BOOL func_ov170_021ef1d4(u32 *x, u32 *y);
void func_ov170_021ea808(Box2SysWork *syswk);
int func_ov170_021ea798(Box2SysWork *syswk, u32 mode);
int func_ov170_021ea928(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ef194(void);
int func_ov170_021e9ec0(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee8a8(Box2SysWork *);
int func_ov170_021ea020(Box2SysWork *syswk, u32 pos, u32 val);
int func_ov170_021ea0bc(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021eeea4(Box2SysWork *);
u32 func_ov170_021ef1a4(void);
int func_ov170_021ea39c(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee96c(Box2SysWork *syswk);
int func_ov170_021ea514(Box2SysWork *syswk, u32 pos);
int func_ov170_021e9fa8(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee474(Box2SysWork *syswk);
int func_ov170_021ea598(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021eeb40(Box2SysWork *syswk);
int func_ov170_021ea66c(Box2SysWork *syswk, u32 pos);
int func_ov170_021e9f3c(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021eede0(Box2SysWork *);
int func_ov170_021e9df8(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee57c(Box2SysWork *syswk);
int func_ov170_021e9e7c(Box2SysWork *syswk, u32 pos, u32 val);
int func_ov170_021e9d98(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021eece4(Box2SysWork *);
int func_ov170_021ea064(Box2SysWork *syswk, u32 pos, u32 val);
int func_ov170_021ea14c(Box2SysWork *syswk, u32 pos, int seq);
int func_ov170_021e9d2c(Box2SysWork *syswk, u32 pos);
u32 func_ov170_021ee65c(Box2SysWork *);
u32 func_ov170_021ee850(Box2SysWork *);


// pokebw2's Box2Seq_TrgWait


int func_ov170_021e1e80(Box2SysWork *syswk) {
    GFL_OvlLoad(OVERLAY_APP_UI);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(0);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BOX2_APP, 0x80000);
    syswk->app = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2AppWork), TRUE, "box2_seq.c", 0x2b5);
    syswk->app->pokeIconArc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, HEAPID_BOX2_APP);
    func_02036370(&syswk->app->keyRepeatWait, &syswk->app->keyRepeatStart);
    setKeypressFramecounts(6, 6);
    func_ov170_021dc9c4();
    func_ov170_021dc9e0(syswk);
    func_ov170_021dcdb8(syswk);
    func_ov170_021dcb8c(syswk);
    func_ov170_021dce14(syswk);
    func_ov170_021dd8d0(syswk);
    func_ov170_021eaa80(syswk);
    func_ov170_021ebd94(syswk);
    func_ov170_021ec4e0(syswk);
    func_ov170_021ec4fc(syswk);
    func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), BOX2_TRAY_SCROLL_NONE);
    func_ov170_021ef274(syswk);
    func_ov170_021eaca8(syswk);
    func_ov170_021eacfc(syswk);
    func_ov170_021edb1c(syswk);
    func_ov170_021ee2c4(syswk);
    func_ov170_021dcec0(syswk);
    func_ov170_021dcdf0(TRUE);
    func_0203d34c(TRUE, HEAPID_BOX2_APP);
    func_ov170_021dc94c(syswk);
    return syswk->nextSeq;
}

int func_ov170_021e1f90(Box2SysWork *syswk) {
    func_ov170_021dc964(syswk);
    func_ov170_021dcf30(syswk);
    func_ov170_021ee3cc(syswk);
    func_ov170_021ef310(syswk->app);
    func_ov170_021ebdd8(syswk->app);
    func_ov170_021eaac4(syswk);
    func_ov170_021dd8f8(syswk);
    func_ov170_021dce80(syswk);
    func_ov170_021dcdd8(syswk);
    func_ov170_021dcb48(syswk);
    setKeypressFramecounts(syswk->app->keyRepeatWait, syswk->app->keyRepeatStart);
    GFL_ArcToolFree(syswk->app->pokeIconArc);
    GFL_HeapFree(syswk->app);
    GFL_HeapDelete(HEAPID_BOX2_APP);
    syswk->app = NULL;
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_OvlUnload(OVERLAY_APP_UI);
    return syswk->nextSeq;
}

int func_ov170_021e2020(Box2SysWork *syswk) {
    if (GFL_WipeIsFinished() == TRUE) {
        return syswk->app->oldCurPos;
    }
    return 2;
}

int func_ov170_021e203c(Box2SysWork *syswk) {
    if (func_02021ed8(syswk->app->palFade) == 0) {
        return syswk->nextSeq;
    }
    return 3;
}

int func_ov170_021e2054(Box2SysWork *syswk) {
    if (syswk->app->unkA18C == 0) {
        return syswk->nextSeq;
    }
    syswk->app->unkA18C--;
    return 4;
}

int func_ov170_021e2070(Box2SysWork *syswk) {
    if (syswk->app->vfunk.func != NULL) {
        if (syswk->app->vfunk.func(syswk) == 0) {
            syswk->app->vfunk.func = NULL;
            return syswk->app->unk68;
        }
    }
    return 5;
}


int func_ov170_021e2094(Box2SysWork *syswk) {
    if (func_02035c60() == TRUE) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_020355d8(TRUE);
        return syswk->nextSeq;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_020355d8(FALSE);
        return syswk->nextSeq;
    }
    return BOX2SEQ_TRGWAIT;
}

// pokebw2's Box2Seq_YesNo
int func_ov170_021e20d4(Box2SysWork *syswk) {
    u32 sel;

    func_02027974(syswk->app->yesNoMenu);
    if (func_020279e8(syswk->app->yesNoMenu) == TRUE) {
        sel = func_02027a04(syswk->app->yesNoMenu);
        func_02027858(syswk->app->yesNoMenu);
        setKeypressFramecounts(6, 6);
        if (sel == 0) {
            return data_ov170_021efa4c[syswk->app->ynID].yes(syswk);
        }
        return data_ov170_021efa4c[syswk->app->ynID].no(syswk);
    }
    return BOX2SEQ_YESNO;
}

// pokebw2's Box2Seq_ButtonAnm
int func_ov170_021e2140(Box2SysWork *syswk) {
    if (func_ov170_021dcfc0(syswk) == FALSE) {
        return syswk->nextSeq;
    }
    return BOX2SEQ_BUTTON_ANM;
}

// pokebw2's Box2Seq_SubProcCall
int func_ov170_021e2154(Box2SysWork *syswk) {
    data_ov170_021efa28[syswk->subProcType].call(syswk);
    return BOX2SEQ_SUBPROC_MAIN;
}

// pokebw2's Box2Seq_SubProcMain
int func_ov170_021e216c(Box2SysWork *syswk) {
    if (syswk->procMgrResult != TRUE) {
        data_ov170_021efa28[syswk->subProcType].exit(syswk);
        syswk->nextSeq = data_ov170_021efa28[syswk->subProcType].nextSeq;
        return BOX2SEQ_INIT;
    }
    return BOX2SEQ_SUBPROC_MAIN;
}

int func_ov170_021e21a0(Box2SysWork *syswk) {
    GFL_SndSEPlay(SEQ_SE_PC_LOGIN);
    switch (syswk->param->mode) {
    case 0:
        func_ov170_021ea784(syswk);
        func_ov170_021ed888(syswk->app, FALSE);
        func_ov170_021ecb20(syswk, 1, TRUE);
        func_ov170_021defec(syswk, BOX2_PARTY_POS);
        syswk->app->unkA198 = 0x31;
        break;
    case 1:
        func_ov170_021defec(syswk, 0);
        syswk->app->unkA198 = 0x2c;
        break;
    case 2:
        func_ov170_021defec(syswk, 0);
        func_ov170_021ef670(syswk->app);
        syswk->app->unkA198 = 0x11;
        break;
    case 3:
        func_ov170_021ecb20(syswk, 0x81, TRUE);
        func_ov170_021defec(syswk, 0);
        func_ov170_021ef670(syswk->app);
        syswk->app->unkA198 = 0x39;
        break;
    case 4:
        if (PokeParty_GetPkmCount(syswk->param->party) == 0) {
            func_ov170_021ef670(syswk->app);
            func_ov170_021defec(syswk, 0);
            func_ov170_021ee364(syswk, 0xd, 0);
            syswk->app->unkA198 = 0x23;
        } else {
            func_ov170_021ea784(syswk);
            func_ov170_021ed888(syswk->app, FALSE);
            func_ov170_021ecb20(syswk, 1, TRUE);
            func_ov170_021ef68c(syswk->app);
            func_ov170_021defec(syswk, BOX2_PARTY_POS);
            func_ov170_021ee364(syswk, 0xe, 0);
            syswk->app->unkA198 = 0x27;
        }
        break;
    case 5:
        func_ov170_021defec(syswk, 0);
        func_ov170_021ecb20(syswk, 0x81, TRUE);
        func_ov170_021ee038(syswk, 0, 1, 1);
        syswk->app->unkA198 = 0x4b;
        break;
    }
    return func_ov170_021e22b8(syswk);
}


int func_ov170_021e22b8(Box2SysWork *syswk) {
    if (func_0201c6b4(syswk->app->printQueue) == 0) {
        return 0xc;
    }
    return func_ov170_021e8f60(syswk, syswk->app->unkA198);
}

int func_ov170_021e22e0(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->vfunk.work);
    if (syswk->app->unkA172 == 1) {
        if (syswk->param->mode == 3) {
            if (syswk->app->itemId == 0) {
                func_ov170_021ebcb4(syswk, 0, 0x18);
            } else {
                func_ov170_021ebcb4(syswk, 1, 0x18);
            }
        } else {
            func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        }
        syswk->app->unkA172 = 0;
    }
    return syswk->nextSeq;
}

int func_ov170_021e2334(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        switch (syswk->param->mode) {
        case 1:
            func_ov170_021eb988(syswk, data_ov170_021ef9b4, 5);
            break;
        case 0:
            func_ov170_021eb988(syswk, data_ov170_021ef950, 5);
            break;
        case 2:
            func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
            break;
        case 3:
            func_ov170_021ea878(syswk, syswk->app->itemId);
            break;
        case 4:
            func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
            break;
        case 5:
            func_ov170_021eba5c(syswk->app, 0x1b);
            func_ov170_021eb988(syswk, data_ov170_021ef8ec, 2);
            break;
        }
        if (syswk->app->unkA170[0] == 1) {
            syswk->app->unkA170[0] = 0;
            if (syswk->param->mode == 3) {
                if (syswk->pos >= BOX2_PARTY_POS) {
                    func_ov170_021ecb20(syswk, 0x82, FALSE);
                } else {
                    func_ov170_021ed888(syswk->app, TRUE);
                    func_ov170_021ecb20(syswk, 0x81, FALSE);
                }
            } else if (syswk->pos >= BOX2_PARTY_POS) {
                func_ov170_021ecb20(syswk, 2, FALSE);
            } else {
                func_ov170_021ed888(syswk->app, TRUE);
                func_ov170_021ecb20(syswk, 1, FALSE);
            }
        }
        func_ov170_021ef534(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0xe);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed55c(syswk, TRUE);
        func_ov170_021ed508(syswk, 0xe, TRUE);
        switch (syswk->param->mode) {
        case 1:
            func_ov170_021ebc58(syswk, syswk->pos, TRUE, 0x18);
            return 0x2c;
        case 0:
            func_ov170_021ebc58(syswk, syswk->pos, TRUE, 0x18);
            return 0x31;
        case 2:
            func_ov170_021ebc58(syswk, syswk->pos, TRUE, 0x18);
            if (syswk->pos < BOX2_PARTY_POS) {
                return 0x11;
            }
            return 0x1b;
        case 3:
            if (syswk->app->itemId == 0) {
                func_ov170_021ebcb4(syswk, FALSE, 0x18);
            } else {
                func_ov170_021ebcb4(syswk, TRUE, 0x18);
            }
            if (syswk->pos < BOX2_PARTY_POS) {
                return 0x39;
            }
            return 0x46;
        case 4:
            func_ov170_021ebc58(syswk, syswk->pos, TRUE, 0x18);
            if (syswk->pos < BOX2_PARTY_POS) {
                return 0x23;
            }
            return 0x27;
        case 5:
            func_ov170_021ebc58(syswk, syswk->pos, TRUE, 0x18);
            return 0x4b;
        }
        break;
    }
    return 0xe;
}


int func_ov170_021e24ec(Box2SysWork *syswk) {
    u32 x;
    u32 y;
    Box2BoxListDrag *work;
    u32 oldY;
    int diff;

    if (func_ov170_021ef1fc(&x, &y) == FALSE) {
        GFL_HeapFree(syswk->app->unkA190);
        return syswk->nextSeq;
    }
    work = (Box2BoxListDrag *)syswk->app->unkA190;
    oldY = syswk->app->tpy;
    syswk->app->tpy = y;
    diff = oldY - y;
    if (MATH_ABS(diff) >= 3) {
        work->cnt = MATH_ABS(diff) / 8;
        if (y < oldY) {
            work->dir = 1;
            func_ov170_021dd1c4(syswk, 1);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (syswk->nextSeq == 21) {
                return func_ov170_021e8fc0(syswk, func_ov170_021e19d4, 15);
            }
            return func_ov170_021e8fc0(syswk, func_ov170_021e1a78, 15);
        }
        if (y > oldY) {
            work->dir = -1;
            func_ov170_021dd1c4(syswk, -1);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (syswk->nextSeq == 21) {
                return func_ov170_021e8fc0(syswk, func_ov170_021e1a28, 15);
            }
            return func_ov170_021e8fc0(syswk, func_ov170_021e1acc, 15);
        }
    }
    if (work->cnt != 0) {
        work->cnt--;
        if (work->dir == 1) {
            func_ov170_021dd1c4(syswk, 1);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (syswk->nextSeq == 21) {
                return func_ov170_021e8fc0(syswk, func_ov170_021e19d4, 15);
            }
            return func_ov170_021e8fc0(syswk, func_ov170_021e1a78, 15);
        }
        if (work->dir == -1) {
            func_ov170_021dd1c4(syswk, -1);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            if (syswk->nextSeq == 21) {
                return func_ov170_021e8fc0(syswk, func_ov170_021e1a28, 15);
            }
            return func_ov170_021e8fc0(syswk, func_ov170_021e1acc, 15);
        }
    }
    return 15;
}


int func_ov170_021e2648(Box2SysWork *syswk) {
    return func_ov170_021e9284(syswk);
}

int func_ov170_021e2650(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        syswk->unk1B = 0;
        return func_ov170_021ea9f4(syswk, 0x14);
    }
    res = func_ov170_021ef170();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
            }
            func_ov170_021defec(syswk, res);
            return func_ov170_021e9d2c(syswk, res);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef670(syswk->app);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x11);
        }
        return 0x11;
    }

    res = func_ov170_021ee65c(syswk);
    switch (res) {
    case 30:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9c94(syswk, 1, 0x11);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9ce0(syswk, 1, 0x11);
    case 33:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 10, func_ov170_021e8f50(syswk, 0x1a));
    case 34:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 35:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 36:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk1B = 0;
        syswk->moveMode = 0;
        return func_ov170_021e949c(syswk, 0, func_ov170_021e8f50(syswk, 0x14));
    case 37:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 0;
        syswk->getTray = syswk->tray;
        syswk->curRcvPos = 0x25;
        return func_ov170_021e949c(syswk, 1, 0x4f);
    case 38:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 0;
        syswk->curRcvPos = 0x26;
        return func_ov170_021e949c(syswk, 2, func_ov170_021e8f50(syswk, 0x50));
    case 39:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 3, func_ov170_021e8f50(syswk, 0x55));
    case 40:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x59));
    case 41:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x13);
    case 42:
        syswk->unk1B = 0;
        return func_ov170_021eaa3c(syswk, 0x14);
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9c94(syswk, 1, 0x11);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9ce0(syswk, 1, 0x11);
        }
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < BOX2_PARTY_POS) {
            func_ov170_021defec(syswk, cur);
        } else if (cur != 36 && cur != 37 && cur != 38 && cur != 39 && cur != 40 && cur != 41) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x11));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x13);
        }
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    default:
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
        func_ov170_021defec(syswk, res);
        func_ov170_021eef74(syswk->app, 0x24, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x24);
        return func_ov170_021e9e7c(syswk, res, 0x11);
    }
    return 0x11;
}


int func_ov170_021e2b04(Box2SysWork *syswk) {
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_020260e4(syswk->app->cursorMove, 0x24);
    func_ov170_021ee3e4(syswk, 0x24);
    func_ov170_021ed55c(syswk, 1);
    func_ov170_021ee038(syswk, 0, 0, 1);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    return 0x11;
}


int func_ov170_021e2b48(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef670(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x11));
}

int func_ov170_021e2b88(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        syswk->trayScroll = syswk->tray;
        if (syswk->moveMode == 1) {
            u8 pos;

            syswk->getTray = BOX2_GET_NONE;
            pos = func_020260e0(syswk->app->cursorMove);
            syswk->pos = pos;
            if (syswk->pos >= BOX2_PARTY_POS) {
                syswk->pos = 0;
                func_ov170_021defec(syswk, syswk->pos);
            }
        } else {
            syswk->getTray = syswk->tray;
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ecb84(syswk->app, syswk->pos);
        }
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x14);
    case 1:
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        syswk->app->unkA194++;
        return func_ov170_021ea83c(syswk, 0x14);
    case 2:
        func_ov170_021ee364(syswk, 4, syswk->pos);
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ee038(syswk, 0, 1, 0);
        syswk->app->curPos = syswk->pos;
        syswk->app->unkA194 = 0;
        if (syswk->moveMode == 1) {
            if (func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
                func_ov170_021ee038(syswk, 0, 1, 2);
            }
            func_ov170_021ed508(syswk, 0xd, TRUE);
            return 0x15;
        }
        if (syswk->moveMode == 2) {
            func_ov170_021ed508(syswk, 0xd, TRUE);
        } else {
            func_ov170_021ed508(syswk, 0xe, TRUE);
        }
        return func_ov170_021ea114(syswk, syswk->pos);
    }
    return 0x14;
}

int func_ov170_021e2cbc(Box2SysWork *syswk) {
    u32 x;
    u32 y;
    u32 res;
    u8 cur;

    if (syswk->unk1C_4 == 1) {
        syswk->moveMode = 0;
        syswk->unk1C_4 = 0;
        return func_ov170_021e8f50(syswk, 0x16);
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && syswk->moveMode != 0 && syswk->unk18 == 0) {
        func_ov170_021dd238(syswk);
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x16);
    }
    if (syswk->unk18 == 0) {
        res = func_ov170_021ef170();
        if (res != CURSORMOVE_NONE) {
            func_020260f4(syswk->app->cursorMove, FALSE);
            func_ov170_021ee008(syswk, 0, FALSE);
            if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                GFL_SndSEPlay(SEQ_SE_SYS_39);
                func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
                func_ov170_021defec(syswk, res);
                func_020260e4(syswk->app->cursorMove, res);
                return func_ov170_021ea064(syswk, res, 0x15);
            }
            func_020260e4(syswk->app->cursorMove, res);
            func_ov170_021ee754(syswk, res);
            syswk->app->curPos = res;
            func_ov170_021ee3e4(syswk, res);
            func_ov170_021df040(syswk);
            return 0x15;
        }
    }
    if (func_ov170_021ef1d4(&x, &y) == TRUE) {
        syswk->app->tpy = y;
        syswk->nextSeq = 0x15;
        func_ov170_021ea808(syswk);
        return 0xf;
    }
    res = func_ov170_021ee850(syswk);
    switch (res) {
    case 30:
        func_ov170_021ee008(syswk, 0, FALSE);
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021ee008(syswk, 0, FALSE);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        func_ov170_021ee754(syswk, BOX2_PARTY_POS);
        syswk->app->curPos = BOX2_PARTY_POS;
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9c94(syswk, 0, 0x15);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021ee008(syswk, 0, FALSE);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        func_ov170_021ee754(syswk, BOX2_PARTY_POS);
        syswk->app->curPos = BOX2_PARTY_POS;
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9ce0(syswk, 0, 0x15);
    case 39:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->unk18 == 0) {
            func_ov170_021df040(syswk);
        }
        return func_ov170_021e949c(syswk, 10, func_ov170_021e8f50(syswk, 0x19));
    case 40:
        if (syswk->unk18 == 0) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur >= BOX2_PARTY_POS) {
                break;
            }
            if (func_ov170_021dd25c(syswk, cur, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
                break;
            }
            syswk->pos = cur;
            syswk->getTray = syswk->tray;
            syswk->unk13 = 2;
        } else {
            syswk->unk13 = 3;
            syswk->unk1D = func_020260e0(syswk->app->cursorMove);
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e954c(syswk, 8, 1, 0x4f);
    case 41:
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x16));
    case 42:
        if (syswk->moveMode == 0) {
            break;
        }
        if (syswk->unk18 != 0) {
            break;
        }
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x16);
    case CURSORMOVE_CANCEL:
        if (syswk->unk18 == 1) {
            return func_ov170_021ea14c(syswk, 0xff, 0x15);
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x16));
    case CURSORMOVE_UNK_8:
        if (func_020260e0(syswk->app->cursorMove) != 0x22) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, -1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e1a28, 0x15);
    case CURSORMOVE_UNK_7:
        if (func_020260e0(syswk->app->cursorMove) != 0x25) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, 1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e19d4, 0x15);
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) != BOX2_PARTY_POS) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e9c94(syswk, 0, 0x15);
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) != BOX2_PARTY_POS) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e9ce0(syswk, 0, 0x15);
    case CURSORMOVE_CURSOR_MOVE:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        cur = func_020260e0(syswk->app->cursorMove);
        if (syswk->unk18 == 0) {
            if (cur < BOX2_PARTY_POS) {
                func_ov170_021defec(syswk, cur);
            } else {
                func_ov170_021df040(syswk);
            }
        }
        if (cur >= 0x22 && cur <= 0x25) {
            func_ov170_021ee008(syswk, cur - 0x22, TRUE);
        } else {
            func_ov170_021ee008(syswk, 0, FALSE);
        }
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x15));
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case 33:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, -1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e1a28, 0x15);
    case 38:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, 1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e19d4, 0x15);
    case 34:
    case 35:
    case 36:
    case 37:
        func_ov170_021ee008(syswk, res - 0x22, TRUE);
        if (syswk->unk18 == 1) {
            return func_ov170_021ea14c(syswk, res + 2, 0x15);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df040(syswk);
        syswk->app->unkA17F = syswk->trayScroll + res - 0x22;
        if (syswk->app->unkA17F >= syswk->trayMax) {
            syswk->app->unkA17F -= syswk->trayMax;
        }
        if (syswk->app->unkA17F == syswk->tray) {
            break;
        }
        return func_ov170_021ea798(syswk, 0);
    case CURSORMOVE_NONE:
        break;
    default:
        func_ov170_021ee008(syswk, 0, FALSE);
        if (syswk->unk18 == 1) {
            return func_ov170_021ea14c(syswk, res, 0x15);
        }
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        syswk->pos = res;
        syswk->getTray = syswk->tray;
        func_ov170_021ecb84(syswk->app, syswk->pos);
        return func_ov170_021ea114(syswk, syswk->pos);
    }
    return 0x15;
}


int func_ov170_021e3258(Box2SysWork *syswk) {
    u32 pos;
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021ee008(syswk, 0, 0);
        func_ov170_021ee038(syswk, 0, 0, 1);
        if (func_020260e0(syswk->app->cursorMove) >= 0x1e) {
            func_ov170_021df040(syswk);
        }
        syswk->app->unkA194++;
        GFL_SndSEPlay(0x669);
        return func_ov170_021ea858(syswk, 0x16);
    case 1:
        if (syswk->unk1B == 1) {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            syswk->app->unkA194++;
            return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x16);
        }
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= 0x1e) {
            pos = 0;
        }
        func_ov170_021ee364(syswk, 3, pos);
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ed508(syswk, 0xe, 1);
        func_ov170_021defec(syswk, pos);
        syswk->app->unkA194 = 0;
        return 0x11;
    case 2:
        func_ov170_021ef68c(syswk->app);
        syswk->app->unkA194 = 0;
        return 0x21;
    }
    return 0x16;
}

int func_ov170_021e3340(Box2SysWork *syswk) {
    u8 pos;
    u8 n;

    func_ov170_021dd530(syswk);
    syswk->unk18 = 0;
    syswk->getTray = BOX2_GET_NONE;
    if (syswk->app->moveErr == BOX2_MOVE_ERR_NONE) {
        pos = func_020260e0(syswk->app->cursorMove);
    } else {
        func_020260e4(syswk->app->cursorMove, syswk->pos);
        func_ov170_021ee3e4(syswk, syswk->pos);
        func_ov170_021ed55c(syswk, TRUE);
        pos = syswk->pos;
    }
    syswk->app->curPos = pos;
    func_ov170_021ea1e8(syswk);
    if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
        if (syswk->nextSeq == 0x20) {
            func_ov170_021ee364(syswk, 4, pos);
            func_ov170_021defec(syswk, pos);
            syswk->nextSeq = 0x15;
            if (func_ov170_021dd25c(syswk, pos, syswk->tray, 0xa9, NULL) != 0) {
                n = 0;
            } else {
                n = 2;
            }
        } else if (pos < BOX2_PARTY_POS && func_ov170_021dd25c(syswk, pos, syswk->tray, 0xa9, NULL) != 0) {
            func_ov170_021defec(syswk, pos);
            n = 0;
        } else {
            func_ov170_021df040(syswk);
            n = 2;
        }
    } else if (func_ov170_021ef45c(syswk->app->bgWinFrame) == TRUE) {
        if (syswk->nextSeq == 0x15) {
            pos = func_020260e0(syswk->app->cursorMove);
            if (pos > BOX2_BOXLIST_POS - 4) {
                pos = syswk->pos;
            }
            func_ov170_021ee364(syswk, 6, pos);
            func_ov170_021defec(syswk, pos);
            syswk->nextSeq = 0x20;
            if (func_ov170_021dd25c(syswk, pos, syswk->tray, 0xa9, NULL) != 0) {
                n = 0;
            } else {
                n = 2;
            }
        } else if (pos < BOX2_BOXLIST_POS && func_ov170_021dd25c(syswk, pos, syswk->tray, 0xa9, NULL) != 0) {
            func_ov170_021defec(syswk, pos);
            n = 0;
        } else {
            func_ov170_021df040(syswk);
            n = 2;
        }
    } else if (pos < BOX2_BOXLIST_POS && func_ov170_021dd25c(syswk, pos, syswk->tray, 0xa9, NULL) != 0) {
        func_ov170_021defec(syswk, pos);
        n = 0;
    } else {
        func_ov170_021df040(syswk);
        n = 2;
    }
    if (syswk->moveMode == 0) {
        syswk->unk1C_4 = 1;
        n = syswk->unk21;
    }
    func_ov170_021ee038(syswk, 0, 1, n);
    switch (syswk->app->moveErr) {
    case BOX2_MOVE_ERR_MAIL:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ebd1c(syswk, 2, 0x18);
        return 0x18;
    case BOX2_MOVE_ERR_LAST_BATTLER:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ebd1c(syswk, 3, 0x18);
        return 0x18;
    case 4:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ebd1c(syswk, 4, 0x18);
        return 0x18;
    case 5:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ebd78(syswk, 0);
        return 0x18;
    }
    if (syswk->param->mode == 2) {
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef670(syswk->app);
        } else {
            func_ov170_021ef68c(syswk->app);
        }
    }
    return syswk->nextSeq;
}


int func_ov170_021e3568(Box2SysWork *syswk) {
    if (func_ov170_021e2094(syswk) == 6) {
        return 0x18;
    }
    if (func_020260f0(syswk->app->cursorMove) == 1) {
        func_ov170_021ec000(syswk->app, 4, 1);
    }
    if (syswk->app->moveErr == 5) {
        func_ov170_021eba5c(syswk->app, 0x1b);
    } else {
        func_ov170_021eba5c(syswk->app, 0x18);
    }
    func_ov170_021dd14c(syswk);
    if (syswk->param->mode == 2) {
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ef670(syswk->app);
        } else {
            func_ov170_021ef68c(syswk->app);
        }
    }
    syswk->app->moveErr = 0;
    return syswk->nextSeq;
}

int func_ov170_021e35d8(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ee008(syswk, 0, 0);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021ea858(syswk, 0x19);
    case 1:
        func_ov170_021ef35c(syswk->app->bgWinFrame);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        if (syswk->unk18 == 1) {
            func_ov170_021ec5a0(syswk, 1);
        } else {
            func_ov170_021ec5a0(syswk, 0);
        }
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x19);
    case 2:
        func_ov170_021ee364(syswk, 6, 0x1e);
        func_ov170_021ee754(syswk, 0x1e);
        if (syswk->unk18 == 0) {
            func_ov170_021defec(syswk, 0x1e);
        }
        func_ov170_021ef68c(syswk->app);
        syswk->app->curPos = 0x1e;
        syswk->app->unkA194 = 0;
        return 0x20;
    }
    return 0x19;
}

int func_ov170_021e36a8(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021df0bc(syswk);
        func_ov170_021dd14c(syswk);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021ef34c(syswk->app->bgWinFrame);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        func_ov170_021ec5a0(syswk, 0);
        if (syswk->param->mode == 3) {
            func_ov170_021ecb20(syswk, 0x82, 1);
        }
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x1a);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021defec(syswk, 0x1e);
        func_ov170_021ef68c(syswk->app);
        if (syswk->param->mode == 3) {
            func_ov170_021ee364(syswk, 8, 0);
            return 0x46;
        }
        func_ov170_021ee364(syswk, 5, 0);
        return 0x1b;
    }
    return 0x1a;
}

int func_ov170_021e376c(Box2SysWork *syswk) {
    u32 res;
    u32 seq;
    u8 pos;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        syswk->unk1B = 1;
        return func_ov170_021ea9f4(syswk, 0x1f);
    }
    res = func_ov170_021ef194();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, 0xa9, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
            }
            func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
            return func_ov170_021e9ec0(syswk, res + BOX2_PARTY_POS);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef68c(syswk->app);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x1b);
        }
        return 0x1b;
    }

    res = func_ov170_021ee8a8(syswk);
    switch (res) {
    case 6:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        seq = func_ov170_021e8f50(syswk, 0x1c);
        return func_ov170_021e949c(syswk, 10, seq);
    case 7:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 8:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x1c));
    case 9:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk1B = 1;
        syswk->moveMode = 0;
        seq = func_ov170_021e8f50(syswk, 0x1f);
        return func_ov170_021e949c(syswk, 0, seq);
    case 10:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->getTray = syswk->tray;
        syswk->unk13 = 1;
        return func_ov170_021e949c(syswk, 1, 0x4f);
    case 11:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 1;
        seq = func_ov170_021e8f50(syswk, 0x50);
        return func_ov170_021e949c(syswk, 2, seq);
    case 12:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        seq = func_ov170_021e8f50(syswk, 0x55);
        return func_ov170_021e949c(syswk, 3, seq);
    case 13:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        seq = func_ov170_021e8f50(syswk, 0x59);
        return func_ov170_021e949c(syswk, 4, seq);
    case 14:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos - BOX2_PARTY_POS;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x1d);
    case 15:
        syswk->unk1B = 1;
        return func_ov170_021eaa3c(syswk, 0x1f);
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    case CURSORMOVE_SCROLL_L:
    case CURSORMOVE_SCROLL_R:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
    case CURSORMOVE_NONE:
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < 6) {
            func_ov170_021defec(syswk, cur + BOX2_PARTY_POS);
        } else if (cur == 6) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x1b));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos - BOX2_PARTY_POS;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x1d);
        }
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x1c));
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    default:
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, 0xa9, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
        func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
        func_ov170_021eef74(syswk->app, 9, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 9);
        return func_ov170_021ea020(syswk, res + BOX2_PARTY_POS, 0x1b);
    }
    return 0x1b;
}


int func_ov170_021e3b50(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021df0bc(syswk);
        func_ov170_021dd14c(syswk);
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ef3a0(syswk->app->bgWinFrame);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x1c);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ef670(syswk->app);
        if (syswk->param->mode == 3) {
            func_ov170_021ecb20(syswk, 0x81, 1);
            func_ov170_021ee364(syswk, 7, 0x21);
            return 0x39;
        }
        func_ov170_021ecb20(syswk, 1, 0);
        func_ov170_021ee364(syswk, 3, 0x21);
        return 0x11;
    }
    return 0x1c;
}


int func_ov170_021e3bf8(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef68c(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x1b));
}


int func_ov170_021e3c38(Box2SysWork *syswk) {
    GFL_SndSEPlay(SEQ_SE_SYS_40);
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_020260e4(syswk->app->cursorMove, 9);
    func_ov170_021ee3e4(syswk, 9);
    func_ov170_021ed55c(syswk, 1);
    func_ov170_021ee038(syswk, 0, 0, 1);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    return 0x1b;
}

int func_ov170_021e3c88(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        if (syswk->moveMode == 1) {
            syswk->getTray = BOX2_GET_NONE;
            syswk->pos = func_020260e0(syswk->app->cursorMove);
            if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
                if (!(syswk->pos < 6)) {
                    syswk->pos = BOX2_PARTY_POS;
                } else {
                    syswk->pos += BOX2_PARTY_POS;
                }
            } else if (syswk->pos >= BOX2_PARTY_POS) {
                syswk->pos = 0;
            }
            func_ov170_021defec(syswk, syswk->pos);
        } else {
            if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
                syswk->getTray = BOX2_GET_NONE;
            } else {
                syswk->getTray = syswk->tray;
            }
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef68c(syswk->app);
            func_ov170_021ed6e4(syswk, FALSE);
        }
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x1f);
    case 1:
        if (syswk->param->mode == 4 && syswk->param->unk14 == 1) {
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov170_021ebd78(syswk, 0);
            syswk->moveMode = 0;
            syswk->app->unkA194 = 0;
            syswk->nextSeq = 0x2b;
            return BOX2SEQ_TRGWAIT;
        }
        syswk->app->unkA194++;
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef3d8(syswk->app->bgWinFrame);
        } else {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, FALSE);
        }
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x1f);
    case 2:
        func_ov170_021ecb20(syswk, 1, FALSE);
        func_ov170_021ed888(syswk->app, TRUE);
        func_ov170_021ee038(syswk, 0, 1, 0);
        func_ov170_021ee364(syswk, 6, syswk->pos);
        func_ov170_021ed544(syswk, 6);
        syswk->app->curPos = syswk->pos;
        syswk->app->unkA194 = 0;
        if (syswk->moveMode == 1) {
            if (func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
                func_ov170_021ee038(syswk, 0, 1, 2);
            }
            if (syswk->param->mode == 4) {
                func_ov170_021ef680(syswk->app);
                func_ov170_021ef69c(syswk->app);
                func_02026124(syswk->app->cursorMove, 0x27);
            }
            func_ov170_021ed508(syswk, 0xd, TRUE);
            return 0x20;
        }
        if (syswk->moveMode == 2) {
            func_ov170_021ed508(syswk, 0xd, TRUE);
        } else {
            func_ov170_021ed508(syswk, 0xe, TRUE);
        }
        func_ov170_021ecb84(syswk->app, syswk->pos);
        return func_ov170_021ea0bc(syswk, syswk->pos);
    }
    return 0x1f;
}

int func_ov170_021e3e84(Box2SysWork *syswk) {
    u32 res;
    u8 cur;

    if (syswk->unk1C_4 == 1) {
        syswk->moveMode = 0;
        syswk->unk1C_4 = 0;
        return func_ov170_021e8f50(syswk, 0x21);
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && syswk->moveMode != 0 && syswk->unk18 == 0) {
        func_ov170_021dd238(syswk);
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x21);
    }
    if (syswk->unk18 == 0) {
        res = func_ov170_021ef170();
        if (res != CURSORMOVE_NONE) {
            func_020260f4(syswk->app->cursorMove, FALSE);
            if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                GFL_SndSEPlay(SEQ_SE_SYS_39);
                func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
                func_ov170_021defec(syswk, res);
                return func_ov170_021ea064(syswk, res, 0x20);
            }
            func_020260e4(syswk->app->cursorMove, res);
            func_ov170_021ee754(syswk, res);
            syswk->app->curPos = res;
            func_ov170_021ee3e4(syswk, res);
            func_ov170_021df040(syswk);
            return 0x20;
        }
        res = func_ov170_021ef1a4();
        if (res != CURSORMOVE_NONE) {
            func_020260f4(syswk->app->cursorMove, FALSE);
            if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                GFL_SndSEPlay(SEQ_SE_SYS_39);
                func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
                func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
                res += BOX2_PARTY_POS;
                return func_ov170_021ea064(syswk, res, 0x20);
            }
            func_020260e4(syswk->app->cursorMove, res + BOX2_PARTY_POS);
            res += BOX2_PARTY_POS;
            func_ov170_021ee754(syswk, res);
            syswk->app->curPos = res;
            func_ov170_021ee3e4(syswk, res);
            func_ov170_021df040(syswk);
            return 0x20;
        }
    }

    res = func_ov170_021ee96c(syswk);
    switch (res) {
    case 36:
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 37:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_020260e4(syswk->app->cursorMove, 0x24);
        func_ov170_021ee3e4(syswk, 0x24);
        func_ov170_021ee754(syswk, 0x24);
        syswk->app->curPos = 0x24;
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9c94(syswk, 0, 0x20);
    case 38:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_020260e4(syswk->app->cursorMove, 0x24);
        func_ov170_021ee3e4(syswk, 0x24);
        func_ov170_021ee754(syswk, 0x24);
        syswk->app->curPos = 0x24;
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9ce0(syswk, 0, 0x20);
    case 39:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->unk18 == 0) {
            func_ov170_021df040(syswk);
        }
        return func_ov170_021e949c(syswk, 0xb, func_ov170_021e8f50(syswk, 0x22));
    case 40:
        if (syswk->unk18 == 0) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur >= BOX2_BOXLIST_POS) {
                break;
            }
            if (func_ov170_021dd25c(syswk, cur, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
                break;
            }
            syswk->pos = cur;
            syswk->getTray = syswk->tray;
            syswk->unk13 = 4;
        } else {
            syswk->unk13 = 5;
            syswk->unk1D = func_020260e0(syswk->app->cursorMove);
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e954c(syswk, 8, 1, 0x4f);
    case 41:
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x21));
    case 42:
        if (syswk->moveMode == 0) {
            break;
        }
        if (syswk->unk18 != 0) {
            break;
        }
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x21);
    case CURSORMOVE_CANCEL:
        if (syswk->unk18 == 1) {
            return func_ov170_021ea14c(syswk, 0xff, 0x20);
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x21));
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == 0x24) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov170_021e9c94(syswk, 0, 0x20);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == 0x24) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov170_021e9ce0(syswk, 0, 0x20);
        }
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        if (syswk->unk18 == 0) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur < 0x24) {
                func_ov170_021defec(syswk, cur);
            } else {
                func_ov170_021df040(syswk);
            }
        }
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x20));
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_8:
    case CURSORMOVE_UNK_7:
        break;
    default:
        if (syswk->unk18 == 1) {
            return func_ov170_021ea14c(syswk, res, 0x20);
        }
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        syswk->pos = res;
        syswk->getTray = syswk->tray;
        func_ov170_021ecb84(syswk->app, syswk->pos);
        return func_ov170_021ea0bc(syswk, syswk->pos);
    }
    return 0x20;
}


int func_ov170_021e432c(Box2SysWork *syswk) {
    u8 pos;

    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
        func_ov170_021ee038(syswk, 0, 0, 1);
        if (syswk->unk1B == 0) {
            func_ov170_021ef3a0(syswk->app->bgWinFrame);
            syswk->app->unkA194 = 2;
        } else {
            func_ov170_021ecb20(syswk, 1, TRUE);
            func_ov170_021ed888(syswk->app, FALSE);
            func_ov170_021ef3ec(syswk->app->bgWinFrame);
            syswk->app->unkA194 = 1;
        }
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x21);
    case 1:
        syswk->app->unkA194 = 0;
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos < BOX2_PARTY_POS || pos >= BOX2_BOXLIST_POS) {
            pos = 0;
        } else {
            pos -= BOX2_PARTY_POS;
        }
        func_ov170_021defec(syswk, pos + BOX2_PARTY_POS);
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ed508(syswk, 0xe, TRUE);
        if (syswk->param->mode == 2) {
            func_ov170_021ee364(syswk, 5, pos);
            return 0x1b;
        }
        func_ov170_021ef68c(syswk->app);
        func_ov170_021ee364(syswk, 0xe, pos);
        return 0x27;
    case 2:
        syswk->app->unkA194 = 0;
        pos = func_020260e0(syswk->app->cursorMove);
        if (pos >= BOX2_PARTY_POS) {
            pos = 0;
        }
        func_ov170_021defec(syswk, pos);
        func_ov170_021ef670(syswk->app);
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ed508(syswk, 0xe, TRUE);
        if (syswk->param->mode == 4) {
            func_ov170_021ee364(syswk, 0xd, pos);
            return 0x23;
        }
        func_ov170_021ee364(syswk, 3, pos);
        return 0x11;
    }
    return 0x21;
}


int func_ov170_021e4470(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ef3a0(syswk->app->bgWinFrame);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x22);
    case 1:
        GFL_SndSEPlay(0x669);
        syswk->trayScroll = syswk->tray;
        syswk->app->unkA194++;
        return func_ov170_021ea83c(syswk, 0x22);
    case 2:
        func_ov170_021ee364(syswk, 4, 0x27);
        func_ov170_021ef670(syswk->app);
        syswk->app->curPos = 0x27;
        syswk->app->unkA194 = 0;
        return 0x15;
    }
    return 0x22;
}

int func_ov170_021e4504(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        syswk->unk1B = 0;
        return func_ov170_021ea9f4(syswk, 0x1f);
    }
    res = func_ov170_021ef170();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
            }
            func_ov170_021defec(syswk, res);
            syswk->unk1B = 0;
            return func_ov170_021e9d98(syswk, res);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef670(syswk->app);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x23);
        }
        return 0x23;
    }

    res = func_ov170_021eece4(syswk);
    switch (res) {
    case 30:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9c94(syswk, 1, 0x23);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9ce0(syswk, 1, 0x23);
    case 33:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 10, func_ov170_021e8f50(syswk, 0x26));
    case 34:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 35:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 36:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk1B = 0;
        syswk->moveMode = 0;
        return func_ov170_021e949c(syswk, 2, func_ov170_021e8f50(syswk, 0x1f));
    case 37:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 0;
        syswk->getTray = syswk->tray;
        syswk->curRcvPos = 0x25;
        return func_ov170_021e949c(syswk, 3, 0x4f);
    case 38:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 0;
        syswk->curRcvPos = 0x26;
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x50));
    case 39:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x25);
    case 40:
        syswk->unk1B = 0;
        return func_ov170_021eaa3c(syswk, 0x1f);
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9c94(syswk, 1, 0x23);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9ce0(syswk, 1, 0x23);
        }
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < BOX2_PARTY_POS) {
            func_ov170_021defec(syswk, cur);
        } else if (cur != 36 && cur != 37 && cur != 38 && cur != 39) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x23));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x25);
        }
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    default:
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
        func_ov170_021defec(syswk, res);
        func_ov170_021eef74(syswk->app, 0x24, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x24);
        return func_ov170_021e9e7c(syswk, res, 0x23);
    }
    return 0x23;
}


int func_ov170_021e4944(Box2SysWork *syswk) {
    GFL_SndSEPlay(SEQ_SE_SYS_40);
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_020260e4(syswk->app->cursorMove, 0x24);
    func_ov170_021ee3e4(syswk, 0x24);
    func_ov170_021ee038(syswk, 0, 0, 1);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    return 0x23;
}

int func_ov170_021e498c(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef670(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x23));
}

int func_ov170_021e49cc(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021df0bc(syswk);
        func_ov170_021dd14c(syswk);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021ef34c(syswk->app->bgWinFrame);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        func_ov170_021ec5a0(syswk, 0);
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 38);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021defec(syswk, BOX2_PARTY_POS);
        func_ov170_021ef68c(syswk->app);
        func_ov170_021ee364(syswk, 14, 0);
        return 39;
    }
    return 38;
}

int func_ov170_021e4a64(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        syswk->unk1B = 1;
        return func_ov170_021ea9f4(syswk, 0x1f);
    }
    res = func_ov170_021ef194();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
            }
            func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
            return func_ov170_021e9f3c(syswk, res + BOX2_PARTY_POS);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef68c(syswk->app);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x27);
        }
        return 0x27;
    }

    res = func_ov170_021eede0(syswk);
    switch (res) {
    case 6:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 10, func_ov170_021e8f50(syswk, 0x28));
    case 7:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 8:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x28));
    case 9:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk1B = 1;
        syswk->moveMode = 0;
        return func_ov170_021e949c(syswk, 2, func_ov170_021e8f50(syswk, 0x1f));
    case 10:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 1;
        syswk->getTray = syswk->tray;
        return func_ov170_021e949c(syswk, 3, 0x4f);
    case 11:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->unk13 = 1;
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x50));
    case 12:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos - BOX2_PARTY_POS;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x29);
    case 13:
        syswk->unk1B = 1;
        return func_ov170_021eaa3c(syswk, 0x1f);
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos - BOX2_PARTY_POS;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x29);
        }
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x28));
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < 6) {
            func_ov170_021defec(syswk, cur + BOX2_PARTY_POS);
        } else if (cur == 6) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x27));
    case CURSORMOVE_UNK_8:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_SCROLL_L:
    case CURSORMOVE_SCROLL_R:
    case CURSORMOVE_NONE:
        break;
    default:
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
        func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
        func_ov170_021eef74(syswk->app, 9, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 9);
        res += BOX2_PARTY_POS;
        return func_ov170_021ea020(syswk, res, 0x27);
    }
    return 0x27;
}


int func_ov170_021e4de8(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021df0bc(syswk);
        func_ov170_021dd14c(syswk);
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ef3a0(syswk->app->bgWinFrame);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x28);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 1, 0);
        func_ov170_021ef670(syswk->app);
        func_ov170_021ee364(syswk, 0xd, 0x21);
        return 0x23;
    }
    return 0x28;
}


int func_ov170_021e4e6c(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef68c(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x27));
}


int func_ov170_021e4eac(Box2SysWork *syswk) {
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_020260e4(syswk->app->cursorMove, 9);
    func_ov170_021ee3e4(syswk, 9);
    func_ov170_021ee038(syswk, 0, 0, 1);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    return 0x27;
}

int func_ov170_021e4ee8(Box2SysWork *syswk) {
    func_ov170_021eba5c(syswk->app, 0x1b);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021dd14c(syswk);
    if (func_020355c4() == 0) {
        func_020260f4(syswk->app->cursorMove, 1);
    }
    if (func_ov170_021ef480(syswk->app->bgWinFrame) == 1) {
        func_020260e4(syswk->app->cursorMove, (u8)(syswk->pos - 0x1e));
        func_ov170_021ee3e4(syswk, syswk->pos - 0x1e);
        func_ov170_021ef68c(syswk->app);
        return 0x27;
    }
    func_020260e4(syswk->app->cursorMove, syswk->pos);
    func_ov170_021ee3e4(syswk, syswk->pos);
    func_ov170_021ef670(syswk->app);
    return 0x23;
}

int func_ov170_021e4f60(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    res = func_ov170_021ef170();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021ef9b4, 5);
            }
            func_ov170_021defec(syswk, res);
            return func_ov170_021e9df8(syswk, res);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x2c);
        }
        return 0x2c;
    }

    res = func_ov170_021ee57c(syswk);
    switch (res) {
    case 30:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9c94(syswk, 0, 0x2c);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9ce0(syswk, 0, 0x2c);
    case 33:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 34:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 35:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 1, func_ov170_021e8f50(syswk, 0x2e));
    case 36:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->getTray = syswk->tray;
        syswk->curRcvPos = 0x24;
        return func_ov170_021e949c(syswk, 2, 0x4f);
    case 37:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 3, func_ov170_021e8f50(syswk, 0x55));
    case 38:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x59));
    case 39:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x2d);
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9c94(syswk, 0, 0x2c);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9ce0(syswk, 0, 0x2c);
        }
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < BOX2_PARTY_POS) {
            func_ov170_021defec(syswk, cur);
        } else if (cur != 35 && cur != 36 && cur != 37 && cur != 38 && cur != 39) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x2c));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x2d);
        }
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    default:
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021ef9b4, 5);
        func_ov170_021defec(syswk, res);
        func_ov170_021eef74(syswk->app, 0x23, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x23);
        return func_ov170_021e9e7c(syswk, res, 0x2c);
    }
    return 0x2c;
}


int func_ov170_021e5374(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x2c));
}

int func_ov170_021e53b0(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x2e);
    case 1:
        if (PokeParty_GetPkmCount(syswk->param->party) != 6) {
            return func_ov170_021e8f50(syswk, 0x2f);
        }
        GFL_SndSEPlay(0x557);
        func_ov170_021ebc58(syswk, 0, 2, 0x18);
        syswk->nextSeq = 0xe;
        return func_ov170_021e8f50(syswk, 6);
    }
    return 0x2e;
}

int func_ov170_021e543c(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        func_ov170_021ec61c(syswk);
        func_ov170_021ef35c(syswk->app->bgWinFrame);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        syswk->unk1A = 2;
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 47);
    case 1:
        syswk->getTray = syswk->tray;
        func_ov170_021ecb84(syswk->app, syswk->pos);
        func_ov170_021ea1c0(syswk);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0068, 47);
    case 2:
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        func_ov170_021dd530(syswk);
        func_ov170_021ea1e8(syswk);
        func_ov170_021df040(syswk);
        func_ov170_021ef3a0(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 47);
    case 3:
        func_020260e4(syswk->app->cursorMove, syswk->pos);
        func_ov170_021ee3e4(syswk, syswk->pos);
        func_ov170_021ed55c(syswk, TRUE);
        syswk->pos = BOX2_GET_NONE;
        syswk->app->unkA194 = 0;
        return 44;
    }
    return 47;
}


int func_ov170_021e5524(Box2SysWork *syswk) {
    Box2PokeMoveWork *work = (Box2PokeMoveWork *)syswk->app->vfunk.work;
    u32 pos = work->getPos;
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_ov170_021ed888(syswk->app, 1);
    func_ov170_021ee038(syswk, 0, 0, 1);
    if (syswk->pos != BOX2_GET_NONE) {
        func_ov170_021ebc58(syswk, syswk->pos, 1, 24);
        pos = 35;
    }
    func_020260e4(syswk->app->cursorMove, pos);
    func_ov170_021ee3e4(syswk, pos);
    return 44;
}

int func_ov170_021e557c(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    res = func_ov170_021ef194();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
                func_ov170_021eb988(syswk, data_ov170_021ef950, 5);
            }
            func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
            return func_ov170_021e9fa8(syswk, res + BOX2_PARTY_POS);
        }
        func_020260e4(syswk->app->cursorMove, res);
        res += 60;
        func_ov170_021ee3e4(syswk, res);
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x31);
        }
        return 0x31;
    }

    res = func_ov170_021ee474(syswk);
    switch (res) {
    case 6:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 7:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 8:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 1, func_ov170_021e8f50(syswk, 0x34));
    case 9:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->curRcvPos = 9;
        return func_ov170_021e949c(syswk, 2, 0x4f);
    case 10:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 3, func_ov170_021e8f50(syswk, 0x55));
    case 11:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x59));
    case 12:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos - BOX2_PARTY_POS;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x32);
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < 6) {
            func_ov170_021defec(syswk, cur + BOX2_PARTY_POS);
        } else if (cur != 8 && cur != 9 && cur != 10 && cur != 11 && cur != 12) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x31));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos - BOX2_PARTY_POS;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x32);
        }
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    default:
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021eb988(syswk, data_ov170_021ef950, 5);
        func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
        func_ov170_021eef74(syswk->app, 8, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 8);
        return func_ov170_021ea020(syswk, res + BOX2_PARTY_POS, 0x31);
    case CURSORMOVE_SCROLL_L:
    case CURSORMOVE_SCROLL_R:
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    }
    return 0x31;
}


int func_ov170_021e58d8(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x31));
}


int func_ov170_021e5914(Box2SysWork *syswk) {
    func_020260e4(syswk->app->cursorMove, (u8)(syswk->pos - 0x1e));
    func_ov170_021ee3e4(syswk, syswk->pos - 0x1e);
    func_ov170_021ed55c(syswk, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    return 0x31;
}

int func_ov170_021e594c(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x34);
    case 1:
        switch (func_ov170_021ea8cc(syswk)) {
        case 0:
            syswk->unk18 = 1;
            func_ov170_021ecb84(syswk->app, syswk->pos);
            func_ov170_021ec768(syswk->app, 0x24, FALSE);
            func_ov170_021ed55c(syswk, TRUE);
            func_ov170_021ee3e4(syswk, syswk->pos - BOX2_PARTY_POS);
            syswk->app->unkA194++;
            return func_ov170_021e8fc0(syswk, func_ov170_021e0778, 0x34);
        case 1:
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021ebbc8(syswk, 6, 0x18);
            syswk->nextSeq = 0x33;
            syswk->app->unkA194 = 0;
            return 6;
        case 2:
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021ebc58(syswk, 0, 5, 0x18);
            syswk->nextSeq = 0x33;
            syswk->app->unkA194 = 0;
            return 6;
        }
        break;
    case 2:
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        func_ov170_021ef3a0(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x34);
    case 3:
        func_ov170_021ee520(syswk, syswk->pos - BOX2_PARTY_POS);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e066c, 0x34);
    case 4:
        GFL_HeapFree(syswk->app->vfunk.work);
        func_ov170_021ed888(syswk->app, TRUE);
        func_ov170_021ebc58(syswk, 0, 3, 0x1a);
        func_ov170_021ee038(syswk, 0, 1, 1);
        func_ov170_021ee364(syswk, 1, 0);
        syswk->app->unkA194 = 0;
        return 0x35;
    }
    return 0x34;
}

int func_ov170_021e5ad4(Box2SysWork *syswk) {
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            res = 2;
        } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            res = 3;
        }
    }

    switch (res) {
    case 0:
    case 1:
        func_020260e4(syswk->app->cursorMove, 0);
        if (countEmptySlotsInBox(syswk->param->boxes, syswk->tray) == 0) {
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov170_021ebc58(syswk, 0, 4, 0x18);
            syswk->nextSeq = 0x37;
            return 6;
        } else {
            int slot;
            int box;

            box = syswk->tray;
            slot = 0;
            BoxSaveAccessor_GetNextFreeBoxSlot(syswk->param->boxes, &box, &slot);
            syswk->app->pokePutKey = slot;
            syswk->unk1A = 0;
            syswk->getTray = syswk->tray;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return func_ov170_021e8f50(syswk, 0x36);
        }
    case 2:
        func_020260e4(syswk->app->cursorMove, 0);
    case CURSORMOVE_SCROLL_L:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e9c94(syswk, 0, 0x35);
    case 3:
        func_020260e4(syswk->app->cursorMove, 0);
    case CURSORMOVE_SCROLL_R:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e9ce0(syswk, 0, 0x35);
    case 4:
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->app->pokePutKey = BOX2_GET_NONE;
        syswk->getTray = syswk->tray;
        syswk->unk1A = 1;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x36));
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    }
    return 0x35;
}

int func_ov170_021e5c5c(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        if (syswk->app->pokePutKey == BOX2_GET_NONE) {
            func_ov170_021ee544(syswk, syswk->pos);
            syswk->app->unkA194 = 1;
        } else {
            func_ov170_021ee544(syswk, syswk->app->pokePutKey);
            syswk->app->unkA194 = 4;
        }
        func_ov170_021ed888(syswk->app, FALSE);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        return func_ov170_021e8fc0(syswk, func_ov170_021e066c, 0x36);
    case 1:
        GFL_HeapFree(syswk->app->vfunk.work);
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        syswk->app->unkA194 = 2;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x36);
    case 2:
        func_ov170_021ea1c0(syswk);
        syswk->app->unkA194 = 6;
        return func_ov170_021e8fc0(syswk, func_ov170_021e087c, 0x36);
    case 3:
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
        func_ov170_021ef36c(syswk->app->bgWinFrame);
        syswk->app->unkA194 = 7;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x36);
    case 4:
        GFL_HeapFree(syswk->app->vfunk.work);
        func_ov170_021ea1c0(syswk);
        func_ov170_021ed6e4(syswk, FALSE);
        syswk->app->unkA194 = 5;
        return func_ov170_021e8fc0(syswk, func_ov170_021e09e0, 0x36);
    case 5:
        func_ov170_021dd530(syswk);
        func_ov170_021ea1e8(syswk);
        func_ov170_021df040(syswk);
        func_ov170_021ecb20(syswk, 1, TRUE);
        func_ov170_021ecb20(syswk, 2, FALSE);
        syswk->app->unkA194 = 3;
        break;
    case 6:
        func_ov170_021ea1e8(syswk);
    case 7:
        syswk->unk18 = 0;
        func_ov170_021ed6e4(syswk, FALSE);
        func_ov170_021ee038(syswk, 0, 0, 1);
        func_ov170_021ee364(syswk, 0, syswk->pos - BOX2_PARTY_POS);
        func_ov170_021defec(syswk, syswk->pos);
        syswk->app->unkA194 = 0;
        return 0x31;
    }
    return 0x36;
}


int func_ov170_021e5dd8(Box2SysWork *syswk) {
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ebc58(syswk, 0, 3, 0x1a);
    func_ov170_021ee038(syswk, 0, 1, 1);
    return 0x35;
}

int func_ov170_021e5e08(Box2SysWork *syswk) {
    func_ov170_021dd530(syswk);
    func_ov170_021ea1e8(syswk);
    func_ov170_021ee038(syswk, 0, 0, 1);
    if (syswk->pos == 0xff) {
        func_020260e4(syswk->app->cursorMove, 0);
        func_ov170_021ee3e4(syswk, 0);
        func_ov170_021defec(syswk, 0x1e);
    } else {
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        func_020260e4(syswk->app->cursorMove, 8);
        func_ov170_021ee3e4(syswk, 8);
    }
    return 0x31;
}

int func_ov170_021e5e68(Box2SysWork *syswk) {
    u32 res;
    u32 seq;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        res = func_ov170_021ea9f4(syswk, 0x3f);
        if (syswk->moveMode == 1) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur < BOX2_PARTY_POS) {
                syswk->pos = cur;
            } else {
                syswk->pos = 0;
            }
        }
        return res;
    }
    res = func_ov170_021ef170();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res, syswk->tray, 0xa9, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            func_ov170_021defec(syswk, res);
            return func_ov170_021ea1f4(syswk, res);
        }
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            cur = func_020260e0(syswk->app->cursorMove);
            func_ov170_021eef74(syswk->app, res, cur);
            func_020260e4(syswk->app->cursorMove, res);
            return func_ov170_021e62bc(syswk);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        return 0x39;
    }

    res = func_ov170_021eea48(syswk);
    switch (res) {
    case 30:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9c94(syswk, 1, 0x39);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9ce0(syswk, 1, 0x39);
    case 33:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        seq = func_ov170_021e8f50(syswk, 0x1a);
        return func_ov170_021e949c(syswk, 10, seq);
    case 34:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 35:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 36:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->moveMode = 0;
        seq = func_ov170_021e8f50(syswk, 0x3f);
        return func_ov170_021e949c(syswk, 3, seq);
    case 37:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->itemId == 0) {
            syswk->unk13 = 0;
            syswk->curRcvPos = 0x25;
            seq = func_ov170_021e8f50(syswk, 0x50);
            return func_ov170_021e949c(syswk, 4, seq);
        }
        seq = func_ov170_021e8f50(syswk, 0x50);
        return func_ov170_021e949c(syswk, 4, seq);
    case 38:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        cur = syswk->pos;
        func_ov170_021eef74(syswk->app, cur, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, cur);
        return func_ov170_021e949c(syswk, 5, 0x3a);
    case 39:
        res = func_ov170_021eaa3c(syswk, 0x3f);
        if (syswk->moveMode == 1) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur < BOX2_PARTY_POS) {
                syswk->pos = cur;
            } else {
                syswk->pos = 0;
            }
        }
        return res;
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9c94(syswk, 1, 0x39);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9ce0(syswk, 1, 0x39);
        }
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < BOX2_PARTY_POS) {
            func_ov170_021defec(syswk, cur);
        } else if (cur != 36 && cur != 37 && cur != 38) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x39));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            cur = syswk->pos;
            func_ov170_021eef74(syswk->app, cur, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, cur);
            return func_ov170_021e949c(syswk, 5, 0x3a);
        }
        syswk->param->unk28 = 0;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case CURSORMOVE_UNK_8:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_NONE:
        break;
    default:
        if (func_ov170_021dd25c(syswk, res, syswk->tray, 0xa9, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return func_ov170_021ea2dc(syswk, res);
        }
        break;
    }
    return 0x39;
}


int func_ov170_021e62bc(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef670(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e0e50, func_ov170_021e8fe0(syswk, 0x39));
}

int func_ov170_021e62fc(Box2SysWork *syswk) {
    if (func_ov170_021ebfec(syswk->app, 10) == TRUE) {
        return 0x3b;
    }
    func_ov170_021ea878(syswk, 0);
    func_ov170_021ed108(syswk->app, 0);
    func_ov170_021ec000(syswk->app, 10, 0);
    func_ov170_021ebcb4(syswk, 3, 0x18);
    if (syswk->pos < 0x1e) {
        func_ov170_021ecb20(syswk, 0x81, 1);
        func_ov170_021ed888(syswk->app, 1);
    }
    else {
        func_ov170_021ecb20(syswk, 0x82, 1);
    }
    syswk->app->itemId = 0;
    syswk->nextSeq = 0x3d;
    return func_ov170_021e8f50(syswk, 6);
}


int func_ov170_021e6374(Box2SysWork *syswk) {
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->pos < 0x1e) {
        func_ov170_021ef670(syswk->app);
    } else {
        func_ov170_021ef68c(syswk->app);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0f4c, 0x3d);
}

int func_ov170_021e63b0(Box2SysWork *syswk) {
    int seq;
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->pos < 0x1e) {
        func_020260e4(syswk->app->cursorMove, syswk->pos);
        func_ov170_021ee3e4(syswk, syswk->pos);
        func_ov170_021ef670(syswk->app);
        seq = 0x39;
    } else {
        func_020260e4(syswk->app->cursorMove, (u8)(syswk->pos - 0x1e));
        func_ov170_021ee3e4(syswk, syswk->pos - 0x1e);
        func_ov170_021ef68c(syswk->app);
        seq = 0x46;
    }
    func_ov170_021ed55c(syswk, 1);
    func_ov170_021ed508(syswk, 0xe, 1);
    return seq;
}


int func_ov170_021e6418(Box2SysWork *syswk) {
    u32 pos = 0x25;
    if (syswk->app->itemId != 0) {
        pos = 0x24;
    }
    func_020260e4(syswk->app->cursorMove, (u8)pos);
    func_ov170_021ee3e4(syswk, pos);
    func_ov170_021ed55c(syswk, 1);
    return 0x39;
}

int func_ov170_021e644c(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        syswk->app->unkA194++;
        func_ov170_021ed55c(syswk, FALSE);
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
            syswk->unk1E = TRUE;
        } else {
            syswk->unk1E = FALSE;
        }
        if (syswk->moveMode == 1) {
            syswk->getTray = BOX2_GET_NONE;
            syswk->app->itemId = 0;
            func_ov170_021dd14c(syswk);
            func_ov170_021defec(syswk, syswk->pos);
        } else {
            if (syswk->moveMode == 2 && syswk->app->itemId == 0) {
                syswk->getTray = BOX2_GET_NONE;
                syswk->moveMode = 1;
            }
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0ed4, 0x3f);
        }
    case 1:
        syswk->app->unkA194++;
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef3d8(syswk->app->bgWinFrame);
        } else {
            func_ov170_021ef35c(syswk->app->bgWinFrame);
            func_ov170_021ef36c(syswk->app->bgWinFrame);
            func_ov170_021ec5a0(syswk, FALSE);
        }
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x3f);
    case 2:
        syswk->app->unkA194 = 0;
        func_ov170_021ecb20(syswk, 0x81, TRUE);
        func_ov170_021ecb20(syswk, 0x82, TRUE);
        func_ov170_021ed888(syswk->app, TRUE);
        func_ov170_021ee364(syswk, 6, syswk->pos);
        func_ov170_021ed544(syswk, 6);
        func_02026124(syswk->app->cursorMove, 0x27);
        func_02026124(syswk->app->cursorMove, 0x28);
        if (syswk->moveMode == 1) {
            func_ov170_021ee038(syswk, 0, 1, 1);
            func_ov170_021ed508(syswk, 0xd, TRUE);
            return 0x40;
        }
        if (syswk->moveMode == 2) {
            func_ov170_021ed508(syswk, 0xd, TRUE);
        } else {
            func_ov170_021ed508(syswk, 0xe, TRUE);
        }
        func_ov170_021ee038(syswk, 2, 1, 1);
        return func_ov170_021ea458(syswk, syswk->pos);
    }
    return 0x3f;
}

int func_ov170_021e65e4(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    if (syswk->unk1C_4 == 1) {
        syswk->moveMode = 0;
        syswk->unk1C_4 = 0;
        return func_ov170_021e8f50(syswk, 0x41);
    }
    if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) && syswk->moveMode != 0 && syswk->unk18 == 0) {
        func_ov170_021dd238(syswk);
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x41);
    }
    if (syswk->unk18 == 0) {
        res = func_ov170_021ef1a4();
        if (res != CURSORMOVE_NONE) {
            func_020260f4(syswk->app->cursorMove, FALSE);
            if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
                func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
                return func_ov170_021ea39c(syswk, res + BOX2_PARTY_POS);
            }
            func_ov170_021df040(syswk);
            func_020260e4(syswk->app->cursorMove, res + BOX2_PARTY_POS);
            return 0x40;
        }
        res = func_ov170_021ef170();
        if (res != CURSORMOVE_NONE) {
            func_020260f4(syswk->app->cursorMove, FALSE);
            if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                func_ov170_021defec(syswk, res);
                func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
                return func_ov170_021ea39c(syswk, res);
            }
            func_ov170_021df040(syswk);
            func_020260e4(syswk->app->cursorMove, res);
            return 0x40;
        }
    }
    res = func_ov170_021ee96c(syswk);
    switch (res) {
    case 36:
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov170_021df040(syswk);
        return func_ov170_021e8f50(syswk, 0x5d);
    case 37:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_020260e4(syswk->app->cursorMove, 0x24);
        func_ov170_021ee3e4(syswk, 0x24);
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9c94(syswk, 0, 0x40);
    case 38:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_020260e4(syswk->app->cursorMove, 0x24);
        func_ov170_021ee3e4(syswk, 0x24);
        if (syswk->unk18 == 0) {
            func_ov170_021df0bc(syswk);
        } else {
            func_ov170_021ecc1c(syswk);
        }
        return func_ov170_021e9ce0(syswk, 0, 0x40);
    case 39:
    case 40:
        break;
    case 41:
        if (syswk->unk18 != 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x41));
    case 42:
        if (syswk->moveMode == 0) {
            break;
        }
        if (syswk->unk18 != 0) {
            break;
        }
        syswk->moveMode = 0;
        return func_ov170_021e8f50(syswk, 0x41);
    case CURSORMOVE_CANCEL:
        if (syswk->unk18 == 1) {
            return func_ov170_021ea514(syswk, 0xff);
        }
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->moveMode = 0;
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x41));
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == 0x24) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov170_021e9c94(syswk, 0, 0x40);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == 0x24) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov170_021e9ce0(syswk, 0, 0x40);
        }
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        if (syswk->unk18 == 0) {
            pos = func_020260e0(syswk->app->cursorMove);
            if (pos < BOX2_BOXLIST_POS) {
                func_ov170_021defec(syswk, pos);
            } else {
                func_ov170_021df040(syswk);
            }
        }
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x40));
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    default:
        if (syswk->unk18 == 1) {
            return func_ov170_021ea514(syswk, res);
        }
        return func_ov170_021ea458(syswk, res);
    }
    return 0x40;
}


int func_ov170_021e695c(Box2SysWork *syswk) {
    u8 pos;
    u8 cursorPos;

    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021df040(syswk);
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
        if (syswk->unk1E == 0) {
            func_ov170_021ef3a0(syswk->app->bgWinFrame);
        } else {
            func_ov170_021ed888(syswk->app, FALSE);
            func_ov170_021ecb20(syswk, 1, TRUE);
            func_ov170_021ef3ec(syswk->app->bgWinFrame);
        }
        GFL_SndSEPlay(SEQ_SE_SYS_42);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x41);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed544(syswk, 6);
        func_ov170_021ed508(syswk, 0xe, TRUE);
        func_ov170_021ee038(syswk, 0, 0, 1);
        if (syswk->unk1E == 0) {
            pos = func_020260e0(syswk->app->cursorMove);
            if (pos >= BOX2_BOXLIST_POS) {
                pos = 0;
            } else if (pos >= BOX2_PARTY_POS) {
                pos = 0;
            }
            func_ov170_021ee364(syswk, 7, pos);
            func_ov170_021defec(syswk, pos);
            func_ov170_021ef670(syswk->app);
            return 0x39;
        } else {
            pos = 0;
            cursorPos = func_020260e0(syswk->app->cursorMove);
            if (cursorPos < BOX2_BOXLIST_POS && cursorPos >= BOX2_PARTY_POS) {
                pos = cursorPos - BOX2_PARTY_POS;
            }
            func_ov170_021ee364(syswk, 8, pos);
            func_ov170_021defec(syswk, pos + BOX2_PARTY_POS);
            func_ov170_021ef68c(syswk->app);
            return 0x46;
        }
    }
    return 0x41;
}

int func_ov170_021e6a60(Box2SysWork *syswk) {
    Box2ItemMoveWork *work;
    u16 item;
    BoxPkm *pkm;

    if (syswk->moveMode == 0) {
        syswk->unk1C_4 = 1;
    }
    func_ov170_021ed6e4(syswk, FALSE);
    work = (Box2ItemMoveWork *)syswk->app->vfunk.work;
    if (work->putPos == syswk->pos && (syswk->getTray == BOX2_GET_NONE || syswk->getTray == syswk->tray)) {
        u16 setPos = work->setPos;

        func_ov170_021ea750(syswk);
        func_ov170_021ecb60(syswk->app, syswk->pos, FALSE);
        func_020260e4(syswk->app->cursorMove, syswk->pos);
        func_ov170_021ee038(syswk, 0, 1, 1);
        if (PML_ItemIsMail(syswk->app->itemId) == TRUE && setPos != syswk->pos) {
            GFL_SndSEPlay(SEQ_SE_BEEP);
            func_ov170_021ebb28(syswk, 0x18);
            syswk->nextSeq = 0x44;
            return 6;
        }
        return 0x40;
    }
    if (syswk->getTray == syswk->tray || syswk->pos >= BOX2_PARTY_POS) {
        func_ov170_021ecb60(syswk->app, syswk->pos, TRUE);
    }
    func_ov170_021ecb60(syswk->app, work->putPos, FALSE);
    item = func_ov170_021dd25c(syswk, work->putPos, syswk->tray, 6, NULL);
    func_ov170_021dd2b4(syswk, work->putPos, syswk->tray, 6, syswk->app->itemId);
    pkm = func_ov170_021dd2d0(syswk, syswk->tray, work->putPos);
    if (func_ov170_021dd718(syswk, pkm) == TRUE) {
        func_ov170_021dd8a8(syswk, work->putPos);
        func_ov170_021ec48c(syswk, syswk->tray, work->putPos, syswk->app->pokeIconId[work->putPos]);
    }
    func_ov170_021defec(syswk, work->putPos);
    func_020260e4(syswk->app->cursorMove, work->putPos);
    syswk->app->itemId = item;
    func_ov170_021dd2b4(syswk, syswk->pos, syswk->getTray, 6, syswk->app->itemId);
    pkm = func_ov170_021dd2d0(syswk, syswk->getTray, syswk->pos);
    if (func_ov170_021dd718(syswk, pkm) == TRUE) {
        func_ov170_021dd8a8(syswk, syswk->pos);
        if (syswk->getTray == BOX2_GET_NONE || syswk->getTray == syswk->tray) {
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
    }
    if (syswk->app->itemId == 0) {
        syswk->pos = work->putPos;
        func_ov170_021ea750(syswk);
        func_ov170_021ee038(syswk, 0, 1, 1);
        return 0x40;
    }
    func_ov170_021ed094(syswk->app, syswk->app->itemId);
    func_ov170_021ed108(syswk->app, 1);
    func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
    func_ov170_021ed1c8(syswk->app);
    GFL_SndSEPlay(SEQ_SE_SYS_39);
    return func_ov170_021e8fc0(syswk, func_ov170_021e122c, 0x43);
}


int func_ov170_021e6c3c(Box2SysWork *syswk) {
    Box2ItemMoveWork *work;
    u8 oldPos;
    u16 newPos;
    u16 item;
    u8 cursorPos;

    if (syswk->moveMode == 0) {
        syswk->unk1C_4 = 1;
    }
    work = (Box2ItemMoveWork *)syswk->app->vfunk.work;
    func_ov170_021ecb60(syswk->app, work->putPos, FALSE);
    oldPos = syswk->pos;
    newPos = work->setPos;
    syswk->pos = work->putPos;
    func_ov170_021ea750(syswk);
    item = syswk->app->itemId;
    syswk->app->itemId = 0;
    if (PML_ItemIsMail(item) == TRUE && newPos != oldPos) {
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021ebb28(syswk, 0x18);
        syswk->nextSeq = 0x44;
        return 6;
    }
    func_ov170_021ed55c(syswk, TRUE);
    cursorPos = func_020260e0(syswk->app->cursorMove);
    if (cursorPos < BOX2_BOXLIST_POS) {
        func_ov170_021defec(syswk, cursorPos);
    }
    func_ov170_021ee038(syswk, 0, 1, 1);
    return 0x40;
}


int func_ov170_021e6cd4(Box2SysWork *syswk) {
    func_ov170_021eba70(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (func_020260f0(syswk->app->cursorMove) == 1) {
        func_ov170_021ec000(syswk->app, 4, 1);
    }
    return 0x40;
}

int func_ov170_021e6d00(Box2SysWork *syswk) {
    u16 item;
    BoxPkm *pkm;

    if (syswk->moveMode == 0) {
        syswk->unk1C_4 = 1;
    }
    if (syswk->app->pokePutKey == syswk->app->unkA16E && (syswk->getTray == BOX2_GET_NONE || syswk->getTray == syswk->tray)) {
        syswk->app->itemId = 0;
        func_ov170_021ee038(syswk, 0, 1, 1);
        return 0x40;
    }
    if (syswk->getTray == syswk->tray || syswk->app->unkA16E >= BOX2_PARTY_POS) {
        func_ov170_021ecb60(syswk->app, syswk->app->unkA16E, TRUE);
    }
    func_ov170_021ecb60(syswk->app, syswk->app->pokePutKey, FALSE);
    item = func_ov170_021dd25c(syswk, syswk->app->pokePutKey, syswk->tray, 6, NULL);
    func_ov170_021dd2b4(syswk, syswk->app->pokePutKey, syswk->tray, 6, syswk->app->itemId);
    pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->app->pokePutKey);
    if (func_ov170_021dd718(syswk, pkm) == TRUE) {
        func_ov170_021dd8a8(syswk, syswk->app->pokePutKey);
        func_ov170_021ec48c(syswk, syswk->tray, syswk->app->pokePutKey, syswk->app->pokeIconId[syswk->app->pokePutKey]);
    }
    func_ov170_021defec(syswk, syswk->app->pokePutKey);
    syswk->app->itemId = item;
    func_ov170_021dd2b4(syswk, syswk->app->unkA16E, syswk->getTray, 6, syswk->app->itemId);
    pkm = func_ov170_021dd2d0(syswk, syswk->getTray, syswk->app->unkA16E);
    if (func_ov170_021dd718(syswk, pkm) == TRUE) {
        func_ov170_021dd8a8(syswk, syswk->app->unkA16E);
        if (syswk->getTray == BOX2_GET_NONE || syswk->getTray == syswk->tray) {
            func_ov170_021ec48c(syswk, syswk->tray, syswk->app->unkA16E, syswk->app->pokeIconId[syswk->app->unkA16E]);
        }
    }
    if (syswk->app->itemId == 0) {
        func_ov170_021ee038(syswk, 0, 1, 1);
        return 0x40;
    }
    func_ov170_021ea728(syswk);
    ((Box2ItemMoveWork *)syswk->app->vfunk.work)->putPos = syswk->app->pokePutKey;
    func_ov170_021ed094(syswk->app, syswk->app->itemId);
    func_ov170_021ed108(syswk->app, 1);
    func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
    func_ov170_021ed1c8(syswk->app);
    GFL_SndSEPlay(SEQ_SE_SYS_39);
    return func_ov170_021e8fc0(syswk, func_ov170_021e122c, 0x43);
}

int func_ov170_021e6ec0(Box2SysWork *syswk) {
    u32 res;
    u8 pos;
    u8 cur;

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        res = func_ov170_021ea9f4(syswk, 0x3f);
        if (syswk->moveMode == 1) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur < 6) {
                syswk->pos = cur + BOX2_PARTY_POS;
            } else {
                syswk->pos = BOX2_PARTY_POS;
            }
        }
        return res;
    }
    res = func_ov170_021ef194();
    if (res != CURSORMOVE_NONE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_39);
            func_02035d20(&syswk->app->tpx, &syswk->app->tpy);
            func_ov170_021defec(syswk, res + BOX2_PARTY_POS);
            return func_ov170_021ea598(syswk, res + BOX2_PARTY_POS);
        }
        func_ov170_021df040(syswk);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = res;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, res + BOX2_PARTY_POS);
            return func_ov170_021e7258(syswk);
        }
        func_020260e4(syswk->app->cursorMove, res);
        func_ov170_021ee3e4(syswk, res);
        return 0x46;
    }

    res = func_ov170_021eeb40(syswk);
    switch (res) {
    case 6:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 10, func_ov170_021e8f50(syswk, 0x1c));
    case 7:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        syswk->param->unk28 = 1;
        return func_ov170_021e954c(syswk, 7, 8, 0x10);
    case 8:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x1c));
    case 9:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        syswk->moveMode = 0;
        return func_ov170_021e949c(syswk, 3, func_ov170_021e8f50(syswk, 0x3f));
    case 10:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->itemId == 0) {
            syswk->unk13 = 1;
            syswk->curRcvPos = 10;
            return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x50));
        }
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x50));
    case 11:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos - BOX2_PARTY_POS;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x47);
    case 12:
        res = func_ov170_021eaa3c(syswk, 0x3f);
        if (syswk->moveMode == 1) {
            cur = func_020260e0(syswk->app->cursorMove);
            if (cur < 6) {
                syswk->pos = cur + BOX2_PARTY_POS;
            } else {
                syswk->pos = BOX2_PARTY_POS;
            }
        }
        return res;
    case CURSORMOVE_UNK_8:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_SCROLL_L:
    case CURSORMOVE_SCROLL_R:
    case CURSORMOVE_NONE:
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE:
        cur = func_020260e0(syswk->app->cursorMove);
        if (cur < 6) {
            func_ov170_021defec(syswk, cur + BOX2_PARTY_POS);
        } else if (cur == 6) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x46));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            pos = syswk->pos - BOX2_PARTY_POS;
            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x47);
        }
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x1c));
    default:
        if (func_ov170_021dd25c(syswk, res + BOX2_PARTY_POS, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        res += BOX2_PARTY_POS;
        return func_ov170_021ea66c(syswk, res);
    }
    return 0x46;
}


int func_ov170_021e7258(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef68c(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e0e50, func_ov170_021e8fe0(syswk, 0x46));
}

int func_ov170_021e7298(Box2SysWork *syswk) {
    Box2ItemMoveWork *work = (Box2ItemMoveWork *)syswk->app->vfunk.work;

    if (syswk->app->itemId == 0) {
        func_ov170_021ea750(syswk);
        func_ov170_021ebcb4(syswk, 0, 0x18);
        func_020260e4(syswk->app->cursorMove, 10);
        func_ov170_021ee3e4(syswk, 10);
        func_ov170_021ed55c(syswk, TRUE);
        return 70;
    }
    if (work->putPos == syswk->pos) {
        u16 setPos = work->setPos;

        func_ov170_021ea750(syswk);
        func_020260e4(syswk->app->cursorMove, 9);
        func_ov170_021ee3e4(syswk, 9);
        if (PML_ItemIsMail(syswk->app->itemId) == TRUE && setPos != syswk->pos) {
            func_ov170_021ecb60(syswk->app, syswk->pos, FALSE);
            func_ov170_021ebcb4(syswk, 7, 0x18);
            GFL_SndSEPlay(SEQ_SE_BEEP);
            syswk->nextSeq = 14;
            return func_ov170_021e8f50(syswk, 6);
        }
        func_ov170_021ebcb4(syswk, 1, 0x18);
        func_ov170_021ed55c(syswk, TRUE);
        return 70;
    }
    if (PML_ItemIsMail(syswk->app->itemId) == TRUE) {
        func_ov170_021ea750(syswk);
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ecb60(syswk->app, syswk->pos, FALSE);
        func_ov170_021ebcb4(syswk, 7, 0x18);
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_020260e4(syswk->app->cursorMove, 9);
        func_ov170_021ee3e4(syswk, 9);
        func_ov170_021ed55c(syswk, TRUE);
        syswk->nextSeq = 14;
        return func_ov170_021e8f50(syswk, 6);
    }
    func_ov170_021ecb60(syswk->app, work->putPos, FALSE);
    func_ov170_021ecb60(syswk->app, syswk->pos, TRUE);
    func_ov170_021ed6e4(syswk, FALSE);
    func_ov170_021ed108(syswk->app, 1);
    func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
    return 73;
}

int func_ov170_021e73d8(Box2SysWork *syswk) {
    Box2ItemMoveWork *work;
    u16 item;
    BoxPkm *pkm;

    if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) == TRUE) {
        return 73;
    }
    func_ov170_021ed108(syswk->app, 0);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
    work = (Box2ItemMoveWork *)syswk->app->vfunk.work;
    item = func_ov170_021dd25c(syswk, work->putPos, syswk->tray, 6, NULL);
    func_ov170_021dd2b4(syswk, work->putPos, syswk->tray, 6, syswk->app->itemId);
    pkm = func_ov170_021dd2d0(syswk, syswk->tray, work->putPos);
    if (func_ov170_021dd718(syswk, pkm) == TRUE) {
        func_ov170_021dd8a8(syswk, work->putPos);
        func_ov170_021ec48c(syswk, syswk->tray, work->putPos, syswk->app->pokeIconId[work->putPos]);
    }
    func_ov170_021defec(syswk, work->putPos);
    syswk->app->itemId = item;
    func_ov170_021dd2b4(syswk, syswk->pos, syswk->getTray, 6, syswk->app->itemId);
    if (syswk->getTray == BOX2_GET_NONE || syswk->getTray == syswk->tray) {
        pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        if (func_ov170_021dd718(syswk, pkm) == TRUE) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
    }
    if (syswk->app->itemId == 0) {
        syswk->pos = work->putPos;
        func_ov170_021ea750(syswk);
        func_ov170_021ef68c(syswk->app);
        func_020260e4(syswk->app->cursorMove, syswk->pos - BOX2_PARTY_POS);
        func_ov170_021defec(syswk, syswk->pos);
        func_020260f4(syswk->app->cursorMove, TRUE);
        return 70;
    }
    GFL_SndSEPlay(SEQ_SE_SYS_39);
    func_ov170_021ed094(syswk->app, syswk->app->itemId);
    func_ov170_021ed108(syswk->app, 1);
    func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 1);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, TRUE);
    func_ov170_021ed1c8(syswk->app);
    return func_ov170_021e8fc0(syswk, func_ov170_021e1988, func_ov170_021e8f50(syswk, 0x4a));
}


int func_ov170_021e754c(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_SYS_40);
        func_ov170_021ecb60(syswk->app, ((Box2ItemMoveWork *)syswk->app->vfunk.work)->putPos, FALSE);
        func_ov170_021ed6e4(syswk, FALSE);
        func_ov170_021ed108(syswk->app, 1);
        func_ov170_021ebfa8(syswk->app, BOX2_ACTOR_ITEM_ICON, 2);
        func_020260e4(syswk->app->cursorMove, syswk->pos - BOX2_PARTY_POS);
        func_ov170_021ea750(syswk);
        syswk->app->unkA194++;
        break;
    case 1:
        if (func_ov170_021ebfec(syswk->app, BOX2_ACTOR_ITEM_ICON) != TRUE) {
            func_ov170_021defec(syswk, syswk->pos);
            func_020260f4(syswk->app->cursorMove, TRUE);
            func_ov170_021ef68c(syswk->app);
            return 70;
        }
        break;
    }
    return 74;
}

int func_ov170_021e75e0(Box2SysWork *syswk) {
    u32 res;

    res = func_ov170_021eeea4(syswk);
    switch (res) {
    case 30:
        func_ov170_021df0bc(syswk);
        break;
    case 31:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9c94(syswk, 0, 0x4b);
    case 32:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021df0bc(syswk);
        func_020260e4(syswk->app->cursorMove, BOX2_PARTY_POS);
        func_ov170_021ee3e4(syswk, BOX2_PARTY_POS);
        return func_ov170_021e9ce0(syswk, 0, 0x4b);
    case 33:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        syswk->param->unk28 = 0;
        syswk->param->tray = BOX2_GET_NONE;
        syswk->param->position = BOX2_GET_NONE;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case 34:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x4d));
    case 35: {
        u8 pos;

        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        pos = syswk->pos;
        func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, pos);
        return func_ov170_021e949c(syswk, 5, 0x4e);
    }
    case CURSORMOVE_SCROLL_L:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9c94(syswk, 0, 0x4b);
        }
        break;
    case CURSORMOVE_SCROLL_R:
        if (func_020260e0(syswk->app->cursorMove) == BOX2_PARTY_POS) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021df0bc(syswk);
            return func_ov170_021e9ce0(syswk, 0, 0x4b);
        }
        break;
    case CURSORMOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSORMOVE_CURSOR_MOVE: {
        u8 pos = func_020260e0(syswk->app->cursorMove);

        if (pos < BOX2_PARTY_POS) {
            func_ov170_021defec(syswk, pos);
        } else if (pos != 0x22 && pos != 35) {
            func_ov170_021df040(syswk);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x4b));
    }
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            u8 pos = syswk->pos;

            func_ov170_021eef74(syswk->app, pos, func_020260e0(syswk->app->cursorMove));
            func_020260e4(syswk->app->cursorMove, pos);
            return func_ov170_021e949c(syswk, 5, 0x4e);
        }
        syswk->param->unk28 = 0;
        syswk->param->tray = BOX2_GET_NONE;
        syswk->param->position = BOX2_GET_NONE;
        return func_ov170_021e954c(syswk, 6, 9, 0x68);
    case CURSORMOVE_NONE:
    case CURSORMOVE_UNK_7:
    case CURSORMOVE_UNK_8:
        break;
    default:
        if (func_ov170_021dd25c(syswk, res, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            syswk->pos = res;
            func_ov170_021eb988(syswk, data_ov170_021ef8ec, 2);
            func_ov170_021defec(syswk, res);
            func_ov170_021ed588(syswk);
            func_020260e4(syswk->app->cursorMove, 0x22);
            if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
                return func_ov170_021e78f4(syswk);
            }
            func_ov170_021eef74(syswk->app, 0x22, res);
            func_ov170_021ef534(syswk->app->bgWinFrame);
            return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x4c));
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021defec(syswk, res);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            func_ov170_021ed6e4(syswk, FALSE);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x4b);
        }
        break;
    }
    return 0x4b;
}


int func_ov170_021e78f4(Box2SysWork *syswk) {
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    return 0x4b;
}

int func_ov170_021e7904(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x4d);
    case 1:
        if (func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, HEAPID_BOX2_APP, 0) != 0) {
            func_ov170_021ebd1c(syswk, 4, 0x18);
            syswk->nextSeq = 0xe;
            return func_ov170_021e8f50(syswk, 6);
        }
        if (func_ov170_021dd908(syswk, syswk->pos) == 0) {
            func_ov170_021ebd1c(syswk, 6, 0x1b);
            syswk->nextSeq = 0xe;
            return func_ov170_021e8f50(syswk, 6);
        }
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ecb60(syswk->app, syswk->pos, 0);
        func_ov170_021ebd50(syswk);
        syswk->app->unkA194 = 0;
        return func_ov170_021e8fe8(syswk, 5);
    }
    return 0x4d;
}


int func_ov170_021e79c8(Box2SysWork *syswk) {
    func_ov170_021ef57c(syswk->app->bgWinFrame);
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, 0x4b));
}


int func_ov170_021e7a04(Box2SysWork *syswk) {
    return func_ov170_021e95cc(syswk, 0);
}

int func_ov170_021e7a10(Box2SysWork *syswk) {
    u32 pkm;

    switch (syswk->app->unkA194) {
    case 0:
        if (syswk->param->mode == 4 && syswk->param->unk14 == 1) {
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            syswk->app->unkA194 = 3;
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x50);
        }
        if (func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 0x4c, NULL) == 0) {
            pkm = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, NULL);
            if (pkm == 0) {
                return func_ov170_021e95cc(syswk, 1);
            }
            func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
            func_ov170_021ed508(syswk, 0xe, FALSE);
            func_ov170_021ed888(syswk->app, FALSE);
            if (PML_ItemIsMail(pkm) == TRUE) {
                func_ov170_021ef57c(syswk->app->bgWinFrame);
                syswk->app->unkA194 = 1;
                return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x50);
            }
            if (syswk->pos < BOX2_PARTY_POS) {
                func_ov170_021ecb20(syswk, 1, TRUE);
            } else {
                func_ov170_021ecb20(syswk, 2, TRUE);
            }
            func_ov170_021ecb60(syswk->app, syswk->pos, FALSE);
            syswk->app->unkA170[0] = 1;
            func_ov170_021ef508(syswk->app->bgWinFrame);
            func_ov170_021ebaf8(syswk, pkm, 0x18);
            if (syswk->param->mode == 3) {
                return func_ov170_021e8fe8(syswk, 2);
            }
            return func_ov170_021e8fe8(syswk, 0);
        }
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194 = 2;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x50);
    case 1:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ebb1c(syswk, 0x18);
        syswk->nextSeq = 0xe;
        return func_ov170_021e8f50(syswk, 6);
    case 2:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ebcb4(syswk, 5, 0x18);
        syswk->nextSeq = 0xe;
        return func_ov170_021e8f50(syswk, 6);
    case 3:
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov170_021ebd78(syswk, 1);
        syswk->nextSeq = 0x2b;
        return 6;
    }
    return 0x50;
}


int func_ov170_021e7bd0(Box2SysWork *syswk) {
    if (syswk->getTray != 0xff) {
        func_ov170_021def98(syswk, syswk->getTray, syswk->pos);
    } else {
        func_ov170_021defec(syswk, syswk->pos);
    }
    func_ov170_021ed544(syswk, 6);
    if (syswk->moveMode == 0) {
        func_ov170_021ed508(syswk, 0xe, 1);
    } else {
        func_ov170_021ed508(syswk, 0xd, 1);
    }
    switch (syswk->param->mode) {
    case 0:
        syswk->app->unkA198 = func_ov170_021e9968(syswk);
        break;
    case 1:
        syswk->app->unkA198 = func_ov170_021e9938(syswk);
        break;
    case 2:
        syswk->app->unkA198 = func_ov170_021e99b0(syswk);
        break;
    case 4:
        syswk->app->unkA198 = func_ov170_021e9b74(syswk);
        break;
    }
    return func_ov170_021e22b8(syswk);
}


int func_ov170_021e7c58(Box2SysWork *syswk) {
    func_ov170_021df040(syswk);
    switch (syswk->param->mode) {
    case 1:
        func_ov170_021ee364(syswk, 2, 0x1e);
        syswk->app->curPos = 0x1e;
        syswk->app->unkA198 = 0x2c;
        break;
    case 2:
        func_ov170_021ef670(syswk->app);
        func_ov170_021ee364(syswk, 3, 0x1e);
        syswk->app->curPos = 0x1e;
        syswk->app->unkA198 = 0x11;
        break;
    case 3:
        func_ov170_021ecb20(syswk, 0x81, 1);
        func_ov170_021ef670(syswk->app);
        func_ov170_021ee364(syswk, 7, 0x1e);
        syswk->app->curPos = 0x1e;
        syswk->app->unkA198 = 0x39;
        break;
    case 4:
        func_ov170_021ef670(syswk->app);
        func_ov170_021ee364(syswk, 0xd, 0x1e);
        syswk->app->curPos = 0x1e;
        syswk->app->unkA198 = 0x23;
        break;
    }
    return func_ov170_021e22b8(syswk);
}


int func_ov170_021e7d04(Box2SysWork *syswk) {
    switch (syswk->param->mode) {
    case 2:
        syswk->app->unkA198 = func_ov170_021e95f8(syswk);
        break;
    case 3:
        syswk->app->unkA198 = func_ov170_021e96e8(syswk);
        break;
    case 4:
        syswk->app->unkA198 = func_ov170_021e9848(syswk);
        break;
    }
    return func_ov170_021e22b8(syswk);
}


int func_ov170_021e7d3c(Box2SysWork *syswk) {
    func_ov170_021ebb64(syswk, syswk->subRet, 0x18);
    if (syswk->param->mode == 3) {
        syswk->nextSeq = 0x3c;
    } else {
        syswk->nextSeq = 0xe;
    }
    return func_ov170_021e8f50(syswk, 6);
}

int func_ov170_021e7d64(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ed508(syswk, 0xe, 0);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x55);
    case 1:
        syswk->app->unkA174 = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 0xb, 0);
        func_ov170_021df110(syswk, syswk->app->unkA174);
        func_ov170_021ef6a8(syswk->app->bgWinFrame);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e01a8, 0x55);
    case 2:
        func_ov170_021ee364(syswk, 9, 0);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ecb20(syswk, 2, 1);
        func_ov170_021ecb60(syswk->app, syswk->pos, 0);
        func_ov170_021ebbbc(syswk, 0x18);
        return func_ov170_021e8f50(syswk, 0x56);
    }
    return 0x55;
}

int func_ov170_021e7e54(Box2SysWork *syswk) {
    switch (func_02025de8(syswk->app->cursorMove)) {
    case 0:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 0);
        break;
    case 1:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 1);
        break;
    case 2:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 2);
        break;
    case 3:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 3);
        break;
    case 4:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 4);
        break;
    case 5:
        GFL_SndSEPlay(0x54c);
        func_ov170_021ea908(syswk, 5);
        break;
    case 6: {
        u8 flags = syswk->app->unkA174;
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 0xb, flags);
        func_ov170_021df11c(syswk, flags);
        GFL_SndSEPlay(0x54c);
        return func_ov170_021e94ac(syswk, 0, 0x57);
    }
    case 7:
        GFL_SndSEPlay(0x551);
        return func_ov170_021e94ac(syswk, 1, 0x57);
    case CURSOR_MOVE_CANCEL:
        GFL_SndSEPlay(0x551);
        return func_ov170_021e94ac(syswk, 1, 0x57);
    case CURSOR_MOVE_MOVED:
        GFL_SndSEPlay(0x548);
        return func_ov170_021e8fd0(syswk, 0x58);
    case CURSOR_MOVE_CURSOR_ON:
        GFL_SndSEPlay(0x548);
        break;
    }
    return 0x56;
}


int func_ov170_021e7f6c(Box2SysWork *syswk) {
    switch (syswk->param->mode) {
    case 0:
        func_ov170_021ee364(syswk, 0, 0xa);
        break;
    case 1:
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ee364(syswk, 2, 0x25);
        break;
    case 2:
    default:
        if (syswk->pos < 0x1e) {
            func_ov170_021ed888(syswk->app, 1);
            func_ov170_021ee364(syswk, 3, 0x27);
        } else {
            func_ov170_021ee364(syswk, 5, 0xc);
        }
        break;
    }
    func_ov170_021ed55c(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->pos < 0x1e) {
        func_ov170_021ecb20(syswk, 1, 0);
    }
    func_ov170_021ecb20(syswk, 2, 0);
    func_ov170_021ef6cc(syswk->app->bgWinFrame);
    GFL_SndSEPlay(0x669);
    return func_ov170_021e8fc0(syswk, func_ov170_021e01a8, func_ov170_021e8f50(syswk, 0xe));
}


int func_ov170_021e8014(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->vfunk.work);
    return 0x56;
}

int func_ov170_021e8024(Box2SysWork *syswk) {
    int index;
    BoxPkm *pkm;
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ed508(syswk, 0xe, 0);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x59);
    case 1:
        if (syswk->pos >= 0x1e) {
            func_ov170_021ecb20(syswk, 2, 1);
            func_ov170_021ecb60(syswk->app, syswk->pos, 0);
            if (func_ov170_021dd694(syswk, index = syswk->pos - 0x1e) == 0) {
                GFL_SndSEPlay(0x557);
                func_ov170_021ebbc8(syswk, 6, 0x18);
                syswk->app->unkA170[0] = 1;
                syswk->nextSeq = 0xe;
                return func_ov170_021e8f50(syswk, 6);
            }
            if (PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, index), 6, 0)) == 1) {
                GFL_SndSEPlay(0x557);
                func_ov170_021ebc58(syswk, 0, 5, 0x18);
                syswk->app->unkA170[0] = 1;
                syswk->nextSeq = 0xe;
                return func_ov170_021e8f50(syswk, 6);
            }
        } else {
            func_ov170_021ed888(syswk->app, 0);
            func_ov170_021ecb20(syswk, 1, 1);
            func_ov170_021ecb60(syswk->app, syswk->pos, 0);
        }
        pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        if (PML_PkmGetParam(pkm, 0x4c, 0) != 0) {
            GFL_SndSEPlay(0x557);
            func_ov170_021ebbc8(syswk, 3, 0x18);
            syswk->app->unkA170[0] = 1;
            syswk->nextSeq = 0xe;
            return func_ov170_021e8f50(syswk, 6);
        }
        func_ov170_021ebbc8(syswk, 0, 0x18);
        return func_ov170_021e8f50(syswk, func_ov170_021e8fe8(syswk, 1));
    }
    return 0x59;
}

int func_ov170_021e8188(Box2SysWork *syswk) {
    func_ov170_021de7c8(syswk);
    if (func_ov170_021ec994((Box2PokeFreeWork *)syswk->app->unkA190) == FALSE) {
        if (((Box2PokeFreeWork *)syswk->app->unkA190)->checkFlag != 0) {
            return func_ov170_021e8f50(syswk, 0x5c);
        }
        func_ov170_021eca24((Box2PokeFreeWork *)syswk->app->unkA190);
        func_ov170_021de7b4(syswk);
        return func_ov170_021e8f50(syswk, 0x5b);
    }
    return 0x5a;
}


int func_ov170_021e81d0(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ebbc8(syswk, 1, 0x18);
        syswk->nextSeq = 0x5b;
        syswk->app->unkA194++;
        return 6;
    case 1:
        func_ov170_021ebbc8(syswk, 2, 0x18);
        syswk->nextSeq = 0x5b;
        syswk->app->unkA194++;
        return 6;
    case 2:
        func_ov170_021dd314(syswk, syswk->tray, syswk->pos);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021df040(syswk);
        if (syswk->pos < 0x1e) {
            func_ov170_021edab0(syswk, syswk->tray);
            func_ov170_021edb34(syswk, syswk->tray);
            if (syswk->param->mode == 1) {
                syswk->nextSeq = 0x2c;
            } else {
                syswk->nextSeq = 0x11;
                func_ov170_021ef670(syswk->app);
            }
            func_020260e4(syswk->app->cursorMove, syswk->pos);
            func_ov170_021ee3e4(syswk, syswk->pos);
            func_ov170_021ed55c(syswk, 1);
            func_ov170_021ed888(syswk->app, 1);
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ed508(syswk, 0xe, 1);
            syswk->pos = 0xff;
            syswk->app->unkA194 = 0;
            return syswk->nextSeq;
        } else {
            func_ov170_021ea1c0(syswk);
            syswk->unk1A = 1;
            func_ov170_021ecb20(syswk, 2, 0);
            syswk->app->unkA194++;
            return func_ov170_021e8fc0(syswk, func_ov170_021e002c, 0x5b);
        }
    case 3:
        syswk->app->unkA194 = 0;
        func_ov170_021ea1e8(syswk);
        func_ov170_021defec(syswk, syswk->pos);
        func_020260e4(syswk->app->cursorMove, (u8)(syswk->pos - 0x1e));
        func_ov170_021ee3e4(syswk, syswk->pos - 0x1e);
        func_ov170_021ed55c(syswk, 1);
        syswk->pos = 0xff;
        if (syswk->param->mode == 0) {
            return 0x31;
        }
        func_ov170_021ed508(syswk, 0xe, 1);
        func_ov170_021ef68c(syswk->app);
        return 0x1b;
    }
    return 0x5b;
}


int func_ov170_021e8340(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        if (func_ov170_021ec9d8(syswk->app->unkA190) == 0) {
            func_ov170_021eca38(syswk->app->unkA190);
            func_ov170_021de7b4(syswk);
            func_ov170_021ed588(syswk);
            func_ov170_021ebbc8(syswk, 4, 0x18);
            syswk->nextSeq = 0x5c;
            syswk->app->unkA194++;
            return 6;
        }
        break;
    case 1:
        func_ov170_021ebbc8(syswk, 5, 0x18);
        syswk->nextSeq = 0x5c;
        syswk->app->unkA194++;
        return 6;
    case 2:
        syswk->app->unkA194 = 0;
        return func_ov170_021e912c(syswk);
    }
    return 0x5c;
}

int func_ov170_021e83c0(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        syswk->app->unkA194++;
        func_ov170_021ec000(syswk->app, 4, 0);
        func_ov170_021ed508(syswk, 0xe, 0);
        func_ov170_021ed888(syswk->app, 0);
        if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == 1) {
            if (func_ov170_021ec01c(syswk->app, 0xa) == 1) {
                func_ov170_021ec000(syswk->app, 0xa, 0);
            }
            func_ov170_021ed6e4(syswk, 0);
            func_ov170_021eba5c(syswk->app, 0x18);
            func_ov170_021dd14c(syswk);
            func_ov170_021ef57c(syswk->app->bgWinFrame);
            return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x5d);
        }
        if (func_ov170_021ef770(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ee008(syswk, 0, 0);
            func_ov170_021ef680(syswk->app);
            return func_ov170_021ea858(syswk, 0x5d);
        }
        if (func_ov170_021ef45c(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ef69c(syswk->app);
            func_ov170_021ef3a0(syswk->app->bgWinFrame);
            return func_ov170_021e8fc0(syswk, func_ov170_021e01b0, 0x5d);
        }
        func_ov170_021ef680(syswk->app);
        /* fall through */
    case 1:
        func_ov170_021eb988(syswk, data_ov170_021ef908, 4);
        func_ov170_021ef534(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x5d);
    case 2:
        syswk->app->unkA194 = 0;
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ebc28(syswk, 0, 0x18);
        func_ov170_021ee364(syswk, 0xa, 0);
        syswk->moveMode = 0;
        func_ov170_021ed544(syswk, 6);
        return 0x5e;
    }
    return 0x5d;
}


int func_ov170_021e8508(Box2SysWork *syswk) {
    u32 seq;
    switch (func_02025de8(syswk->app->cursorMove)) {
    case 0:
        GFL_SndSEPlay(0x54c);
        seq = func_ov170_021e8f50(syswk, 0x60);
        return func_ov170_021e949c(syswk, 2, seq);
    case 1:
        GFL_SndSEPlay(0x54c);
        syswk->app->unkA17C = 0;
        seq = func_ov170_021e8f50(syswk, 0x64);
        return func_ov170_021e949c(syswk, 3, seq);
    case 2:
        GFL_SndSEPlay(0x54c);
        return func_ov170_021e949c(syswk, 4, 0x67);
    case 3:
        GFL_SndSEPlay(0x551);
        seq = func_ov170_021e8f50(syswk, 0x5f);
        return func_ov170_021e949c(syswk, 5, seq);
    case -2:
        GFL_SndSEPlay(0x551);
        seq = func_ov170_021e8f50(syswk, 0x5f);
        return func_ov170_021e949c(syswk, 5, seq);
    case -3:
        GFL_SndSEPlay(0x548);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x5e));
    }
    return 0x5e;
}

int func_ov170_021e85f0(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ee038(syswk, 0, 0, 1);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x5f);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ed508(syswk, 0xe, 1);
        if (syswk->param->mode == 2) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 3, 0x1e);
            return 0x11;
        } else if (syswk->param->mode == 1) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ee364(syswk, 2, 0x1e);
            return 0x2c;
        } else if (syswk->param->mode == 3) {
            func_ov170_021ecb20(syswk, 0x81, 1);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 7, 0x1e);
            return 0x39;
        } else if (syswk->param->mode == 4) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 0xd, 0x1e);
            return 0x23;
        }
        break;
    }
    return 0x5f;
}

int func_ov170_021e86ec(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        syswk->app->unkA17A[0] = 1;
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x60);
    case 1:
        syswk->trayScroll = syswk->tray;
        syswk->app->unkA194++;
        GFL_SndSEPlay(0x669);
        return func_ov170_021ea83c(syswk, 0x60);
    case 2:
        syswk->app->unkA194 = 0;
        func_ov170_021ee008(syswk, 0, 1);
        func_ov170_021ee364(syswk, 0xc, 1);
        syswk->app->curPos = 1;
        func_ov170_021ebc28(syswk, 1, 0x19);
        func_ov170_021ee038(syswk, 0, 1, 1);
        return 0x61;
    }
    return 0x60;
}

int func_ov170_021e87a0(Box2SysWork *syswk) {
    u32 x;
    u32 y;
    u32 res;

    if (func_ov170_021ef1d4(&x, &y) == TRUE) {
        syswk->app->tpy = y;
        syswk->nextSeq = 0x61;
        func_ov170_021ea808(syswk);
        func_020260f4(syswk->app->cursorMove, FALSE);
        return 0xf;
    }
    res = func_02025de8(syswk->app->cursorMove);
    switch (res) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, -1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e1acc, 0x61);
    case 5:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021dd1c4(syswk, 1);
        return func_ov170_021e8fc0(syswk, func_ov170_021e1a78, 0x61);
    case 1:
    case 2:
    case 3:
    case 4:
        syswk->app->unkA17F = syswk->trayScroll + res - 1;
        if (syswk->app->unkA17F >= syswk->trayMax) {
            syswk->app->unkA17F -= syswk->trayMax;
        }
        if (syswk->app->unkA17F != syswk->tray) {
            func_ov170_021ee008(syswk, 0, FALSE);
            return func_ov170_021ea798(syswk, 1);
        }
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov170_021ee008(syswk, res - 1, TRUE);
        break;
    case 6:
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return func_ov170_021e954c(syswk, 6, 9, func_ov170_021e8f50(syswk, 0x62));
    case CURSORMOVE_CURSOR_MOVE:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x61));
    case CURSORMOVE_UNK_8:
        if (func_020260e0(syswk->app->cursorMove) == 1) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021dd1c4(syswk, -1);
            return func_ov170_021e8fc0(syswk, func_ov170_021e1acc, 0x61);
        }
        break;
    case CURSORMOVE_UNK_7:
        if (func_020260e0(syswk->app->cursorMove) == 4) {
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov170_021dd1c4(syswk, 1);
            return func_ov170_021e8fc0(syswk, func_ov170_021e1a78, 0x61);
        }
        break;
    }
    return 0x61;
}


int func_ov170_021e8950(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        syswk->app->unkA17A[0] = 0;
        func_ov170_021ed55c(syswk, 0);
        func_ov170_021eba5c(syswk->app, 0x19);
        func_ov170_021dd14c(syswk);
        func_ov170_021ee008(syswk, 0, 0);
        GFL_SndSEPlay(0x669);
        syswk->app->unkA194++;
        return func_ov170_021ea858(syswk, 0x62);
    case 1:
        syswk->app->unkA194 = 0;
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ee038(syswk, 0, 0, 1);
        func_ov170_021ed508(syswk, 0xe, 1);
        if (syswk->param->mode == 2) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 3, 0x1e);
            return 0x11;
        } else if (syswk->param->mode == 1) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ee364(syswk, 2, 0x1e);
            return 0x2c;
        } else if (syswk->param->mode == 3) {
            func_ov170_021ecb20(syswk, 0x81, 1);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 7, 0x1e);
            return 0x39;
        } else if (syswk->param->mode == 4) {
            func_ov170_021ecb20(syswk, 1, 0);
            func_ov170_021ef670(syswk->app);
            func_ov170_021ee364(syswk, 0xd, 0x1e);
            return 0x23;
        }
        break;
    }
    return 0x62;
}


int func_ov170_021e8a58(Box2SysWork *syswk) {
    func_ov170_021ee008(syswk, func_020260e0(syswk->app->cursorMove) - 1, 1);
    return 0x61;
}

int func_ov170_021e8a74(Box2SysWork *syswk) {
    BOOL flag;

    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, FALSE);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x64);
    case 1:
        if (syswk->app->unkA17C == 0) {
            if (func_02007a80(syswk->param->boxes, 1) == TRUE) {
                func_ov170_021eb988(syswk, data_ov170_021ef9c8, 6);
            } else {
                func_ov170_021eb988(syswk, data_ov170_021ef93c, 5);
            }
        } else if (syswk->app->unkA17C == 1) {
            func_ov170_021eb988(syswk, data_ov170_021ef964, 5);
        } else if (syswk->app->unkA17C == 2) {
            func_ov170_021eb988(syswk, data_ov170_021ef978, 5);
        } else if (syswk->app->unkA17C == 3) {
            func_ov170_021eb988(syswk, data_ov170_021ef98c, 5);
        } else if (syswk->app->unkA17C == 4) {
            func_ov170_021eb988(syswk, data_ov170_021ef9a0, 5);
        } else if (syswk->app->unkA17C == 5) {
            if (func_02007a80(syswk->param->boxes, 2) == TRUE) {
                func_ov170_021eb988(syswk, data_ov170_021ef9e0, 6);
            } else {
                func_ov170_021eb988(syswk, data_ov170_021ef928, 5);
            }
        } else if (syswk->app->unkA17C == 6) {
            func_ov170_021eb988(syswk, data_ov170_021ef9f8, 6);
        }
        func_ov170_021ef534(syswk->app->bgWinFrame);
        syswk->app->unkA194++;
        return func_ov170_021e8fc0(syswk, func_ov170_021e0190, 0x64);
    case 2:
        syswk->app->unkA194 = 0;
        if (syswk->app->unkA17C == 0) {
            func_ov170_021ebc28(syswk, 2, 0x18);
        } else {
            func_ov170_021ebc28(syswk, 3, 0x18);
        }
        flag = FALSE;
        if (syswk->app->unkA17C == 0) {
            if (func_02007a80(syswk->param->boxes, 1) == FALSE) {
                flag = TRUE;
            }
        } else if (syswk->app->unkA17C == 5) {
            if (func_02007a80(syswk->param->boxes, 2) == FALSE) {
                flag = TRUE;
            }
        } else if (syswk->app->unkA17C != 6) {
            flag = TRUE;
        }
        if (flag == TRUE) {
            func_ov170_021ee364(syswk, 0xb, 1);
            func_02026124(syswk->app->cursorMove, 0);
        } else {
            func_ov170_021ee364(syswk, 0xb, 0);
        }
        return 0x65;
    }
    return 0x64;
}

int func_ov170_021e8bf8(Box2SysWork *syswk) {
    switch (func_02025de8(syswk->app->cursorMove)) {
    case 0:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->unkA17C == 0) {
            syswk->app->unkA17C = 5;
            return func_ov170_021e949c(syswk, 0, func_ov170_021e8f50(syswk, 0x64));
        }
        return func_ov170_021ea928(syswk, 0);
    case 1:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->unkA17C == 0) {
            syswk->app->unkA17C = 1;
            return func_ov170_021e949c(syswk, 1, func_ov170_021e8f50(syswk, 0x64));
        }
        return func_ov170_021ea928(syswk, 1);
    case 2:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->unkA17C == 0) {
            syswk->app->unkA17C = 2;
            return func_ov170_021e949c(syswk, 2, func_ov170_021e8f50(syswk, 0x64));
        }
        return func_ov170_021ea928(syswk, 2);
    case 3:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->unkA17C == 0) {
            syswk->app->unkA17C = 3;
            return func_ov170_021e949c(syswk, 3, func_ov170_021e8f50(syswk, 0x64));
        }
        return func_ov170_021ea928(syswk, 3);
    case 4:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (syswk->app->unkA17C == 0) {
            syswk->app->unkA17C = 4;
            return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x64));
        }
        if (syswk->app->unkA17C == 5 && func_02007a80(syswk->param->boxes, 2) == TRUE) {
            syswk->app->unkA17C = 6;
            return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x64));
        }
        if (syswk->app->unkA17C == 6) {
            syswk->app->unkA17C = 5;
            return func_ov170_021e949c(syswk, 4, func_ov170_021e8f50(syswk, 0x64));
        }
        return func_ov170_021ea928(syswk, 4);
    case 5:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (syswk->app->unkA17C == 0) {
            return func_ov170_021e949c(syswk, 5, func_ov170_021e8f50(syswk, 0x5f));
        }
        syswk->app->unkA17C = 0;
        return func_ov170_021e949c(syswk, 5, func_ov170_021e8f50(syswk, 0x64));
    case CURSORMOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        if (syswk->app->unkA17C == 0) {
            return func_ov170_021e949c(syswk, 5, func_ov170_021e8f50(syswk, 0x5f));
        }
        syswk->app->unkA17C = 0;
        return func_ov170_021e949c(syswk, 5, func_ov170_021e8f50(syswk, 0x64));
    case CURSORMOVE_CURSOR_MOVE:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        return func_ov170_021e8fd0(syswk, func_ov170_021e8fe0(syswk, 0x65));
    }
    return 0x65;
}


int func_ov170_021e8e24(Box2SysWork *syswk) {
    switch (syswk->app->unkA194) {
    case 0:
        func_ov170_021ed55c(syswk, FALSE);
        func_020216d4(syswk->app->palFade, 0, 0, 0x200);
        func_0202173c(syswk->app->palFade, 1, 0xc000, 0, 0, 16, 0x7fff, GFL_VBlankGetTCBMgr());
        syswk->nextSeq = 102;
        syswk->app->unkA194++;
        return 3;
    case 1:
        func_ov170_021ded58(syswk, syswk->app->unkA17A[1]);
        func_02007838(syswk->param->boxes, syswk->tray, syswk->app->unkA17A[1]);
        func_ov170_021edab0(syswk, syswk->tray);
        syswk->app->unkA194++;
        break;
    case 2:
        func_0202173c(syswk->app->palFade, 1, 0xc000, 0, 16, 0, 0x7fff, GFL_VBlankGetTCBMgr());
        syswk->nextSeq = 102;
        syswk->app->unkA194++;
        return 3;
    case 3:
        func_ov170_021ed55c(syswk, TRUE);
        syswk->app->unkA194 = 0;
        return 101;
    }
    return 102;
}


int func_ov170_021e8f14(Box2SysWork *syswk) {
    return func_ov170_021e95cc(syswk, 2);
}

int func_ov170_021e8f20(Box2SysWork *syswk) {
    func_ov170_021ec000(syswk->app, 4, 0);
    func_ov170_021ed508(syswk, 0xe, 0);
    func_ov170_021ebadc(syswk, 0x18);
    func_ov170_021ea984(syswk);
    return func_ov170_021e8fe8(syswk, 4);
}


u32 func_ov170_021e8f50(Box2SysWork *syswk, u32 seq) {
    syswk->app->unkA194 = 0;
    return seq;
}

int func_ov170_021e8f60(Box2SysWork *syswk, u32 seq) {
    GFL_WipeSet(0, 1, 1, 0, 6, 1, 0x4c);
    syswk->app->oldCurPos = seq;
    return 2;
}

int func_ov170_021e8f90(Box2SysWork *syswk, u32 seq) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, 0x4c);
    syswk->app->oldCurPos = seq;
    return 2;
}

int func_ov170_021e8fc0(Box2SysWork *syswk, Box2VFunc func, u32 val) {
    syswk->app->unk68 = val;
    func_ov170_021dc9a0(syswk->app, func);
    return 5;
}

int func_ov170_021e8fd0(Box2SysWork *syswk, u32 val) {
    syswk->app->unk68 = val;
    func_ov170_021dc9b8(syswk->app);
    return 5;
}


int func_ov170_021e8fe0(Box2SysWork *syswk, int seq) {
    syswk->nextSeq = seq;
    return 13;
}

int func_ov170_021e8fe8(Box2SysWork *syswk, s16 flag) {
    syswk->app->ynID = flag;
    if (flag == 1) {
        func_ov170_021dcf58(syswk, 1);
    } else {
        func_ov170_021dcf58(syswk, 0);
    }
    setKeypressFramecounts(syswk->app->keyRepeatWait, syswk->app->keyRepeatStart);
    return 7;
}


// Takes the Pokémon's held item into the bag
int func_ov170_021e9010(Box2SysWork *syswk) {
    u32 item = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, 0);
    if (BagSave_AddItem(syswk->param->bag, item, 1, HEAPID_BOX2_APP) == 1) {
        BoxPkm *pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        func_ov170_021ebb34(syswk, item, 0x18);
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 6, 0);
        if (func_ov170_021dd718(syswk, pkm) == 1) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
        func_ov170_021deff8(syswk, syswk->pos);
    } else {
        GFL_SndSEPlay(0x557);
        func_ov170_021ebb58(syswk, 0x18);
    }
    if (func_020355c4() == 1) {
        func_020260f4(syswk->app->cursorMove, 0);
    } else {
        func_020260f4(syswk->app->cursorMove, 1);
        func_ov170_021ed55c(syswk, 0);
    }
    syswk->nextSeq = 0xe;
    return func_ov170_021e8f50(syswk, 6);
}

int func_ov170_021e90d4(Box2SysWork *syswk) {
    func_ov170_021ed6e4(syswk, 0);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021de728(syswk);
    func_ov170_021ec94c(syswk->app->unkA190);
    if (func_020355c4() == 1) {
        func_020260f4(syswk->app->cursorMove, 0);
    } else {
        func_020260f4(syswk->app->cursorMove, 1);
        func_ov170_021ed55c(syswk, 0);
    }
    return 0x5a;
}

int func_ov170_021e912c(Box2SysWork *syswk) {
    func_ov170_021ed6e4(syswk, 1);
    func_ov170_021eba70(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->pos < 0x1e) {
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 1, 0);
    } else {
        func_ov170_021ecb20(syswk, 2, 0);
    }
    if (func_020355c4() == 1) {
        func_020260f4(syswk->app->cursorMove, 0);
    } else {
        func_020260f4(syswk->app->cursorMove, 1);
        func_ov170_021ed55c(syswk, 0);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0190, func_ov170_021e8f50(syswk, 0xe));
}

int func_ov170_021e91a0(Box2SysWork *syswk) {
    BoxPkm *pkm;
    if (func_020355c4() == 1) {
        func_020260f4(syswk->app->cursorMove, 0);
    } else {
        func_020260f4(syswk->app->cursorMove, 1);
        func_ov170_021ed55c(syswk, 0);
    }
    if (BagSave_AddItem(syswk->param->bag, syswk->app->itemId, 1, HEAPID_BOX2_APP) == 1) {
        pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 6, 0);
        if (func_ov170_021dd718(syswk, pkm) == 1) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
        func_ov170_021defec(syswk, syswk->pos);
        func_ov170_021ed6e4(syswk, 0);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        func_ov170_021ef670(syswk->app);
        func_ov170_021ed108(syswk->app, 1);
        func_ov170_021ebfa8(syswk->app, 0xa, 2);
        return 0x3b;
    }
    GFL_SndSEPlay(0x557);
    func_ov170_021ebcb4(syswk, 6, 0x18);
    syswk->nextSeq = 0xe;
    return func_ov170_021e8f50(syswk, 6);
}


int func_ov170_021e9284(Box2SysWork *syswk) {
    GFL_SndSEPlay(SEQ_SE_PC_LOGOFF);
    func_ov170_021de898(syswk);
    syswk->nextSeq = 0x69;
    return func_ov170_021e8f90(syswk, 1);
}


// Closes a message and goes back to the mode's main state
int func_ov170_021e92a8(Box2SysWork *syswk) {
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (func_020355c4() == 1) {
        func_020260f4(syswk->app->cursorMove, 0);
    } else {
        func_020260f4(syswk->app->cursorMove, 1);
    }
    func_ov170_021ed508(syswk, 0xe, 1);
    switch (syswk->param->mode) {
    case 0:
        func_ov170_021ecb20(syswk, 2, 0);
        return 0x31;
    case 1:
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 1, 0);
        return 0x2c;
    case 2:
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ecb20(syswk, 2, 0);
            func_ov170_021ef68c(syswk->app);
            return 0x1b;
        }
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 1, 0);
        func_ov170_021ef670(syswk->app);
        return 0x11;
    case 3:
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ecb20(syswk, 0x82, 0);
            func_ov170_021ef68c(syswk->app);
            return 0x46;
        }
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 0x81, 0);
        func_ov170_021ef670(syswk->app);
        return 0x39;
    case 4:
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ecb20(syswk, 2, 0);
            func_ov170_021ef68c(syswk->app);
            return 0x27;
        }
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 1, 0);
        func_ov170_021ef670(syswk->app);
        return 0x23;
    case 5:
        func_ov170_021ed888(syswk->app, 1);
        func_ov170_021ecb20(syswk, 0x81, 1);
        return 0x4b;
    }
    return 0x11;
}

int func_ov170_021e93f4(Box2SysWork *syswk) {
    func_ov170_021de898(syswk);
    syswk->param->tray = syswk->tray;
    syswk->param->position = syswk->pos;
    syswk->nextSeq = 0x69;
    return func_ov170_021e8f90(syswk, 1);
}


// Mode 5: the Pokémon wasn't picked
int func_ov170_021e941c(Box2SysWork *syswk) {
    func_ov170_021eba5c(syswk->app, 27);
    func_ov170_021dd14c(syswk);
    func_020260e4(syswk->app->cursorMove, syswk->pos);
    func_ov170_021ecb20(syswk, 0x81, 1);
    func_ov170_021ed6e4(syswk, 0);
    if (func_020355c4() == TRUE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
    } else {
        func_020260f4(syswk->app->cursorMove, TRUE);
    }
    return 75;
}

int func_ov170_021e946c(Box2SysWork *syswk) {
    if (func_020355c4() == TRUE) {
        func_020260f4(syswk->app->cursorMove, FALSE);
    } else {
        func_020260f4(syswk->app->cursorMove, TRUE);
        func_ov170_021ed55c(syswk, 0);
    }
    return func_ov170_021e2334(syswk);
}

// Animates a frame's button, then goes to seq
int func_ov170_021e949c(Box2SysWork *syswk, u32 frame, int seq) {
    func_ov170_021dd090(syswk, frame);
    syswk->nextSeq = seq;
    return BOX2SEQ_BUTTON_ANM;
}

int func_ov170_021e94ac(Box2SysWork *syswk, u32 button, int seq) {
    syswk->app->bawk.mode = BOX2_BTN_ANM_MODE_BG;
    syswk->app->bawk.id = func_0202cf8c(syswk->app->bgWinFrame, 7);
    syswk->app->bawk.pal1 = 13;
    syswk->app->bawk.pal2 = 12;
    syswk->app->bawk.seq = 0;
    syswk->app->bawk.cnt = 0;
    if (button == 0) {
        syswk->app->bawk.py = 14;
    } else {
        syswk->app->bawk.py = 17;
    }
    syswk->app->bawk.px = 21;
    syswk->app->bawk.sx = 11;
    syswk->app->bawk.sy = 3;
    syswk->nextSeq = seq;
    return BOX2SEQ_BUTTON_ANM;
}


int func_ov170_021e954c(Box2SysWork *syswk, u32 id, u32 pal, int seq) {
    syswk->app->bawk.mode = BOX2_BTN_ANM_MODE_OBJ;
    syswk->app->bawk.id = id;
    syswk->app->bawk.pal1 = pal;
    syswk->app->bawk.pal2 = 0;
    syswk->app->bawk.seq = 0;
    syswk->app->bawk.cnt = 0;
    syswk->app->bawk.px = 0;
    syswk->app->bawk.py = 0;
    syswk->app->bawk.sx = 0;
    syswk->app->bawk.sy = 0;
    syswk->nextSeq = seq;
    return BOX2SEQ_BUTTON_ANM;
}


int func_ov170_021e95cc(Box2SysWork *syswk, u8 type) {
    if (func_020260f0(syswk->app->cursorMove) == 1) {
        func_020355d8(0);
    } else {
        func_020355d8(1);
    }
    syswk->nextSeq = 9;
    syswk->subProcType = type;
    return func_ov170_021e8f90(syswk, 1);
}

int func_ov170_021e95f8(Box2SysWork *syswk) {
    u32 seq;
    switch (syswk->unk13) {
    case 0:
        func_ov170_021ef670(syswk->app);
        seq = 0x11;
        break;
    case 1:
        func_ov170_021ea75c(syswk);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ee364(syswk, 5, 0xb);
        func_ov170_021ef68c(syswk->app);
        seq = 0x1b;
        break;
    }
    func_ov170_021ed588(syswk);
    if (syswk->subRet == 0) {
        func_ov170_021defec(syswk, syswk->pos);
        func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        return seq;
    }
    if (PML_ItemIsMail(syswk->subRet) == 0) {
        BoxPkm *pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 6, syswk->subRet);
        if (func_ov170_021dd718(syswk, pkm) == 1) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
        BagSave_SubItem(syswk->param->bag, syswk->subRet, 1, HEAPID_TAIL(HEAPID_BOX2_APP));
    }
    func_ov170_021defec(syswk, syswk->pos);
    func_ov170_021ed55c(syswk, 0);
    return 0x54;
}

int func_ov170_021e96e8(Box2SysWork *syswk) {
    u32 seq;
    switch (syswk->unk13) {
    case 0:
        func_ov170_021ed164(syswk->app, syswk->pos, 0);
        seq = 0x39;
        break;
    case 1:
        func_ov170_021ea75c(syswk);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ed164(syswk->app, syswk->pos, 1);
        func_ov170_021ee364(syswk, 8, 0xa);
        seq = 0x46;
        break;
    }
    func_ov170_021ea878(syswk, syswk->subRet);
    if (syswk->subRet == 0) {
        func_ov170_021defec(syswk, syswk->pos);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebcb4(syswk, 0, 0x18);
        if (syswk->unk13 == 0) {
            func_ov170_021ecb20(syswk, 0x81, 1);
        } else {
            func_ov170_021ecb20(syswk, 1, 1);
            func_ov170_021ecb20(syswk, 0x82, 1);
        }
        return seq;
    }
    if (PML_ItemIsMail(syswk->subRet) == 0) {
        BoxPkm *pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 6, syswk->subRet);
        if (func_ov170_021dd718(syswk, pkm) == 1) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
        BagSave_SubItem(syswk->param->bag, syswk->subRet, 1, HEAPID_TAIL(HEAPID_BOX2_APP));
        syswk->app->itemId = syswk->subRet;
        func_ov170_021ed094(syswk->app, syswk->app->itemId);
        func_ov170_021ec000(syswk->app, 0xa, 1);
        func_ov170_021ed1c8(syswk->app);
        func_ov170_021ed2f4(syswk->app);
    }
    if (syswk->unk13 == 0) {
        func_ov170_021ecb20(syswk, 0x81, 1);
        func_ov170_021ef670(syswk->app);
    } else {
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ecb20(syswk, 0x82, 1);
        func_ov170_021ef68c(syswk->app);
    }
    func_ov170_021defec(syswk, syswk->pos);
    func_ov170_021ed55c(syswk, 0);
    return 0x54;
}


int func_ov170_021e9848(Box2SysWork *syswk) {
    u32 seq;
    switch (syswk->unk13) {
    case 0:
        func_ov170_021ef670(syswk->app);
        seq = 0x23;
        break;
    case 1:
        func_ov170_021ea75c(syswk);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ee364(syswk, 0xe, 0xb);
        func_ov170_021ef68c(syswk->app);
        seq = 0x27;
        break;
    }
    func_ov170_021ed588(syswk);
    if (syswk->subRet == 0) {
        func_ov170_021defec(syswk, syswk->pos);
        func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        return seq;
    }
    if (PML_ItemIsMail(syswk->subRet) == 0) {
        BoxPkm *pkm = func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos);
        func_ov170_021dd2b4(syswk, syswk->pos, syswk->tray, 6, syswk->subRet);
        if (func_ov170_021dd718(syswk, pkm) == 1) {
            func_ov170_021dd8a8(syswk, syswk->pos);
            func_ov170_021ec48c(syswk, syswk->tray, syswk->pos, syswk->app->pokeIconId[syswk->pos]);
        }
        BagSave_SubItem(syswk->param->bag, syswk->subRet, 1, HEAPID_TAIL(HEAPID_BOX2_APP));
    }
    func_ov170_021defec(syswk, syswk->pos);
    func_ov170_021ed55c(syswk, 0);
    return 0x54;
}


int func_ov170_021e9938(Box2SysWork *syswk) {
    func_ov170_021eb988(syswk, data_ov170_021ef9b4, 5);
    func_ov170_021ef4e4(syswk->app->bgWinFrame);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    func_ov170_021ed588(syswk);
    return 0x2c;
}

int func_ov170_021e9968(Box2SysWork *syswk) {
    func_ov170_021eb988(syswk, data_ov170_021ef950, 5);
    func_ov170_021ef4e4(syswk->app->bgWinFrame);
    func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
    func_ov170_021ecb20(syswk, 1, 1);
    func_ov170_021ed888(syswk->app, 0);
    func_ov170_021ea75c(syswk);
    func_ov170_021ed588(syswk);
    return 0x31;
}

int func_ov170_021e99b0(Box2SysWork *syswk) {
    switch (syswk->unk13) {
    case 0:
        func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        func_ov170_021ed588(syswk);
        return 0x11;
    case 1:
        func_ov170_021eb988(syswk, data_ov170_021efa10, 6);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ee364(syswk, 5, 0xa);
        func_ov170_021ea75c(syswk);
        func_ov170_021ed588(syswk);
        return 0x1b;
    case 2:
        if (syswk->unk1B == 1) {
            func_ov170_021ec61c(syswk);
        }
        func_ov170_021ee364(syswk, 4, syswk->pos);
        func_ov170_021dd1e8(syswk);
        while (func_ov170_021e01c4(syswk) != 0) {
        }
        func_ov170_021edf5c(syswk);
        func_ov170_021ef670(syswk->app);
        syswk->pos = 0xff;
        return 0x15;
    case 3:
        if (syswk->pos >= 0x1e) {
            func_ov170_021ec61c(syswk);
            func_ov170_021ecb84(syswk->app, syswk->pos);
        } else if (syswk->tray == syswk->getTray) {
            func_ov170_021ecb84(syswk->app, syswk->pos);
        } else {
            func_ov170_021ec48c(syswk, syswk->getTray, syswk->pos, syswk->app->pokeIconId[0x24]);
        }
        func_ov170_021e072c(syswk);
        func_ov170_021ed75c(syswk, 0x24);
        func_ov170_021ee364(syswk, 4, syswk->unk1D);
        func_ov170_021dd1e8(syswk);
        while (func_ov170_021e01c4(syswk) != 0) {
        }
        func_ov170_021edf5c(syswk);
        func_ov170_021ef670(syswk->app);
        if (syswk->unk1D >= 0x22 && syswk->unk1D <= 0x25) {
            func_ov170_021ee008(syswk, syswk->unk1D - 0x22, 1);
        }
        func_ov170_021ed594(syswk, 0x24);
        return 0x15;
    case 4:
        func_ov170_021ea770(syswk);
        func_ov170_021ee364(syswk, 6, syswk->pos);
        func_ov170_021ef68c(syswk->app);
        syswk->pos = 0xff;
        return 0x20;
    case 5:
        func_ov170_021ea770(syswk);
        if (syswk->pos < 0x1e && syswk->tray != syswk->getTray) {
            func_ov170_021ec48c(syswk, syswk->getTray, syswk->pos, syswk->app->pokeIconId[0x24]);
        } else {
            func_ov170_021ecb84(syswk->app, syswk->pos);
        }
        func_ov170_021e072c(syswk);
        func_ov170_021ed75c(syswk, 0x24);
        func_ov170_021ee364(syswk, 6, syswk->unk1D);
        func_ov170_021ef68c(syswk->app);
        func_ov170_021ed594(syswk, 0x24);
        return 0x20;
    }
    return 0;
}


int func_ov170_021e9b74(Box2SysWork *syswk) {
    switch (syswk->unk13) {
    case 0:
        func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        func_ov170_021ee364(syswk, 0xd, 0x25);
        func_ov170_021ed588(syswk);
        return 0x23;
    case 1:
        func_ov170_021eb988(syswk, data_ov170_021ef918, 4);
        func_ov170_021ef4e4(syswk->app->bgWinFrame);
        func_ov170_021ebc58(syswk, syswk->pos, 1, 0x18);
        func_ov170_021ecb20(syswk, 1, 1);
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ee364(syswk, 0xe, 0xa);
        func_ov170_021ea75c(syswk);
        func_ov170_021ed588(syswk);
        return 0x27;
    case 4:
        func_ov170_021ea770(syswk);
        func_ov170_021ee364(syswk, 6, syswk->pos);
        func_02026124(syswk->app->cursorMove, 0x27);
        syswk->pos = 0xff;
        return 0x20;
    case 5:
        func_ov170_021ea770(syswk);
        if (syswk->pos < 0x1e && syswk->tray != syswk->getTray) {
            func_ov170_021ec48c(syswk, syswk->getTray, syswk->pos, syswk->app->pokeIconId[0x24]);
        } else {
            func_ov170_021ecb84(syswk->app, syswk->pos);
        }
        func_ov170_021e072c(syswk);
        func_ov170_021ed75c(syswk, 0x24);
        func_ov170_021ee364(syswk, 6, syswk->unk1D);
        func_02026124(syswk->app->cursorMove, 0x27);
        func_ov170_021ed594(syswk, 0x24);
        return 0x20;
    }
    return 0;
}


int func_ov170_021e9c94(Box2SysWork *syswk, u32 a, u32 b) {
    func_ov170_021dd16c(syswk, 0);
    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        if (a == 1) {
            func_ov170_021ef670(syswk->app);
        }
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e00c4, b);
}

int func_ov170_021e9ce0(Box2SysWork *syswk, u32 a, u32 b) {
    func_ov170_021dd16c(syswk, 1);
    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == TRUE) {
        func_ov170_021ef57c(syswk->app->bgWinFrame);
        func_ov170_021eba5c(syswk->app, 0x18);
        func_ov170_021dd14c(syswk);
        if (a == 1) {
            func_ov170_021ef670(syswk->app);
        }
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0128, b);
}

int func_ov170_021e9d2c(Box2SysWork *syswk, u32 pos) {
    syswk->unk1B = 0;
    syswk->moveMode = 0;
    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    syswk->unk1B = 0;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef670(syswk->app);
    syswk->nextSeq = 0x15;
    syswk->unk1A = 0;
    return func_ov170_021e8fc0(syswk, func_ov170_021dfa30, 0x12);
}

int func_ov170_021e9d98(Box2SysWork *syswk, u32 pos) {
    syswk->getTray = syswk->tray;
    syswk->pos = pos;
    syswk->unk1B = 0;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    syswk->unk1A = 0;
    syswk->nextSeq = 0x20;
    return func_ov170_021e8fc0(syswk, func_ov170_021e1b1c, 0x24);
}

int func_ov170_021e9df8(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ed888(syswk->app, 0);
    func_ov170_021ec61c(syswk);
    func_ov170_021ecb20(syswk, 1, 1);
    func_ov170_021ecb20(syswk, 2, 1);
    func_ov170_021ecb60(syswk->app, 0x24, 0);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    syswk->unk1A = 2;
    return func_ov170_021e8fc0(syswk, func_ov170_021e0470, 0x30);
}


int func_ov170_021e9e7c(Box2SysWork *syswk, u32 pos, u32 val) {
    syswk->pos = pos;
    syswk->app->unkA172 = 1;
    func_ov170_021ec768(syswk->app, syswk->pos, 1);
    func_ov170_021ed588(syswk);
    func_ov170_021ef534(syswk->app->bgWinFrame);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, val));
}

int func_ov170_021e9ec0(Box2SysWork *syswk, u32 pos) {
    syswk->moveMode = 0;
    syswk->getTray = syswk->tray;
    syswk->pos = pos;
    syswk->unk1B = 1;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    PokeParty_GetPkm(syswk->param->party, pos - 0x1e);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    func_ov170_021ef68c(syswk->app);
    syswk->nextSeq = 0x20;
    syswk->unk1A = 1;
    return func_ov170_021e8fc0(syswk, func_ov170_021df484, 0x1e);
}


int func_ov170_021e9f3c(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    syswk->unk1B = 1;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    PokeParty_GetPkm(syswk->param->party, pos - 0x1e);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    syswk->nextSeq = 0x20;
    syswk->unk1A = 1;
    return func_ov170_021e8fc0(syswk, func_ov170_021df484, 0x2a);
}

int func_ov170_021e9fa8(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    func_ov170_021ec768(syswk->app, syswk->pos, 0);
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ecb20(syswk, 2, 1);
    func_ov170_021ecb60(syswk->app, 0x24, 0);
    PokeParty_GetPkm(syswk->param->party, pos - 0x1e);
    func_ov170_021ea1c0(syswk);
    func_ov170_021ee038(syswk, 2, 2, 1);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    syswk->unk1A = 1;
    return func_ov170_021e8fc0(syswk, func_ov170_021e0200, 0x38);
}


int func_ov170_021ea020(Box2SysWork *syswk, u32 pos, u32 val) {
    syswk->pos = pos;
    syswk->app->unkA172 = 1;
    func_ov170_021ec768(syswk->app, syswk->pos, 1);
    func_ov170_021ed588(syswk);
    func_ov170_021ef534(syswk->app->bgWinFrame);
    return func_ov170_021e8fc0(syswk, func_ov170_021e070c, func_ov170_021e8fe0(syswk, val));
}

int func_ov170_021ea064(Box2SysWork *syswk, u32 pos, u32 val) {
    syswk->getTray = syswk->tray;
    syswk->pos = pos;
    syswk->app->curPos = pos;
    func_ov170_021ecb84(syswk->app, syswk->pos);
    func_ov170_021ec768(syswk->app, 0x24, 0);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ee038(syswk, 2, 1, 2);
    func_ov170_021ea1c0(syswk);
    syswk->nextSeq = val;
    syswk->unk1A = 2;
    return func_ov170_021e8fc0(syswk, func_ov170_021dfa30, 0x17);
}

int func_ov170_021ea0bc(Box2SysWork *syswk, u32 pos) {
    syswk->unk18 = 1;
    syswk->pos = pos;
    func_ov170_021ec768(syswk->app, 0x24, 0);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ee038(syswk, 2, 1, 0);
    if (syswk->param->mode == 4) {
        func_ov170_021ef680(syswk->app);
        func_ov170_021ef69c(syswk->app);
        func_02026124(syswk->app->cursorMove, 0x27);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0778, 0x20);
}


int func_ov170_021ea114(Box2SysWork *syswk, u32 pos) {
    syswk->unk18 = 1;
    syswk->pos = pos;
    func_ov170_021ec768(syswk->app, 0x24, 0);
    func_ov170_021ed594(syswk, 0x24);
    func_ov170_021ee038(syswk, 2, 1, 0);
    return func_ov170_021e8fc0(syswk, func_ov170_021e0778, 0x15);
}

int func_ov170_021ea14c(Box2SysWork *syswk, u32 pos, int seq) {
    if (pos >= BOX2_PARTY_POS) {
        if (pos >= BOX2_BOXLIST_POS && pos != BOX2_GET_NONE) {
            u32 tray = syswk->trayScroll + pos - BOX2_BOXLIST_POS;

            if (tray >= syswk->trayMax) {
                tray -= syswk->trayMax;
            }
            if (tray == syswk->tray) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                return 21;
            }
            if (countEmptySlotsInBox(syswk->param->boxes, tray) == 0) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                return 21;
            }
        }
    }
    syswk->app->pokePutKey = pos;
    syswk->unk1A = 2;
    syswk->nextSeq = seq;
    func_ov170_021ea1c0(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e087c, 23);
}


void func_ov170_021ea1c0(Box2SysWork *syswk) {
    syswk->app->vfunk.work = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2PokeMoveWork), TRUE, "box2_seq.c", 0x2130);
}


void func_ov170_021ea1e8(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->vfunk.work);
}

int func_ov170_021ea1f4(Box2SysWork *syswk, u32 pos) {
    s16 x, y;

    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    syswk->app->unkA172 = 1;
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, NULL);
    func_ov170_021ef508(syswk->app->bgWinFrame);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->app->itemId != 0) {
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_ov170_021ed164(syswk->app, syswk->pos, 0);
        func_ov170_021ed094(syswk->app, syswk->app->itemId);
        func_ov170_021ed1c8(syswk->app);
        func_ov170_021ee038(syswk, 2, 2, 1);
    } else if (func_ov170_021ec01c(syswk->app, BOX2_ACTOR_ITEM_ICON) == TRUE) {
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_ov170_021ed6e4(syswk, FALSE);
    }
    func_ov170_021ec548(syswk->pos, &x, &y, 0);
    syswk->app->tpx = x + 8;
    syswk->app->tpy = y + 8;
    func_ov170_021ea728(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e0abc, func_ov170_021e8fe0(syswk, 62));
}


int func_ov170_021ea2dc(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    syswk->app->unkA172 = 1;
    func_ov170_021defec(syswk, syswk->pos);
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, 0);
    func_ov170_021ea878(syswk, syswk->app->itemId);
    func_ov170_021ef534(syswk->app->bgWinFrame);
    if (syswk->app->itemId != 0) {
        func_ov170_021ec000(syswk->app, 0xa, 0);
        func_ov170_021ed164(syswk->app, syswk->pos, 0);
        func_ov170_021ed094(syswk->app, syswk->app->itemId);
        func_ov170_021ed1c8(syswk->app);
        func_ov170_021eef74(syswk->app, 0x24, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x24);
    } else {
        func_ov170_021eef74(syswk->app, 0x25, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x25);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0a40, func_ov170_021e8fe0(syswk, 0x39));
}

int func_ov170_021ea39c(Box2SysWork *syswk, u32 pos) {
    s16 x, y;

    syswk->pos = pos;
    if (pos < BOX2_PARTY_POS) {
        syswk->getTray = syswk->tray;
    } else {
        syswk->getTray = BOX2_GET_NONE;
    }
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, NULL);
    if (syswk->app->itemId == 0) {
        func_020260e4(syswk->app->cursorMove, syswk->pos);
        return 64;
    }
    GFL_SndSEPlay(SEQ_SE_SYS_39);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
    func_ov170_021ed164(syswk->app, syswk->pos, 2);
    func_ov170_021ed094(syswk->app, syswk->app->itemId);
    func_ov170_021ed1c8(syswk->app);
    func_ov170_021ee038(syswk, 2, 1, 1);
    func_ov170_021ec548(syswk->pos, &x, &y, 2);
    syswk->app->tpx = x + 8;
    syswk->app->tpy = y + 8;
    func_ov170_021ea728(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e0fd8, 66);
}


int func_ov170_021ea458(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    syswk->app->unkA16E = pos;
    if (pos < 0x1e) {
        syswk->getTray = syswk->tray;
    } else {
        syswk->getTray = 0xff;
    }
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, 0);
    if (syswk->app->itemId == 0) {
        return 0x40;
    }
    syswk->unk18 = 1;
    func_ov170_021ec000(syswk->app, 0xa, 0);
    func_ov170_021ed198(syswk->app, syswk->pos, 2);
    func_ov170_021ed094(syswk->app, syswk->app->itemId);
    func_ov170_021ee038(syswk, 2, 1, 1);
    return func_ov170_021e8fc0(syswk, func_ov170_021e1324, 0x40);
}


int func_ov170_021ea4dc(Box2SysWork *syswk) {
    u32 pos = func_020260e0(syswk->app->cursorMove);
    if (pos < 0x24) {
        func_ov170_021defec(syswk, pos);
    } else {
        func_ov170_021df040(syswk);
    }
    func_ov170_021ea728(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e152c, 0x43);
}

int func_ov170_021ea514(Box2SysWork *syswk, u32 pos) {
    s16 x, y;

    syswk->app->pokePutKey = pos;
    syswk->unk18 = 0;
    func_ov170_021ec0b0(syswk->app, BOX2_ACTOR_CURSOR, &x, &y, 0);
    func_ov170_021ec08c(syswk->app, BOX2_ACTOR_ITEM_ICON, x, y + 8, 0);
    func_ov170_021ed6e4(syswk, FALSE);
    if (syswk->app->pokePutKey == BOX2_GET_NONE) {
        return func_ov170_021ea4dc(syswk);
    }
    if (func_ov170_021dde80(syswk, syswk->pos, pos) == FALSE) {
        return func_ov170_021ea4dc(syswk);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e1458, 69);
}

int func_ov170_021ea598(Box2SysWork *syswk, u32 pos) {
    s16 x, y;

    syswk->pos = pos;
    syswk->getTray = BOX2_GET_NONE;
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, NULL);
    func_ov170_021ef508(syswk->app->bgWinFrame);
    func_ov170_021eba5c(syswk->app, 0x18);
    func_ov170_021dd14c(syswk);
    if (syswk->app->itemId != 0) {
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_ov170_021ed164(syswk->app, syswk->pos, 1);
        func_ov170_021ed094(syswk->app, syswk->app->itemId);
        func_ov170_021ed1c8(syswk->app);
        func_ov170_021ee038(syswk, 2, 2, 1);
    } else if (func_ov170_021ec01c(syswk->app, BOX2_ACTOR_ITEM_ICON) == TRUE) {
        func_ov170_021ec000(syswk->app, BOX2_ACTOR_ITEM_ICON, FALSE);
        func_ov170_021ed6e4(syswk, FALSE);
    }
    func_ov170_021ec548(syswk->pos, &x, &y, 1);
    syswk->app->tpx = x + 8;
    syswk->app->tpy = y + 8;
    func_ov170_021ea728(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e1628, 72);
}


int func_ov170_021ea66c(Box2SysWork *syswk, u32 pos) {
    syswk->pos = pos;
    syswk->getTray = syswk->tray;
    syswk->app->unkA172 = 1;
    func_ov170_021defec(syswk, syswk->pos);
    syswk->app->itemId = func_ov170_021dd25c(syswk, syswk->pos, syswk->tray, 6, 0);
    func_ov170_021ea878(syswk, syswk->app->itemId);
    func_ov170_021ef534(syswk->app->bgWinFrame);
    if (syswk->app->itemId != 0) {
        func_ov170_021ec000(syswk->app, 0xa, 0);
        func_ov170_021ed164(syswk->app, syswk->pos, 1);
        func_ov170_021ed094(syswk->app, syswk->app->itemId);
        func_ov170_021ed1c8(syswk->app);
        func_ov170_021eef74(syswk->app, 0x9, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0x9);
    } else {
        func_ov170_021eef74(syswk->app, 0xa, func_020260e0(syswk->app->cursorMove));
        func_020260e4(syswk->app->cursorMove, 0xa);
    }
    return func_ov170_021e8fc0(syswk, func_ov170_021e0a40, func_ov170_021e8fe0(syswk, 0x46));
}


void func_ov170_021ea728(Box2SysWork *syswk) {
    syswk->app->vfunk.work = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2ItemMoveWork), FALSE, "box2_seq.c", 0x228f);
}


void func_ov170_021ea750(Box2SysWork *syswk) {
    GFL_HeapFree(syswk->app->vfunk.work);
}

void func_ov170_021ea75c(Box2SysWork *syswk) {
    func_ov170_021ef32c(syswk->app->bgWinFrame);
    func_ov170_021ec688(syswk);
}

void func_ov170_021ea770(Box2SysWork *syswk) {
    func_ov170_021ef33c(syswk->app->bgWinFrame);
    func_ov170_021ec6f4(syswk);
}


void func_ov170_021ea784(Box2SysWork *syswk) {
    func_ov170_021ec688(syswk);
    func_ov170_021ef32c(syswk->app->bgWinFrame);
}

int func_ov170_021ea798(Box2SysWork *syswk, u32 mode) {
    u32 ok = func_ov170_021dd210(syswk, syswk->tray, syswk->app->unkA17F);
    Box2VFunc func;
    syswk->tray = syswk->app->unkA17F;
    func_ov170_021ec35c(syswk, syswk->tray);
    func_ov170_021dec98(syswk, func_ov170_021dedcc(syswk, syswk->tray), ok);
    if (ok == 1) {
        func = func_ov170_021e0128;
    } else {
        func = func_ov170_021e00c4;
    }
    switch (mode) {
    case 0:
        syswk->nextSeq = 0x15;
        break;
    case 1:
        syswk->nextSeq = 0x63;
        break;
    }
    GFL_SndSEPlay(0x548);
    return func_ov170_021e8fc0(syswk, func, syswk->nextSeq);
}


void func_ov170_021ea808(Box2SysWork *syswk) {
    u16 *work = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_BOX2_APP), 4, FALSE, "box2_seq.c", 0x2305);
    work[0] = 0;
    work[1] = 0;
    syswk->app->unkA190 = work;
}

int func_ov170_021ea83c(Box2SysWork *syswk, u32 seq) {
    func_ov170_021dd1e8(syswk);
    return func_ov170_021e8fc0(syswk, func_ov170_021e01c4, seq);
}

int func_ov170_021ea858(Box2SysWork *syswk, u32 seq) {
    func_ov170_021ef75c(syswk->app->bgWinFrame);
    return func_ov170_021e8fc0(syswk, func_ov170_021e01e0, seq);
}

void func_ov170_021ea878(Box2SysWork *syswk, s16 item) {
    if (item != 0) {
        if (syswk->pos < 0x1e) {
            func_0202614c(syswk->app->cursorMove, 0x24);
        } else {
            func_0202614c(syswk->app->cursorMove, 9);
        }
        func_ov170_021eb988(syswk, data_ov170_021ef8fc, 3);
    } else {
        if (syswk->pos < 0x1e) {
            func_02026124(syswk->app->cursorMove, 0x24);
        } else {
            func_02026124(syswk->app->cursorMove, 9);
        }
        func_ov170_021eb988(syswk, data_ov170_021ef8f4, 2);
    }
}


// Mode 0: whether the party Pokémon can be deposited: 0 if so, 1 for the last battler, 2 for mail
u32 func_ov170_021ea8cc(Box2SysWork *syswk) {
    // The party position, then the result
    u32 val = syswk->pos - BOX2_PARTY_POS;

    if (func_ov170_021dd694(syswk, val) == FALSE) {
        return 1;
    }
    if (PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(syswk->param->party, val), PKM_PARAM_ITEM, NULL))
        == TRUE) {
        val = 2;
    } else {
        val = 0;
    }
    return val;
}

void func_ov170_021ea908(Box2SysWork *syswk, u32 bit) {
    syswk->app->unkA174 ^= 1 << bit;
    func_ov170_021df110(syswk, syswk->app->unkA174);
}

int func_ov170_021ea928(Box2SysWork *syswk, u32 pos) {
    if (syswk->app->unkA17C <= 4 || (syswk->app->unkA17C == 5 && func_02007a80(syswk->param->boxes, 2) == FALSE)) {
        syswk->app->unkA17A[1] = pos + (syswk->app->unkA17C - 1) * 4 - 1;
    } else {
        syswk->app->unkA17A[1] = pos + (syswk->app->unkA17C - 1) * 4;
    }
    return func_ov170_021e949c(syswk, pos, func_ov170_021e8f50(syswk, 0x66));
}


void func_ov170_021ea984(Box2SysWork *syswk) {
    switch (syswk->param->mode) {
    case 0:
        func_ov170_021ecb20(syswk, 2, 1);
        break;
    case 1:
    case 3:
        func_ov170_021ed888(syswk->app, 0);
        func_ov170_021ecb20(syswk, 1, 1);
        break;
    case 2:
    case 4:
    case 5:
        func_ov170_021ed888(syswk->app, 0);
        if (func_ov170_021ef480(syswk->app->bgWinFrame) == 1) {
            func_ov170_021ecb20(syswk, 2, 1);
        } else {
            func_ov170_021ecb20(syswk, 1, 1);
        }
        break;
    }
}


