#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "sys_task.h"
#include "battle/battler_sprite_context.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleMonOBJData;

struct BattleSystem;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct Pokepic;

struct PokemonSpriteData;

struct PokepicDrawParam;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteSystemContext;

struct ManagedSpriteTemplate;

struct SysTask;

struct SysTaskQueue;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

BOOL Pokepic_IsActive(Pokepic *monSprite);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

NNSG2dImageProxy *Sprite_GetImageProxy(Sprite *sprite);

NNSG2dImagePaletteProxy *Sprite_GetPaletteProxy(Sprite *paletteProxy);

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    Pokepic *pokemonSprites[4];
} BattleMonOBJData;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

void Heap_Free(void *ptr);

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

SpriteManager *BattleSystem_GetSpriteManager(BattleSystem *battleSys);

PaletteData *BattleSystem_GetPaletteData(BattleSystem *battleSys);

PokemonSpriteData *ov12_0223BB88(BattleSystem *battleSys, int battler);

void ov12_0223C1C4(BattleSystem *battleSys, u8 *battlerTypes);

void ov12_0223C1F4(BattleSystem *battleSys, Pokepic **monSprites);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

u32 ObjPlttTransfer_GetPaletteVramOffset(const NNSG2dImagePaletteProxy *paletteProxy, NNS_G2D_VRAM_TYPE vramType);

BOOL GF_CreateNewVramTransferTask(NNS_GFD_DST_TYPE type, u32 destAddr, void *buf, u32 size);

BattleMonOBJData *ov07_0221FDFC(BattleSystem *battleSys, enum HeapID heapID);

BattleMonOBJData *ov07_0221FB90(BattleSystem *battleSys, enum HeapID heapID, int battlerIdx);

typedef struct {
    int a[6];
} ov07_02234BD8_Array;

extern const ov07_02234BD8_Array ov07_02234BD8;

