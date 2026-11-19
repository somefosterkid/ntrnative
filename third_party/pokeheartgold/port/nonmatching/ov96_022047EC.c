#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
s32 ov96_021E6104(void);

s32 ov96_021E6108(void *arg0);

s32 ov96_021E6138(void *arg0);

void ov96_021EB3A4(void *arg0);

void ov96_02204F40(BgConfig *bgConfig);

void ov96_02205048(void *arg0);

void ov96_02207740(void *arg0);


void *ov96_021E60C0(PokeathlonCourseData *, u32, u32);

void ov96_021E6168(PokeathlonCourseData *, u32, u32, u16 *);

u32 ov96_021E6290(PokeathlonCourseData *, u32, u32, u32);

void ov96_021E634C(PokeathlonCourseData *, u32, u32, u32, u32, u32, u16 *);

void ov96_021E6670(PokeathlonCourseData *, u32);

void ov96_021E92B0(u32 *, u32, u32, u32, u32);

u32 ov96_021E9A78(u32, u32, u32);

u32 ov96_021EA854(u32, u32, u32, u32, u32);

void ov96_021EA8A8(u32, u32, u16 *, u32 *, u32, u32);

s32 ov96_021EAA00(u32);

u32 ov96_021EAA04(u32, u32);

void ov96_021EAB38(u32, u32);

void ov96_021EABDC(u32, u32);

void ov96_021EAC0C(u32, u32);

void ov96_021EAF6C(u32, s32);

void ov96_021EAF70(u32, s32, s32);

void ov96_021EAF94(u32, s32, s32);

void ov96_021EB0A4(u32, s32, u32, s32 *, u32 *);

u32 ov96_021EB180(u32, void *);

void ov96_021EB29C(u32, u32, u32);

void ov96_021EB5C8(u32, u32, u32, u32, u32);

u32 ov96_021EB5E8(u32);

void ov96_02204F20(void);

void ov96_022050B4(u32);

void ov96_022050F8(u32 *, u32);

void ov96_02205AFC(u32 *, u8 *);

void ov96_02206E88(PokeathlonCourseData *, u8 *, u32, u32, u32 *);

void ov96_022077F4(u32 *);

u32 ov96_02207CCC(u32, u32, PokeathlonCourseData *);

void ov96_02207E7C(u32, u32);

void ov96_02207F18(u32, u32, u32);

void ov96_022080F4(u32, u16 *);

void ov96_02208250(u32, u16 *);

void ov96_022082BC(u32, u32, u32);

void ov96_02208740(u32, u16);

void ov96_02208784(u32, BOOL);

u32 ov96_02208AA8(u32, u32, u32);

void ov96_02208AE8(u32, u32, u32 *);

typedef struct { u32 w[3]; } UnkOv96Triple; /* invented */
extern UnkOv96Triple ov96_0221CAD4;

