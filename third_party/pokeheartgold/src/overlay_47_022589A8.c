#include "global.h"

#include "constants/heap.h"
#include "overlay_47_unk_struct_ov68_0225_d128.h"
#include "overlay_45_wifi_plaza_system.h"
#include "bg_window.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "overlay_47_unk_struct_ov68_0225c91c.h"
#include "overlay_manager.h"
#include "msgdata.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct G2dRenderer;

struct Menu;

struct MsgData;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct MessageFormat;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

void OverlayManager_FreeData(OverlayManager *appMan);

void *OverlayManager_GetArgs(OverlayManager *appMan);

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

void Heap_Destroy(enum HeapID heapID);

typedef struct Menu Menu;

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void sub_0200616C(int fadeOutFrames);

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
    Window unk_00[6];
    u16 unk_60;
    u16 unk_62;
    s32 unk_64;
    s32 unk_68[3];
    s32 unk_74[3];
    void *unk_80;
    NNSG2dCharacterData *unk_84;
    UnkStruct_ov68_0225DB8C unk_88;
    UnkStruct_ov68_0225DB8C unk_A8;
    UnkStruct_ov68_0225DC74 unk_C8;
    UnkStruct_ov68_0225DC74 unk_DC;
} UnkStruct_ov68_0225D388;

typedef union {
    UnkStruct_ov68_0225D0F8 val1;
    UnkStruct_ov68_0225D388 val2;
} UnkUnion_ov68_0225CCA8;

typedef struct {
    MsgData *unk_00[9];
    MessageFormat *unk_24;
    String *unk_28;
    String *unk_2C;
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov68_0225CB70;

typedef struct {
    UnkStruct_ov68_0225C91C unk_00;
    UnkStruct_ov68_0225CB70 unk_1A8;
    UnkUnion_ov68_0225CCA8 unk_1DC;
    UnkStruct_ov68_0225D128 unk_2CC;
} UnkStruct_ov68_0225C700;

void ov47_02258A60(UnkStruct_ov68_0225C91C *param0);

void ov47_02258CC0(UnkStruct_ov68_0225CB70 *param0);

void ov68_0225CCC8(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

void ov68_0225D380(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2);

void ov47_02259278(UnkStruct_ov68_0225D128 *param0, u32 param1);

extern void (*_02259FA0[2])(UnkUnion_ov68_0225CCA8 *, UnkStruct_ov68_0225CB70 *, UnkStruct_ov68_0225C91C *);

void ov47_02258A1C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov47_02258AA0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);


void ov47_02259488(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);


void ov47_022599F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov47_02259C8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

int ov47_022589A8(OverlayManager *appMan, int *param1);

int ov47_022589A8(OverlayManager *appMan, int *param1) {
    UnkStruct_ov68_0225C700 *v0 = OverlayManager_GetData(appMan);
    UnkStruct_ov66_02231374 *v1 = OverlayManager_GetArgs(appMan);

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();

    _02259FA0[v1->unk_08](&v0->unk_1DC, &v0->unk_1A8, &v0->unk_00);

    ov47_02259278(&v0->unk_2CC, 129);
    ov47_02258CC0(&v0->unk_1A8);
    ov47_02258A60(&v0->unk_00);

    OverlayManager_FreeData(appMan);
    Heap_Destroy((enum HeapID)129);
    sub_0200616C(0);

    return 1;
}
