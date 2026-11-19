#ifndef POKEHEARTGOLD_OVERLAY_73_UNK_STRUCT_020961_E8_T_H
#define POKEHEARTGOLD_OVERLAY_73_UNK_STRUCT_020961_E8_T_H
#include "sprite_resources_header.h"

// UnkStruct_020961E8: the ov73 record-mixing app's data (OverlayManager_CreateAndGetData in ov73_021E5900, DS size
// 0x4A8C). Platinum's ov59 twin (pokeplatinum include/struct_defs/struct_020961E8_t.h) names the fields; those names
// are Platinum offsets. HGSS: G2dRenderer is 0x64 bytes smaller, the Window[3] + Window *[2] after unk_2FC are gone,
// a YesNoPrompt replaces the Menu, and unk_30C / unk_310 / unk_314 / unk_4A38 (HGSS offsets) are new.
// Only pointer members use forward declarations so that the cut files that still carry local copies of other
// records can include this header.

#include "bg_window.h"
#include "g2d_renderer.h"
#include "menu_input_state.h"
#include "unk_02009D48.h"

struct GF_2DGfxResMan;
struct MessageFormat;
struct MsgData;
struct NNSG2dCharacterData;
struct NNSG2dPaletteData;
struct NNSG2dScreenData;
struct PlayerProfile;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;
struct SysTask;
struct UnkStruct_0203F4F8;
struct YesNoPrompt;

typedef struct UnkStruct_ov59_021D109C {
    int unk_00;
    int unk_04;
    int unk_08;
    void *unk_0C;
    struct NNSG2dPaletteData *unk_10;
} UnkStruct_ov59_021D109C;

typedef struct UnkStruct_ov59_021D30E0 {
    u8 unk_00[3000];
    u32 unk_BB8;
    u32 unk_BBC;
} UnkStruct_ov59_021D30E0;

typedef union UnkStruct_020961E8_sub1 {
    u64 val1;
    struct {
        u32 unk_00;
        u32 unk_04;
    } val2;
} UnkStruct_020961E8_sub1;

struct UnkStruct_020961E8_t {
    BgConfig *unk_00;
    BOOL unk_04;
    struct UnkStruct_0203F4F8 *unk_08; // the app args (sub_0203F4F8)
    UnkStruct_ov59_021D109C unk_0C;
    struct SysTask *unk_20;
    struct MessageFormat *unk_24;
    struct MsgData *unk_28;
    struct String *unk_2C[5];
    struct String *unk_40;
    struct String *unk_44;
    struct String *unk_48;
    int unk_4C;
    struct SpriteList *unk_50;
    G2dRenderer unk_54;
    struct GF_2DGfxResMan *unk_1E0[4];
    struct SpriteResource *unk_1F0[3][4];
    SpriteResourcesHeader unk_220;
    SpriteResourcesHeader unk_244;
    SpriteResourcesHeader unk_268;
    struct Sprite *unk_28C[14];
    struct Sprite *unk_2C4[14];
    Window unk_2FC[5];
    struct YesNoPrompt *unk_384;
    void *unk_388[2];
    struct NNSG2dCharacterData *unk_390[2];
    void *unk_398[2];
    struct NNSG2dPaletteData *unk_3A0[2];
    struct NNSG2dScreenData *unk_30C;
    void *unk_310;
    MenuInputState unk_314;
    int unk_3A8;
    int unk_3AC;
    int unk_3B0;
    int unk_3B4;
    u8 unk_3B8[8][2];
    struct PlayerProfile *unk_3C8[5][2];
    int unk_3F0[5];
    int unk_404;
    u16 unk_408;
    u16 *unk_40C;
    u8 unk_410;
    u8 unk_411;
    int unk_414;
    UnkStruct_ov59_021D30E0 unk_418;
    UnkStruct_ov59_021D30E0 unk_FD8[5];
    int unk_4A98;
    struct SaveData *saveData;
    int unk_4AA0;
    u8 unk_4AA4;
    u8 unk_4A15;
    volatile int unk_4AA8;
    int unk_4AAC;
    int unk_4AB0;
    u32 unk_4AB4;
    u8 unk_4AB8;
    s8 unk_4AB9;
    u8 unk_4ABA;
    u8 unk_4ABB;
    s32 unk_4ABC;
    u16 unk_4AC0;
    s16 unk_4AC2;
    int unk_4AC4;
    int unk_4A38;
    UnkStruct_020961E8_sub1 unk_4AC8[5][2];
};

typedef struct UnkStruct_020961E8_t UnkStruct_020961E8;

// sizeof(UnkStruct_020961E8) == 0x4A8C

#endif // POKEHEARTGOLD_OVERLAY_73_UNK_STRUCT_020961_E8_T_H
