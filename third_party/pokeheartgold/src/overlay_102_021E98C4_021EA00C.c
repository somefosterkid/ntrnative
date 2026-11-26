#include "global.h"

#include "bg_window.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"
#include "sprite.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_102_easy_chat_sentence_view.h"
#include "touch_hitbox_controller.h"

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

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

int ov102_021E9044(const EasyChatApp *easyChatApp);

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

void ov102_021EBFA0(UnkStruct_ov20_021D4210 *param0, int param1);

BOOL ov102_021EC05C(UnkStruct_ov20_021D4210 *param0);

typedef struct UnkStruct_ov20_021D4AD4_t UnkStruct_ov20_021D4AD4;

void ov102_021EC290(UnkStruct_ov20_021D4AD4 *param0, BOOL param1);

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

typedef struct {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    u32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov20_021D2238;

void ov102_021E94A4(UnkStruct_ov20_021D2238 *param0);

void ov102_021EA00C(SysTask *param0, void *param1);

void ov102_021EA00C(SysTask *param0, void *param1) {
    UnkStruct_ov20_021D2238 *v0 = param1;
    UnkStruct_ov20_021D2128 *v1 = v0->unk_00;

    switch (v0->unk_14) {
    case 0:
        ov102_021EBFA0(v1->unk_250, ov102_021E9044(v1->unk_18));
        v0->unk_14++;
        break;
    case 1:
        if (ov102_021EC05C(v1->unk_250)) {
            ov102_021EC290(v1->unk_254, 1);
            ov102_021E94A4(v0);
        }
        break;
    }
}
