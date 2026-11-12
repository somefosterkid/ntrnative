// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/circle_graph.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/circle_graph.h"
#include "app/research_radar/queue.h"
#include "gfl/heap.h"
#include "math.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nnsys/g3d.h"

#define PI 3.1415927f

#define ITEM_MAX 20
// The points of the rim, one per percent
#define POINT_COUNT 100
// Each percent is a triangle from the centre to two points of the rim
#define VERTEX_COUNT (POINT_COUNT * 3)

// The animations of the graph, which run one after another from a queue
enum {
    STATE_NONE,
    STATE_GROW_SLOW,
    STATE_GROW_FAST,
    STATE_SHRINK,
    STATE_FULL,
    STATE_GROW,
};

// The frames of the animations
#define GROW_SLOW_FRAMES 120
#define GROW_FAST_FRAMES 30
#define SHRINK_FRAMES 20
#define GROW_FRAMES 60

typedef struct {
    u8 id;
    u32 count;
    u8 percent;
    // The first and last percents of the slice
    u8 start;
    u8 end;
    u8 color[3];
    u8 centerColor[3];
} CircleGraphItem;

typedef struct {
    VecFx16 pos;
    GXRgb color;
    u8 polygonId;
} CircleGraphVertex;

struct CircleGraph {
    HeapID heapId;
    BOOL visible;
    u32 state;
    Queue *queue;
    u32 timer;
    // While waiting, the next animation doesn't start
    BOOL waiting;
    u32 waitFrames;
    CircleGraphItem items[ITEM_MAX];
    u8 itemCount;
    fx32 radius;
    // The centre
    VecFx16 pos;
    VecFx16 points[POINT_COUNT];
    CircleGraphVertex vertices[VERTEX_COUNT];
};

static void func_ov217_021c1194(CircleGraph *graph, const VecFx16 *pos);
static void func_ov217_021c124c(CircleGraph *graph);
static void func_ov217_021c13e8(CircleGraph *graph);
static void func_ov217_021c14dc(CircleGraph *graph);
static void func_ov217_021c15b8(CircleGraph *graph);
static void func_ov217_021c1610(CircleGraph *graph);
static void func_ov217_021c1628(CircleGraph *graph);
static void func_ov217_021c1638(CircleGraph *graph);
static void func_ov217_021c1648(CircleGraph *graph);
static void func_ov217_021c1658(CircleGraph *graph);
static void func_ov217_021c1670(CircleGraph *graph);
static void func_ov217_021c1680(CircleGraph *graph);
static void func_ov217_021c16c4(CircleGraph *graph);
static void func_ov217_021c16dc(CircleGraph *graph, u32 state);
static void func_ov217_021c16e8(CircleGraph *graph);
static void func_ov217_021c16fc(CircleGraph *graph, u32 state);
static void func_ov217_021c1704(CircleGraph *graph);
static void func_ov217_021c1720(CircleGraph *graph, const CircleGraphData *data);
static void func_ov217_021c17c4(CircleGraph *graph);
static void func_ov217_021c1850(CircleGraph *graph);
static void func_ov217_021c18c4(CircleGraph *graph);
static void func_ov217_021c18f8(CircleGraph *graph);
static void func_ov217_021c1a58(CircleGraph *graph, BOOL visible);
static void func_ov217_021c1a5c(CircleGraph *graph, u32 frames);
static void func_ov217_021c1a64(CircleGraph *graph, const VecFx16 *pos);
static u8 func_ov217_021c1a84(CircleGraph *graph, u8 id);
static CircleGraphItem *func_ov217_021c1ab4(CircleGraph *graph, int index);
static CircleGraphItem *func_ov217_021c1ac0(CircleGraph *graph, u8 id);
static u32 func_ov217_021c1ad4(CircleGraph *graph);
static int func_ov217_021c1afc(CircleGraph *graph);
static void func_ov217_021c1b28(const CircleGraphItem *item, u8 *start, u8 *end);
static GXRgb func_ov217_021c1b4c(const CircleGraphItem *item);
static GXRgb func_ov217_021c1b60(const CircleGraphItem *item);
static void func_ov217_021c1b74(CircleGraph *graph, const CircleGraphItem *item, VecFx16 *pos);
static BOOL func_ov217_021c1d24(CircleGraph *graph);
static BOOL func_ov217_021c1d28(CircleGraph *graph);
static void func_ov217_021c1d64(CircleGraph *graph);
static void func_ov217_021c1d74(CircleGraph *graph);
static void func_ov217_021c1d7c(CircleGraph *graph, HeapID heapId);
static CircleGraph *func_ov217_021c1dbc(HeapID heapId);
static void func_ov217_021c1de8(CircleGraph *graph);
static void func_ov217_021c1df0(CircleGraph *graph);
static void func_ov217_021c1e00(CircleGraph *graph);
static void func_ov217_021c1e0c(CircleGraph *graph);
static void func_ov217_021c1f44(const VecFx16 *pos, int *x, int *y);