BattleMonOBJData *ov07_0221FB90(BattleSystem *battleSys, enum HeapID heapID, int battlerIdx) {
    int i;
    ov07_02234BD8_Array sPriorityByBattlerType = ov07_02234BD8;
    BattleMonOBJData *btlMonObjData = Heap_Alloc(heapID, sizeof(BattleMonOBJData));

    btlMonObjData->heapID = heapID;
    btlMonObjData->battlerIdx = battlerIdx;
    btlMonObjData->spriteContext.spriteSys = BattleSystem_GetSpriteSystem(battleSys);
    btlMonObjData->spriteContext.spriteMan = BattleSystem_GetSpriteManager(battleSys);
    btlMonObjData->spriteContext.plttData = BattleSystem_GetPaletteData(battleSys);

    {
        for (i = 0; i < 4; i++) {
            btlMonObjData->managedSprites[i] = 0L;
            btlMonObjData->pokemonSpriteDataArray[i] = ov12_0223BB88(battleSys, i);
        }

        ov12_0223C1C4(battleSys, &(btlMonObjData->battlerTypes[0]));
        ov12_0223C1F4(battleSys, &(btlMonObjData->pokemonSprites[0]));
    }

    {
        int spriteResIDs[6];
        NARC *spriteNarc;

        spriteNarc = NARC_New((enum NarcId)8, heapID);

        for (i = 0; i < 4; i++) {
            if ((i != btlMonObjData->battlerIdx) && (btlMonObjData->battlerIdx != 0xFF)) {
                continue;
            }

            if (btlMonObjData->pokemonSprites[i] == 0L) {
                continue;
            }

            spriteResIDs[0] = 55555 + i + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[1] = 55555 + i + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[2] = 55555 + i + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[3] = 55555 + i + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[4] = 0;
            spriteResIDs[5] = 0;

            SpriteSystem_LoadCharResObjFromOpenNarc(btlMonObjData->spriteContext.spriteSys, btlMonObjData->spriteContext.spriteMan, spriteNarc, 76, 0, NNS_G2D_VRAM_TYPE_2DMAIN, spriteResIDs[GF_GFX_RES_TYPE_CHAR]);
            SpriteSystem_LoadPaletteBufferFromOpenNarc(btlMonObjData->spriteContext.plttData, (enum PaletteBufferId)(2), btlMonObjData->spriteContext.spriteSys, btlMonObjData->spriteContext.spriteMan, spriteNarc, 75, 0, NNS_G2D_VRAM_TYPE_2DMAIN, (enum NNS_G2D_VRAM_TYPE)(1), spriteResIDs[GF_GFX_RES_TYPE_PLTT]);
            SpriteSystem_LoadCellResObjFromOpenNarc(btlMonObjData->spriteContext.spriteSys, btlMonObjData->spriteContext.spriteMan, spriteNarc, 77, 0, spriteResIDs[GF_GFX_RES_TYPE_CELL]);
            SpriteSystem_LoadAnimResObjFromOpenNarc(btlMonObjData->spriteContext.spriteSys, btlMonObjData->spriteContext.spriteMan, spriteNarc, 78, 0, spriteResIDs[GF_GFX_RES_TYPE_ANIM]);
        }

        NARC_Delete(spriteNarc);
    }

    {
        int battler;
        int narcID;
        int paletteIdx;
        void *tiles;
        int spriteResIDs[6];
        ManagedSprite *managedSprite;

        for (battler = 0; battler < 4; battler++) {
            if ((battler != btlMonObjData->battlerIdx) && (btlMonObjData->battlerIdx != 0xFF)) {
                continue;
            }

            btlMonObjData->resourceIDs[battler] = 55555 + battler + ((btlMonObjData->battlerIdx) * 5000);

            spriteResIDs[0] = 55555 + battler + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[1] = 55555 + battler + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[2] = 55555 + battler + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[3] = 55555 + battler + ((btlMonObjData->battlerIdx) * 5000);
            spriteResIDs[4] = 0;
            spriteResIDs[5] = 0;

            narcID = btlMonObjData->pokemonSpriteDataArray[battler]->narcID;
            paletteIdx = btlMonObjData->pokemonSpriteDataArray[battler]->palette;
            tiles = btlMonObjData->pokemonSpriteDataArray[battler]->tiles;

            {
                int resourceIdx;
                ManagedSpriteTemplate spriteTemplate;
                Pokepic *pokemonSprite;
                s16 centerX, centerY;

                pokemonSprite = btlMonObjData->pokemonSprites[battler];

                if (pokemonSprite != 0L) {
                    centerX = Pokepic_GetAttr(pokemonSprite, MON_SPRITE_X_CENTER);
                    centerY = Pokepic_GetAttr(pokemonSprite, MON_SPRITE_Y_CENTER);
                    centerY -= Pokepic_GetAttr(pokemonSprite, MON_SPRITE_SHADOW_HEIGHT);
                } else {
                    continue;
                }

                spriteTemplate.x = centerX;
                spriteTemplate.y = centerY;
                spriteTemplate.z = 0;
                spriteTemplate.animation = 0;
                spriteTemplate.drawPriority = sPriorityByBattlerType.a[btlMonObjData->battlerTypes[battler]];
                spriteTemplate.pal = 0;
                spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
                spriteTemplate.bgPriority = 1;
                spriteTemplate.vramTransfer = 0;

                for (resourceIdx = 0; resourceIdx < 6; resourceIdx++) {
                    spriteTemplate.resIdList[resourceIdx] = spriteResIDs[resourceIdx];
                }

                managedSprite = SpriteSystem_NewSprite(btlMonObjData->spriteContext.spriteSys, btlMonObjData->spriteContext.spriteMan, &spriteTemplate);
                ManagedSprite_TickFrame(managedSprite);
                btlMonObjData->managedSprites[battler] = managedSprite;

                if (pokemonSprite == 0L) {
                    ManagedSprite_SetDrawFlag(managedSprite, 0);
                } else if (Pokepic_IsActive(pokemonSprite) == 0) {
                    ManagedSprite_SetDrawFlag(managedSprite, 0);
                } else {
                    int isHidden = Pokepic_GetAttr(pokemonSprite, MON_SPRITE_HIDE);

                    if (isHidden == 1) {
                        ManagedSprite_SetDrawFlag(managedSprite, 0);
                    }
                }

                if (pokemonSprite != 0L) {
                    NNSG2dImageProxy *imageProxy;

                    imageProxy = Sprite_GetImageProxy(managedSprite->sprite);
                    GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_OBJ_CHAR_MAIN, imageProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN], tiles, 10 * 10 * ((8 / 2) * 8));
                }

                if (pokemonSprite != 0L) {
                    NNSG2dImagePaletteProxy *paletteProxy;
                    int paletteOffset;

                    paletteProxy = Sprite_GetPaletteProxy(managedSprite->sprite);
                    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(paletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN);

                    PaletteData_LoadNarc(btlMonObjData->spriteContext.plttData, (enum NarcId)(narcID), paletteIdx, btlMonObjData->heapID, PLTTBUF_MAIN_OBJ, ((16) * sizeof(u16)), ((paletteOffset) * (16)));
                }
            }
        }
    }

    return btlMonObjData;
}
