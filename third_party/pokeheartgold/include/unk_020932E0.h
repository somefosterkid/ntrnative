#ifndef POKEHEARTGOLD_UNK_020932E0_H
#define POKEHEARTGOLD_UNK_020932E0_H

#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "sprite.h"
#include "touchscreen.h"
#include "g2d_renderer.h"
#include "sprite_resources_header.h"
#include "yes_no_prompt.h"
#include "party.h"
#include "pm_string.h"
#include "filesystem.h"
#include "font.h"
#include "unk_020957B0.h"

struct PCStorage;
struct GF_2DGfxResMan;
struct SpriteResource;

typedef struct UnkStruct_020932E0_Info {
    u32 unk_0;
    u8 unk_4[4];
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
    u8 unk_10[0x20];
} UnkStruct_020932E0_Info; // 0x30

typedef struct UnkStruct_020932E0_Slot {
    Sprite *unk_0;
    UnkStruct_020932E0_Info unk_4;
} UnkStruct_020932E0_Slot; // 0x34

typedef struct UnkStruct_020932E0_Slot10 {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
} UnkStruct_020932E0_Slot10;

typedef struct UnkStruct_020932E0_Slot20C {
    u32 unk_0;
    s32 unk_4;
    Sprite *unk_8;
    u8 unk_C[0x200];
} UnkStruct_020932E0_Slot20C;

typedef struct UnkStruct_020932E0_SlotC {
    Sprite *unk_0;
    s32 unk_4;
    s32 unk_8;
} UnkStruct_020932E0_SlotC;

typedef struct UnkStruct_020932E0_Rect8 {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    s32 unk_4;
} UnkStruct_020932E0_Rect8;

typedef struct UnkStruct_020932E0_Pair {
    s32 unk_0;
    s32 unk_4;
} UnkStruct_020932E0_Pair;

typedef void (*UnkStruct_020932E0_SlotsFunc)(UnkStruct_020932E0_Slot20C *slots);
typedef s32 (*UnkStruct_020932E0_GetInfoFunc)(struct PCStorage *pcStorage, u32 boxno, u32 slotno, UnkStruct_020932E0_Info *info);
typedef void (*UnkStruct_020932E0_BoxNameFunc)(String *dest, struct PCStorage *pcStorage, u32 boxno);
typedef void (*UnkStruct_020932E0_SelectFunc)(s32 box, s32 slot, void *arg);

typedef struct UnkStruct_sub_020932E0_Heap_Alloc {
    BgConfig *unk_0;
    enum HeapID unk_4;
    YesNoPrompt *unk_8;
    u8 unk_C;
    u8 unk_D;
    u8 unk_E;
    u8 unk_F;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
    s32 unk_14;
    SpriteList *unk_18;
    G2dRenderer unk_1C;
    struct GF_2DGfxResMan *unk_144[6];
    struct SpriteResource *unk_15C;
    struct SpriteResource *unk_160;
    struct SpriteResource *unk_164;
    struct SpriteResource *unk_168;
    u8 unk_16C[8];
    SpriteResourcesHeader unk_174;
    UnkStruct_020932E0_Slot unk_198[30];
    UnkStruct_020932E0_Slot unk_7B0;
    TouchscreenHitbox *unk_7E4;
    UnkStruct_020932E0_Slot unk_7E8[3];
    UnkStruct_020932E0_Slot10 unk_884[3];
    Sprite *unk_8B4;
    Sprite *unk_8B8;
    Sprite *unk_8BC;
    Sprite *unk_8C0;
    Sprite *unk_8C4;
    Sprite *unk_8C8[2];
    UnkStruct_020932E0_SlotC *unk_8D0;
    UnkStruct_020932E0_Slot20C unk_8D4[30];
    struct PCStorage *unk_463C;
    Party *unk_4640;
    UnkStruct_020932E0_SlotsFunc unk_4644;
    UnkStruct_020932E0_GetInfoFunc unk_4648;
    UnkStruct_020932E0_BoxNameFunc unk_464C;
    Window unk_4650;
    Window unk_4660;
    Window unk_4670;
    s32 unk_4680;
    s32 unk_4684;
    VecFx32 unk_4688;
    s32 unk_4694;
    s32 unk_4698;
    s32 unk_469C;
    UnkStruct_020932E0_Slot *unk_46A0;
    UnkStruct_020932E0_Rect8 *unk_46A4;
    u8 unk_46A8;
    u8 unk_46A9;
    u8 unk_46AA;
    u8 unk_46AB;
    u32 unk_46AC;
    s32 unk_46B0;
    UnkStruct_020932E0_Pair *unk_46B4;
    CursorNav *unk_46B8;
    s32 unk_46BC;
    s32 unk_46C0;
    s32 unk_46C4;
    void *unk_46C8;
    void *unk_46CC;
    struct NNSG2dScreenData *unk_46D0;
    struct NNSG2dScreenData *unk_46D4;
    UnkStruct_020932E0_SelectFunc unk_46D8;
    void *unk_46DC;
    s32 unk_46E0;
    s32 unk_46E4;
} UnkStruct_sub_020932E0_Heap_Alloc; // 0x46E8

typedef UnkStruct_sub_020932E0_Heap_Alloc UnkStruct_020932E0;

