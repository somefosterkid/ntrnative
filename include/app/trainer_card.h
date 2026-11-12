// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/trainer_card.h: the lines in use so far.
#ifndef BWPORT_APP_TRAINER_CARD_H
#define BWPORT_APP_TRAINER_CARD_H

#include "types.h"
#include "field/trcard_sys.h"
#include "gfl/clact.h"
#include "nitro/fx/fx.h"
#include "nnsys/g2d.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The heap of the trainer card screen and of the badge case
#define HEAPID_TRAINER_CARD_SCREEN 0x27

// a/0/4/0, the screens' graphics, and a/0/4/1, the trainer pictures of the union room appearances
#define ARCID_TRAINER_CARD_GRA 40

#define ARCID_TRAINER_CARD_UNION_GRA 41
// The Y button's shortcuts of the card's front, its back and the badge case
#define SHORTCUT_TRAINER_CARD_FRONT 21

#define SHORTCUT_TRAINER_CARD_BACK 22

// The signature: 192x64 pixels, one bit each in the save and one byte each while it is edited
#define TRCARD_SIGN_WIDTH 192

#define TRCARD_SIGN_HEIGHT 64
#define TRCARD_SIGN_PIXELS (TRCARD_SIGN_WIDTH * TRCARD_SIGN_HEIGHT)
#define TRCARD_SIGN_BYTES (TRCARD_SIGN_PIXELS / 8)

// Black 2's, not checked against Black
#define TRCARD_WIN_COUNT 8

#define TRCARD_ACTOR_COUNT 8
// The rows of the back's list, of which five show at a time
#define TRCARD_ROW_MAX 19

// Black 2's, not checked against Black
// The actors
enum {
    TRCARD_ACTOR_RETURN,
    TRCARD_ACTOR_EXIT,
    TRCARD_ACTOR_BADGES,
    TRCARD_ACTOR_3,
    TRCARD_ACTOR_ZOOM,
    TRCARD_ACTOR_BRUSH,
    TRCARD_ACTOR_FLIP,
    TRCARD_ACTOR_SHORTCUT,
    // Five that depend on the side shown, from sTrainerCardSideActors
    TRCARD_ACTOR_SIDE,
    TRCARD_ACTOR_SCROLL_UP = 11,
    TRCARD_ACTOR_SCROLL_DOWN,
    TRCARD_ACTOR_RANK,
};

// Black 2's, not checked against Black
// The windows
enum {
    TRCARD_WIN_NAME,
    TRCARD_WIN_UNION,
    TRCARD_WIN_PERSONALITY,
    TRCARD_WIN_POKEDEX,
    TRCARD_WIN_MONEY,
    TRCARD_WIN_GREETING,
    TRCARD_WIN_LIST,
    TRCARD_WIN_MESSAGE,
};

// Where the pen is on the signature
typedef struct {
    u8 x;
    u8 y;
    // 0 the pen, 1 the eraser
    u8 brush;
    u8 on;
} TrainerCardPen;

typedef struct {
    u32 palette;
    u32 chars;
    u32 cellAnims;
} TrainerCardObjRes;

// Black 2's; matching code confirms the offsets of heapId, unit, actors and res in Black
typedef struct {
    u16 heapId;
    ClActUnit *unit;
    u8 unk08[0x90];
    ClActor *actors[TRCARD_ACTOR_COUNT];
    // The card's own, and the touch bar's common ones
    TrainerCardObjRes res[2];
} TrainerCardObjWork;

