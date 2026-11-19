#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "text.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"

struct MessageLoader;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct StringTemplate;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

typedef struct MessageLoader MessageLoader;

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

typedef struct SpriteResource SpriteResource;

void DestroySingle2DGfxResObj(SpriteResourceCollection *spriteResources, SpriteResource *resource);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

typedef struct StringTemplate StringTemplate;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    String *unk_10;
    String *unk_14[3];
} UnkStruct_ov68_0225DB8C;

typedef struct {
    Window unk_00[1];
    s32 unk_10;
    Sprite *unk_14;
    SpriteResource *unk_18[4];
    u16 unk_28;
    u16 unk_2A;
    UnkStruct_ov68_0225DB8C unk_2C;
} UnkStruct_ov68_0225D0F8;

typedef struct {
    MessageLoader *unk_00[9];
    StringTemplate *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

void ov47_02259D24(UnkStruct_ov68_0225DB8C *param0);

void ov47_0225912C(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

void ov47_0225912C(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2) {
    ov47_02259D24(&param0->unk_2C);

    {
        int v0;

        for (v0 = 0; v0 < 1; v0++) {
            RemoveWindow(&param0->unk_00[v0]);
        }
    }

    {
        Sprite_Delete(param0->unk_14);
    }

    {
        int v1;

        SpriteTransfer_DeleteCharTransferTask(param0->unk_18[0]);
        SpriteTransfer_DeletePlttTransferTask(param0->unk_18[1]);

        for (v1 = 0; v1 < 4; v1++) {
            DestroySingle2DGfxResObj(param2->unk_194[v1], param0->unk_18[v1]);
        }
    }
}

typedef u32 TextColor;

String *ov47_02259D58(const UnkStruct_ov68_0225DB8C *param0, UnkStruct_ov68_0225CB70 *param1, u32 param2);

void ov47_0225916C(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

void ov47_0225916C(UnkStruct_ov68_0225D0F8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2) {
    String *v0;

    FillWindowPixelBuffer(&param0->unk_00[0], 0);

    v0 = ov47_02259D58(&param0->unk_2C, param1, 0);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[0], FONT_MESSAGE, v0, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);

    v0 = ov47_02259D58(&param0->unk_2C, param1, 1);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[0], FONT_MESSAGE, v0, 0, 24, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);

    v0 = ov47_02259D58(&param0->unk_2C, param1, 2);
    AddTextPrinterParameterizedWithColor(&param0->unk_00[0], FONT_MESSAGE, v0, 0, 48, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    ScheduleWindowCopyToVram(&param0->unk_00[0]);
}
