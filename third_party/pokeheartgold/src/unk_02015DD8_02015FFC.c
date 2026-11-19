#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

void sub_02015FFC(void *arg0, s8 arg1);

struct GameStats;
struct JournalEntry;
struct MsgData;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_0205C22C_t;
struct UnkStruct_ov109_021D0F70_t;
struct UnkStruct_ov109_021D5140_t;
typedef struct MapPropAnimation {
    NNSG3dAnmObj *animationObj;
    BOOL loaded;
    int currentLoop;
    int animeArchiveID;
    BOOL paused;
    void *animationFile;
    int loopCount;
    BOOL looping;
    BOOL reversed;
    int pastoriaGymButtonGroup;
} MapPropAnimation;
typedef struct SoftwareSprite SoftwareSprite;
struct SoftwareSprite {
    NNSG2dSVec2 position;
    NNSG2dSVec2 center;
    NNSG2dSVec2 dimensions;
    fx32 scaleX;
    fx32 scaleY;
    int priority;
    int rotation;
    BOOL inUse;
    BOOL visible;
    NNSG2dImageAttr *attributes;
    u32 vramAddressChars;
    u32 vramAddressPalette;
    u32 paletteSlot;
    BOOL flipH;
    BOOL flipV;
    GXRgb diffuse;
    u8 alpha;
};
typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;
typedef struct JournalEntry JournalEntry;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
struct SoftwareSpriteChars {
    NNSGfdTexKey vramKey;
    NNSG2dImageProxy proxy;
};
void sub_02016004(MapPropAnimation *animation, const int pastoriaGymButtonGroup);
void sub_02016008(SoftwareSprite *sprite, u32 address);
void sub_0201600C(SoftwareSprite *sprite, u32 address);
void sub_02016010(UnkStruct_ov109_021D0F70 *param0, int param1);
void sub_02016014(SoftwareSprite *sprite, int direction, BOOL flip);
void sub_02016020(MapPropAnimation *animation, const int loopCount);
void sub_02016024(SoftwareSprite *sprite);
void sub_02016044(SoftwareSpriteChars *chars);

void sub_02015FFC(void *arg0, s8 arg1) {
    *(s8 *)((u8 *)arg0 + 0x3E) = arg1;
}

void sub_02016004(MapPropAnimation *animation, const int pastoriaGymButtonGroup) {
    animation->pastoriaGymButtonGroup = pastoriaGymButtonGroup;
}

void sub_02016008(SoftwareSprite *sprite, u32 address) {
    sprite->vramAddressChars = address;
}

void sub_0201600C(SoftwareSprite *sprite, u32 address) {
    sprite->vramAddressPalette = address;
}

void sub_02016010(UnkStruct_ov109_021D0F70 *param0, int param1) {
    param0->unk_2C.unk_04 = param1;
}

void sub_02016014(SoftwareSprite *sprite, int direction, BOOL flip) {
    if (direction == 0) {
        sprite->flipH = flip;
    } else {
        sprite->flipV = flip;
    }
}

void sub_02016020(MapPropAnimation *animation, const int loopCount) {
    animation->loopCount = loopCount;
}

void sub_02016024(SoftwareSprite *sprite) {
    memset(sprite, 0, sizeof(SoftwareSprite));
    sprite->alpha = 31;
    sprite->diffuse = (((31) << 10) | ((31) << 5) | (31));
}

void sub_02016044(SoftwareSpriteChars *chars) {
    chars->vramKey = 0;
    NNS_G2dInitImageProxy(&chars->proxy);
}