// pokebw2's CircleGraph_Create
CircleGraph *func_ov217_021c10c8(HeapID heapId) {
    CircleGraph *graph = func_ov217_021c1dbc(heapId);

    func_ov217_021c1d64(graph);
    return graph;
}

// pokebw2's CircleGraph_Delete
void func_ov217_021c10d8(CircleGraph *graph) {
    func_ov217_021c1d74(graph);
    func_ov217_021c1de8(graph);
}

// pokebw2's CircleGraph_SetData
void func_ov217_021c10e8(CircleGraph *graph, const CircleGraphData *data, int num) {
    int i;

    func_ov217_021c1704(graph);
    for (i = 0; i < num; i++) {
        func_ov217_021c1720(graph, &data[i]);
    }
    func_ov217_021c17c4(graph);
    func_ov217_021c1850(graph);
    func_ov217_021c18c4(graph);
    func_ov217_021c18f8(graph);
}

// pokebw2's CircleGraph_Main
void func_ov217_021c1124(CircleGraph *graph) {
    func_ov217_021c15b8(graph);
}

// pokebw2's CircleGraph_Draw
void func_ov217_021c112c(CircleGraph *graph) {
    if (func_ov217_021c1d24(graph) == TRUE && graph->state != STATE_NONE) {
        func_ov217_021c14dc(graph);
        func_ov217_021c124c(graph);
        func_ov217_021c13e8(graph);
    }
}

// pokebw2's CircleGraph_RequestGrowSlow
void func_ov217_021c1154(CircleGraph *graph) {
    func_ov217_021c16dc(graph, STATE_GROW_SLOW);
}

// pokebw2's CircleGraph_RequestGrowFast
void func_ov217_021c1160(CircleGraph *graph) {
    func_ov217_021c16dc(graph, STATE_GROW_FAST);
}

// pokebw2's CircleGraph_RequestShrink
void func_ov217_021c116c(CircleGraph *graph) {
    func_ov217_021c16dc(graph, STATE_SHRINK);
}

// pokebw2's CircleGraph_RequestGrow
void func_ov217_021c1178(CircleGraph *graph) {
    func_ov217_021c16dc(graph, STATE_GROW);
}

// pokebw2's CircleGraph_SetVisible
void func_ov217_021c1184(CircleGraph *graph, BOOL visible) {
    func_ov217_021c1a58(graph, visible);
}

// pokebw2's CircleGraph_SetWait
void func_ov217_021c118c(CircleGraph *graph, u32 frames) {
    func_ov217_021c1a5c(graph, frames);
}

// pokebw2's CircleGraph_SetPos
static void func_ov217_021c1194(CircleGraph *graph, const VecFx16 *pos) {
    func_ov217_021c1a64(graph, pos);
    func_ov217_021c18f8(graph);
}

// pokebw2's CircleGraph_SetDepth
void func_ov217_021c11a4(CircleGraph *graph, fx16 z) {
    VecFx16 pos;

    VEC_Fx16Set(&pos, graph->pos.x, graph->pos.y, z);
    func_ov217_021c1194(graph, &pos);
}

// pokebw2's CircleGraph_GetItemIndex
u8 func_ov217_021c11c8(CircleGraph *graph, u8 id) {
    return func_ov217_021c1a84(graph, id);
}

// pokebw2's CircleGraph_GetItemId
u8 func_ov217_021c11d0(CircleGraph *graph, u8 index) {
    return func_ov217_021c1ab4(graph, index)->id;
}

// pokebw2's CircleGraph_GetPercentById
u8 func_ov217_021c11dc(CircleGraph *graph, u8 id) {
    return func_ov217_021c1ac0(graph, id)->percent;
}

