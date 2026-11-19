#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_ED0_A4_T_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_ED0_A4_T_H

#include "global.h"

// The map object graphics manager (pokeplatinum ov5_021ECE40.c's UnkStruct_ov5_021ED0A4), embedded in the
// MapObjectManager at 0x18; its size on the DS is 0x108. The ov01_021FA1C8..ov01_021FA234 accessors are
// typed on it (hk111): one definition for the whole ov01_021F944C family.

struct Billboard;
struct BillboardList;
struct BillboardResources;
struct GF_2DGfxRawResMan;
struct GF_3DGfxRawResMan;
struct LocalMapObject;
struct MapObjectManager;
struct ResourceHeap;
struct SysTask;

typedef struct UnkStruct_ov5_021ED110 {
    u32 unk_00;
    struct BillboardResources *unk_04;
} UnkStruct_ov5_021ED110;

typedef struct UnkStruct_ov5_021EDFBC {
    u32 unk_00;
    int unk_04;
    int unk_08;
    struct GF_3DGfxRawResMan *unk_0C;
} UnkStruct_ov5_021EDFBC;

typedef struct UnkStruct_ov5_021EE134 {
    vu32 unk_00;
    int unk_04;
    struct GF_3DGfxRawResMan *unk_08;
} UnkStruct_ov5_021EE134;

typedef struct UnkStruct_ov5_021EE294 {
    int unk_00;
    struct LocalMapObject *unk_04;
    struct Billboard **unk_08;
} UnkStruct_ov5_021EE294;

struct UnkStruct_ov5_021ED0A4_t;

typedef struct UnkStruct_ov5_021EDF3C {
    s16 unk_00;
    s16 unk_02;
    vs16 unk_04;
    vu16 unk_06;
    UnkStruct_ov5_021EE294 *unk_08;
    UnkStruct_ov5_021EDFBC *unk_0C;
    UnkStruct_ov5_021EE134 *unk_10;
    struct UnkStruct_ov5_021ED0A4_t *unk_14;
    struct SysTask *unk_18;
    struct SysTask *unk_1C;
    struct SysTask *unk_20;
    struct SysTask *unk_24;
} UnkStruct_ov5_021EDF3C;

typedef struct UnkStruct_ov5_021ED0A4_t {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20[8];
    int unk_40[8];
    int unk_60[32];
    struct BillboardList *unk_E0;
    struct BillboardResources *unk_E4;
    struct GF_2DGfxRawResMan *unk_E8;
    struct GF_2DGfxRawResMan *unk_EC;
    struct GF_3DGfxRawResMan *unk_F0;
    UnkStruct_ov5_021ED110 *unk_F4;
    struct ResourceHeap *unk_F8;
    struct ResourceHeap *unk_FC;
    UnkStruct_ov5_021EDF3C *unk_100;
    struct MapObjectManager *unk_104;
} UnkStruct_ov5_021ED0A4;

typedef struct UnkStruct_ov5_021ED0A4_t UnkStruct_ov5_021ED0A4_t;

// sizeof(UnkStruct_ov5_021ED0A4) == 0x108 on the DS

void ov01_021FA8F8(UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA1C8(UnkStruct_ov5_021ED0A4 *gfx, struct BillboardList *list);
struct BillboardList *ov01_021FA1D0(UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA1D8(UnkStruct_ov5_021ED0A4 *gfx, int value);
struct GF_2DGfxRawResMan *ov01_021FA1DC(const UnkStruct_ov5_021ED0A4 *gfx);
struct GF_2DGfxRawResMan *ov01_021FA1E4(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA1EC(UnkStruct_ov5_021ED0A4 *gfx, struct GF_3DGfxRawResMan *resMan);
struct GF_3DGfxRawResMan *ov01_021FA1F4(const UnkStruct_ov5_021ED0A4 *gfx);
int *ov01_021FA1FC(UnkStruct_ov5_021ED0A4 *gfx);
int *ov01_021FA200(UnkStruct_ov5_021ED0A4 *gfx);
int *ov01_021FA204(UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA208(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA20C(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA210(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA214(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA218(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA21C(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA220(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA224(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA228(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA22C(const UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021FA230(UnkStruct_ov5_021ED0A4 *gfx, int value);
int ov01_021FA234(const UnkStruct_ov5_021ED0A4 *gfx);

// the three id lists (0x20 / 0x40 / 0x60) with their offset and count, and the helpers over them
void ov01_021F9AAC(int *list, int value, int count);
int ov01_021F9AB4(int *list, int id, int emptyValue, int count);
int ov01_021F9AD0(int *list, int id, int count);
int ov01_021F9AE4(int *list, int id, int newValue, int count);
void ov01_021F9808(UnkStruct_ov5_021ED0A4 *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 num, s32 arg6);
void ov01_021F9890(UnkStruct_ov5_021ED0A4 *gfx);
s32 ov01_021F98CC(UnkStruct_ov5_021ED0A4 *gfx, struct ResourceHeap *heap, s32 id, s32 emptyValue, u16 *table);
s32 ov01_021F9968(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
s32 ov01_021F9974(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
void ov01_021F9980(UnkStruct_ov5_021ED0A4 *gfx, const int *list);
void ov01_021F99A4(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
void ov01_021F99D0(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
void ov01_021F99FC(UnkStruct_ov5_021ED0A4 *gfx, const int *list);
void ov01_021F9A18(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
void ov01_021F9A44(UnkStruct_ov5_021ED0A4 *gfx, s32 id);
void ov01_021F9A70(UnkStruct_ov5_021ED0A4 *gfx, const int *list);
void ov01_021F9A8C(UnkStruct_ov5_021ED0A4 *gfx, const int *list0, const int *list1, const int *list2);
void ov01_021F9B00(UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021F9B10(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9B38(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9B54(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9B84(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9BAC(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9BD4(struct MapObjectManager *mgr, UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021F9C24(UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021F9C34(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9C5C(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9C78(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9CA8(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9CD0(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9CF8(struct MapObjectManager *mgr, UnkStruct_ov5_021ED0A4 *gfx);
void ov01_021F9D48(UnkStruct_ov5_021ED0A4 *gfx);
int ov01_021F9D88(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9DA4(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9DD0(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9E04(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9E30(struct MapObjectManager *mgr, UnkStruct_ov5_021ED0A4 *gfx);
int ov01_021F9E9C(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9EC4(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9EEC(UnkStruct_ov5_021ED0A4 *gfx, int id);
int ov01_021F9F14(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9F3C(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9F54(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9F6C(UnkStruct_ov5_021ED0A4 *gfx, int id);
void ov01_021F9F84(UnkStruct_ov5_021ED0A4 *gfx, int id);

// The key of a sprite id in unk_F8 / unk_FC (ov01_021FA28C: the model, ov01_021FA2A0: the animation). u8, as ov01_021FA248 stores it:
// a caller declaring s32 reads r0 on the DS, but on the host the bits above AL are undefined.
u8 ov01_021FA28C(s32 id);
u8 ov01_021FA2A0(s32 id);
void ov01_021F9FB0(struct MapObjectManager *mgr, UnkStruct_ov5_021ED0A4 *gfx);

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021_ED0_A4_T_H