// Black 2's; matching code confirms the offsets of heapId, param, data, signPixels, charRes, charData, side, scaleX,
// scaleY, centerX, centerY, offsetX, offsetY, zoomSide, zoomed, touchMode, obj, scrRes, scrData, palRes, palData,
// palFrame, font, msgData, wordSet, numBuf, colonBuf, nameBuf, tmpBuf, unk3234, unk3240, labels, formats, blinkCount,
// blinkMode, affineDirty, pen, penNew, brush and penTouch in Black
typedef struct {
    u16 heapId;
    BmpWin *win[TRCARD_WIN_COUNT];
    TrainerCardParam *param;
    // The card shown
    TrainerCardData *data;
    u8 signPixels[TRCARD_SIGN_PIXELS];
    void *charRes;
    NNSG2dCharacterData *charData;
    // 0 the front, 1 the back
    int side;
    u8 unk303C[4];
    // The card's affine BGs: the scale while it flips or zooms, and its center
    fx32 scaleX;
    fx32 scaleY;
    u8 unk3048[4];
    s32 flipAngle;
    int centerX;
    int centerY;
    int offsetX;
    int offsetY;
    // Which half of the zoomed signature shows, and whether it is zoomed
    u16 zoomSide;
    u16 zoomed;
    BOOL touchMode;
    u8 unk3068[0xc];
    TrainerCardObjWork obj;
    u8 unk315C[0x84];
    u32 unk31E0;
    u8 unk31E4[0x10];
    void *scrRes;
    NNSG2dScreenData *scrData;
    u8 unk31FC[8];
    void *palRes;
    NNSG2dPaletteData *palData;
    int palFrame;
    TCB *vblankTask;
    TCBExManager *tcbl;
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *numBuf;
    StrBuf *colonBuf;
    StrBuf *nameBuf;
    StrBuf *tmpBuf;
    StrBuf *unk3234[3];
    StrBuf *unk3240[2];
    StrBuf *labels[26];
    StrBuf *formats[5];
    u8 unk32CC[8];
    PMSDraw *pmsDraw;
    u8 badges[16];
    u8 unk32E8;
    u8 unk32E9;
    u8 subSeq;
    // The eraser button's blinking while the eraser is chosen
    u8 blinkCount;
    u8 blinkMode;
    u8 unk32ED;
    u8 isCleared : 1;
    u8 isOnline : 1;
    u8 affineDirty : 1;
    PrintQueue *printQueue;
    u8 unk32F4[4];
    u8 frameType;
    u8 msgSpeed;
    u8 frame;
    u32 touchX;
    u32 touchY;
    u8 unk3304[2];
    // The back's list: how far it is scrolled (0 or less), where it was last frame, and where the touch that drags it
    // started
    s16 scroll;
    s16 oldScroll;
    s16 dragY;
    s16 dragScroll;
    TrainerCardPen pen;
    TrainerCardPen penNew;
    TrainerCardPen penOld;
    u8 brush;
    u8 scrolled;
    u8 unk331C;
    u8 seWait;
    u8 rows[TRCARD_ROW_MAX];
    u8 rowCount;
    u8 scrollTouch;
    u8 penTouch;
    // Black's ends here (0x3310 bytes, func_ov111_021c3cc0): it has none of Black 2's rank fields
} TrainerCardWork;

// pokebw2's TrCardObj_SetAnime
void func_ov111_021c6fa0(TrainerCardObjWork *obj, int actor, u8 sequence, u8 visible);

// Black's value, from func_ov111_021c8078
#define SHORTCUT_TRAINER_CARD_BADGES 23

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's TrCardObj_Exit
void func_ov111_021c6f4c(TrainerCardObjWork *obj);
// pokebw2's TrCardBmp_Exit
void func_ov111_021c60a0(TrainerCardWork *wk);
// pokebw2's TrCardBmp_DrawFront
void func_ov111_021c60cc(TrainerCardWork *wk, BmpWin **win, const TrainerCardData *data);
// pokebw2's TrCardBmp_Init
void func_ov111_021c6000(TrainerCardWork *wk);
// pokebw2's TrCardBmp_InitRows, without the player's own card
void func_ov111_021c6804(TrainerCardWork *wk, TrainerCardData *data);
// pokebw2's TrCardBmp_Stub
void func_ov111_021c6d98(TrainerCardWork *wk, BmpWin *win, int a2, StrBuf *str);
// Redraws the union room appearance's window and the personality's
void func_ov111_021c61ec(TrainerCardWork *wk, int view, BOOL flush);
void func_ov111_021c628c(TrainerCardWork *wk, u32 personality, BOOL flush);
// pokebw2's TrCardBmp_Flush
void func_ov111_021c631c(TrainerCardWork *wk, BmpWin **win);
// pokebw2's TrCardBmp_DrawBack
void func_ov111_021c636c(TrainerCardWork *wk, BmpWin **win, TrainerCardData *data);
// pokebw2's TrCardBmp_DrawList
void func_ov111_021c6874(TrainerCardWork *wk, int scroll, BOOL flush, int color);
// pokebw2's TrainerCard_Init
// trainer_card.c
BOOL func_ov111_021c3cc0(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's TrainerCard_Main
BOOL func_ov111_021c3f54(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's TrainerCard_Exit
BOOL func_ov111_021c40dc(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's TrCardObj_Init
// trcard_obj.c
void func_ov111_021c6d9c(TrainerCardObjWork *obj, const BGSysVRAMConfig *vram);
// pokebw2's TrCardObj_AddActors
void func_ov111_021c6e8c(TrainerCardObjWork *obj);

#endif // BWPORT_APP_TRAINER_CARD_H