// pokebw2's CircleGraph_GetPercent
u8 func_ov217_021c11e8(CircleGraph *graph, u8 index) {
    return func_ov217_021c1ab4(graph, index)->percent;
}

// pokebw2's CircleGraph_IsMoving
BOOL func_ov217_021c11f4(CircleGraph *graph) {
    return func_ov217_021c1d28(graph);
}

// pokebw2's CircleGraph_GetLabelScreenPosById
void func_ov217_021c11fc(CircleGraph *graph, u8 id, int *x, int *y) {
    VecFx16 pos;

    func_ov217_021c1b74(graph, func_ov217_021c1ac0(graph, id), &pos);
    func_ov217_021c1f44(&pos, x, y);
}

// pokebw2's CircleGraph_GetLabelScreenPos
void func_ov217_021c1224(CircleGraph *graph, u8 index, int *x, int *y) {
    VecFx16 pos;

    func_ov217_021c1b74(graph, func_ov217_021c1ab4(graph, index), &pos);
    func_ov217_021c1f44(&pos, x, y);
}

// Draws the slices' triangles shown so far, which end at 100 percent and grow backwards from it
// pokebw2's CircleGraph_DrawSlices
static void func_ov217_021c124c(CircleGraph *graph) {
    int num;
    int start;
    int vtx;
    int i;

    switch (graph->state) {
    case STATE_NONE:
        num = 0;
        break;
    case STATE_GROW_SLOW:
        num = graph->timer * 100 / GROW_SLOW_FRAMES;
        start = 100 - num;
        break;
    case STATE_GROW_FAST:
        num = graph->timer * 100 / GROW_FAST_FRAMES;
        start = 100 - num;
        break;
    case STATE_SHRINK:
        num = (SHRINK_FRAMES - graph->timer) * 100 / SHRINK_FRAMES;
        start = 100 - num;
        break;
    case STATE_FULL:
        num = 100;
        start = 0;
        break;
    case STATE_GROW:
        num = graph->timer * 100 / GROW_FRAMES;
        start = 100 - num;
        break;
    }

    // start isn't set in STATE_NONE, where num is 0 and nothing is drawn
    vtx = start * 3;
    for (i = 0; i < num; i++) {
        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, graph->vertices[vtx].polygonId, 31, 0);
        G3_Begin(GX_BEGIN_TRIANGLES);
        G3_Color(graph->vertices[vtx].color);
        G3_Vtx(graph->vertices[vtx].pos.x, graph->vertices[vtx].pos.y, graph->vertices[vtx].pos.z);
        vtx = (vtx + 1) % VERTEX_COUNT;
        G3_Color(graph->vertices[vtx].color);
        G3_Vtx(graph->vertices[vtx].pos.x, graph->vertices[vtx].pos.y, graph->vertices[vtx].pos.z);
        vtx = (vtx + 1) % VERTEX_COUNT;
        G3_Color(graph->vertices[vtx].color);
        G3_Vtx(graph->vertices[vtx].pos.x, graph->vertices[vtx].pos.y, graph->vertices[vtx].pos.z);
        vtx = (vtx + 1) % VERTEX_COUNT;
        G3_End();
    }
}

// Draws a black line from the centre to the start of each slice of at least 3 percent, once the graph is still
// pokebw2's CircleGraph_DrawBorders
static void func_ov217_021c13e8(CircleGraph *graph) {
    int i;
    int itemCount;
    CircleGraphItem *item;
    u8 start;
    u8 end;
    fx16 centerX;
    fx16 centerY;
    fx16 x;
    fx16 y;

    if (func_ov217_021c1d28(graph) == FALSE) {
        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
        itemCount = graph->itemCount;
        for (i = 0; i < itemCount; i++) {
            item = func_ov217_021c1ab4(graph, i);
            func_ov217_021c1b28(item, &start, &end);
            if (item->percent >= 3) {
                centerX = graph->pos.x;
                centerY = graph->pos.y;
                x = centerX + graph->points[start].x;
                y = centerY + graph->points[start].y;
                G3_Begin(GX_BEGIN_TRIANGLES);
                G3_Color(GX_RGB(0, 0, 0));
                G3_Vtx(centerX, centerY, FX16_CONST(4.0));
                G3_Vtx(centerX, centerY, FX16_CONST(4.0));
                G3_Vtx(x, y, FX16_CONST(4.0));
                G3_End();
            }
        }
    }
}

