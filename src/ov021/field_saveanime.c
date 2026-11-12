// Black's code at ov021 0x021d1164-0x021d14c4. The name is the file name its first function's assert passes
// (tools/name_files.py); calls join all its functions, and files are on both sides.

#include "types.h"
#include "field/field_effect.h"
#include "field/field.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/field_camera.h"
#include "nitro/fx/fx_vec.h"

// pokebw2's struct and field names
struct FieldSaveAnime {
    Field *field;       // 0x00
    BmpWin *window;     // 0x04
    // Where the icon sits in its window's first tile, in pixels
    u16 offsetX;        // 0x08
    u16 offsetY;        // 0x0a
    TCB *tcb;           // 0x0c
    GFLBitmap *bitmap;  // 0x10
    int timer;          // 0x14
    u32 frame;          // 0x18
};

const u32 data_ov021_021db97c[2] = { 0x00000000, 0x00000001 };
const VecFx32 data_ov021_021db984 = { 0, 0x1000, 0 };
const u16 data_ov021_021db990[4][2] = { { 0x0000, 0x0014 }, { 0x0001, 0x0028 }, { 0x0000, 0x003c }, { 0x0002, 0x0050 } };

void func_ov021_021d11f4(FieldSaveAnime *effect, HeapID heapId);
void func_ov021_021d1264(FieldSaveAnime *effect);
void func_ov021_021d13f0(FieldSaveAnime *anime);
void func_ov021_021d1254(FieldSaveAnime *anime);
void func_ov021_021d1400(FieldSaveAnime *anime);
void func_ov021_021d1418(FieldSaveAnime *anime);
void func_ov021_021d1430(FieldSaveAnime *anime, s32 frame);
void func_ov021_021d1488(TCB *tcb, void *data);

FieldSaveAnime *func_ov021_021d1164(HeapID heapId, Field *field) {
    FieldSaveAnime *effect;

    effect = GFL_HeapAllocate(heapId, sizeof(FieldSaveAnime), 1, "field_saveanime.c", 0x8f);
    effect->field = field;
    func_ov021_021d11f4(effect, heapId);
    func_ov021_021d1264(effect);
    return effect;
}

void func_ov021_021d1194(FieldSaveAnime *effect) {
    func_ov021_021d11d8(effect);
    func_ov021_021d13f0(effect);
    func_ov021_021d1254(effect);
    GFL_HeapFree(effect);
}

void func_ov021_021d11b0(FieldSaveAnime *effect) {
    GFL_BGSysClearBG(3);
    effect->timer = 0;
    func_ov021_021d1400(effect);
    effect->tcb = GFL_VBlankTCBAdd(func_ov021_021d1488, effect, 0x80);
}

void func_ov021_021d11d8(FieldSaveAnime *anime) {
    if (anime->tcb != NULL) {
        GFL_TCBRemove(anime->tcb);
        anime->tcb = NULL;
        func_ov021_021d1418(anime);
    }
}

void func_ov021_021d11f4(FieldSaveAnime *effect, HeapID heapId) {
    ArcTool *arc;
    GameSystem *gsys;
    GameData *gameData;
    PlayerInfo *info;
    u32 gender;

    arc = GFL_ArcSysCreateFileHandle(200, heapId);
    gsys = Field_GetGameSystem(effect->field);
    gameData = GSYS_GetGameData(gsys);
    info = GetGameDataPlayerInfo(gameData);
    gender = getTrainerGender(info);
    effect->bitmap = GFL_G2DIOLoadBitmap(200, 0, 0, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 1, 0, data_ov021_021db97c[gender] << 5, 0x120, 0x20, heapId);
    GFL_ArcToolFree(arc);
}



void func_ov021_021d1254(FieldSaveAnime *anime) {
    GFL_BitmapFree(anime->bitmap);
    anime->bitmap = NULL;
}

void func_ov021_021d1264(FieldSaveAnime *effect) {
    VecFx32 pos;
    VecFx32 diff;
    VecFx32 flat;
    VecFx32 cross;
    VecFx32 up;
    VecFx32 lookatPos;
    VecFx32 target;
    s32 sx;
    s32 sy;
    FieldPlayer *player;
    FieldCamera *camera;
    G3DCamera *cam;
    BmpWin *window;
    GFLBitmap *bitmap;

    player = Field_GetPlayer(effect->field);
    camera = Field_GetCameraSystem(effect->field);
    cam = FieldCamera_GetG3DCamera(camera);
    GFL_G3DCameraFlush(cam);
    GFL_G3DCameraGetLookatTarget(cam, &target);
    GFL_G3DCameraGetLookatPos(cam, &lookatPos);
    VEC_Subtract(&target, &lookatPos, &diff);
    flat = diff;
    flat.y = 0;
    VEC_Normalize(&flat, &flat);
    VEC_CrossProduct(&flat, &data_ov021_021db984, &cross);
    VEC_CrossProduct(&diff, &cross, &up);
    VEC_Normalize(&up, &up);
    FieldPlayer_GetWPos(player, &pos);
    pos.x += FX_Mul(up.x, -0x1c000);
    pos.y += FX_Mul(up.y, -0x1c000);
    pos.z += FX_Mul(up.z, -0x1c000);
    NNS_G3dWorldPosToScrPos(&pos, &sx, &sy);
    sx -= 0x10;
    sy -= 0x18;
    effect->offsetX = sx % 8;
    effect->offsetY = sy % 8;
    sx = sx / 8;
    sy = sy / 8;
    if (sx < 0) {
        sx = 0;
    }
    if (sy < 0) {
        sy = 0;
    }
    if (sx > 0x1f) {
        sx = 0x1f;
    }
    if (sy > 0x18) {
        sy = 0x18;
    }
    window = BmpWin_CreateDynamic(3, sx, sy, 5, 5, 9, 1);
    effect->window = window;
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0);
    BmpWin_FlushChar(effect->window);
    func_ov021_021d1430(effect, 0);
}


void func_ov021_021d13f0(FieldSaveAnime *anime) {
    BmpWin_Free(anime->window);
    anime->window = NULL;
}

void func_ov021_021d1400(FieldSaveAnime *anime) {
    BmpWin_FlushMap(anime->window);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysSetBGEnabled(3, 1);
}

void func_ov021_021d1418(FieldSaveAnime *anime) {
    func_020456f8(anime->window);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysSetBGEnabled(3, 0);
}

void func_ov021_021d1430(FieldSaveAnime *anime, s32 frame) {
    u16 offset;
    GFLBitmap *dest;
    u8 *src;
    u32 size;

    offset = func_02045768(anime->window);
    dest = BmpWin_GetBitmap(anime->window);
    GFL_BitmapCopyArea(anime->bitmap, dest, 0, frame << 5, anime->offsetX, anime->offsetY, 0x20, 0x20, 0xffff);
    src = GFL_BitmapGetPixelData(dest);
    size = GFL_BitmapCalcPixelDataSize(dest);
    GFL_BGSysLoadChar(3, src, size, (u32)offset);
}

void func_ov021_021d1488(TCB *tcb, void *data) {
    FieldSaveAnime *effect = data;
    int timer;
    u32 index;

    timer = effect->timer + 1;
    effect->timer = timer;
    if (data_ov021_021db990[effect->frame][1] <= timer) {
        index = (effect->frame = effect->frame + 1);
        if (index >= 4) {
            effect->frame = 0;
            effect->timer = 0;
        }
        func_ov021_021d1430(effect, data_ov021_021db990[effect->frame][0]);
    }
}
