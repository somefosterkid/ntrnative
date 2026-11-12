#ifndef FIELD_OV021_021BD068_H
#define FIELD_OV021_021BD068_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// One letter of the place name sign: where it is and how it moves to where it stops (Black's layout)
typedef struct Ov021Motion {
    f32 pos[2];
    f32 vel[2];
    f32 acc[2];
    f32 target[2];
    f32 endVel[2];
    s32 frames[2];
} Ov021Motion;

typedef struct Ov021Work Ov021Work;
struct Ov021Work {
    HeapID heapId;
    Font *font;
    GFLBitmap *bitmap;
    BmpOamActor *actor;
    Ov021Motion motion;
    u8 width;
    u8 height;
    u32 active;
    u16 letter;
};

// What the sign takes from the caller to place a letter: the positions and speeds, as eight floats
typedef struct PlaceNameWidths {
    f32 unk_0[8];
    Font *unk_20;
    u16 unk_24;
} PlaceNameWidths;

struct PlaceName {
    GameSystem *unk_0;
    GameData *unk_4;
    WordSet *unk_8;
    HeapID unk_c;
    u8 unk_e[0x2];
    ArcTool *unk_10;
    MsgData *unk_14;
    Font *unk_18;
    StrBuf *unk_1c;
    u8 unk_20;
    u8 unk_21[0x3];
    BmpWin *unk_24;
    u32 unk_28;
    u32 unk_2c;
    BmpOamSys *unk_30;
    ClActUnit *unk_34[1];
    ClActRenderer *unk_38;
    u32 unk_3c;
    u8 unk_40;
    u8 unk_41;
    u16 unk_42;
    u16 unk_44;
    u16 unk_46;
    u32 unk_48;
    u32 unk_4c;
    Ov021Work *unk_50[0x1e];
    Ov021Work *unk_c8[0x20];
    u8 unk_148;
    u8 unk_149;
    u8 unk_14a;
    u8 unk_14b;
    void *unk_14c;
    NNSG2dPaletteData *unk_150;
};

Ov021Work *func_ov021_021bd148(HeapID heapID, BmpOamSys *x, u16 y, u32 z);
void SetPlaceNameDispZone(PlaceName *placeName, u16 zoneId);
u32 func_ov021_021bd140(u16 zoneId);

void func_ov021_021bd1d4(Ov021Work *work);
void func_ov021_021bd1e4(Ov021Work *work, BOOL flag);

void func_ov021_021bd1cc(Ov021Work *work);
GFLBitmap *func_ov021_021bd1dc(Ov021Work *work);
u32 func_ov021_021bd1ec(Ov021Work *work);
u32 func_ov021_021bd1f4(Ov021Work *work);
u32 func_ov021_021bd1fc(Ov021Work *work);
u32 func_ov021_021bd204(Ov021Work *work);

void func_ov021_021bd068(PlaceName *placeName);
void func_ov021_021bd07c(PlaceName *placeName);
HeapID func_ov021_021bd090(PlaceName *placeName);
ArcTool *func_ov021_021bd094(PlaceName *placeName);
u32 func_ov021_021bd0a4(PlaceName *placeName);
u8 func_ov021_021bd0c0(PlaceName *placeName);
void func_ov021_021bd0fc(PlaceName *placeName);
u8 func_ov021_021bd138(PlaceName *placeName);

void func_ov021_021bd0a0(PlaceName *placeName, u32 param_2);
void func_ov021_021bd0b0(PlaceName *placeName);
void func_ov021_021bd0d0(PlaceName *placeName);
void func_ov021_021bd184(Ov021Work *work);
void func_ov021_021bd0a8(PlaceName *placeName);
void func_ov021_021bd0c8(PlaceName *placeName);

u32 func_ov021_021bd20c(Ov021Work *work);

u16 func_ov021_021bd118(PlaceName *placeName);

StrBuf *func_ov021_021bd134(PlaceName *placeName);
void func_ov021_021bd19c(Ov021Work *work, PlaceNameWidths *args);

void func_ov021_021bd098(PlaceName *placeName, HeapID heapId);
void func_ov021_021bd09c(PlaceName *placeName, Font *font);
void func_ov021_021bd0e0(PlaceName *placeName);

#endif