// pokebw2's CircleGraph_SetCamera
static void func_ov217_021c14dc(CircleGraph *graph) {
    VecFx32 camPos;
    VecFx32 camUp;
    VecFx32 target;

    VEC_Set(&camPos, 0, 0, FX32_ONE);
    VEC_Set(&camUp, 0, FX32_ONE, 0);
    VEC_Set(&target, 0, 0, 0);

    G3i_LookAt_(&camPos, &camUp, &target, TRUE, NULL);
    G3i_OrthoW_(FX32_ONE, -FX32_ONE, FX32_CONST(-1.333), FX32_CONST(1.333), FX32_CONST(0.1), FX32_CONST(5.0), FX32_ONE,
             TRUE, NULL);
    NNS_G3dGlbInit();
    NNS_G3dGlbLookAt(&camPos, &camUp, &target);
    NNS_G3dGlbOrtho(FX32_ONE, -FX32_ONE, FX32_CONST(-1.333), FX32_CONST(1.333), FX32_CONST(0.1), FX32_CONST(5.0));
    NNS_G3dGlbFlush();
}

// pokebw2's CircleGraph_Update
static void func_ov217_021c15b8(CircleGraph *graph) {
    if (graph->waiting == FALSE) {
        switch (graph->state) {
        case STATE_NONE:
            func_ov217_021c1610(graph);
            break;
        case STATE_GROW_SLOW:
            func_ov217_021c1628(graph);
            break;
        case STATE_GROW_FAST:
            func_ov217_021c1638(graph);
            break;
        case STATE_SHRINK:
            func_ov217_021c1648(graph);
            break;
        case STATE_FULL:
            func_ov217_021c1658(graph);
            break;
        case STATE_GROW:
            func_ov217_021c1670(graph);
            break;
        }
    }
    func_ov217_021c1680(graph);
    func_ov217_021c16c4(graph);
}

// pokebw2's CircleGraph_UpdateNone
static void func_ov217_021c1610(CircleGraph *graph) {
    if (func_ov217_021c0e64(graph->queue) == FALSE) {
        func_ov217_021c16e8(graph);
    }
}

// pokebw2's CircleGraph_UpdateGrowSlow
static void func_ov217_021c1628(CircleGraph *graph) {
    if (graph->timer >= GROW_SLOW_FRAMES) {
        func_ov217_021c16fc(graph, STATE_FULL);
    }
}

// pokebw2's CircleGraph_UpdateGrowFast
static void func_ov217_021c1638(CircleGraph *graph) {
    if (graph->timer >= GROW_FAST_FRAMES) {
        func_ov217_021c16fc(graph, STATE_FULL);
    }
}

// pokebw2's CircleGraph_UpdateShrink
static void func_ov217_021c1648(CircleGraph *graph) {
    if (graph->timer >= SHRINK_FRAMES) {
        func_ov217_021c16fc(graph, STATE_NONE);
    }
}

// pokebw2's CircleGraph_UpdateFull
static void func_ov217_021c1658(CircleGraph *graph) {
    if (func_ov217_021c0e64(graph->queue) == FALSE) {
        func_ov217_021c16e8(graph);
    }
}

// pokebw2's CircleGraph_UpdateGrow
static void func_ov217_021c1670(CircleGraph *graph) {
    if (graph->timer >= GROW_FRAMES) {
        func_ov217_021c16fc(graph, STATE_FULL);
    }
}

// Counts the state's frames, up to its length
// pokebw2's CircleGraph_CountFrame
static void func_ov217_021c1680(CircleGraph *graph) {
    u32 max;

    graph->timer++;
    switch (graph->state) {
    case STATE_NONE:
        max = 0xffffffff;
        break;
    case STATE_GROW_SLOW:
        max = GROW_SLOW_FRAMES;
        break;
    case STATE_GROW_FAST:
        max = GROW_FAST_FRAMES;
        break;
    case STATE_SHRINK:
        max = SHRINK_FRAMES;
        break;
    case STATE_FULL:
        max = 0xffffffff;
        break;
    case STATE_GROW:
        max = GROW_FRAMES;
        break;
    }
    if (max < graph->timer) {
        graph->timer = max;
    }
}

