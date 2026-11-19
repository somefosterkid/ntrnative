#ifndef POKEHEARTGOLD_UNK_02095EAC_H
#define POKEHEARTGOLD_UNK_02095EAC_H

// The Overlay 37 app object (OverlayManager_CreateAndGetData(HEAP_ID_DRAWING, sizeof(UnkStruct_02095EAC)), DS size 0x940c).
// Field names keep the numbers of the old per-file copies (they are not the DS offsets: unk_43D8 is at 0x4374).

#include "global.h"

#include "bg_window.h"
#include "g2d_renderer.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"

struct JournalEntry;
struct UnkStruct_0205C22C_t;

typedef struct UnkStruct_0203DDFC {
    struct UnkStruct_0205C22C_t *unk_00;
    Options *options;
    struct JournalEntry *unk_08;
} UnkStruct_0203DDFC;

typedef struct UnkStruct_02095EAC_sub1 {
    u8 unk_00[1000];
    u32 unk_3E8;
    u8 unk_3EC;
    u8 unk_3ED[3];
} UnkStruct_02095EAC_sub1;

typedef struct UnkStruct_ov58_021D2754 {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov58_021D2754;

typedef struct UnkStruct_ov58_021D2820 {
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08_0 : 3;
    u8 unk_08_3 : 3;
    u8 unk_08_6 : 2;
    u8 unk_09;
} UnkStruct_ov58_021D2820;

typedef struct UnkStruct_02095EAC {
    BgConfig *unk_00;
    BOOL unk_04;
    UnkStruct_0203DDFC *unk_08;
    MessageFormat *unk_0C;
    MsgData *unk_10;
    String *unk_14[5];
    String *unk_28;
    String *unk_2C;
    int unk_30;
    SpriteList *unk_34;
    G2dRenderer unk_38;
    GF_2DGfxResMan *unk_1C4[4];
    SpriteResource *unk_1D4[2][4];
    SpriteResourcesHeader unk_1F4;
    SpriteResourcesHeader unk_218;
    Sprite *unk_23C[14];
    Sprite *unk_274[14];
    Sprite *unk_2AC[12];
    Window unk_2DC[5];
    Window unk_32C;
    Window unk_33C;
    Window unk_34C;
    Window *unk_35C[2];
    int unk_364;
    int unk_368;
    int unk_36C;
    int unk_370;
    int unk_374;
    int unk_378;
    int unk_37C;
    int unk_380;
    int unk_384;
    u8 unk_388[8][2];
    PlayerProfile *unk_398[8][2];
    u8 unk_3D8[16384];
    u16 unk_43D8;
    u8 unk_43DA;
    u8 unk_43DB;
    UnkStruct_ov58_021D2820 unk_43DC;
    UnkStruct_ov58_021D2820 unk_43E6[5];
    UnkStruct_ov58_021D2754 unk_4418[5];
    u8 *unk_442C;
    int unk_4430;
    u8 unk_4434[14400];
    UnkStruct_02095EAC_sub1 unk_7C74;
    UnkStruct_02095EAC_sub1 unk_8064[5];
    s32 unk_9414;
    u32 unk_9418;
    u16 unk_941C;
    s16 unk_941E;
    u8 unk_9420;
    UnkStruct_ov58_021D2820 unk_9421[5];
    YesNoPrompt *unk_9454;
    int unk_9458;
    int unk_945C;
    int unk_9460;
    int unk_9464;
    int unk_9468;
    int unk_946C;
} UnkStruct_02095EAC;

#endif // POKEHEARTGOLD_UNK_02095EAC_H
