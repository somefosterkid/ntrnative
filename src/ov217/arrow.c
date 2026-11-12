// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/arrow.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/arrow.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

#define PIECE_MAX 26

// The length of a piece, in pixels
#define PIECE_SIZE 8

enum {
    STATE_WAIT,
    STATE_RUN,
    STATE_DONE,
};

// The kinds of piece, which pick their animation
enum {
    PIECE_ROW,
    PIECE_COLUMN,
    PIECE_CORNER,
    PIECE_HEAD,
};

typedef struct {
    BOOL shown;
    // The frame when the piece is shown
    u16 showFrame;
    // Whether the piece's animation stops, and when
    BOOL stops;
    u16 stopFrame;
    u8 x;
    u8 y;
    u32 kind;
    ClActor *actor;
} ArrowPiece;

struct Arrow {
    HeapID heapId;
    ClActUnit *unit;
    ArrowResources resources;
    u32 state;
    u32 frames;
    ArrowPiece pieces[PIECE_MAX];
    u8 pieceCount;
};

static void func_ov217_021c20ec(Arrow *arrow);
static void func_ov217_021c2118(Arrow *arrow);
static void func_ov217_021c211c(Arrow *arrow);
static void func_ov217_021c2188(Arrow *arrow);
static void func_ov217_021c218c(Arrow *arrow, u32 state);
static BOOL func_ov217_021c2194(Arrow *arrow);
static void func_ov217_021c21c4(Arrow *arrow, u8 index);
static void func_ov217_021c2250(Arrow *arrow, u8 index);
static void func_ov217_021c2260(Arrow *arrow);
static void func_ov217_021c2288(Arrow *arrow, int startX, int startY, int endX, int endY);
static void func_ov217_021c253c(ArrowPiece *piece, u8 x, u8 y, u32 frame, u32 kind);
static Arrow *func_ov217_021c2550(HeapID heapId);
static void func_ov217_021c256c(Arrow *arrow, HeapID heapId);
static void func_ov217_021c25a8(Arrow *arrow);
static void func_ov217_021c25b0(Arrow *arrow);
static void func_ov217_021c25c4(Arrow *arrow);
static void func_ov217_021c25d0(Arrow *arrow);
static void func_ov217_021c2614(Arrow *arrow);
static void func_ov217_021c2630(Arrow *arrow, const ArrowResources *resources);

// pokebw2's Arrow_Create
Arrow *func_ov217_021c2070(HeapID heapId, const ArrowResources *resources) {
    Arrow *arrow = func_ov217_021c2550(heapId);

    func_ov217_021c256c(arrow, heapId);
    func_ov217_021c2630(arrow, resources);
    func_ov217_021c25b0(arrow);
    func_ov217_021c25d0(arrow);
    return arrow;
}

// pokebw2's Arrow_Delete
void func_ov217_021c209c(Arrow *arrow) {
    func_ov217_021c2614(arrow);
    func_ov217_021c25c4(arrow);
    func_ov217_021c25a8(arrow);
}

// pokebw2's Arrow_SetPath
void func_ov217_021c20b4(Arrow *arrow, int startX, int startY, int endX, int endY) {
    func_ov217_021c2288(arrow, startX, startY, endX, endY);
}

// pokebw2's Arrow_Update
void func_ov217_021c20c4(Arrow *arrow) {
    func_ov217_021c20ec(arrow);
}

// pokebw2's Arrow_Start
void func_ov217_021c20cc(Arrow *arrow) {
    func_ov217_021c218c(arrow, STATE_RUN);
}

// pokebw2's Arrow_Hide
void func_ov217_021c20d8(Arrow *arrow) {
    func_ov217_021c218c(arrow, STATE_WAIT);
    func_ov217_021c2260(arrow);
}

// pokebw2's Arrow_UpdateCore
static void func_ov217_021c20ec(Arrow *arrow) {
    switch (arrow->state) {
    case STATE_WAIT:
        func_ov217_021c2118(arrow);
        break;
    case STATE_RUN:
        func_ov217_021c211c(arrow);
        break;
    case STATE_DONE:
        func_ov217_021c2188(arrow);
        break;
    }
    arrow->frames++;
}