// pokebw2's CircleGraph_UpdateWait
static void func_ov217_021c16c4(CircleGraph *graph) {
    if (graph->waiting) {
        if (graph->waitFrames != 0) {
            graph->waitFrames--;
            if (graph->waitFrames == 0) {
                graph->waiting = FALSE;
            }
        }
    }
}

// pokebw2's CircleGraph_Request
static void func_ov217_021c16dc(CircleGraph *graph, u32 state) {
    func_ov217_021c0e4c(graph->queue, state);
}

// pokebw2's CircleGraph_StartNext
static void func_ov217_021c16e8(CircleGraph *graph) {
    func_ov217_021c16fc(graph, func_ov217_021c0e54(graph->queue));
}

// pokebw2's CircleGraph_SetState
static void func_ov217_021c16fc(CircleGraph *graph, u32 state) {
    graph->state = state;
    graph->timer = 0;
}

// pokebw2's CircleGraph_ClearItems
static void func_ov217_021c1704(CircleGraph *graph) {
    graph->itemCount = 0;
    func_ov217_021c0e5c(graph->queue);
    func_ov217_021c16fc(graph, STATE_NONE);
}

// pokebw2's CircleGraph_AddItem
static void func_ov217_021c1720(CircleGraph *graph, const CircleGraphData *data) {
    graph->items[graph->itemCount].id = data->id;
    graph->items[graph->itemCount].count = data->count;
    graph->items[graph->itemCount].percent = 0;
    graph->items[graph->itemCount].start = 0;
    graph->items[graph->itemCount].end = 0;
    graph->items[graph->itemCount].color[0] = data->color[0];
    graph->items[graph->itemCount].color[1] = data->color[1];
    graph->items[graph->itemCount].color[2] = data->color[2];
    graph->items[graph->itemCount].centerColor[0] = data->centerColor[0];
    graph->items[graph->itemCount].centerColor[1] = data->centerColor[1];
    graph->items[graph->itemCount].centerColor[2] = data->centerColor[2];
    graph->itemCount++;
}

// Sorts the items by count, largest first
// pokebw2's CircleGraph_SortItems
static void func_ov217_021c17c4(CircleGraph *graph) {
    int i;
    int itemCount = graph->itemCount;
    int j;
    CircleGraphItem item;

    for (i = 0; i < itemCount - 1; i++) {
        for (j = 0; j < itemCount - 1 - i; j++) {
            if (graph->items[j].count < graph->items[j + 1].count) {
                item = graph->items[j];
                graph->items[j] = graph->items[j + 1];
                graph->items[j + 1] = item;
            }
        }
    }
}

// Gives each item its share in whole percents, and the percents lost to rounding down to the first items
// pokebw2's CircleGraph_CalcPercents
static void func_ov217_021c1850(CircleGraph *graph) {
    int i;
    u32 total = func_ov217_021c1ad4(graph);
    int itemCount = graph->itemCount;
    int rest;
    CircleGraphItem *item;

    for (i = 0; i < itemCount; i++) {
        item = &graph->items[i];
        item->percent = item->count * 100 / total;
    }

    rest = 100 - func_ov217_021c1afc(graph);
    i = 0;
    while (rest > 0) {
        graph->items[i].percent++;
        rest--;
        i = (i + 1) % itemCount;
    }
}

// pokebw2's CircleGraph_CalcRanges
static void func_ov217_021c18c4(CircleGraph *graph) {
    int i;
    int itemCount = graph->itemCount;
    int percent = 0;
    CircleGraphItem *item;

    for (i = 0; i < itemCount; i++) {
        item = &graph->items[i];
        item->start = percent;
        item->end = percent + item->percent - 1;
        percent += item->percent;
    }
}