UnkStruct_020932E0 *sub_020932E0(enum HeapID heapID, u8 arg1, s8 arg2);
void sub_02093354(UnkStruct_020932E0 *arg0);
void sub_02093440(UnkStruct_020932E0 *arg0, BgConfig *bgConfig, struct PCStorage *pcStorage, Party *party, s32 arg4, s32 arg5, s32 arg6, s32 arg7, UnkStruct_020932E0_GetInfoFunc arg8, UnkStruct_020932E0_BoxNameFunc arg9, UnkStruct_020932E0_Pair *argA, UnkStruct_020932E0_SelectFunc argB, void *argC);
void sub_02093594(UnkStruct_020932E0 *arg0);
s32 sub_020935E0(UnkStruct_020932E0 *arg0);
s32 sub_02093630(UnkStruct_020932E0 *arg0);
void sub_0209389C(UnkStruct_020932E0 *arg0);
void sub_02093908(UnkStruct_020932E0 *arg0);
void sub_0209396C(UnkStruct_020932E0 *arg0);
void sub_020939B8(UnkStruct_020932E0 *arg0);
void sub_02093A40(UnkStruct_020932E0 *arg0);
void sub_02093A50(UnkStruct_020932E0 *arg0);
void sub_02093B40(UnkStruct_020932E0 *arg0);
void sub_02093B84(UnkStruct_020932E0 *arg0);
s32 sub_02093CE4(UnkStruct_020932E0 *arg0);
void sub_02093F84(UnkStruct_020932E0 *arg0, u8 arg1, u8 arg2, u32 arg3);
void sub_02094004(UnkStruct_020932E0 *arg0);
void sub_02094150(SpriteTemplate *tmpl, UnkStruct_020932E0 *arg1, NNS_G2D_VRAM_TYPE whichScreen);
Sprite *sub_0209417C(UnkStruct_020932E0 *arg0, s32 x, s32 y, s32 animSeq, BOOL drawFlag);
void sub_020941CC(UnkStruct_020932E0 *arg0, BgConfig *bgConfig, enum HeapID heapID);
void sub_0209428C(TouchscreenHitbox *rect, int param1, int param2, int param3, int param4);
void sub_020942B0(UnkStruct_020932E0 *arg0);
void sub_020943EC(UnkStruct_020932E0 *arg0);
void sub_02094400(UnkStruct_020932E0 *arg0);
void sub_02094528(UnkStruct_020932E0 *arg0);
void sub_02094668(UnkStruct_020932E0 *arg0, u8 arg1, s32 arg2, s32 arg3);
void sub_02094758(UnkStruct_020932E0_Slot20C *slots);
s32 sub_02094794(UnkStruct_020932E0 *arg0, s32 arg1);
s32 sub_020947C0(UnkStruct_020932E0 *arg0, s32 arg1);
s32 sub_02094860(UnkStruct_020932E0 *arg0, s32 arg1);
s32 sub_02094894(UnkStruct_020932E0 *arg0, s32 arg1);
void sub_020948C4(UnkStruct_020932E0 *arg0, s32 arg1, s32 arg2);
void sub_020949F4(UnkStruct_020932E0 *arg0);
#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02094A70(UnkStruct_020932E0 *arg0);
#else
s32 sub_02094A70(UnkStruct_020932E0 *arg0);
#endif
void sub_02094A90(UnkStruct_020932E0 *arg0, s32 mode);
void sub_02094C08(UnkStruct_020932E0_Info *info, UnkStruct_020932E0_Slot20C *slots, s32 idx, Sprite *sprite, void *buf, NARC *narc, s32 arg6);
void sub_02094C6C(u32 memberIndex, NNSG2dCharacterData **charData, void *buf, NARC *narc);
void sub_02094C88(UnkStruct_020932E0 *arg0, s32 arg1);
void sub_02094D1C(UnkStruct_020932E0 *arg0);
void sub_02094D9C(BgConfig *bgConfig, enum HeapID heapID);
void sub_02094DF8(BgConfig *bgConfig);
void sub_02094EB4(UnkStruct_020932E0 *arg0);
void sub_02094F14(UnkStruct_020932E0 *arg0, s32 idx, UnkStruct_020932E0_Info *info);
void sub_02094F5C(UnkStruct_020932E0 *arg0, s32 arg1);
void sub_0209501C(UnkStruct_020932E0 *arg0, s32 strno, enum Font fontId);
void sub_020950C0(UnkStruct_020932E0 *arg0);
void sub_020950D4(UnkStruct_020932E0 *arg0);
void sub_020950F8(Window *window, s32 arg1);
void sub_0209511C(UnkStruct_020932E0 *arg0);
void sub_0209515C(UnkStruct_020932E0 *arg0);
void sub_02095238(UnkStruct_020932E0 *arg0);
u32 sub_02095354(UnkStruct_020932E0 *arg0);
u32 sub_020954CC(UnkStruct_020932E0 *arg0);
void sub_02095540(UnkStruct_020932E0 *arg0);
void sub_020955EC(UnkStruct_020932E0 *arg0);
void sub_0209569C(UnkStruct_020932E0 *arg0);
void sub_020956B8(UnkStruct_020932E0 *arg0);
void sub_02095780(void);
void sub_02095794(void);

#endif // POKEHEARTGOLD_UNK_020932E0_H
