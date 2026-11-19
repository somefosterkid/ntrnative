#ifndef POKEHEARTGOLD_OVERWORLD_ANIM_MANAGER_H
#define POKEHEARTGOLD_OVERWORLD_ANIM_MANAGER_H

#include "global.h"

#include "constants/heap.h"

struct SysTask;

// The DS context area is 0x80 bytes. Renderer contexts hold pointers, so the area grows with the
// pointer width (0x100 bytes with 8-byte pointers) and the manager is never indexed with a literal size.
#define OVERWORLD_ANIM_MANAGER_FUNCS_CONTEXT_SIZE (0x80 * sizeof(void *) / 4)

typedef struct OverworldAnimManager OverworldAnimManager;

typedef BOOL (*OverworldAnimManager_InitFunc)(OverworldAnimManager *manager, void *ctx);
typedef void (*OverworldAnimManager_ExitFunc)(OverworldAnimManager *manager, void *ctx);
typedef void (*OverworldAnimManager_TickFunc)(OverworldAnimManager *manager, void *ctx);
typedef void (*OverworldAnimManager_RenderFunc)(OverworldAnimManager *manager, void *ctx);
typedef void (*OverworldAnimManager_Unused1Func)(OverworldAnimManager *manager, void *ctx);
typedef void (*OverworldAnimManager_Unused2Func)(OverworldAnimManager *manager, void *ctx);

typedef struct OverworldAnimManagerFuncs {
    int ctxSize;
    OverworldAnimManager_InitFunc initFunc;
    OverworldAnimManager_ExitFunc exitFunc;
    OverworldAnimManager_TickFunc tickFunc;
    OverworldAnimManager_RenderFunc renderFunc;
} OverworldAnimManagerFuncs;

// A renderer's funcs table whose context is a ctxType (the callbacks take it as their second argument). The DS asm
// tables hold the 32-bit size of the type as a literal, which is too small once the type holds pointers; the C tables
// (PC) take it from the type. ctxType must fit OVERWORLD_ANIM_MANAGER_FUNCS_CONTEXT_SIZE.
#define OVERWORLD_ANIM_MANAGER_FUNCS(ctxType, init, exit, tick, render) \
    { sizeof(ctxType), (OverworldAnimManager_InitFunc)(init), (OverworldAnimManager_ExitFunc)(exit), (OverworldAnimManager_TickFunc)(tick), (OverworldAnimManager_RenderFunc)(render) }

typedef struct OverworldAnimManagerExtendedFuncs {
    OverworldAnimManagerFuncs baseFuncs;
    // always the dummy functions, never called
    OverworldAnimManager_Unused1Func unused1Func;
    OverworldAnimManager_Unused2Func unused2Func;
} OverworldAnimManagerExtendedFuncs;

typedef struct OverworldAnimManagerList {
    int count;
    int activeCount;
    enum HeapID heapID;
    OverworldAnimManager *managers;
} OverworldAnimManagerList;

enum OverworldAnimManagerFlags {
    OVERWORLD_ANIM_MANAGER_ACTIVE_FLAG = 1 << 0,
    OVERWORLD_ANIM_MANAGER_TICKING_FLAG = 1 << 1,
};

// What the renderers' init/tick/render/exit functions receive as their context, sized for the
// pointers it holds (see OVERWORLD_ANIM_MANAGER_FUNCS_CONTEXT_SIZE).
typedef union OverworldAnimManagerFuncsContext {
    u8 bytes[OVERWORLD_ANIM_MANAGER_FUNCS_CONTEXT_SIZE];
    void *align;
} OverworldAnimManagerFuncsContext;

// sizeof(OverworldAnimManager) == 0xC8 on the DS
struct OverworldAnimManager {
    u32 flags;
    int userInt;
    int unk_08;
    const void *userData;
    struct SysTask *tickSysTask;
    OverworldAnimManagerList *list;
    int dummy18;
    int dummy1C;
    int dummy20;
    VecFx32 position;
    OverworldAnimManagerFuncsContext funcsCtx;
    OverworldAnimManager_InitFunc initFunc;
    OverworldAnimManager_TickFunc tickFunc;
    OverworldAnimManager_RenderFunc renderFunc;
    OverworldAnimManager_ExitFunc exitFunc;
    OverworldAnimManager_Unused1Func unused1Func;
    OverworldAnimManager_Unused2Func unused2Func;
};