// Builds the triangles of the slices, each slice a little closer to the camera than the one before
// pokebw2's CircleGraph_BuildVertices
static void func_ov217_021c18f8(CircleGraph *graph) {
    int itemCount = graph->itemCount;
    int vtx = 0;
    int i;
    int j;
    CircleGraphItem *item;
    GXRgb centerColor;
    GXRgb color;
    u8 start;
    u8 end;
    VecFx16 center;

    for (i = 0; i < itemCount; i++) {
        item = func_ov217_021c1ab4(graph, i);
        centerColor = func_ov217_021c1b60(item);
        color = func_ov217_021c1b4c(item);
        func_ov217_021c1b28(item, &start, &end);
        center.x = graph->pos.x;
        center.y = graph->pos.y;
        center.z = graph->pos.z + i * FX16_CONST(0.05);
        for (j = start; j <= end; j++) {
            graph->vertices[vtx].color = centerColor;
            graph->vertices[vtx].pos = center;
            graph->vertices[vtx].polygonId = i;
            graph->vertices[vtx + 1].color = color;
            VEC_Fx16Add(&center, &graph->points[j], &graph->vertices[vtx + 1].pos);
            graph->vertices[vtx + 1].polygonId = i;
            graph->vertices[vtx + 2].color = color;
            VEC_Fx16Add(&center, &graph->points[(j + 1) % POINT_COUNT], &graph->vertices[vtx + 2].pos);
            graph->vertices[vtx + 2].polygonId = i;
            vtx += 3;
        }
    }
}

// pokebw2's CircleGraph_SetVisibleCore
static void func_ov217_021c1a58(CircleGraph *graph, BOOL visible) {
    graph->visible = visible;
}

// pokebw2's CircleGraph_SetWaitCore
static void func_ov217_021c1a5c(CircleGraph *graph, u32 frames) {
    graph->waiting = TRUE;
    graph->waitFrames = frames;
}

// pokebw2's CircleGraph_SetPosCore
static void func_ov217_021c1a64(CircleGraph *graph, const VecFx16 *pos) {
    VEC_Fx16Set(&graph->pos, pos->x, pos->y, pos->z);
}

// pokebw2's CircleGraph_GetItemIndexCore
static u8 func_ov217_021c1a84(CircleGraph *graph, u8 id) {
    int i;
    int itemCount = graph->itemCount;

    for (i = 0; i < itemCount; i++) {
        u8 itemId = graph->items[i].id;

        if (itemId == id) {
            return i;
        }
    }
    return 0;
}

// pokebw2's CircleGraph_GetItem
static CircleGraphItem *func_ov217_021c1ab4(CircleGraph *graph, int index) {
    return &graph->items[index];
}

// pokebw2's CircleGraph_GetItemById
static CircleGraphItem *func_ov217_021c1ac0(CircleGraph *graph, u8 id) {
    return func_ov217_021c1ab4(graph, func_ov217_021c1a84(graph, id));
}

// pokebw2's CircleGraph_GetTotalCount
static u32 func_ov217_021c1ad4(CircleGraph *graph) {
    int i;
    int itemCount = graph->itemCount;
    u32 total = 0;

    for (i = 0; i < itemCount; i++) {
        total += graph->items[i].count;
    }
    return total;
}

// pokebw2's CircleGraph_GetPercentTotal
static int func_ov217_021c1afc(CircleGraph *graph) {
    int i;
    int itemCount = graph->itemCount;
    int total = 0;

    for (i = 0; i < itemCount; i++) {
        total += graph->items[i].percent;
    }
    return total;
}

// The range goes through a float, which changes nothing
// pokebw2's CircleGraph_GetItemRange
static void func_ov217_021c1b28(const CircleGraphItem *item, u8 *start, u8 *end) {
    int first = (f32)item->start;
    int last = (f32)item->end;

    *start = first;
    *end = last;
}

// pokebw2's CircleGraph_GetItemColor
static GXRgb func_ov217_021c1b4c(const CircleGraphItem *item) {
    return GX_RGB(item->color[0], item->color[1], item->color[2]);
}

// pokebw2's CircleGraph_GetItemCenterColor
static GXRgb func_ov217_021c1b60(const CircleGraphItem *item) {
    return GX_RGB(item->centerColor[0], item->centerColor[1], item->centerColor[2]);
}

