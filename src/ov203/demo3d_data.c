// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov293/demo3d_data.c, ported by tools/port_file.py; its data are Black's.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d/demo3d_data.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/zones.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"

// The 3D demos' scenes, named after the ROM's "demo3d_data.c". A demo has a 3D set-up, a list of commands for its
// frames and one for its end, and its models ("units"). A model has one or more variants, picked by the player's sex,
// the demo's parameter or the season, and a variant is the files of ARCID_DEMO3D_RESOURCE that make it up, the model
// first and then its animations, either one set or one for each period of the day.
//
// The data are Black's (18 demos, numbered from 1; the numbers of the names are the demo's). MWCC sorts a file's
// read-only objects by size, with the order of definition deciding between those of one size, so the definitions
// are in the order that gives the ROM's layout, which is not the order of the original file (every table is defined
// before the ones that point to it).

// The most files a model has
#define RESOURCE_MAX 6

typedef struct {
    const Demo3DSetup *setup;
    const Demo3DCmd *cmds;
    const Demo3DCmd *endCmds;
    const Demo3DUnit *units;
    u32 unitCount;
} Demo3DScene;

static const Demo3DVariant *func_ov203_021be8f4(const Demo3DUnit *unit, const Demo3DSceneParam *param);

// pokebw2's sDemo3D10_FileIds2_0_0
static const u16 data_ov203_021c02ca[] = { 167, 169, 168, 165, 166 };
// pokebw2's sDemo3D14_EndCmds
static const Demo3DCmd data_ov203_021c06ac[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D05_FileIds2_3_0
static const u16 data_ov203_021bfdfa[] = { 128, 130, 129 };
// pokebw2's sDemo3D05_FileIds2_1_0
static const u16 data_ov203_021bfda0[] = { 124, 126, 125 };
// pokebw2's sDemo3D09_FileIds1_0_0
static const u16 data_ov203_021c0290[] = { 150, 151, 148, 149 };
// pokebw2's sDemo3D18_FileIds0_0_0
static const u16 data_ov203_021bfe1e[] = { 235, 236, 234 };
// pokebw2's sDemo3D14_Cmds
static const Demo3DCmd data_ov203_021c1564[] = {
    { DEMO3D_CMD_SE_PLAY, 80, { SEQ_SE_MEET_03, 70 } },
    { DEMO3D_CMD_SE_PLAY, 302, { SEQ_SE_MEET_04, 127, 0, -500, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 430, { 1, 60, 0, 16 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 490, { 1, 60, 16, -16 } },
    { DEMO3D_CMD_SE_STOP, 549, { SEQ_SE_MEET_03 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D07_FileIds1_0_0
static const u16 data_ov203_021c02de[] = { 42, 44, 43, 40, 41 };
// pokebw2's sDemo3D05_FileIds1_0_0
static const u16 data_ov203_021bfdc4[] = { 113, 114, 112 };
// pokebw2's sDemo3D05_FileIds0_0_0
static const u16 data_ov203_021bfd70[] = { 111, 110 };
// pokebw2's sDemo3D05_FileIds2_2_0
static const u16 data_ov203_021bfddc[] = { 116, 118, 117 };
// pokebw2's sDemo3D16_FileIds0_0_0
static const u16 data_ov203_021bfd34[] = { 242, 243 };
// pokebw2's sDemo3D16_Files0_0
static const Demo3DFiles data_ov203_021c00a8[] = { { data_ov203_021bfd34, NELEMS(data_ov203_021bfd34) } };
// pokebw2's sDemo3D16_FileIds2_0_4
static const u16 data_ov203_021bfd54[] = { 249, 251 };
// pokebw2's sDemo3D11_Cmds
static const Demo3DCmd data_ov203_021c1d98[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_LEGEND_COME_B } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W307_EXP, 120, 0, -200 } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W465_01, 100, 0, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W008_01, 100, 0, -1000, 2 } },
    { DEMO3D_CMD_SE_PLAY, 207, { SEQ_SE_W401_01, 100, 0, -500 } },
    { DEMO3D_CMD_SE_PLAY, 250, { SEQ_SE_W401_01, 100, 100, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 298, { SEQ_SE_W401_01, 100, -100, -800, 2 } },
    { DEMO3D_CMD_SE_PLAY, 317, { SEQ_SE_LBDEMO_04, 127, 0, -200, 2 } },
    { DEMO3D_CMD_SE_PLAY, 371, { SEQ_SE_W014_02, 100, 0, -1000 } },
    { DEMO3D_CMD_SE_PLAY, 583, { SEQ_SE_LBDEMO_09, 127 } },
    { DEMO3D_CMD_SE_PLAY, 686, { SEQ_SE_W348_02, 100, -70 } },
    { DEMO3D_CMD_SE_PLAY, 686, { SEQ_SE_W307_EXP, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 713, { SEQ_SE_LBDEMO_05, 100, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 750, { SEQ_SE_SDEMO_03, 100, 0, -100 } },
    { DEMO3D_CMD_SE_PLAY, 807, { SEQ_SE_SDEMO_03, 105, 0, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 878, { SEQ_SE_FLD_166, 100, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 987, { SEQ_SE_SHINKA_W060C, 100 } },
    { DEMO3D_CMD_SE_PLAY, 1000, { SEQ_SE_SDEMO_03, 70, 0, -200, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1110, { 1, 10, 0, -16 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1121, { 1, 20, -16 } },
    { DEMO3D_CMD_BGM_FADE, 1220, { 1, 30 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1220, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D11_FileIds2_0_0
static const u16 data_ov203_021bff18[] = { 180, 181, 178, 179 };
// pokebw2's sDemo3D03_FileIds2_3_0
static const u16 data_ov203_021bfd4c[] = { 69, 70 };
// pokebw2's sDemo3D03_FileIds0_1_0
static const u16 data_ov203_021bfda6[] = { 86, 91, 109 };
// pokebw2's sDemo3D12_Cmds
static const Demo3DCmd data_ov203_021c29f0[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_LEGEND_COME_W } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W307_EXP, 127, 0, -200 } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W465_01, 110, 0, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 155, { SEQ_SE_W008_01, 110, 0, -1000, 2 } },
    { DEMO3D_CMD_SE_PLAY, 207, { SEQ_SE_W401_01, 110, 0, -500 } },
    { DEMO3D_CMD_SE_PLAY, 250, { SEQ_SE_W401_01, 110, 100, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 267, { SEQ_SE_LCDEMO_02, 100, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 272, { SEQ_SE_LCDEMO_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 281, { SEQ_SE_LCDEMO_02, 100, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 289, { SEQ_SE_LCDEMO_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 297, { SEQ_SE_LCDEMO_02, 100, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 306, { SEQ_SE_LCDEMO_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 317, { SEQ_SE_LBDEMO_04, 127, 0, -200, 2 } },
    { DEMO3D_CMD_SE_PLAY, 371, { SEQ_SE_W014_02, 110, 0, -1000 } },
    { DEMO3D_CMD_SE_PLAY, 478, { SEQ_SE_LCDEMO_01, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 492, { SEQ_SE_LCDEMO_02, 100, 0, 200, 2 } },
    { DEMO3D_CMD_SE_PLAY, 507, { SEQ_SE_LCDEMO_01, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 583, { SEQ_SE_LBDEMO_06, 110 } },
    { DEMO3D_CMD_SE_PLAY, 690, { SEQ_SE_W307_EXP, 110, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 713, { SEQ_SE_LBDEMO_05, 120 } },
    { DEMO3D_CMD_SE_PLAY, 750, { SEQ_SE_SDEMO_03, 110, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 807, { SEQ_SE_SDEMO_03, 110, 0, -500, 2 } },
    { DEMO3D_CMD_SE_PLAY, 878, { SEQ_SE_LBDEMO_08, 110 } },
    { DEMO3D_CMD_SE_PLAY, 987, { SEQ_SE_FLD_131, 110, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1000, { SEQ_SE_SDEMO_03, 100, 0, -200, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1005, { SEQ_SE_DUMMY5 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1110, { 1, 10, 0, -16 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1121, { 1, 20, -16 } },
    { DEMO3D_CMD_BGM_FADE, 1220, { 1, 30 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1220, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D18_FileIds2_0_0
static const u16 data_ov203_021bfd58[] = { 240, 239 };
// pokebw2's sDemo3D18_Files0_0
static const Demo3DFiles data_ov203_021c0200[] = { { data_ov203_021bfe1e, NELEMS(data_ov203_021bfe1e) } };
// pokebw2's sDemo3D12_FileIds1_0_0
static const u16 data_ov203_021bfd82[] = { 189, 187, 188 };
// pokebw2's sDemo3D12_Files1_0
static const Demo3DFiles data_ov203_021c0040[] = { { data_ov203_021bfd82, NELEMS(data_ov203_021bfd82) } };
// pokebw2's sDemo3D08_FileIds1_0_0
static const u16 data_ov203_021c02f2[] = { 55, 57, 56, 53, 54 };
// pokebw2's sDemo3D01_Setup
static const Demo3DSetup data_ov203_021c0ebc = {
    35, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 1, 0, 0, 1, 1, 0, 16,
    FX32_CONST(40), FX32_CONST(40), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D08_Setup
static const Demo3DSetup data_ov203_021c0f10 = {
    58, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 1, 0, 0, 0, 1, 0, 16,
    FX32_CONST(13), FX32_CONST(13), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    58, -57, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    TRUE, 30, TRUE, 0,
};
// pokebw2's sDemo3D13_FileIds1_0_0
static const u16 data_ov203_021bfd64[] = { 202, 201 };
// pokebw2's sDemo3D14_FileIds1_0_0
static const u16 data_ov203_021bfd48[] = { 210, 209 };
// pokebw2's sDemo3D11_Files2_0
static const Demo3DFiles data_ov203_021bff20[] = { { data_ov203_021bff18, NELEMS(data_ov203_021bff18) } };
// pokebw2's sDemo3D11_FileIds1_0_0
static const u16 data_ov203_021bfdb8[] = { 177, 175, 176 };
// pokebw2's sDemo3D01_FileIds5_0_0
static const u16 data_ov203_021bfd1c[] = { 28, 29 };
// pokebw2's sDemo3D01_FileIds3_0_0
static const u16 data_ov203_021bfdbe[] = { 25, 26, 24 };
// pokebw2's sDemo3D03_FileIds2_2_0
static const u16 data_ov203_021bfd74[] = { 63, 64 };
// pokebw2's sDemo3D03_Files2_3
static const Demo3DFiles data_ov203_021bff08[] = { { data_ov203_021bfd4c, NELEMS(data_ov203_021bfd4c) } };
// pokebw2's sDemo3D09_Cmds
static const Demo3DCmd data_ov203_021c2540[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_LEGEND_BORN_B } },
    { DEMO3D_CMD_SE_PLAY, 42, { SEQ_SE_LBDEMO_02, 40 } },
    { DEMO3D_CMD_SE_PLAY, 135, { SEQ_SE_LBDEMO_01, 80, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 270, { SEQ_SE_FLD_127, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 382, { SEQ_SE_LBDEMO_03, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 390, { SEQ_SE_DUMMY5 } },
    { DEMO3D_CMD_SE_PLAY, 430, { SEQ_SE_LBDEMO_03, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 495, { SEQ_SE_LBDEMO_03, 127, 0, 0, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 510, { 1, 15, 0, 8 } },
    { DEMO3D_CMD_SE_PLAY, 525, { SEQ_SE_LBDEMO_05, 127, 0, 0, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 545, { 1, 25, 8 } },
    { DEMO3D_CMD_SE_PLAY, 589, { SEQ_SE_W307_02, 127 } },
    { DEMO3D_CMD_SE_PLAY, 601, { SEQ_SE_W307_02, 127, 0, -500, 2 } },
    { DEMO3D_CMD_SE_PLAY, 616, { SEQ_SE_LBDEMO_04, 127, -128 } },
    { DEMO3D_CMD_SE_PLAY, 616, { SEQ_SE_LBDEMO_04, 127, 127, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 746, { SEQ_SE_LBDEMO_06, 117 } },
    { DEMO3D_CMD_SE_PLAY, 827, { SEQ_SE_FLD_127, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 832, { SEQ_SE_W307_EXP, 127, -128, -1000, 2 } },
    { DEMO3D_CMD_SE_PLAY, 832, { SEQ_SE_W307_EXP, 127, 127, -1000 } },
    { DEMO3D_CMD_SE_PLAY, 837, { SEQ_SE_FLD_127, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 853, { SEQ_SE_W307_EXP, 127, 0, -1000, 2 } },
    { DEMO3D_CMD_SE_PLAY, 876, { SEQ_SE_LBDEMO_07, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1080, { SEQ_SE_FLD_131, 127 } },
    { DEMO3D_CMD_SE_PLAY, 1114, { SEQ_SE_DUMMY5, 0, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1114, { SEQ_SE_DUMMY5, 0, 0, 0, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1159, { 1, 15, 0, -16 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1179, { 1, 15, -16 } },
    { DEMO3D_CMD_BGM_FADE, 1310, { 1, 50 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1310, { 1, 50, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D09_FileIds0_0_0
static const u16 data_ov203_021bfe0c[] = { 146, 147, 145 };
// pokebw2's sDemo3D02_EndCmds
static const Demo3DCmd data_ov203_021c06d4[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D02_FileIds2_0_0
static const u16 data_ov203_021bfde8[] = { 6, 7, 5 };
// pokebw2's sDemo3D16_Variants0
static const Demo3DVariant data_ov203_021c0278[] = { { data_ov203_021c00a8, NELEMS(data_ov203_021c00a8) } };
// pokebw2's sDemo3D16_FileIds2_0_2
static const u16 data_ov203_021bfd18[] = { 249, 251 };
// pokebw2's sDemo3D14_FileIds0_0_0
static const u16 data_ov203_021c02fc[] = { 206, 208, 207, 204, 205 };
// pokebw2's sDemo3D09_Setup
static const Demo3DSetup data_ov203_021c100c = {
    157, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(28), FX32_CONST(28), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    149, -142, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D03_Setup
static const Demo3DSetup data_ov203_021c1060 = {
    79, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 1, 0, 0, 1, 1, 0, 16,
    FX32_CONST(40), FX32_CONST(40), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D03_FileIds1_1_0
static const u16 data_ov203_021bfdac[] = { 84, 89, 225 };
// pokebw2's sDemo3D03_Files0_1
static const Demo3DFiles data_ov203_021c0298[] = { { data_ov203_021bfda6, NELEMS(data_ov203_021bfda6) } };
// pokebw2's sDemo3D03_EndCmds
static const Demo3DCmd data_ov203_021c0724[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D15_FileIds1_0_4
static const u16 data_ov203_021bfe48[] = { 215, 216, 219 };
// pokebw2's sDemo3D15_Setup
static const Demo3DSetup data_ov203_021c0acc = {
    224, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(60), FX32_CONST(60), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(4, 3, 1), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D05_FileIds2_0_0
static const u16 data_ov203_021bfe00[] = { 120, 122, 121 };
// pokebw2's sDemo3D10_EndCmds
static const Demo3DCmd data_ov203_021c07fc[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_FileIds1_5_0
static const u16 data_ov203_021bfe5a[] = { 88, 92, 225 };
// pokebw2's sDemo3D03_FileIds2_1_0
static const u16 data_ov203_021bfcfc[] = { 67, 68 };
// pokebw2's sDemo3D03_FileIds1_3_0
static const u16 data_ov203_021bfd7c[] = { 87, 89, 225 };
// pokebw2's sDemo3D04_FileIds1_0_0
static const u16 data_ov203_021bfe2a[] = { 100, 101, 99 };
// pokebw2's sDemo3D04_FileIds3_0_0
static const u16 data_ov203_021bfd10[] = { 106, 107 };
// pokebw2's sDemo3D04_FileIds0_0_0
static const u16 data_ov203_021bfd3c[] = { 97, 98 };
// pokebw2's sDemo3D15_FileIds1_0_2
static const u16 data_ov203_021bfdca[] = { 215, 216, 220 };
// pokebw2's sDemo3D09_Files0_0
static const Demo3DFiles data_ov203_021bfe78[] = { { data_ov203_021bfe0c, NELEMS(data_ov203_021bfe0c) } };
// pokebw2's sDemo3D03_FileIds3_0_0
static const u16 data_ov203_021bfd14[] = { 76, 75 };
// pokebw2's sDemo3D03_Files2_2
static const Demo3DFiles data_ov203_021bffb0[] = { { data_ov203_021bfd74, NELEMS(data_ov203_021bfd74) } };
// pokebw2's sDemo3D01_FileIds1_0_0
static const u16 data_ov203_021bfdb2[] = { 19, 18, 20 };
// pokebw2's sDemo3D10_Setup
static const Demo3DSetup data_ov203_021c0b74 = {
    170, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(28), FX32_CONST(28), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    149, -142, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D10_FileIds0_0_0
static const u16 data_ov203_021bfe4e[] = { 159, 160, 158 };
// pokebw2's sDemo3D17_FileIds0_0_0
static const u16 data_ov203_021bfdd6[] = { 227, 228, 226 };
// pokebw2's sDemo3D15_FileIds0_0_0
static const u16 data_ov203_021bfe18[] = { 213, 214, 212 };
// pokebw2's sDemo3D01_Files5_0
static const Demo3DFiles data_ov203_021c01d0[] = { { data_ov203_021bfd1c, NELEMS(data_ov203_021bfd1c) } };
// pokebw2's sDemo3D01_FileIds2_0_0
static const u16 data_ov203_021bfe36[] = { 22, 21, 23 };
// pokebw2's sDemo3D01_Files1_0
static const Demo3DFiles data_ov203_021c00a0[] = { { data_ov203_021bfdb2, NELEMS(data_ov203_021bfdb2) } };
// pokebw2's sDemo3D01_FileIds4_0_0
static const u16 data_ov203_021bfcf8[] = { 27 };
// pokebw2's sDemo3D08_FileIds0_0_0
static const u16 data_ov203_021bfe06[] = { 52, 50, 51 };
// pokebw2's sDemo3D10_FileIds1_0_0
static const u16 data_ov203_021bfea0[] = { 163, 164, 161, 162 };
// pokebw2's sDemo3D10_Files1_0
static const Demo3DFiles data_ov203_021bffa8[] = { { data_ov203_021bfea0, NELEMS(data_ov203_021bfea0) } };
// pokebw2's sDemo3D04_FileIds3_0_3
static const u16 data_ov203_021bfd6c[] = { 106, 108 };
// pokebw2's sDemo3D04_FileIds2_0_0
static const u16 data_ov203_021bfcfa[] = { 102 };
// pokebw2's sDemo3D18_Files2_0
static const Demo3DFiles data_ov203_021bfe90[] = { { data_ov203_021bfd58, NELEMS(data_ov203_021bfd58) } };
// pokebw2's sDemo3D01_FileIds0_1_0
static const u16 data_ov203_021bfde2[] = { 16, 17, 15 };
// pokebw2's sDemo3D03_FileIds5_0_3
static const u16 data_ov203_021bfd40[] = { 71, 74 };
// pokebw2's sDemo3D03_FileIds1_8_0
static const u16 data_ov203_021bfd9a[] = { 82, 92, 225 };
// pokebw2's sDemo3D10_Files2_0
static const Demo3DFiles data_ov203_021bfeb8[] = { { data_ov203_021c02ca, NELEMS(data_ov203_021c02ca) } };
// pokebw2's sDemo3D03_FileIds5_0_2
static const u16 data_ov203_021bfd20[] = { 71, 73 };
// pokebw2's sDemo3D14_Files1_0
static const Demo3DFiles data_ov203_021c0260[] = { { data_ov203_021bfd48, NELEMS(data_ov203_021bfd48) } };
// pokebw2's sDemo3D01_FileIds0_0_0
static const u16 data_ov203_021bfd8e[] = { 13, 14, 12 };
// pokebw2's sDemo3D15_EndCmds
static const Demo3DCmd data_ov203_021c04f4[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D07_Files1_0
static const Demo3DFiles data_ov203_021c0110[] = { { data_ov203_021c02de, NELEMS(data_ov203_021c02de) } };
// pokebw2's sDemo3D08_Files0_0
static const Demo3DFiles data_ov203_021bff78[] = { { data_ov203_021bfe06, NELEMS(data_ov203_021bfe06) } };
// pokebw2's sDemo3D04_FileIds3_0_4
static const u16 data_ov203_021bfd30[] = { 106, 108 };
// pokebw2's sDemo3D11_Setup
static const Demo3DSetup data_ov203_021c0c1c = {
    182, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(28), FX32_CONST(28), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    149, -142, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D18_Variants0
static const Demo3DVariant data_ov203_021c02b8[] = { { data_ov203_021c0200, NELEMS(data_ov203_021c0200) } };
// pokebw2's sDemo3D18_Cmds
static const Demo3DCmd data_ov203_021c084c[] = {
    { DEMO3D_CMD_BRIGHTNESS_FADE, 701, { 1, 20, 16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D14_Files0_0
static const Demo3DFiles data_ov203_021bff60[] = { { data_ov203_021c02fc, NELEMS(data_ov203_021c02fc) } };
static const G3DSceneAnimationSetup data_ov203_021c0348[] = {
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
    { 4, 0 },
    { 5, 0 },
};
// pokebw2's sDemo3D11_FileIds0_0_0
static const u16 data_ov203_021bff58[] = { 172, 174, 173, 171 };
// pokebw2's sDemo3D04_Files0_0
static const Demo3DFiles data_ov203_021bfef8[] = { { data_ov203_021bfd3c, NELEMS(data_ov203_021bfd3c) } };
// pokebw2's sDemo3D04_EndCmds
static const Demo3DCmd data_ov203_021c056c[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D11_Files0_0
static const Demo3DFiles data_ov203_021bff00[] = { { data_ov203_021bff58, NELEMS(data_ov203_021bff58) } };
// pokebw2's sDemo3D03_FileIds5_0_4
static const u16 data_ov203_021bfd2c[] = { 71, 74 };
// pokebw2's sDemo3D14_Variants0
static const Demo3DVariant data_ov203_021c0280[] = { { data_ov203_021bff60, NELEMS(data_ov203_021bff60) } };
// pokebw2's sDemo3D04_Files2_0
static const Demo3DFiles data_ov203_021bff40[] = { { data_ov203_021bfcfa, NELEMS(data_ov203_021bfcfa) } };
// pokebw2's sDemo3D04_FileIds3_0_2
static const u16 data_ov203_021bfd00[] = { 106, 108 };
// pokebw2's sDemo3D04_FileIds3_0_1
static const u16 data_ov203_021bfd44[] = { 106, 107 };
// pokebw2's sDemo3D15_FileIds1_0_3
static const u16 data_ov203_021bfe42[] = { 215, 216, 219 };
// pokebw2's sDemo3D18_EndCmds
static const Demo3DCmd data_ov203_021c05bc[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D02_Files2_0
static const Demo3DFiles data_ov203_021c0250[] = { { data_ov203_021bfde8, NELEMS(data_ov203_021bfde8) } };
// pokebw2's sDemo3D02_FileIds0_0_0
static const u16 data_ov203_021bfd38[] = { 0, 1 };
// pokebw2's sDemo3D11_Variants0
static const Demo3DVariant data_ov203_021c0240[] = { { data_ov203_021bff00, NELEMS(data_ov203_021bff00) } };
// pokebw2's sDemo3D04_Variants0
static const Demo3DVariant data_ov203_021c0238[] = { { data_ov203_021bfef8, NELEMS(data_ov203_021bfef8) } };
// pokebw2's sDemo3D04_Cmds
static const Demo3DCmd data_ov203_021c1154[] = {
    { DEMO3D_CMD_SE_PLAY, 0, { SEQ_SE_VDEMO_01, 127 } },
    { DEMO3D_CMD_SE_PLAY, 75, { SEQ_SE_VDEMO_02, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_STOP, 130, { SEQ_SE_VDEMO_01 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 130, { 1, 20, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_FileIds0_0_0
static const u16 data_ov203_021bfe12[] = { 85, 91, 109 };
// pokebw2's sDemo3D03_FileIds1_6_0
static const u16 data_ov203_021bfd88[] = { 96, 89, 225 };
// pokebw2's sDemo3D02_FileIds1_0_0
static const u16 data_ov203_021bfd94[] = { 3, 4, 2 };
// pokebw2's sDemo3D11_Variants2
static const Demo3DVariant data_ov203_021bff80[] = { { data_ov203_021bff20, NELEMS(data_ov203_021bff20) } };
// pokebw2's sDemo3D02_Files0_0
static const Demo3DFiles data_ov203_021bffe0[] = { { data_ov203_021bfd38, NELEMS(data_ov203_021bfd38) } };
// pokebw2's sDemo3D03_FileIds1_2_0
static const u16 data_ov203_021bfe54[] = { 83, 89, 225 };
// pokebw2's sDemo3D03_Files0_0
static const Demo3DFiles data_ov203_021c01f8[] = { { data_ov203_021bfe12, NELEMS(data_ov203_021bfe12) } };
// pokebw2's sDemo3D05_Files0_0
static const Demo3DFiles data_ov203_021bfff0[] = { { data_ov203_021bfd70, NELEMS(data_ov203_021bfd70) } };
// pokebw2's sDemo3D05_EndCmds
static const Demo3DCmd data_ov203_021c093c[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D05_Files1_0
static const Demo3DFiles data_ov203_021c01e0[] = { { data_ov203_021bfdc4, NELEMS(data_ov203_021bfdc4) } };
// pokebw2's sDemo3D05_Files2_0
static const Demo3DFiles data_ov203_021c0018[] = { { data_ov203_021bfe00, NELEMS(data_ov203_021bfe00) } };
// pokebw2's sDemo3D12_FileIds0_0_0
static const u16 data_ov203_021c0020[] = { 184, 186, 185, 183 };
// pokebw2's sDemo3D12_Files0_0
static const Demo3DFiles data_ov203_021c0030[] = { { data_ov203_021c0020, NELEMS(data_ov203_021c0020) } };
// pokebw2's sDemo3D16_FileIds2_0_1
static const u16 data_ov203_021bfd28[] = { 249, 250 };
// pokebw2's sDemo3D16_Setup
static const Demo3DSetup data_ov203_021c0d6c = {
    248, 0xffff, ZONE_CASTELIA_CITY_8,
    1, 0, 1, 1, 0, 0, 0, 1, 0, 16,
    FX32_CONST(35), FX32_CONST(35), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D05_Files2_2
static const Demo3DFiles data_ov203_021c0038[] = { { data_ov203_021bfddc, NELEMS(data_ov203_021bfddc) } };
// pokebw2's sDemo3D05_Cmds
static const Demo3DCmd data_ov203_021c1744[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_OPENING } },
    { DEMO3D_CMD_TELOP, 40, { 255, 0, 4, 1, 90, 8, 8 } },
    { DEMO3D_CMD_TELOP, 190, { 21, 0, 4, 2, 90, 8, 8 } },
    { DEMO3D_CMD_TELOP, 190, { 20, 0, 4, 3, 90, 8, 8 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 450, { 3, 60, 0, 16 } },
    { DEMO3D_CMD_BGM_FADE, 480, { 1, 30 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D05_Files2_3
static const Demo3DFiles data_ov203_021c01a0[] = { { data_ov203_021bfdfa, NELEMS(data_ov203_021bfdfa) } };
// pokebw2's sDemo3D05_Variants1
static const Demo3DVariant data_ov203_021c0050[] = { { data_ov203_021c01e0, NELEMS(data_ov203_021c01e0) } };
// pokebw2's sDemo3D12_FileIds2_0_0
static const u16 data_ov203_021c0058[] = { 192, 193, 190, 191 };
// pokebw2's sDemo3D12_Files2_0
static const Demo3DFiles data_ov203_021c0188[] = { { data_ov203_021c0058, NELEMS(data_ov203_021c0058) } };
// pokebw2's sDemo3D18_FileIds1_0_0
static const u16 data_ov203_021bfd68[] = { 238, 237 };
// pokebw2's sDemo3D18_Files1_0
static const Demo3DFiles data_ov203_021c0178[] = { { data_ov203_021bfd68, NELEMS(data_ov203_021bfd68) } };
// pokebw2's sDemo3D03_Files1_1
static const Demo3DFiles data_ov203_021c0078[] = { { data_ov203_021bfdac, NELEMS(data_ov203_021bfdac) } };
// pokebw2's sDemo3D06_FileIds1_0_0
static const u16 data_ov203_021c0088[] = { 138, 139, 136, 137 };
// pokebw2's sDemo3D06_FileIds0_0_0
static const u16 data_ov203_021bfdf4[] = { 135, 133, 134 };
// pokebw2's sDemo3D06_Files0_0
static const Demo3DFiles data_ov203_021c0158[] = { { data_ov203_021bfdf4, NELEMS(data_ov203_021bfdf4) } };
// pokebw2's sDemo3D06_Cmds
static const Demo3DCmd data_ov203_021c2ec8[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_N_CASTLE } },
    { DEMO3D_CMD_SE_PLAY, 170, { SEQ_SE_NDEMO_03, 127, 0, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 180, { SEQ_SE_BGDEMO_02, 70, -50, -500 } },
    { DEMO3D_CMD_SE_PLAY, 185, { SEQ_SE_NDEMO_03, 127, 0, -200, 1 } },
    { DEMO3D_CMD_SE_PLAY, 190, { SEQ_SE_BGDEMO_02, 50, 20, -700 } },
    { DEMO3D_CMD_SE_PLAY, 215, { SEQ_SE_W307_EXP, 127 } },
    { DEMO3D_CMD_SE_PLAY, 215, { SEQ_SE_W157_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 273, { SEQ_SE_W307_EXP, 127 } },
    { DEMO3D_CMD_SE_PLAY, 273, { SEQ_SE_W157_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 330, { SEQ_SE_W307_EXP, 127 } },
    { DEMO3D_CMD_SE_PLAY, 330, { SEQ_SE_W157_01, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 350, { SEQ_SE_W307_EXP, 100, -120 } },
    { DEMO3D_CMD_SE_PLAY, 350, { SEQ_SE_W157_01, 100, -120, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 375, { SEQ_SE_W307_EXP, 100, 120 } },
    { DEMO3D_CMD_SE_PLAY, 375, { SEQ_SE_W157_01, 100, 120, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 413, { SEQ_SE_W307_EXP, 100, -60 } },
    { DEMO3D_CMD_SE_PLAY, 413, { SEQ_SE_W157_01, 100, -60, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 420, { SEQ_SE_W307_EXP, 100, -80, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 420, { SEQ_SE_W157_01, 100, -80, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 442, { SEQ_SE_W307_EXP, 70, -100, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 442, { SEQ_SE_W157_01, 70, -100 } },
    { DEMO3D_CMD_SE_PLAY, 472, { SEQ_SE_W307_EXP, 127, -120, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 472, { SEQ_SE_W157_01, 80, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 555, { SEQ_SE_W157_01, 80, 0, -20 } },
    { DEMO3D_CMD_SE_PLAY, 565, { SEQ_SE_W157_01, 80, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 580, { SEQ_SE_W157_01, 80, 0, 20, 2 } },
    { DEMO3D_CMD_SE_PLAY, 585, { SEQ_SE_W157_01, 80, -120, 10 } },
    { DEMO3D_CMD_SE_PLAY, 608, { SEQ_SE_W157_01, 80, 120, -10, 1 } },
    { DEMO3D_CMD_SE_PLAY, 630, { SEQ_SE_W157_01, 80, -60, 20, 2 } },
    { DEMO3D_CMD_SE_PLAY, 632, { SEQ_SE_W157_01, 80, 0, 10 } },
    { DEMO3D_CMD_SE_PLAY, 650, { SEQ_SE_W157_01, 80, -120, -20, 1 } },
    { DEMO3D_CMD_SE_PLAY, 671, { SEQ_SE_W157_01, 80, 0, -5, 2 } },
    { DEMO3D_CMD_SE_PLAY, 687, { SEQ_SE_W157_01, 80, 60, 5 } },
    { DEMO3D_CMD_SE_PLAY, 726, { SEQ_SE_W157_01, 80, -120, 10, 1 } },
    { DEMO3D_CMD_SE_PLAY, 750, { SEQ_SE_W157_01, 80, -30, -15, 2 } },
    { DEMO3D_CMD_SE_PLAY, 785, { SEQ_SE_W157_01, 80, 50 } },
    { DEMO3D_CMD_SE_PLAY, 819, { SEQ_SE_W157_01, 80, 127, 10, 1 } },
    { DEMO3D_CMD_SE_PLAY, 827, { SEQ_SE_W157_01, 70, -120, -10, 2 } },
    { DEMO3D_CMD_SE_PLAY, 827, { SEQ_SE_W157_01, 70, 127 } },
    { DEMO3D_CMD_SE_PLAY, 893, { SEQ_SE_W157_01, 70, -120, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 893, { SEQ_SE_W157_01, 70, 127, 20, 2 } },
    { DEMO3D_CMD_SE_PLAY, 947, { SEQ_SE_FLD_74, 127, 0, -500 } },
    { DEMO3D_CMD_SE_PLAY, 966, { SEQ_SE_FLD_74, 127, -50, -500, 1 } },
    { DEMO3D_CMD_SE_PLAY, 992, { SEQ_SE_FLD_74, 127, 0, -500, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1015, { SEQ_SE_FLD_74, 127, 30, -500 } },
    { DEMO3D_CMD_SE_PLAY, 1025, { SEQ_SE_FLD_74, 110, 0, -550, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1043, { SEQ_SE_FLD_74, 127, -20, -500, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1051, { SEQ_SE_FLD_74, 80, -50, -500 } },
    { DEMO3D_CMD_SE_PLAY, 1111, { SEQ_SE_FLD_33, 70, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1195, { SEQ_SE_FLD_39, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1195, { SEQ_SE_NDEMO_02, 100, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1210, { SEQ_SE_NDEMO_02, 70, -120 } },
    { DEMO3D_CMD_SE_PLAY, 1210, { SEQ_SE_NDEMO_02, 70, 120, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1230, { SEQ_SE_FLD_39, 127, -120, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1241, { SEQ_SE_NDEMO_02, 100, -120 } },
    { DEMO3D_CMD_SE_PLAY, 1264, { SEQ_SE_FLD_39, 127, 127, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1270, { SEQ_SE_NDEMO_02, 100, 127, 150, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1286, { SEQ_SE_NDEMO_02, 100, 127, 150 } },
    { DEMO3D_CMD_SE_PLAY, 1291, { SEQ_SE_NDEMO_02, 80, 60, 150, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1296, { SEQ_SE_NDEMO_02, 75, 0, 150, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1300, { SEQ_SE_NDEMO_02, 80, -120, 150 } },
    { DEMO3D_CMD_SE_PLAY, 1310, { SEQ_SE_NDEMO_02, 90, -60, 140, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1322, { SEQ_SE_NDEMO_02, 100, 0, 150, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1355, { SEQ_SE_NDEMO_02, 90, -60, 160 } },
    { DEMO3D_CMD_SE_PLAY, 1366, { SEQ_SE_NDEMO_02, 80, -120, 150, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1388, { SEQ_SE_W225_01, 127, -120, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1391, { SEQ_SE_W225_01, 127, -80 } },
    { DEMO3D_CMD_SE_PLAY, 1395, { SEQ_SE_W225_01, 127, -50, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1396, { SEQ_SE_W225_01, 127, -20, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1398, { SEQ_SE_W225_01, 127 } },
    { DEMO3D_CMD_SE_PLAY, 1401, { SEQ_SE_W225_01, 127, 20, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1403, { SEQ_SE_W225_01, 127, 50, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1406, { SEQ_SE_W225_01, 127, 80 } },
    { DEMO3D_CMD_SE_PLAY, 1408, { SEQ_SE_W225_01, 127, 100, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1411, { SEQ_SE_W225_01, 127, 120, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1412, { SEQ_SE_W307_EXP, 127, 0, -500 } },
    { DEMO3D_CMD_SE_PLAY, 1473, { SEQ_SE_FLD_33, 80, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1541, { SEQ_SE_FLD_33, 50, 120, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 1541, { SEQ_SE_FLD_33, 50, -120 } },
    { DEMO3D_CMD_BGM_FADE, 1582, { 1, 30 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1582, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D06_Files1_0
static const Demo3DFiles data_ov203_021c0148[] = { { data_ov203_021c0088, NELEMS(data_ov203_021c0088) } };
// pokebw2's sDemo3D13_EndCmds
static const Demo3DCmd data_ov203_021c065c[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_FileIds5_0_0
static const u16 data_ov203_021bfd78[] = { 71, 72 };
// pokebw2's sDemo3D03_Files1_2
static const Demo3DFiles data_ov203_021c00b0[] = { { data_ov203_021bfe54, NELEMS(data_ov203_021bfe54) } };
// pokebw2's sDemo3D06_Variants1
static const Demo3DVariant data_ov203_021c0128[] = { { data_ov203_021c0148, NELEMS(data_ov203_021c0148) } };
// pokebw2's sDemo3D18_Variants1
static const Demo3DVariant data_ov203_021c00c0[] = { { data_ov203_021c0178, NELEMS(data_ov203_021c0178) } };
// pokebw2's sDemo3D16_Cmds
static const Demo3DCmd data_ov203_021c121c[] = {
    { DEMO3D_CMD_SE_PLAY, 1, { SEQ_SE_VDEMO_01, 100 } },
    { DEMO3D_CMD_SE_PLAY, 160, { SEQ_SE_VDEMO_02, 100, 0, -300, 1 } },
    { DEMO3D_CMD_SE_STOP, 179, { SEQ_SE_VDEMO_01 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 210, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D01_Files4_0
static const Demo3DFiles data_ov203_021c00d0[] = { { data_ov203_021bfcf8, NELEMS(data_ov203_021bfcf8) } };
// pokebw2's sDemo3D03_FileIds4_0_0
static const u16 data_ov203_021bfd50[] = { 78, 77 };
// pokebw2's sDemo3D08_Cmds
static const Demo3DCmd data_ov203_021c1b68[] = {
    { DEMO3D_CMD_BRIGHTNESS_FADE, 0, { 1, 30, 16 } },
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_EPIROGUE_W } },
    { DEMO3D_CMD_SE_PLAY, 5, { SEQ_SE_LCDEMO_02, 30, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 31, { SEQ_SE_LCDEMO_01, 30, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 65, { SEQ_SE_LCDEMO_02, 30, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 103, { SEQ_SE_LCDEMO_01, 30, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 139, { SEQ_SE_LCDEMO_02, 30, 0, -100, 1 } },
    { DEMO3D_CMD_SE_PLAY, 185, { SEQ_SE_END_04, 127 } },
    { DEMO3D_CMD_SE_PLAY, 220, { SEQ_SE_END_06, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 223, { SEQ_SE_END_05, 100, 0, -200 } },
    { DEMO3D_CMD_SE_PLAY, 295, { SEQ_SE_END_07, 30, 0, 200, 1 } },
    { DEMO3D_CMD_BGM_FADE, 390, { 1, 25 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 390, { 1, 30, 0, 16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D07_FileIds0_0_0
static const u16 data_ov203_021c00f8[] = { 38, 39, 36, 37 };
// pokebw2's sDemo3D07_Files0_0
static const Demo3DFiles data_ov203_021c0100[] = { { data_ov203_021c00f8, NELEMS(data_ov203_021c00f8) } };
// pokebw2's sDemo3D07_FileIds2_0_0
static const u16 data_ov203_021c0118[] = { 48, 49, 46, 47 };
// pokebw2's sDemo3D07_Files2_0
static const Demo3DFiles data_ov203_021c0120[] = { { data_ov203_021c0118, NELEMS(data_ov203_021c0118) } };
// pokebw2's sDemo3D13_FileIds0_0_0
static const u16 data_ov203_021c02d4[] = { 198, 200, 199, 196, 197 };
// pokebw2's sDemo3D13_Files1_0
static const Demo3DFiles data_ov203_021c0150[] = { { data_ov203_021bfd64, NELEMS(data_ov203_021bfd64) } };
// pokebw2's sDemo3D08_EndCmds
static const Demo3DCmd data_ov203_021c0684[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D07_Variants2
static const Demo3DVariant data_ov203_021c0190[] = { { data_ov203_021c0120, NELEMS(data_ov203_021c0120) } };
// pokebw2's sDemo3D07_Variants0
static const Demo3DVariant data_ov203_021c0170[] = { { data_ov203_021c0100, NELEMS(data_ov203_021c0100) } };
// pokebw2's sDemo3D06_FileIds2_0_0
static const u16 data_ov203_021c0140[] = { 142, 143, 140, 141 };
// pokebw2's sDemo3D08_Variants0
static const Demo3DVariant data_ov203_021c0160[] = { { data_ov203_021bff78, NELEMS(data_ov203_021bff78) } };
// pokebw2's sDemo3D07_Variants1
static const Demo3DVariant data_ov203_021c01b0[] = { { data_ov203_021c0110, NELEMS(data_ov203_021c0110) } };
// pokebw2's sDemo3D07_Units
static const Demo3DUnit data_ov203_021c0434[] = {
    { data_ov203_021c0170, NELEMS(data_ov203_021c0170), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c01b0, NELEMS(data_ov203_021c01b0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0190, NELEMS(data_ov203_021c0190), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D15_Files0_0
static const Demo3DFiles data_ov203_021bfea8[] = { { data_ov203_021bfe18, NELEMS(data_ov203_021bfe18) } };
// pokebw2's sDemo3D01_Files0_1
static const Demo3DFiles data_ov203_021c01d8[] = { { data_ov203_021bfde2, NELEMS(data_ov203_021bfde2) } };
// pokebw2's sDemo3D17_FileIds2_0_0
static const u16 data_ov203_021bfd04[] = { 232, 231 };
// pokebw2's sDemo3D17_FileIds1_0_0
static const u16 data_ov203_021bfd24[] = { 230, 229 };
// pokebw2's sDemo3D01_FileIds6_0_0
static const u16 data_ov203_021c02e8[] = { 31, 33, 32, 30, 34 };
// pokebw2's sDemo3D01_Files0_0
static const Demo3DFiles data_ov203_021c0210[] = { { data_ov203_021bfd8e, NELEMS(data_ov203_021bfd8e) } };
// pokebw2's sDemo3D01_Variants1
static const Demo3DVariant data_ov203_021c0218[] = { { data_ov203_021c00a0, NELEMS(data_ov203_021c00a0) } };
// pokebw2's sDemo3D04_Variants2
static const Demo3DVariant data_ov203_021c0228[] = { { data_ov203_021bff40, NELEMS(data_ov203_021bff40) } };
// pokebw2's sDemo3D14_Setup
static const Demo3DSetup data_ov203_021c0f64 = {
    211, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(35), FX32_CONST(35), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D02_Variants2
static const Demo3DVariant data_ov203_021c0268[] = { { data_ov203_021c0250, NELEMS(data_ov203_021c0250) } };
// pokebw2's sDemo3D02_Setup
static const Demo3DSetup data_ov203_021c0fb8 = {
    10, 0xffff, ZONE_CASTELIA_CITY_12,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(40), FX32_CONST(40), 0x1555, FX32_CONST(100), FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 15, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D01_Files3_0
static const Demo3DFiles data_ov203_021c0270[] = { { data_ov203_021bfdbe, NELEMS(data_ov203_021bfdbe) } };
// pokebw2's sDemo3D01_Files6_0
static const Demo3DFiles data_ov203_021c0288[] = { { data_ov203_021c02e8, NELEMS(data_ov203_021c02e8) } };
// pokebw2's sDemo3D15_FileIds2_0_0
static const u16 data_ov203_021bfe30[] = { 222, 223, 221 };
// pokebw2's sDemo3D15_Files2_0
static const Demo3DFiles data_ov203_021bff28[] = { { data_ov203_021bfe30, NELEMS(data_ov203_021bfe30) } };
// pokebw2's sDemo3D09_Files1_0
static const Demo3DFiles data_ov203_021c02a0[] = { { data_ov203_021c0290, NELEMS(data_ov203_021c0290) } };
// pokebw2's sDemo3D14_Variants1
static const Demo3DVariant data_ov203_021c02a8[] = { { data_ov203_021c0260, NELEMS(data_ov203_021c0260) } };
// pokebw2's sDemo3D09_FileIds2_0_0
static const u16 data_ov203_021c02c0[] = { 154, 156, 155, 152, 153 };
// pokebw2's sDemo3D09_Files2_0
static const Demo3DFiles data_ov203_021bfe60[] = { { data_ov203_021c02c0, NELEMS(data_ov203_021c02c0) } };
// pokebw2's sDemo3D09_Variants0
static const Demo3DVariant data_ov203_021bfff8[] = { { data_ov203_021bfe78, NELEMS(data_ov203_021bfe78) } };
// pokebw2's sDemo3D09_Variants1
static const Demo3DVariant data_ov203_021bfe68[] = { { data_ov203_021c02a0, NELEMS(data_ov203_021c02a0) } };
// pokebw2's sDemo3D09_Variants2
static const Demo3DVariant data_ov203_021bffe8[] = { { data_ov203_021bfe60, NELEMS(data_ov203_021bfe60) } };
// pokebw2's sDemo3D09_Units
static const Demo3DUnit data_ov203_021c035c[] = {
    { data_ov203_021bfff8, NELEMS(data_ov203_021bfff8), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bfe68, NELEMS(data_ov203_021bfe68), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bffe8, NELEMS(data_ov203_021bffe8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D01_Variants4
static const Demo3DVariant data_ov203_021bffd8[] = { { data_ov203_021c00d0, NELEMS(data_ov203_021c00d0) } };
// pokebw2's sDemo3D03_Files2_1
static const Demo3DFiles data_ov203_021c00e0[] = { { data_ov203_021bfcfc, NELEMS(data_ov203_021bfcfc) } };
// pokebw2's sDemo3D03_Files1_8
static const Demo3DFiles data_ov203_021bffc8[] = { { data_ov203_021bfd9a, NELEMS(data_ov203_021bfd9a) } };
// pokebw2's sDemo3D03_Files1_5
static const Demo3DFiles data_ov203_021bffc0[] = { { data_ov203_021bfe5a, NELEMS(data_ov203_021bfe5a) } };
// pokebw2's sDemo3D10_Files0_0
static const Demo3DFiles data_ov203_021bffb8[] = { { data_ov203_021bfe4e, NELEMS(data_ov203_021bfe4e) } };
// pokebw2's sDemo3D17_Setup
static const Demo3DSetup data_ov203_021c0bc8 = {
    233, 0xffff, ZONE_NUVEMA_TOWN,
    0, 0, 1, 1, 0, 0, 0, 1, 0, 16,
    FX32_CONST(60), FX32_CONST(60), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    TRUE, 10, FALSE, 0,
};
// pokebw2's sDemo3D03_FileIds2_0_0
static const u16 data_ov203_021bfd5c[] = { 65, 66 };
// pokebw2's sDemo3D03_Files3_0
static const Demo3DFiles data_ov203_021bffa0[] = { { data_ov203_021bfd14, NELEMS(data_ov203_021bfd14) } };
// pokebw2's sDemo3D17_Files0_0
static const Demo3DFiles data_ov203_021bff98[] = { { data_ov203_021bfdd6, NELEMS(data_ov203_021bfdd6) } };
// pokebw2's sDemo3D10_Variants0
static const Demo3DVariant data_ov203_021bfec0[] = { { data_ov203_021bffb8, NELEMS(data_ov203_021bffb8) } };
// pokebw2's sDemo3D10_Variants1
static const Demo3DVariant data_ov203_021bff88[] = { { data_ov203_021bffa8, NELEMS(data_ov203_021bffa8) } };
// pokebw2's sDemo3D10_Variants2
static const Demo3DVariant data_ov203_021bfed8[] = { { data_ov203_021bfeb8, NELEMS(data_ov203_021bfeb8) } };
// pokebw2's sDemo3D10_Units
static const Demo3DUnit data_ov203_021c03a4[] = {
    { data_ov203_021bfec0, NELEMS(data_ov203_021bfec0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff88, NELEMS(data_ov203_021bff88), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bfed8, NELEMS(data_ov203_021bfed8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D10_Cmds
static const Demo3DCmd data_ov203_021c2130[] = {
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_LEGEND_BORN_W } },
    { DEMO3D_CMD_SE_PLAY, 42, { SEQ_SE_LBDEMO_02, 40 } },
    { DEMO3D_CMD_SE_PLAY, 135, { SEQ_SE_LBDEMO_01, 80, 0, 0, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 220, { 1, 50, 0, 12 } },
    { DEMO3D_CMD_SE_PLAY, 263, { SEQ_SE_FLD_127, 127, 0, 0, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 271, { 1, 59, 12 } },
    { DEMO3D_CMD_SE_PLAY, 382, { SEQ_SE_LBDEMO_03, 127 } },
    { DEMO3D_CMD_SE_PLAY, 430, { SEQ_SE_LBDEMO_03, 127 } },
    { DEMO3D_CMD_SE_PLAY, 495, { SEQ_SE_LBDEMO_03, 127 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 510, { 1, 15, 0, 8 } },
    { DEMO3D_CMD_SE_PLAY, 525, { SEQ_SE_LBDEMO_05, 127, 0, 0, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 545, { 1, 25, 8 } },
    { DEMO3D_CMD_SE_PLAY, 616, { SEQ_SE_LBDEMO_04, 127, -128, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 616, { SEQ_SE_LBDEMO_04, 127, 127 } },
    { DEMO3D_CMD_SE_PLAY, 746, { SEQ_SE_LBDEMO_09, 127, 0, 0, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 826, { 1, 1, 0, 12 } },
    { DEMO3D_CMD_SE_PLAY, 827, { SEQ_SE_W307_EXP, 127 } },
    { DEMO3D_CMD_SE_PLAY, 827, { SEQ_SE_FLD_166, 115, 0, 0, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 827, { 1, 2, 12 } },
    { DEMO3D_CMD_SE_PLAY, 840, { SEQ_SE_W348_02, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 1080, { SEQ_SE_SHINKA_W060C, 100, 0, 0, 2 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1159, { 1, 15, 0, -16 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1179, { 1, 15, -16 } },
    { DEMO3D_CMD_BGM_FADE, 1310, { 1, 50 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 1310, { 1, 50, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_Files4_0
static const Demo3DFiles data_ov203_021bffd0[] = { { data_ov203_021bfd50, NELEMS(data_ov203_021bfd50) } };
// pokebw2's sDemo3D17_Cmds
static const Demo3DCmd data_ov203_021c089c[] = {
    { DEMO3D_CMD_BRIGHTNESS_FADE, 701, { 1, 20, 16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D17_EndCmds
static const Demo3DCmd data_ov203_021c0544[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_FileIds1_0_0
static const u16 data_ov203_021bfe24[] = { 81, 94, 225 };
// pokebw2's sDemo3D04_Files1_0
static const Demo3DFiles data_ov203_021bff48[] = { { data_ov203_021bfe2a, NELEMS(data_ov203_021bfe2a) } };
// pokebw2's sDemo3D03_FileIds1_7_0
static const u16 data_ov203_021bfe3c[] = { 80, 89, 225 };
// pokebw2's sDemo3D03_FileIds1_4_0
static const u16 data_ov203_021bfdd0[] = { 95, 89, 225 };
// pokebw2's sDemo3D04_Files3_0
static const Demo3DFiles data_ov203_021c05e4[] = {
    { data_ov203_021bfd10, NELEMS(data_ov203_021bfd10) },
    { data_ov203_021bfd44, NELEMS(data_ov203_021bfd44) },
    { data_ov203_021bfd00, NELEMS(data_ov203_021bfd00) },
    { data_ov203_021bfd6c, NELEMS(data_ov203_021bfd6c) },
    { data_ov203_021bfd30, NELEMS(data_ov203_021bfd30) },
};
// pokebw2's sDemo3D05_Setup
static const Demo3DSetup data_ov203_021c0cc4 = {
    131, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 1, 0, 1, 1, 1, 0, 16,
    FX32_CONST(40), FX32_CONST(40), 0x1555, FX32_CONST(1), FX32_CONST(2048), 0x800,
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(4, 4, 4), GX_RGB(8, 8, 8), GX_RGB(12, 12, 12), GX_RGB(16, 16, 16), GX_RGB(20, 20, 20), GX_RGB(24, 24, 24), GX_RGB(31, 31, 31) },
    TRUE, 30, TRUE, 0,
};
// pokebw2's sDemo3D04_Variants1
static const Demo3DVariant data_ov203_021bff38[] = { { data_ov203_021bff48, NELEMS(data_ov203_021bff48) } };
// pokebw2's sDemo3D04_Variants3
static const Demo3DVariant data_ov203_021bff70[] = { { data_ov203_021c05e4, NELEMS(data_ov203_021c05e4) } };
// pokebw2's sDemo3D03_FileIds5_0_1
static const u16 data_ov203_021bfd0c[] = { 71, 72 };
// pokebw2's sDemo3D03_Variants0
static const Demo3DVariant data_ov203_021c0318[] = {
    { data_ov203_021c01f8, NELEMS(data_ov203_021c01f8) },
    { data_ov203_021c0298, NELEMS(data_ov203_021c0298) },
};
// pokebw2's sDemo3D12_EndCmds
static const Demo3DCmd data_ov203_021c060c[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D18_Variants2
static const Demo3DVariant data_ov203_021c01e8[] = { { data_ov203_021bfe90, NELEMS(data_ov203_021bfe90) } };
// pokebw2's sDemo3D16_EndCmds
static const Demo3DCmd data_ov203_021c0634[] = {
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D16_FileIds2_0_3
static const u16 data_ov203_021bfd60[] = { 249, 251 };
// pokebw2's sDemo3D08_Files1_0
static const Demo3DFiles data_ov203_021c01b8[] = { { data_ov203_021c02f2, NELEMS(data_ov203_021c02f2) } };
// pokebw2's sDemo3D01_Files2_0
static const Demo3DFiles data_ov203_021c01a8[] = { { data_ov203_021bfe36, NELEMS(data_ov203_021bfe36) } };
// pokebw2's sDemo3D17_Files1_0
static const Demo3DFiles data_ov203_021c0060[] = { { data_ov203_021bfd24, NELEMS(data_ov203_021bfd24) } };
// pokebw2's sDemo3D12_Variants0
static const Demo3DVariant data_ov203_021c0070[] = { { data_ov203_021c0030, NELEMS(data_ov203_021c0030) } };
// pokebw2's sDemo3D12_Variants1
static const Demo3DVariant data_ov203_021c0080[] = { { data_ov203_021c0040, NELEMS(data_ov203_021c0040) } };
// pokebw2's sDemo3D16_FileIds2_0_0
static const u16 data_ov203_021bfd08[] = { 249, 250 };
// pokebw2's sDemo3D12_Variants2
static const Demo3DVariant data_ov203_021c0090[] = { { data_ov203_021c0188, NELEMS(data_ov203_021c0188) } };
// pokebw2's sDemo3D12_Units
static const Demo3DUnit data_ov203_021c0404[] = {
    { data_ov203_021c0070, NELEMS(data_ov203_021c0070), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0080, NELEMS(data_ov203_021c0080), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0090, NELEMS(data_ov203_021c0090), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D07_EndCmds
static const Demo3DCmd data_ov203_021c09dc[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D07_Setup
static const Demo3DSetup data_ov203_021c0e68 = {
    45, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 1, 0, 0, 0, 1, 0, 16,
    FX32_CONST(13), FX32_CONST(13), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    58, -57, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D17_Files2_0
static const Demo3DFiles data_ov203_021c00c8[] = { { data_ov203_021bfd04, NELEMS(data_ov203_021bfd04) } };
// pokebw2's sDemo3D17_Variants0
static const Demo3DVariant data_ov203_021c00e8[] = { { data_ov203_021bff98, NELEMS(data_ov203_021bff98) } };
// pokebw2's sDemo3D13_Files0_0
static const Demo3DFiles data_ov203_021c0108[] = { { data_ov203_021c02d4, NELEMS(data_ov203_021c02d4) } };
// pokebw2's sDemo3D06_Variants0
static const Demo3DVariant data_ov203_021c0130[] = { { data_ov203_021c0158, NELEMS(data_ov203_021c0158) } };
// pokebw2's sDemo3D06_Files2_0
static const Demo3DFiles data_ov203_021c0138[] = { { data_ov203_021c0140, NELEMS(data_ov203_021c0140) } };
// pokebw2's sDemo3D03_Files1_4
static const Demo3DFiles data_ov203_021c0168[] = { { data_ov203_021bfdd0, NELEMS(data_ov203_021bfdd0) } };
// pokebw2's sDemo3D13_Cmds
static const Demo3DCmd data_ov203_021c12e4[] = {
    { DEMO3D_CMD_SE_PLAY, 100, { SEQ_SE_MEET_01, 20, 0, -1000 } },
    { DEMO3D_CMD_SE_PLAY, 330, { SEQ_SE_MEET_02, 127, 0, -500, 1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 430, { 1, 120, 0, -16 } },
    { DEMO3D_CMD_SE_STOP, 549, { SEQ_SE_MEET_01 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D13_Variants1
static const Demo3DVariant data_ov203_021c0198[] = { { data_ov203_021c0150, NELEMS(data_ov203_021c0150) } };
// pokebw2's sDemo3D17_Variants1
static const Demo3DVariant data_ov203_021c01c8[] = { { data_ov203_021c0060, NELEMS(data_ov203_021c0060) } };
// pokebw2's sDemo3D08_FileIds2_0_0
static const u16 data_ov203_021c01f0[] = { 61, 62, 59, 60 };
// pokebw2's sDemo3D08_Files2_0
static const Demo3DFiles data_ov203_021c0208[] = { { data_ov203_021c01f0, NELEMS(data_ov203_021c01f0) } };
// pokebw2's sDemo3D08_Variants1
static const Demo3DVariant data_ov203_021c0220[] = { { data_ov203_021c01b8, NELEMS(data_ov203_021c01b8) } };
// pokebw2's sDemo3D08_Variants2
static const Demo3DVariant data_ov203_021c0230[] = { { data_ov203_021c0208, NELEMS(data_ov203_021c0208) } };
// pokebw2's sDemo3D08_Units
static const Demo3DUnit data_ov203_021c044c[] = {
    { data_ov203_021c0160, NELEMS(data_ov203_021c0160), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0220, NELEMS(data_ov203_021c0220), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0230, NELEMS(data_ov203_021c0230), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D16_Files2_0
static const Demo3DFiles data_ov203_021c06fc[] = {
    { data_ov203_021bfd08, NELEMS(data_ov203_021bfd08) },
    { data_ov203_021bfd28, NELEMS(data_ov203_021bfd28) },
    { data_ov203_021bfd18, NELEMS(data_ov203_021bfd18) },
    { data_ov203_021bfd60, NELEMS(data_ov203_021bfd60) },
    { data_ov203_021bfd54, NELEMS(data_ov203_021bfd54) },
};
// pokebw2's sDemo3D02_Cmds
static const Demo3DCmd data_ov203_021c1654[] = {
    { DEMO3D_CMD_SE_PLAY, 1, { SEQ_SE_VDEMO_01, 117 } },
    { DEMO3D_CMD_SE_PLAY, 125, { SEQ_SE_VDEMO_03, 117, 0, -200, 2 } },
    { DEMO3D_CMD_SE_PLAY, 201, { SEQ_SE_FLD_78, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 269, { SEQ_SE_DUMMY5 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 270, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D13_Variants0
static const Demo3DVariant data_ov203_021c02b0[] = { { data_ov203_021c0108, NELEMS(data_ov203_021c0108) } };
// pokebw2's sDemo3D07_Cmds
static const Demo3DCmd data_ov203_021c19d8[] = {
    { DEMO3D_CMD_BRIGHTNESS_FADE, 0, { 1, 30, -16 } },
    { DEMO3D_CMD_BGM_PLAY, 0, { SEQ_BGM_E_EPIROGUE } },
    { DEMO3D_CMD_SE_PLAY, 1, { SEQ_SE_W245_BUCKL, 10 } },
    { DEMO3D_CMD_SE_PLAY, 205, { SEQ_SE_FLD_167, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 210, { SEQ_SE_FLD_169, 127, 0, 0, 1 } },
    { DEMO3D_CMD_SE_PLAY, 215, { SEQ_SE_FLD_167, 127, 0, 0, 2 } },
    { DEMO3D_CMD_SE_PLAY, 250, { SEQ_SE_END_03, 127 } },
    { DEMO3D_CMD_BGM_FADE, 330, { 1, 30 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 330, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D18_Setup
static const Demo3DSetup data_ov203_021c0b20 = {
    241, 0xffff, ZONE_NUVEMA_TOWN,
    0, 0, 1, 1, 0, 0, 0, 1, 0, 16,
    FX32_CONST(60), FX32_CONST(60), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    TRUE, 10, FALSE, 0,
};
// pokebw2's sDemo3D18_Units
static const Demo3DUnit data_ov203_021c0374[] = {
    { data_ov203_021c02b8, NELEMS(data_ov203_021c02b8), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c00c0, NELEMS(data_ov203_021c00c0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c01e8, NELEMS(data_ov203_021c01e8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D16_Variants2
static const Demo3DVariant data_ov203_021bfe80[] = { { data_ov203_021c06fc, NELEMS(data_ov203_021c06fc) } };
// pokebw2's sDemo3D03_Files2_0
static const Demo3DFiles data_ov203_021bfe98[] = { { data_ov203_021bfd5c, NELEMS(data_ov203_021bfd5c) } };
// pokebw2's sDemo3D15_Cmds
static const Demo3DCmd data_ov203_021c10b4[] = {
    { DEMO3D_CMD_SE_PLAY, 39, { SEQ_SE_RIZDEMO_02, 127 } },
    { DEMO3D_CMD_SE_PLAY, 340, { SEQ_SE_RIZDEMO_01, 127 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 420, { 1, 30, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D15_FileIds1_0_0
static const u16 data_ov203_021bfeb0[] = { 215, 216, 217, 218 };
// pokebw2's sDemo3D15_FileIds1_0_1
static const u16 data_ov203_021bfec8[] = { 215, 216, 217, 218 };
// pokebw2's sDemo3D03_Files5_0
static const Demo3DFiles data_ov203_021c051c[] = {
    { data_ov203_021bfd78, NELEMS(data_ov203_021bfd78) },
    { data_ov203_021bfd0c, NELEMS(data_ov203_021bfd0c) },
    { data_ov203_021bfd20, NELEMS(data_ov203_021bfd20) },
    { data_ov203_021bfd40, NELEMS(data_ov203_021bfd40) },
    { data_ov203_021bfd2c, NELEMS(data_ov203_021bfd2c) },
};
// pokebw2's sDemo3D03_Variants3
static const Demo3DVariant data_ov203_021bfee0[] = { { data_ov203_021bffa0, NELEMS(data_ov203_021bffa0) } };
// pokebw2's sDemo3D03_Files1_3
static const Demo3DFiles data_ov203_021bfef0[] = { { data_ov203_021bfd7c, NELEMS(data_ov203_021bfd7c) } };
// pokebw2's sDemo3D15_Files1_0
static const Demo3DFiles data_ov203_021c0594[] = {
    { data_ov203_021bfeb0, NELEMS(data_ov203_021bfeb0) },
    { data_ov203_021bfec8, NELEMS(data_ov203_021bfec8) },
    { data_ov203_021bfdca, NELEMS(data_ov203_021bfdca) },
    { data_ov203_021bfe42, NELEMS(data_ov203_021bfe42) },
    { data_ov203_021bfe48, NELEMS(data_ov203_021bfe48) },
};
// pokebw2's sDemo3D11_Files1_0
static const Demo3DFiles data_ov203_021bff10[] = { { data_ov203_021bfdb8, NELEMS(data_ov203_021bfdb8) } };
// pokebw2's sDemo3D11_EndCmds
static const Demo3DCmd data_ov203_021c08ec[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D11_Variants1
static const Demo3DVariant data_ov203_021bff50[] = { { data_ov203_021bff10, NELEMS(data_ov203_021bff10) } };
// pokebw2's sDemo3D15_Variants1
static const Demo3DVariant data_ov203_021bff90[] = { { data_ov203_021c0594, NELEMS(data_ov203_021c0594) } };
// pokebw2's sDemo3D15_Variants2
static const Demo3DVariant data_ov203_021c0000[] = { { data_ov203_021bff28, NELEMS(data_ov203_021bff28) } };
// pokebw2's sDemo3D01_Variants5
static const Demo3DVariant data_ov203_021c0010[] = { { data_ov203_021c01d0, NELEMS(data_ov203_021c01d0) } };
// pokebw2's sDemo3D06_EndCmds
static const Demo3DCmd data_ov203_021c098c[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D06_Setup
static const Demo3DSetup data_ov203_021c0dc0 = {
    144, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(35), FX32_CONST(35), 0x1555, 0x19a, FX32_CONST(3000), FX32_CONST(1),
    191, -176, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 10, FALSE, 0,
};
// pokebw2's sDemo3D16_FileIds1_0_0
static const u16 data_ov203_021bfdee[] = { 245, 246, 244 };
// pokebw2's sDemo3D01_Variants2
static const Demo3DVariant data_ov203_021c0098[] = { { data_ov203_021c01a8, NELEMS(data_ov203_021c01a8) } };
// pokebw2's sDemo3D06_Variants2
static const Demo3DVariant data_ov203_021c00b8[] = { { data_ov203_021c0138, NELEMS(data_ov203_021c0138) } };
// pokebw2's sDemo3D01_Variants6
static const Demo3DVariant data_ov203_021c00d8[] = { { data_ov203_021c0288, NELEMS(data_ov203_021c0288) } };
// pokebw2's sDemo3D01_Cmds
static const Demo3DCmd data_ov203_021c1474[] = {
    { DEMO3D_CMD_SE_PLAY, DEMO3D_CMD_FRAME_START, { SEQ_SE_FLD_79, -1, -1, -1, -1 } },
    { DEMO3D_CMD_SE_PLAY, 1350, { SEQ_SE_FLD_78, -1, -1, -1, -1 } },
    { DEMO3D_CMD_SE_PLAY, 2700, { SEQ_SE_FLD_78, -1, -1, -1, -1 } },
    { DEMO3D_CMD_SE_PLAY, 4050, { SEQ_SE_FLD_78, -1, -1, -1, -1 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 5340, { 1, 60, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D01_EndCmds
static const Demo3DCmd data_ov203_021c0a2c[] = {
    { DEMO3D_CMD_SE_STOP, 0, { SEQ_SE_FLD_79, -1 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D01_Variants3
static const Demo3DVariant data_ov203_021c0180[] = { { data_ov203_021c0270, NELEMS(data_ov203_021c0270) } };
// pokebw2's sDemo3D13_Units
static const Demo3DUnit data_ov203_021c0328[] = {
    { data_ov203_021c02b0, NELEMS(data_ov203_021c02b0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0198, NELEMS(data_ov203_021c0198), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D01_Variants0
static const Demo3DVariant data_ov203_021c0338[] = {
    { data_ov203_021c0210, NELEMS(data_ov203_021c0210) },
    { data_ov203_021c01d8, NELEMS(data_ov203_021c01d8) },
};
// pokebw2's sDemo3D17_Variants2
static const Demo3DVariant data_ov203_021c0248[] = { { data_ov203_021c00c8, NELEMS(data_ov203_021c00c8) } };
// pokebw2's sDemo3D17_Units
static const Demo3DUnit data_ov203_021c0464[] = {
    { data_ov203_021c00e8, NELEMS(data_ov203_021c00e8), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c01c8, NELEMS(data_ov203_021c01c8), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0248, NELEMS(data_ov203_021c0248), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D03_Cmds
static const Demo3DCmd data_ov203_021c13ac[] = {
    { DEMO3D_CMD_MESSAGE, 180, { 205, 0, 3, 180, 2, 19, 28, 4 } },
    { DEMO3D_CMD_MESSAGE, 460, { 205, 1, 3, 180, 2, 19, 28, 4 } },
    { DEMO3D_CMD_MESSAGE, 760, { 205, 2, 3, 180, 2, 19, 28, 4 } },
    { DEMO3D_CMD_BRIGHTNESS_FADE, 990, { 1, 60, 0, -16 } },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D03_Files1_0
static const Demo3DFiles data_ov203_021c0008[] = { { data_ov203_021bfe24, NELEMS(data_ov203_021bfe24) } };
// pokebw2's sDemo3D03_Files1_7
static const Demo3DFiles data_ov203_021bfe88[] = { { data_ov203_021bfe3c, NELEMS(data_ov203_021bfe3c) } };
// pokebw2's sDemo3D03_Variants2
static const Demo3DVariant data_ov203_021c0494[] = {
    { data_ov203_021bfe98, NELEMS(data_ov203_021bfe98) },
    { data_ov203_021c00e0, NELEMS(data_ov203_021c00e0) },
    { data_ov203_021bffb0, NELEMS(data_ov203_021bffb0) },
    { data_ov203_021bff08, NELEMS(data_ov203_021bff08) },
};
// pokebw2's sDemo3D03_Variants4
static const Demo3DVariant data_ov203_021bfed0[] = { { data_ov203_021bffd0, NELEMS(data_ov203_021bffd0) } };
// pokebw2's sDemo3D03_Files1_6
static const Demo3DFiles data_ov203_021bfee8[] = { { data_ov203_021bfd88, NELEMS(data_ov203_021bfd88) } };
// pokebw2's sDemo3D03_Variants5
static const Demo3DVariant data_ov203_021bff68[] = { { data_ov203_021c051c, NELEMS(data_ov203_021c051c) } };
// pokebw2's sDemo3D02_Files1_0
static const Demo3DFiles data_ov203_021c0258[] = { { data_ov203_021bfd94, NELEMS(data_ov203_021bfd94) } };
// pokebw2's sDemo3D11_Units
static const Demo3DUnit data_ov203_021c03bc[] = {
    { data_ov203_021c0240, NELEMS(data_ov203_021c0240), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff50, NELEMS(data_ov203_021bff50), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff80, NELEMS(data_ov203_021bff80), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D02_Variants0
static const Demo3DVariant data_ov203_021c0028[] = { { data_ov203_021bffe0, NELEMS(data_ov203_021bffe0) } };
// pokebw2's sDemo3D13_Setup
static const Demo3DSetup data_ov203_021c0e14 = {
    203, 0xffff, ZONE_NUVEMA_TOWN,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(35), FX32_CONST(35), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D03_Variants1
static const Demo3DVariant data_ov203_021c07b4[] = {
    { data_ov203_021c0008, NELEMS(data_ov203_021c0008) },
    { data_ov203_021c0078, NELEMS(data_ov203_021c0078) },
    { data_ov203_021c00b0, NELEMS(data_ov203_021c00b0) },
    { data_ov203_021bfef0, NELEMS(data_ov203_021bfef0) },
    { data_ov203_021c0168, NELEMS(data_ov203_021c0168) },
    { data_ov203_021bffc0, NELEMS(data_ov203_021bffc0) },
    { data_ov203_021bfee8, NELEMS(data_ov203_021bfee8) },
    { data_ov203_021bfe88, NELEMS(data_ov203_021bfe88) },
    { data_ov203_021bffc8, NELEMS(data_ov203_021bffc8) },
};
// pokebw2's sDemo3D06_Units
static const Demo3DUnit data_ov203_021c041c[] = {
    { data_ov203_021c0130, NELEMS(data_ov203_021c0130), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0128, NELEMS(data_ov203_021c0128), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c00b8, NELEMS(data_ov203_021c00b8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D16_Files1_0
static const Demo3DFiles data_ov203_021c00f0[] = { { data_ov203_021bfdee, NELEMS(data_ov203_021bfdee) } };
// pokebw2's sDemo3D05_Files2_1
static const Demo3DFiles data_ov203_021c01c0[] = { { data_ov203_021bfda0, NELEMS(data_ov203_021bfda0) } };
// pokebw2's sDemo3D05_Variants2
static const Demo3DVariant data_ov203_021c04d4[] = {
    { data_ov203_021c0018, NELEMS(data_ov203_021c0018) },
    { data_ov203_021c01c0, NELEMS(data_ov203_021c01c0) },
    { data_ov203_021c0038, NELEMS(data_ov203_021c0038) },
    { data_ov203_021c01a0, NELEMS(data_ov203_021c01a0) },
};
// pokebw2's sDemo3D01_Units
static const Demo3DUnit data_ov203_021c077c[] = {
    { data_ov203_021c0338, NELEMS(data_ov203_021c0338), DEMO3D_UNIT_BY_SEX },
    { data_ov203_021c0218, NELEMS(data_ov203_021c0218), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0098, NELEMS(data_ov203_021c0098), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0180, NELEMS(data_ov203_021c0180), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bffd8, NELEMS(data_ov203_021bffd8), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0010, NELEMS(data_ov203_021c0010), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c00d8, NELEMS(data_ov203_021c00d8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D16_Variants1
static const Demo3DVariant data_ov203_021bfe70[] = { { data_ov203_021c00f0, NELEMS(data_ov203_021c00f0) } };
// pokebw2's sDemo3D16_Units
static const Demo3DUnit data_ov203_021c038c[] = {
    { data_ov203_021c0278, NELEMS(data_ov203_021c0278), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bfe70, NELEMS(data_ov203_021bfe70), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bfe80, NELEMS(data_ov203_021bfe80), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D03_Units
static const Demo3DUnit data_ov203_021c074c[] = {
    { data_ov203_021c0318, NELEMS(data_ov203_021c0318), DEMO3D_UNIT_BY_SEX },
    { data_ov203_021c07b4, NELEMS(data_ov203_021c07b4), DEMO3D_UNIT_BY_PARAM },
    { data_ov203_021c0494, NELEMS(data_ov203_021c0494), DEMO3D_UNIT_BY_SEASON },
    { data_ov203_021bfee0, NELEMS(data_ov203_021bfee0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bfed0, NELEMS(data_ov203_021bfed0), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff68, NELEMS(data_ov203_021bff68), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D12_Setup
static const Demo3DSetup data_ov203_021c0d18 = {
    195, 0xffff, ZONE_NUVEMA_TOWN,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 16,
    FX32_CONST(28), FX32_CONST(28), 0x1555, 0x19a, FX32_CONST(2048), 0x19a,
    149, -142, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 30, FALSE, 0,
};
// pokebw2's sDemo3D05_Variants0
static const Demo3DVariant data_ov203_021c0048[] = { { data_ov203_021bfff0, NELEMS(data_ov203_021bfff0) } };
// pokebw2's sDemo3D05_Units
static const Demo3DUnit data_ov203_021c03ec[] = {
    { data_ov203_021c0048, NELEMS(data_ov203_021c0048), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0050, NELEMS(data_ov203_021c0050), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c04d4, NELEMS(data_ov203_021c04d4), DEMO3D_UNIT_BY_SEASON },
};
// pokebw2's sDemo3D09_EndCmds
static const Demo3DCmd data_ov203_021c0a7c[] = {
    { DEMO3D_CMD_BGM_STOP, 0 },
    { DEMO3D_CMD_END },
};
// pokebw2's sDemo3D02_Variants1
static const Demo3DVariant data_ov203_021c0068[] = { { data_ov203_021c0258, NELEMS(data_ov203_021c0258) } };
// pokebw2's sDemo3D14_Units
static const Demo3DUnit data_ov203_021c0308[] = {
    { data_ov203_021c0280, NELEMS(data_ov203_021c0280), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c02a8, NELEMS(data_ov203_021c02a8), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D04_Setup
static const Demo3DSetup data_ov203_021c0c70 = {
    104, 0xffff, ZONE_CASTELIA_CITY_9,
    1, 0, 1, 0, 0, 0, 1, 1, 0, 16,
    FX32_CONST(40), FX32_CONST(40), 0x1555, 0x19a, FX32_CONST(2048), FX32_CONST(1),
    0, 0, FX32_CONST(0),
    GX_RGB(0, 0, 0), 31, 0, 0x7fff,
    GX_RGB(0, 0, 0), 31, 0, 0, 0,
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0) },
    FALSE, 20, FALSE, 0,
};
// pokebw2's sDemo3D04_Units
static const Demo3DUnit data_ov203_021c04b4[] = {
    { data_ov203_021c0238, NELEMS(data_ov203_021c0238), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff38, NELEMS(data_ov203_021bff38), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0228, NELEMS(data_ov203_021c0228), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff70, NELEMS(data_ov203_021bff70), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D02_Units
static const Demo3DUnit data_ov203_021c047c[] = {
    { data_ov203_021c0028, NELEMS(data_ov203_021c0028), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0068, NELEMS(data_ov203_021c0068), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0268, NELEMS(data_ov203_021c0268), DEMO3D_UNIT_SINGLE },
};
// pokebw2's sDemo3D15_Variants0
static const Demo3DVariant data_ov203_021bff30[] = { { data_ov203_021bfea8, NELEMS(data_ov203_021bfea8) } };
// pokebw2's sDemo3D15_Units
static const Demo3DUnit data_ov203_021c03d4[] = {
    { data_ov203_021bff30, NELEMS(data_ov203_021bff30), DEMO3D_UNIT_SINGLE },
    { data_ov203_021bff90, NELEMS(data_ov203_021bff90), DEMO3D_UNIT_SINGLE },
    { data_ov203_021c0000, NELEMS(data_ov203_021c0000), DEMO3D_UNIT_SINGLE },
};
static const Demo3DScene data_ov203_021c185c[DEMO3D_ID_COUNT] = {
    { NULL, NULL, NULL, NULL, 0 },
    { &data_ov203_021c0ebc, data_ov203_021c1474, data_ov203_021c0a2c, data_ov203_021c077c, NELEMS(data_ov203_021c077c) },
    { &data_ov203_021c0fb8, data_ov203_021c1654, data_ov203_021c06d4, data_ov203_021c047c, NELEMS(data_ov203_021c047c) },
    { &data_ov203_021c1060, data_ov203_021c13ac, data_ov203_021c0724, data_ov203_021c074c, NELEMS(data_ov203_021c074c) },
    { &data_ov203_021c0c70, data_ov203_021c1154, data_ov203_021c056c, data_ov203_021c04b4, NELEMS(data_ov203_021c04b4) },
    { &data_ov203_021c0cc4, data_ov203_021c1744, data_ov203_021c093c, data_ov203_021c03ec, NELEMS(data_ov203_021c03ec) },
    { &data_ov203_021c0dc0, data_ov203_021c2ec8, data_ov203_021c098c, data_ov203_021c041c, NELEMS(data_ov203_021c041c) },
    { &data_ov203_021c0e68, data_ov203_021c19d8, data_ov203_021c09dc, data_ov203_021c0434, NELEMS(data_ov203_021c0434) },
    { &data_ov203_021c0f10, data_ov203_021c1b68, data_ov203_021c0684, data_ov203_021c044c, NELEMS(data_ov203_021c044c) },
    { &data_ov203_021c100c, data_ov203_021c2540, data_ov203_021c0a7c, data_ov203_021c035c, NELEMS(data_ov203_021c035c) },
    { &data_ov203_021c0b74, data_ov203_021c2130, data_ov203_021c07fc, data_ov203_021c03a4, NELEMS(data_ov203_021c03a4) },
    { &data_ov203_021c0c1c, data_ov203_021c1d98, data_ov203_021c08ec, data_ov203_021c03bc, NELEMS(data_ov203_021c03bc) },
    { &data_ov203_021c0d18, data_ov203_021c29f0, data_ov203_021c060c, data_ov203_021c0404, NELEMS(data_ov203_021c0404) },
    { &data_ov203_021c0e14, data_ov203_021c12e4, data_ov203_021c065c, data_ov203_021c0328, NELEMS(data_ov203_021c0328) },
    { &data_ov203_021c0f64, data_ov203_021c1564, data_ov203_021c06ac, data_ov203_021c0308, NELEMS(data_ov203_021c0308) },
    { &data_ov203_021c0acc, data_ov203_021c10b4, data_ov203_021c04f4, data_ov203_021c03d4, NELEMS(data_ov203_021c03d4) },
    { &data_ov203_021c0d6c, data_ov203_021c121c, data_ov203_021c0634, data_ov203_021c038c, NELEMS(data_ov203_021c038c) },
    { &data_ov203_021c0bc8, data_ov203_021c089c, data_ov203_021c0544, data_ov203_021c0464, NELEMS(data_ov203_021c0464) },
    { &data_ov203_021c0b20, data_ov203_021c084c, data_ov203_021c05bc, data_ov203_021c0374, NELEMS(data_ov203_021c0374) },
};

// pokebw2's Demo3DData_GetSetup
const Demo3DSetup *func_ov203_021be7a8(u32 demoId) {
    return data_ov203_021c185c[func_ov203_021be79c(demoId)].setup;
}

// Loads the camera's animation of a demo: the file its set-up names, from the demo resource archive
G3DCurve *func_ov203_021be7bc(u32 demoId, HeapID heapId, u32 preloadCount) {
    return GFL_G3DCurveLoadFileStream(heapId, ARCID_DEMO3D_RESOURCE, func_ov203_021be7a8(demoId)->cameraFile,
                                      preloadCount);
}

// pokebw2's Demo3DData_GetUnitCount
u8 func_ov203_021be7d8(u32 demoId) {
    return data_ov203_021c185c[func_ov203_021be79c(demoId)].unitCount;
}

// pokebw2's Demo3DData_CreateSceneSetup
G3DSceneSetup *func_ov203_021be7f0(HeapID heapId) {
    G3DSceneSetup *setup = GFL_HeapAllocate(heapId, sizeof(G3DSceneSetup), TRUE, "demo3d_data.c", 171);
    G3DSceneResourceSetup *resources =
        GFL_HeapAllocate(heapId, sizeof(G3DSceneResourceSetup) * RESOURCE_MAX, TRUE, "demo3d_data.c", 172);
    G3DSceneActorSetup *actor = GFL_HeapAllocate(heapId, sizeof(G3DSceneActorSetup), TRUE, "demo3d_data.c", 173);
    int i;

    for (i = 0; i < RESOURCE_MAX; i++) {
        resources[i].arcId = ARCID_DEMO3D_RESOURCE;
        resources[i].type = G3D_SCENE_RES_ARCSYS;
    }
    actor->animations = data_ov203_021c0348;
    setup->resources = resources;
    setup->actors = actor;
    return setup;
}

// pokebw2's Demo3DData_FreeSceneSetup
void func_ov203_021be85c(G3DSceneSetup *setup) {
    GFL_HeapFree((void *)setup->actors);
    GFL_HeapFree((void *)setup->resources);
    GFL_HeapFree(setup);
}

// Fills the scene set-up in for one of a demo's models: the variant that the parameters pick, at their period of the
// day
// pokebw2's Demo3DData_SetSceneSetup
void func_ov203_021be874(G3DSceneSetup *setup, const Demo3DSceneParam *param, u8 unit) {
    G3DSceneResourceSetup *resources = (G3DSceneResourceSetup *)setup->resources;
    G3DSceneActorSetup *actor = (G3DSceneActorSetup *)setup->actors;
    u32 period = 0;
    const Demo3DVariant *variant = func_ov203_021be8f4(&data_ov203_021c185c[param->demoId].units[unit], param);
    int i;
    const Demo3DFiles *files;

    if (variant->count > 1) {
        period = param->dayPeriod;
    }
    if (period >= variant->count) {
        period = 0;
    }
    files = &variant->files[period];
    setup->resourceCount = files->count;
    for (i = 0; i < RESOURCE_MAX; i++) {
        if (i < files->count) {
            resources[i].fileId = files->files[i];
        } else {
            resources[i].fileId = 0;
        }
    }
    setup->actorCount = 1;
    actor->animationCount = files->count - 1;
}

// pokebw2's Demo3DData_GetVariant
static const Demo3DVariant *func_ov203_021be8f4(const Demo3DUnit *unit, const Demo3DSceneParam *param) {
    int index = 0;

    switch (unit->type) {
    case DEMO3D_UNIT_BY_SEX:
        index = param->playerSex;
        break;
    case DEMO3D_UNIT_BY_PARAM:
        index = param->param;
        break;
    case DEMO3D_UNIT_BY_SEASON:
        index = param->season;
        break;
    }
    if (index >= unit->count) {
        index = 0;
    }
    return &unit->variants[index];
}

// pokebw2's Demo3DData_GetFrameStep
fx32 func_ov203_021be920(u32 demoId) {
    const Demo3DSetup *setup = func_ov203_021be7a8(demoId);

    return FX32_ONE >> setup->halfRate;
}

// pokebw2's Demo3DData_IsDual3D
BOOL func_ov203_021be938(u32 demoId) {
    const Demo3DSetup *setup = func_ov203_021be7a8(demoId);

    return setup->dual3D;
}

// pokebw2's Demo3DData_GetFadeParams
void func_ov203_021be948(u32 demoId, u8 fadeOut, u8 *white, u8 *frames) {
    const Demo3DSetup *setup = func_ov203_021be7a8(demoId);

    if (!fadeOut) {
        *white = setup->fadeInWhite;
        *frames = setup->fadeInFrames * (setup->halfRate + 1);
    } else {
        *white = setup->fadeOutWhite;
        *frames = setup->fadeOutFrames * (setup->halfRate + 1);
    }
}

// pokebw2's Demo3DData_GetCmds
const Demo3DCmd *func_ov203_021be990(u32 demoId) {
    return data_ov203_021c185c[func_ov203_021be79c(demoId)].cmds;
}

// pokebw2's Demo3DData_GetEndCmds
const Demo3DCmd *func_ov203_021be9a4(u32 demoId) {
    return data_ov203_021c185c[func_ov203_021be79c(demoId)].endCmds;
}
