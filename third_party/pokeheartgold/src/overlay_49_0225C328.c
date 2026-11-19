#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

struct G3DPipelineBuffers;

struct MessageLoader;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct StringTemplate;

struct TrainerInfo;

typedef struct MessageLoader MessageLoader;

typedef struct TrainerInfo TrainerInfo;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

void Sprite_Delete(Sprite *gfxElem);

NNSG2dImageProxy *Sprite_GetImageProxy(Sprite *sprite);

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

void DestroySingle2DGfxResObj(SpriteResourceCollection *spriteResources, SpriteResource *resource);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

typedef struct StringTemplate StringTemplate;

typedef struct {
    StringTemplate *unk_00;
    MessageLoader *unk_04[4];
    String *unk_14;
    String *unk_18;
} UnkStruct_ov70_0225F208;

typedef struct {
    u32 unk_00;
    Window unk_04[5];
    Sprite *unk_54;
    SpriteResource *unk_58[4];
    Sprite *unk_68;
    SpriteResource *unk_6C[4];
    Sprite *unk_7C[12];
    SpriteResource *unk_AC[4];
    SpriteResource *unk_BC[12];
    SpriteResource *unk_EC[4];
    SpriteResource *unk_FC[4];
} UnkStruct_ov70_0225FA84;

void ov49_0225C328(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1);

void ov49_0225B42C(void *, s32, u16);
void ov49_0225C4B0(UnkStruct_ov70_0225FA84 *param_1, void *param_2, u16 *param_3);

void ov49_0225C328(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1) {
    ObjCharTransfer_DeleteTaskCopyByProxyPtr(Sprite_GetImageProxy(param0->unk_68));
    Sprite_Delete(param0->unk_68);

    param0->unk_68 = NULL;

    {
        int v0;

        SpriteTransfer_DeleteCharTransferTask(param0->unk_6C[0]);
        SpriteTransfer_DeletePlttTransferTask(param0->unk_6C[1]);

        for (v0 = 0; v0 < 4; v0++) {
            DestroySingle2DGfxResObj(param1->unk_194[v0], param0->unk_6C[v0]);
        }
    }
}

void ov49_0225C368(NNSG2dPaletteData *param0);

void ov49_0225C368(NNSG2dPaletteData *param0) {
    int v0, v1, v2, v3, v4;
    int v5;
    u16 *v6;

    v5 = param0->szByte / 2;
    v6 = param0->pRawData;

    for (v0 = 0; v0 < v5; v0++) {
        v1 = ((*v6) & 0x1f);
        v2 = (((*v6) >> 5) & 0x1f);
        v3 = (((*v6) >> 10) & 0x1f);
        v4 = ((v1) * 76 + (v2) * 151 + (v3) * 29) >> 8;
        v1 = ((v4) * 256) >> 8;
        v2 = ((v4) * 200) >> 8;
        v3 = ((v4) * 128) >> 8;

        if (v1 > 31) {
            v1 = 31;
        }

        *v6 = (u16)((v3 << 10) | (v2 << 5) | v1);
        v6++;
    }
}

void ov49_0225C3C0(UnkStruct_ov70_0225FA84 *param0);

void ov49_0225C3C0(UnkStruct_ov70_0225FA84 *param0) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        FillWindowPixelBuffer(&param0->unk_04[v0], 0);
    }
}

String *ov49_0225B388(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);

void ov49_0225C3DC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, u32 param2, u32 param3, u8 param4, u8 param5, u32 param6);

void ov49_0225C3DC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, u32 param2, u32 param3, u8 param4, u8 param5, u32 param6) {
    String *v0 = ov49_0225B388(param1, 1, param3);
    AddTextPrinterParameterizedWithColor(&param0->unk_04[param2], FONT_SYSTEM, v0, param4, param5, 0xFF, param6, 0L);
}

void ov49_0225C414(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, u32 param2, u32 param3, u8 param4, u8 param5, u32 param6);

void ov49_0225C414(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, u32 param2, u32 param3, u8 param4, u8 param5, u32 param6) {
    String *v0;
    u32 v1;
    s32 v2;

    v0 = ov49_0225B388(param1, 1, param3);
    v1 = FontID_String_GetWidth(FONT_SYSTEM, v0, 0);
    v2 = param4 - v1;

    if (v2 < 0) {
        v2 = 0;
    }

    AddTextPrinterParameterizedWithColor(&param0->unk_04[param2], FONT_SYSTEM, v0, v2, param5, 0xFF, param6, 0L);
}

void ov49_0225C460(UnkStruct_ov70_0225FA84 *param0, u32 param1);

void ov49_0225C460(UnkStruct_ov70_0225FA84 *param0, u32 param1) {
    ScheduleWindowCopyToVram(&param0->unk_04[param1]);
}

void ov49_0225B3C8(UnkStruct_ov70_0225F208 *param0, const TrainerInfo *param1, u32 param2);

void ov49_0225C470(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, const TrainerInfo *param2);

void ov49_0225C470(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, const TrainerInfo *param2) {
    ov49_0225B3C8(param1, param2, 0);
}

struct WiFiPlazaProfile;

typedef struct WiFiPlazaProfile WiFiPlazaProfile;

u32 ov45_0222AA84(const WiFiPlazaProfile *profile);

u8 ov45_0222AAA8(const WiFiPlazaProfile *profile);

void ov49_0225B408(UnkStruct_ov70_0225F208 *param0, u32 param1, u32 param2);

void ov49_0225B418(UnkStruct_ov70_0225F208 *param0, u32 param1, u32 param2, u32 param3);

void ov49_0225C480(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, const WiFiPlazaProfile *param2);

void ov49_0225C480(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225F208 *param1, const WiFiPlazaProfile *param2) {
    u32 v0;
    u32 v1;

    v0 = ov45_0222AA84(param2);
    v1 = ov45_0222AAA8(param2);

    ov49_0225B408(param1, v0, 0);
    ov49_0225B418(param1, v0, v1, 1);
}

void ov49_0225C4B0(UnkStruct_ov70_0225FA84 *param_1, void *param_2, u16 *param_3) {
    s32 iVar1;

    iVar1 = 0;
    do {
        ov49_0225B42C(param_2, iVar1, *param_3);
        iVar1 = iVar1 + 1;
        param_3 = param_3 + 1;
    } while (iVar1 < 4);
    return;
}