// The position of an item's label, at the middle of its slice, inside the rim
// pokebw2's CircleGraph_GetLabelPos
static void func_ov217_021c1b74(CircleGraph *graph, const CircleGraphItem *item, VecFx16 *pos) {
    u8 start;
    u8 end;
    int first;
    int last;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    f32 centerX;
    f32 centerY;
    f32 centerZ;

    func_ov217_021c1b28(item, &start, &end);
    first = start;
    last = end;
    angle = PI / 2 - (first + last + 1) * PI * 0.01f;
    x = (f32)cos(angle) * 0.42f;
    y = (f32)sin(angle) * 0.42f;
    z = FX_FX16_TO_F32(graph->points[last].z);
    centerX = FX_FX16_TO_F32(graph->pos.x);
    centerY = FX_FX16_TO_F32(graph->pos.y);
    centerZ = FX_FX16_TO_F32(graph->pos.z);
    pos->x = FX_F32_TO_FX16(centerX + x);
    pos->y = FX_F32_TO_FX16(centerY + y);
    pos->z = FX_F32_TO_FX16(centerZ + z);
}

// pokebw2's CircleGraph_IsVisible
static BOOL func_ov217_021c1d24(CircleGraph *graph) {
    return graph->visible;
}

// pokebw2's CircleGraph_IsMovingCore
static BOOL func_ov217_021c1d28(CircleGraph *graph) {
    switch (graph->state) {
    case STATE_NONE:
        return FALSE;
    case STATE_GROW_SLOW:
        return TRUE;
    case STATE_GROW_FAST:
        return TRUE;
    case STATE_SHRINK:
        return TRUE;
    case STATE_FULL:
        return FALSE;
    case STATE_GROW:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CircleGraph_Init
static void func_ov217_021c1d64(CircleGraph *graph) {
    func_ov217_021c1df0(graph);
    func_ov217_021c1e0c(graph);
}

// pokebw2's CircleGraph_Exit
static void func_ov217_021c1d74(CircleGraph *graph) {
    func_ov217_021c1e00(graph);
}

// pokebw2's CircleGraph_InitWork
static void func_ov217_021c1d7c(CircleGraph *graph, HeapID heapId) {
    graph->heapId = heapId;
    graph->visible = FALSE;
    graph->state = STATE_NONE;
    graph->queue = NULL;
    graph->waiting = FALSE;
    graph->waitFrames = 0;
    graph->radius = FX32_CONST(0.491);
    graph->itemCount = 0;
    graph->pos.x = FX16_CONST(-0.699);
    graph->pos.y = FX16_CONST(-0.049);
    graph->pos.z = FX16_CONST(-1.0);
}

// pokebw2's CircleGraph_Alloc
static CircleGraph *func_ov217_021c1dbc(HeapID heapId) {
    CircleGraph *graph = GFL_HeapAllocate(heapId, sizeof(CircleGraph), FALSE, "circle_graph.c", 1588);

    func_ov217_021c1d7c(graph, heapId);
    return graph;
}

// pokebw2's CircleGraph_Free
static void func_ov217_021c1de8(CircleGraph *graph) {
    GFL_HeapFree(graph);
}

// pokebw2's CircleGraph_CreateQueue
static void func_ov217_021c1df0(CircleGraph *graph) {
    graph->queue = func_ov217_021c0e08(10, graph->heapId);
}

// pokebw2's CircleGraph_DeleteQueue
static void func_ov217_021c1e00(CircleGraph *graph) {
    func_ov217_021c0e3c(graph->queue);
}

// The points of the rim, clockwise from the top
// pokebw2's CircleGraph_InitPoints
static void func_ov217_021c1e0c(CircleGraph *graph) {
    int i;
    f32 radius = FX_FX32_TO_F32(graph->radius);
    f32 angle;
    f32 x;
    f32 y;
    f32 z = 0.0f;

    for (i = 0; i < POINT_COUNT; i++) {
        angle = PI / 2 - 2 * PI * i / POINT_COUNT;
        x = (f32)cos(angle) * radius;
        y = (f32)sin(angle) * radius;
        graph->points[i].x = FX_F32_TO_FX16(x);
        graph->points[i].y = FX_F32_TO_FX16(y);
        graph->points[i].z = FX_F32_TO_FX16(z);
    }
}

// pokebw2's CircleGraph_ToScreenPos
static void func_ov217_021c1f44(const VecFx16 *pos, int *x, int *y) {
    VecFx32 world;

    world.x = FX_F32_TO_FX32(FX_FX16_TO_F32(pos->x));
    world.y = FX_F32_TO_FX32(FX_FX16_TO_F32(pos->y));
    world.z = FX_F32_TO_FX32(FX_FX16_TO_F32(pos->z));
    NNS_G3dWorldPosToScrPos(&world, x, y);
}