u32 ov96_022047EC(PokeathlonCourseData *course) {
    u32 *data;
    u32 *ent;
    u32 obj;
    s32 i, j;
    s32 team, rem, myTeam;
    s32 x, y, xx;
    u8 cnt;
    u16 entries[12][8];
    u32 arr[17];
    u8 narcBuf[100];
    u32 oamArgs[4];
    UnkOv96Triple triple;
    u16 pairs[12][2];
    s32 pos[3][2];
    u16 sp50[2];
    s32 sp4c;
    u32 sp48;

    data = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_139, 0x68000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~0x1F00;
        reg_GXS_DB_DISPCNT &= ~0x1F00;
        ov96_02204F20();
        data = PokeathlonCourse_AllocPtr4FromHeap(course, 0x6c0);
        MI_CpuFill8(data, 0, 0x6c0);
        data[0] = (u32)BgConfig_Alloc(HEAP_ID_139);
        PokeathlonCourse_SetField3A4(course, (u32)(data + 0xdd), (u32)(data + 0xde), 4);
        ov96_021E6670(course, 8);
        oamArgs[0] = 0xca;
        oamArgs[1] = 0x40000;
        oamArgs[2] = 0x4000;
        oamArgs[3] = 0x8b;
        ov96_021E92B0(oamArgs, 0x16, 0x8b, 0x300010, 0x10);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 0x7e, 0, 0x20, 0, 0x7e, 0, 0x20, HEAP_ID_139);
        data[5] = 0x8b;
        FontID_Alloc(FONT_UNK_4, HEAP_ID_139);
        ov96_02204F40((BgConfig *)data[0]);
        ov96_02207740(data);
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        data[0xdc] = ov96_02207CCC(data[5], data[0], course);
        cnt = PokeathlonCourse_GetParticipantCount(course);
        data[0xdb] = ov96_02208AA8(data[5], 4 - cnt, PokeathlonCourse_GetMode(course));
        data[0xd1] = ov96_021E9A78(data[5], 0x2bf, 1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        triple = ov96_0221CAD4;
        data[8] = ov96_021EB180(data[5], (u32 *)&triple);
        ov96_021EB5C8(data[8], 0, 0, 0, 0x2bc000);
        data[0xd2] = ov96_021EA854(data[5], 0xc, 2, data[0xd1], ov96_021EB5E8(data[8]));
        ov96_021EB29C(data[8], 0, 0x65);
        ov96_021EB29C(data[8], 1, 0x66);
        ov96_021EB29C(data[8], 2, 0x67);
        ov96_021EB29C(data[8], 3, 0x68);
        ov96_02207E7C(data[0xdc], data[8]);
        ov96_022050B4(data[8]);
        ov96_021EB3A4((void *)data[8]);
        Sprite_SetDrawPriority(*(Sprite **)ov96_021E6290(course, 0, data[0xd1], data[8]), 1);
        ov96_022050F8(data, data[8]);
        ov96_02207F18(data[0xdc], data[8], data[0xd1]);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        for (i = 0; i < 12; i++) {
            ov96_021E6168(course, i / 3, i % 3, entries[i]);
            arr[5 + i] = ov96_021E6108(ov96_021E60C0(course, i / 3, i % 3));
            pairs[i][0] = entries[i][0];
            pairs[i][1] = entries[i][1];
        }
        ov96_022080F4(data[0xdc], pairs[0]);
        cnt = ov96_021E5F24(course);
        ov96_02208250(data[0xdc], entries[cnt * 3]);
        ov96_022082BC(data[0xdc], 0, 1);
        ov96_022082BC(data[0xdc], 1, 2);
        arr[1] = 2;
        arr[0] = 0;
        arr[2] = 0;
        arr[3] = 1;
        arr[4] = 1;
        ov96_021EA8A8(data[0xd2], 0xc, entries[0], arr, 0, 0);
        ov96_02208784(data[0xdc], ov96_021E5F24(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        if (ov96_021EAA00(data[0xd2]) == 0) {
            break;
        }
        myTeam = ov96_021E5F24(course);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)data[0]);
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(pos, NARC_a_1_7_0, 0xe);
        for (i = 0; ; i++) {
            if (!(i < 12)) {
                break;
            }
            obj = ov96_021EAA04(data[0xd2], (u8)i);
            rem = i % 3;
            if (rem == 0) {
                ov96_021EAB38(obj, 1);
            }
            j = ov96_021E6138(ov96_021E60C0(course, i / 3, i % 3));
            ov96_021EAF70(obj, pos[j - 1][0], pos[j - 1][1]);
            obj = ov96_021EAA04(data[0xd2], (u8)i);
            team = i / 3;
            ent = (u32 *)((u8 *)data + 0x24 + team * 0xb8);
            ent[rem] = obj;
            *(u8 *)(ent + 0x2c) = 1;
            ov96_021EAC0C(obj, 1);
            x = team * 32;
            xx = x + 0x80;
            ov96_021EAF94(obj, xx, 0x60);
            ov96_021EAF6C(obj, ov96_021E6104());
            x += 0xd0;
            ent[0x16] = x << 12;
            ent[0x17] = 0x1a0000;
            ov96_021EABDC(obj, 3000);
            if (team == myTeam && rem == 0) {
                ov96_021EB0A4(obj, xx, 0x60, &sp4c, &sp48);
                sp50[0] = 0x80;
                sp50[1] = sp48;
            }
        }
        ReadWholeNarcMemberByIdPair(narcBuf, NARC_a_1_7_0, 4);
        *(u16 *)((u8 *)data + 0x50c) = 0x708;
        ov96_02208740(data[0xdc], *(u16 *)((u8 *)data + 0x50c));
        ov96_022077F4(data);
        for (team = 0; team < 4; team++) {
            data[0xcd + team] = 0;
            for (j = 0; j < 3; j++) {
                ov96_02206E88(course, narcBuf, (u8)team, (u8)j, (u32 *)((u8 *)data + 0x24 + team * 0xb8));
            }
            if (ov96_021E5F24(course) == 0) {
                ov96_02208AE8(data[0xdb], (u8)team, (u32 *)((u8 *)data + 0x24 + team * 0xb8));
            }
            *(u32 *)((u8 *)data + team * 0xb8 + 0xbc) = team;
        }
        if (ov96_021E5F24(course) == 0) {
            ov96_02205AFC(data, ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28));
        }
        ov96_021E634C(course, 0, data[0xd1], data[8], 1, 1, sp50);
        ov96_02205048(data);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        ent = (u32 *)((u8 *)data + 0x24 + myTeam * 0xb8);
        y = (s32)ent[0x17] / 4096;
        x = (s32)ent[0x16] / 4096;
        ScheduleSetBgPosText((BgConfig *)data[0], 0, BG_POS_OP_SET_X, x - 0x80);
        ScheduleSetBgPosText((BgConfig *)data[0], 0, BG_POS_OP_SET_Y, y - 0x60);
        sub_0203A994(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        if (ov96_021E5F24(course) == 0) {
            data[0xdd] = LCRandom() % 20;
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        return 1;
    }
    return 0;
}
