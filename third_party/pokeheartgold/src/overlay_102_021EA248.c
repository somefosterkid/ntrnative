#include "global.h"
#include "sprite_resources_header.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"
#include "sprite.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "touch_hitbox_controller.h"
#include "overlay_45_unk_struct_ov66_02234798.h"


void ov102_021EA248(u8 *param_1, void *arg1, s32 arg2);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct SpriteList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct EasyChatSentenceView;
struct UnkStruct_ov20_021D3980_t;
struct UnkStruct_ov20_021D4210_t;
struct UnkStruct_ov20_021D4AD4_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;
typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;
typedef struct UnkStruct_ov20_021D3980_t UnkStruct_ov20_021D3980;
typedef struct UnkStruct_ov20_021D4210_t UnkStruct_ov20_021D4210;
typedef struct UnkStruct_ov20_021D4AD4_t UnkStruct_ov20_021D4AD4;
struct UnkStruct_ov20_021D2128_t {
    SysTask *unk_00;
    SysTask *unk_04;
    SysTask *unk_08[4];
    const EasyChatApp *unk_18;
    const EasyChatWordList *unk_1C;
    BgConfig *unk_20;
    SpriteList *unk_24;
    G2dRenderer unk_28;
    NNSG2dImageProxy unk_1B4[2];
    NNSG2dImagePaletteProxy unk_1FC[2];
    NNSG2dCellDataBank *unk_224[2];
    NNSG2dCellAnimBankData *unk_22C[2];
    void *unk_234[2];
    void *unk_23C[2];
    EasyChatSentenceView *unk_244;
    UnkStruct_ov20_021D3980 *unk_248;
    EasyChatCategoriesContext *unk_24C;
    UnkStruct_ov20_021D4210 *unk_250;
    UnkStruct_ov20_021D4AD4 *unk_254;
};
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
u32 ov102_021EA258(UnkStruct_ov20_021D2128 *param0);
UnkStruct_ov70_0225CC54 *ov102_021EA268(UnkStruct_ov70_0225CA20 *param0);
UnkStruct_ov70_0225CC54 *ov102_021EA26C(UnkStruct_ov70_0225CA20 *param0);
void ov102_021EA270(UnkStruct_ov20_021D2128 *param0, SpriteResourcesHeader *param1, u32 param2, u32 param3);

void ov102_021EA248(u8 *param_1, void *arg1, s32 arg2) {
    ov102_021EAB10(*(void **)(param_1 + 0x1e0), arg1, arg2);
    return;
}

u32 ov102_021EA258(UnkStruct_ov20_021D2128 *param0) {
    return ov102_021EB130(param0->unk_244);
}

UnkStruct_ov70_0225CC54 *ov102_021EA268(UnkStruct_ov70_0225CA20 *param0) {
    return param0->unk_20;
}

UnkStruct_ov70_0225CC54 *ov102_021EA26C(UnkStruct_ov70_0225CA20 *param0) {
    return param0->unk_24;
}

void ov102_021EA270(UnkStruct_ov20_021D2128 *param0, SpriteResourcesHeader *param1, u32 param2, u32 param3) {
    param1->imageProxy = &param0->unk_1B4[param2];
    param1->plttProxy = &param0->unk_1FC[param2];
    param1->cellData = param0->unk_224[param2];
    param1->cellAnim = param0->unk_22C[param2];
    param1->priority = param3;
    param1->charData = 0L;
    param1->multiCellData = 0L;
    param1->multiCellAnim = 0L;
    param1->flag = 0;
}
