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
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct Options;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteResource;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct Window;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_CreateAndGetData(OverlayManager *appMan, u32 size, enum HeapID heapID);

void *OverlayManager_GetArgs(OverlayManager *appMan, int *);

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct {
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    int unk_08;
} UnkStruct_ov66_02231374;

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

typedef u32 TextColor;

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} ListMenu2D;

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

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

void ov47_02258A14(void *param0);

void ov68_0225C91C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov68_0225CB70(UnkStruct_ov68_0225CB70 *param0, WiFiPlazaSystem *param1, enum HeapID heapID);

void ov68_0225CCA8(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, u32 param4);

void ov68_0225D35C(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, u32 param4);

void ov47_02259228(UnkStruct_ov68_0225D128 *param0, UnkStruct_ov68_0225C91C *param1, SaveData *saveData, enum HeapID heapID);

extern void (*ov47_02259FB0[2])(UnkUnion_ov68_0225CCA8 *, UnkStruct_ov68_0225CB70 *, UnkStruct_ov68_0225C91C *, UnkStruct_ov66_02231374 *, u32);

void ov68_0225C9A0(UnkStruct_ov68_0225C91C *param0, Options *options, enum HeapID heapID);

void ov47_02258BB4(UnkStruct_ov68_0225C91C *param0, enum HeapID heapID);

void ov47_02258D78(UnkStruct_ov68_0225CB70 *param0, u32 param1, enum HeapID heapID);

BOOL ov68_0225CCB4(UnkUnion_ov68_0225CCA8 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov68_0225D128 *param3, UnkStruct_ov66_02231374 *param4, enum HeapID heapID);


void ov68_0225D388(UnkStruct_ov68_0225D388 *param0, UnkStruct_ov68_0225CB70 *param1, UnkStruct_ov68_0225C91C *param2, UnkStruct_ov66_02231374 *param3, enum HeapID heapID);


void ov68_0225D8F0(UnkStruct_ov68_0225D388 *param0, const UnkStruct_ov68_0225DB8C *param1, UnkStruct_ov68_0225CB70 *param2, UnkStruct_ov68_0225C91C *param3, const UnkStruct_ov66_0222E908 *param4, enum HeapID heapID, u32 param6, u32 param7, BOOL param8);

void ov68_0225DB8C(UnkStruct_ov68_0225DB8C *param0, BOOL param1, enum HeapID heapID);

int ov47_02258800(OverlayManager *appMan, int *param1);

void ov47_02258A1C(UnkStruct_ov68_0225C91C *param0, SaveData *saveData, enum HeapID heapID);

void ov47_02258C70(UnkStruct_ov68_0225CB70 *param0, WiFiPlazaSystem *param1, enum HeapID heapID);

int ov47_02258800(OverlayManager *appMan, int *param1) {
    UnkStruct_ov68_0225C700 *v0;
    UnkStruct_ov66_02231374 *v1 = OverlayManager_GetArgs(appMan, param1);

    Heap_Create(HEAP_ID_3, (enum HeapID)(129), 0x50000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov68_0225C700), (enum HeapID)(129));
    memset(v0, 0, sizeof(UnkStruct_ov68_0225C700));

    ov47_02258A1C(&v0->unk_00, v1->saveData, (enum HeapID)(129));
    ov47_02258C70(&v0->unk_1A8, v1->unk_04, (enum HeapID)(129));
    ov47_02259228(&v0->unk_2CC, &v0->unk_00, v1->saveData, (enum HeapID)(129));

    ((v1->unk_08 < 2) ? (void)0 : GF_AssertFail());
    ov47_02259FB0[v1->unk_08](&v0->unk_1DC, &v0->unk_1A8, &v0->unk_00, v1, 129);

    Main_SetVBlankIntrCB(ov47_02258A14, v0);
    HBlankInterruptDisable();

    return 1;
}