// pokebw2's Arrow_StateWait
static void func_ov217_021c2118(Arrow *arrow) {
}

// pokebw2's Arrow_StateRun
static void func_ov217_021c211c(Arrow *arrow) {
    int i;

    for (i = 0; i < arrow->pieceCount; i++) {
        if (arrow->pieces[i].shown == FALSE && arrow->pieces[i].showFrame <= arrow->frames) {
            func_ov217_021c21c4(arrow, i);
        }
        if (arrow->pieces[i].shown == TRUE && arrow->pieces[i].stops == TRUE &&
            arrow->pieces[i].stopFrame <= arrow->frames) {
            func_ov217_021c2250(arrow, i);
        }
    }
    if (func_ov217_021c2194(arrow)) {
        func_ov217_021c218c(arrow, STATE_DONE);
    }
}

// pokebw2's Arrow_StateDone
static void func_ov217_021c2188(Arrow *arrow) {
}

// pokebw2's Arrow_SetState
static void func_ov217_021c218c(Arrow *arrow, u32 state) {
    arrow->state = state;
    arrow->frames = 0;
}

// pokebw2's Arrow_IsAllShown
static BOOL func_ov217_021c2194(Arrow *arrow) {
    int i;

    for (i = 0; i < arrow->pieceCount; i++) {
        if (arrow->pieces[i].shown == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's Arrow_ShowPiece
static void func_ov217_021c21c4(Arrow *arrow, u8 index) {
    ArrowPiece *piece = &arrow->pieces[index];
    ClActor *actor = piece->actor;
    ClActorPos pos;
    u16 sequence;

    pos.x = piece->x;
    pos.y = piece->y;
    piece->shown = TRUE;
    switch (piece->kind) {
    case PIECE_ROW:
        sequence = arrow->resources.sequences[PIECE_ROW];
        break;
    case PIECE_COLUMN:
        sequence = arrow->resources.sequences[PIECE_COLUMN];
        break;
    case PIECE_CORNER:
        sequence = arrow->resources.sequences[PIECE_CORNER];
        break;
    case PIECE_HEAD:
        sequence = arrow->resources.sequences[PIECE_HEAD];
        break;
    }
    func_0204b404(actor, &pos, arrow->resources.surface);
    func_0204b878(actor, sequence);
    func_0204b8c4(actor, 0);
    func_0204b9c4(actor, 1);
    func_0204b92c(actor, TRUE);
    func_0204b954(actor, FX32_CONST(18));
    func_0204b95c(actor);
    func_0204b3dc(actor, TRUE);
}

// pokebw2's Arrow_StopPiece
static void func_ov217_021c2250(Arrow *arrow, u8 index) {
    func_0204b974(arrow->pieces[index].actor);
}

// pokebw2's Arrow_HideAll
static void func_ov217_021c2260(Arrow *arrow) {
    int i;
    int count = arrow->pieceCount;

    for (i = 0; i < count; i++) {
        func_0204b3dc(arrow->pieces[i].actor, FALSE);
    }
}

// Each piece is shown a frame after the one before it. A row or column that isn't a whole number of pieces ends with a
// piece that overlaps the one before it, shown that much earlier
// pokebw2's Arrow_SetPathCore
static void func_ov217_021c2288(Arrow *arrow, int startX, int startY, int endX, int endY) {
    int rowY;
    int width;
    int rowPieces;
    int index;
    f32 frame;
    int i;
    int columnPieces;
    int height;
    int columnX;
    int y;

    frame = 0;
    index = 0;
    width = startX - endX;
    rowPieces = (width - 4) / PIECE_SIZE;
    rowY = startY - 4;
    startX -= PIECE_SIZE;
    for (i = 0; i < rowPieces; i++) {
        func_ov217_021c253c(&arrow->pieces[index], startX, rowY, frame, PIECE_ROW);
        startX -= PIECE_SIZE;
        frame += 1.0f;
        index++;
    }
    if (rowPieces == 0) {
        if (width - 4 > 0) {
            func_ov217_021c253c(&arrow->pieces[index], startX, rowY, frame, PIECE_ROW);
            if (width - 4 < 4) {
                arrow->pieces[index].stops = TRUE;
                arrow->pieces[index].stopFrame = 1;
            }
            frame += (width - 4) * 2.0f / PIECE_SIZE - 1.0f;
            index++;
        }
    } else if ((width - 4) % PIECE_SIZE != 0) {
        frame -= 2.0f - (width - 4 - rowPieces * PIECE_SIZE) * 2.0f / PIECE_SIZE;
        func_ov217_021c253c(&arrow->pieces[index], endX + 4, rowY, frame, PIECE_ROW);
        frame += 1.0f;
        index++;
    }

    func_ov217_021c253c(&arrow->pieces[index], endX - 4, startY - 4, frame, PIECE_CORNER);
    frame += 1.0f;
    index++;

    height = endY - startY;
    columnPieces = (height - 4) / PIECE_SIZE;
    columnX = endX - 4;
    y = startY + 4;
    for (i = 0; i < columnPieces; i++) {
        func_ov217_021c253c(&arrow->pieces[index], columnX, y, frame, PIECE_COLUMN);
        y += PIECE_SIZE;
        frame += 1.0f;
        index++;
    }
    if ((height - 4) % PIECE_SIZE != 0) {
        frame -= 2.0f - (height - 4 - columnPieces * PIECE_SIZE) * 2.0f / PIECE_SIZE;
        func_ov217_021c253c(&arrow->pieces[index], columnX, endY - PIECE_SIZE, frame, PIECE_COLUMN);
        frame += 1.0f;
        index++;
    }

    func_ov217_021c253c(&arrow->pieces[index], endX - 4, endY - 4, frame, PIECE_HEAD);
    arrow->pieceCount = index + 1;
}

// pokebw2's Arrow_InitPiece
static void func_ov217_021c253c(ArrowPiece *piece, u8 x, u8 y, u32 frame, u32 kind) {
    piece->x = x;
    piece->shown = FALSE;
    piece->stops = FALSE;
    piece->stopFrame = 0;
    piece->y = y;
    piece->showFrame = frame;
    piece->kind = kind;
}

// pokebw2's Arrow_Alloc
static Arrow *func_ov217_021c2550(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(Arrow), FALSE, "arrow.c", 558);
}

// pokebw2's Arrow_InitWork
static void func_ov217_021c256c(Arrow *arrow, HeapID heapId) {
    int i;

    arrow->heapId = heapId;
    arrow->state = STATE_WAIT;
    arrow->frames = 0;
    arrow->pieceCount = 0;
    arrow->unit = NULL;
    for (i = 0; i < PIECE_MAX; i++) {
        arrow->pieces[i].shown = FALSE;
        arrow->pieces[i].showFrame = 0;
        arrow->pieces[i].stops = FALSE;
        arrow->pieces[i].stopFrame = 0;
        arrow->pieces[i].x = 0;
        arrow->pieces[i].y = 0;
        arrow->pieces[i].actor = NULL;
    }
}

// pokebw2's Arrow_Free
static void func_ov217_021c25a8(Arrow *arrow) {
    GFL_HeapFree(arrow);
}

// pokebw2's Arrow_CreateUnit
static void func_ov217_021c25b0(Arrow *arrow) {
    arrow->unit = func_0204b100(PIECE_MAX, 0, arrow->heapId);
}

// pokebw2's Arrow_DeleteUnit
static void func_ov217_021c25c4(Arrow *arrow) {
    func_0204b1cc(arrow->unit);
}

// pokebw2's Arrow_CreateActors
static void func_ov217_021c25d0(Arrow *arrow) {
    ClActorSetup setup;
    ArrowResources *resources = &arrow->resources;
    int i;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    for (i = 0; i < PIECE_MAX; i++) {
        arrow->pieces[i].actor = func_0204b294(arrow->unit, resources->chars, resources->palette, resources->cellAnims,
                                               &setup, resources->surface, arrow->heapId);
    }
}

// pokebw2's Arrow_DeleteActors
static void func_ov217_021c2614(Arrow *arrow) {
    int i;

    for (i = 0; i < PIECE_MAX; i++) {
        func_0204b3b4(arrow->pieces[i].actor);
    }
}

// pokebw2's Arrow_SetResources
static void func_ov217_021c2630(Arrow *arrow, const ArrowResources *resources) {
    arrow->resources = *resources;
}
