#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E854_T_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E854_T_H

#include "g2d_renderer.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"

// UnkStruct_ov114_0225E854_t: the ov90 Wi-Fi Plaza minigame app (0x664 on the DS). Field names keep the Platinum (ov114)
// offsets; every sub-object has its own header (overlay_90_unk_struct_ov114_*.h).

struct BgConfig;
struct BufferManager;
struct MsgData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteData;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct NNSG2dScreenData;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
struct String;
struct StringTemplate;
struct SysTask;
struct TrainerInfo;
struct YesNoTouchMenu;

typedef struct UnkStruct_ov114_0225E854_t {
    u16 unk_00;
    u16 heapID;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A[2];
    UnkStruct_ov114_0225C76C unk_0C;
    UnkStruct_ov114_0225C9A8 unk_1C;
    UnkStruct_ov114_0225E0F8 unk_34;
    UnkStruct_ov114_0225CEF0 unk_4C;
    UnkStruct_ov114_0225D084 unk_58;
    UnkStruct_ov114_0225D338 unk_68;
    UnkStruct_ov114_0225CFCC unk_A0;
    UnkStruct_ov114_0225CCD0 unk_230;
    UnkStruct_ov114_0225CDB4 *unk_248;
    UnkStruct_ov114_0225F270 unk_24C;
    UnkStruct_ov114_0225F578 unk_26C;
    void *unk_2B8[1];
    struct NNSG2dScreenData *unk_2BC[1];
    UnkStruct_ov114_0225DAC0 unk_2C0[4];
    UnkStruct_ov114_0225DFFC unk_3A0;
    struct SysTask *unk_6A8;
    struct SysTask *unk_6AC;
    u32 unk_6B0;
    u16 unk_6B4;
    u16 unk_6B6;
    void *unk_6B8;
    struct NNSG2dPaletteData *unk_6BC;
    u16 unk_6C0;
    u16 unk_6C2;
    BOOL unk_6C4;
} UnkStruct_ov114_0225E854_t;

// sizeof(UnkStruct_ov114_0225E854_t) == 0x664

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225_E854_T_H
