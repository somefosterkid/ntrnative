#include "global.h"
#include "sprite_resources_header.h"

#include "wild_encounter.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"
#include "options.h"
#include "pm_string.h"
#include "player_data.h"
#include "bg_window.h"
#include "filesystem.h"
#include "msgdata.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "touch_pad_data_buffer.h"

struct BgConfig;

struct G2dRenderer;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct Options;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct TouchPadDataBuffer;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_02095EAC_t;

struct Window;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

struct YesNoPrompt;

typedef struct UnkStruct_02095EAC_t UnkStruct_02095EAC;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;

typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;

typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;

typedef struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
} JournalEntry;

typedef struct {
    UnkStruct_0205C22C *unk_00;
    JournalEntry *unk_04;
    Options *options;
} UnkStruct_0203DDFC;

typedef struct {
    u8 unk_00[1000];
    u32 unk_3E8;
    u8 unk_3EC;
    u8 unk_3ED[3];
} UnkStruct_02095EAC_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov58_021D2754;

typedef struct {
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08_0 : 3;
    u8 unk_08_3 : 3;
    u8 unk_08_6 : 2;
    u8 unk_09;
} UnkStruct_ov58_021D2820;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

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

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

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

extern int TouchScreen_CheckRectangleHeld(const TouchScreenRect rects[]);

extern int TouchScreen_CheckRectanglePressed(const TouchScreenRect rects[]);

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchHitboxController {
    const TouchscreenHitbox *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchHitboxController;

enum YesNoTouchButton {
    BUTTON_YES = 0,
    BUTTON_NO,
    BUTTON_COUNT,
    BUTTON_NONE,
};

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

typedef struct YesNoPrompt {
    TouchHitboxController *buttonAction;
    TouchscreenHitbox hitTable[BUTTON_COUNT];
    YesNoButtonSprite buttonSprites[BUTTON_COUNT];
    BgConfig *bgConfig;
    u32 bgLayer;
    enum HeapID heapID;
    fx32 frameThresholds[4];
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 pressedButton;
    u8 paramsApplied : 4;
    u8 buttonState : 4;
} YesNoPrompt;

struct UnkStruct_02095EAC_t {
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
    SpriteResourceCollection *unk_1C4[4];
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
};

void sub_02038C1C(int maxNumConnection);

int sub_02037454(void);

u16 sub_0203769C(void);

BOOL PlaySE(u16 seqID);

enum TouchPadExternalBufferWriteMethod {
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING = 1,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITH_WRAPPING,
};

u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold);

u16 sub_02033250(void);

void ov58_021D1A10(Sprite *param0, int param1, int param2);

void ov37_021E762C(UnkStruct_02095EAC *param0, int param1, int param2);

void ov37_021E6818(Sprite **param0, int param1);

void ov37_021E6848(Sprite **param0, BOOL param1);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

void ov58_021D2D4C(UnkStruct_ov58_021D2820 *param0, TouchPadDataBuffer *param1, int param2, int param3);

extern const TouchScreenRect ov37_021E7A4C[13];

extern const TouchScreenRect ov37_021E7970[2];

void ov37_021E65EC(UnkStruct_02095EAC *param0, s32 arg1);

int TouchscreenHitbox_FindRectAtTouchNew(void *, s32);

int TouchscreenHitbox_FindRectAtTouchHeld(void *);

void ov37_021E657C(Sprite *param0, int param1, int param2);

void ov37_021E78E0(UnkStruct_ov58_021D2820 *param0, TouchPadDataBuffer *param1, int param2, int param3);

void ov37_021E65EC(UnkStruct_02095EAC *param0, s32 arg1) {
    int v0 = -1, v1;
    int v2 = 0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(ov37_021E7A4C, arg1);

    if (v0 != 0xffffffff) {
        switch (v0) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (param0->unk_43DA != v0) {
                param0->unk_43DA = v0;
                ov37_021E6818(param0->unk_2AC, v0);
                PlaySE(1500);
            }
            break;
        case 8:
            if (param0->unk_368 == 4) {
                if (sub_0203769C() == 0) {
                    if (param0->unk_380 != sub_02033250()) {
                        PlaySE(1522);
                        break;
                    }

                    sub_02038C1C(sub_02037454());
                    param0->unk_9458 = 2;
                    ov37_021E762C(param0, 1, 1);
                    ov37_021E7844(param0, 5);
                    ov37_021E6848(param0->unk_2AC, 1);
                    v2 = 1;
                    PlaySE(1500);
                } else {
                    if (param0->unk_43E6[0].unk_09 == 2) {
                        PlaySE(1522);
                    } else {
                        ov37_021E762C(param0, 1, 1);
                        ov37_021E7844(param0, 5);
                        ov37_021E6848(param0->unk_2AC, 1);
                        v2 = 1;
                        PlaySE(1500);
                    }
                }
            }
            break;
        case 9:
        case 10:
        case 11: {
            int v3;

            for (v3 = 0; v3 < 3; v3++) {
                if ((v0 - 9) == v3) {
                    Sprite_SetAnimCtrlSeq(param0->unk_2AC[9 + v3], 29 + 2 * v3 + 1);
                } else {
                    Sprite_SetAnimCtrlSeq(param0->unk_2AC[9 + v3], 29 + 2 * v3);
                }
            }

            if (param0->unk_43DB != (0 + v0 - 9)) {
                param0->unk_43DB = 0 + v0 - 9;
                PlaySE(1509);
            }
        } break;
        }
    }

    v1 = TouchscreenHitbox_FindRectAtTouchHeld(ov37_021E7970);
    Sprite_SetDrawFlag(param0->unk_23C[sub_0203769C()], 0);

    if (v1 != 0xffffffff) {
        ov37_021E657C(param0->unk_23C[sub_0203769C()], gSystem.touchX, gSystem.touchY);
        Sprite_SetDrawFlag(param0->unk_23C[sub_0203769C()], 1);
    }

    {
        TouchPadDataBuffer v4;

        if (sub_02021280(&v4, TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING, 64) == 1) {
            ov37_021E78E0(&param0->unk_43DC, &v4, param0->unk_43DA, param0->unk_43DB);

            if (v2 == 1) {
                param0->unk_43DC.unk_08_3 = 0;
            }
        }
    }
}