OverworldAnimManagerList *sub_020689C8(enum HeapID heapID, int managerCount);
void sub_020689F8(OverworldAnimManagerList *managerList);
OverworldAnimManager *sub_02068A08(OverworldAnimManagerList *managerList, const OverworldAnimManagerExtendedFuncs *funcs, const VecFx32 *initialPos, int userInt, const void *userData, int sysTaskPriority);
OverworldAnimManager *sub_02068B0C(OverworldAnimManagerList *managerList, const OverworldAnimManagerFuncs *funcs, const VecFx32 *initialPos, int userInt, const void *userData, int sysTaskPriority);
void sub_02068B48(OverworldAnimManager *manager);
void sub_02068B80(OverworldAnimManagerList *managerList);
void sub_02068BAC(OverworldAnimManagerList *managerList);
struct SysTask *sub_02068BDC(OverworldAnimManager *manager, int priority);
void sub_02068BFC(struct SysTask *sysTask, void *manager);
OverworldAnimManagerList *sub_02068C08(enum HeapID heapID);
void sub_02068C2C(OverworldAnimManagerList *managerList);
OverworldAnimManager *sub_02068C38(enum HeapID heapID, int managerCount);
void sub_02068C5C(OverworldAnimManagerList *managerList);
void sub_02068C6C(OverworldAnimManager *manager);
int sub_02068CC4(const OverworldAnimManagerList *managerList);
OverworldAnimManager *sub_02068CC8(const OverworldAnimManagerList *managerList);
BOOL sub_02068CCC(const OverworldAnimManager *manager);
void sub_02068CD8(OverworldAnimManager *manager);
void sub_02068CE4(OverworldAnimManager *manager, u32 mask);
u32 sub_02068CEC(const OverworldAnimManager *manager, u32 mask);
void sub_02068CF4(OverworldAnimManager *manager, OverworldAnimManager_InitFunc initFunc);
BOOL sub_02068CFC(OverworldAnimManager *manager);
void sub_02068D10(OverworldAnimManager *manager, OverworldAnimManager_TickFunc tickFunc);
void sub_02068D18(OverworldAnimManager *manager);
void sub_02068D2C(OverworldAnimManager *manager, OverworldAnimManager_RenderFunc renderFunc);
void sub_02068D34(OverworldAnimManager *manager);
void sub_02068D48(OverworldAnimManager *manager, OverworldAnimManager_ExitFunc exitFunc);
void sub_02068D50(OverworldAnimManager *manager);
void sub_02068D64(OverworldAnimManager *manager, OverworldAnimManager_Unused1Func unused1Func);
void sub_02068D6C(OverworldAnimManager *manager, OverworldAnimManager_Unused2Func unused2Func);
void *sub_02068D74(OverworldAnimManager *manager);
void sub_02068D78(OverworldAnimManager *manager, int ctxSize);
void sub_02068D8C(OverworldAnimManager *manager, int userInt);
int sub_02068D90(const OverworldAnimManager *manager);
void sub_02068D94(OverworldAnimManager *manager, const void *userData);
const void *sub_02068D98(const OverworldAnimManager *manager);
void sub_02068D9C(OverworldAnimManager *manager, struct SysTask *sysTask);
struct SysTask *sub_02068DA0(const OverworldAnimManager *manager);
void sub_02068DA4(OverworldAnimManager *manager, OverworldAnimManagerList *managerList);
void sub_02068DA8(OverworldAnimManager *manager, const VecFx32 *position);
void sub_02068DB8(const OverworldAnimManager *manager, VecFx32 *position);
BOOL sub_02068DC8(OverworldAnimManager *manager, void *ctx);
void sub_02068DCC(OverworldAnimManager *manager, void *ctx);
void sub_02068DD0(OverworldAnimManager *manager, void *ctx);
void sub_02068DD4(OverworldAnimManager *manager, void *ctx);
void sub_02068DD8(OverworldAnimManager *manager, void *ctx);
void sub_02068DDC(OverworldAnimManager *manager, void *ctx);

#endif // POKEHEARTGOLD_OVERWORLD_ANIM_MANAGER_H
