#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "pokepic.h"

struct BallCapsule;

struct BallSeal;

struct BoxPokemon;

struct MailMessage;

struct Mail;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Pokepic;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct SpriteAnimFrame;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void UnscanPokepic(u8 *rawCharData, enum NarcId narcID);

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;


u32 GetMonData(Pokemon *mon, int param, void *dest);

void sub_02070130(PokepicTemplate *spriteTemplate, Pokemon *mon, u8 face);

u8 sub_02070848(Pokemon *mon, u8 face);

void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);

void *GfGfxLoader_LoadFromNarc(enum NarcId narcID, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd);

void Heap_Free(void *ptr);

int ov41_0224989C(char *param0, int param1);

void ov41_02249978(TouchScreenRect *rect, int param1, int param2, int param3, int param4);

void ov41_022498E8(char *param0, int param1, int param2, UnkStruct_ov22_022596B0 *param3);

void ov41_02249604(UnkStruct_020298D8 *param0, PokepicManager *param1, Pokemon *param2, PokepicTemplate *param3, int param4, BOOL param5);

void ov41_02249604(UnkStruct_020298D8 *param0, PokepicManager *param1, Pokemon *param2, PokepicTemplate *param3, int param4, BOOL param5) {
    int v0 = GetMonData(param2, MON_DATA_SPECIES, 0L);

    sub_02070130(param3, param2, 2);
    param0->unk_00 = PokepicManager_CreatePokepic(param1, param3, 192, 56, 0, 0, 0L, 0L);

    {
        int v1, v2;

        ov41_022497A0(param0, &v1, &v2);

        v1 /= 2;
        v2 /= 2;

        param0->unk_04.rect.top = 56 - v2;
        param0->unk_04.rect.bottom = 56 + v2;
        param0->unk_04.rect.left = 192 - v1;
        param0->unk_04.rect.right = 192 + v1;

        ov41_02249978(&param0->unk_04, 192, 56, v1, v2);
    }

    {
        u8 v3 = sub_02070848(param2, 2);
        void *v4;
        NNSG2dCharacterData *v5;

        v4 = GfGfxLoader_LoadFromNarc((enum NarcId)(param3->narcID), param3->charDataID, 0, (enum HeapID)(param4), 0);

        NNS_G2dGetUnpackedCharacterData(v4, &v5);
        UnscanPokepic((u8 *)v5->pRawData, (enum NarcId)(param3->narcID));

        if (param5 == 0) {
            ov41_022498E8(v5->pRawData, v5->W * 8, v5->H * 8, &param0->unk_08);
        } else {
            param0->unk_08.unk_00 = ov41_0224989C(v5->pRawData, v5->W * 8);
            param0->unk_08.unk_01 = param0->unk_08.unk_00;
            param0->unk_08.unk_03 = v3;
            param0->unk_08.unk_02 = v3;
        }

        Heap_Free(v4);
    }

    param0->unk_0C = param2;
}
