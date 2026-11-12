// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_script.h.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_SCRIPT_H
#define BWPORT_APP_MUSICAL_STA_ACT_SCRIPT_H

// Overlay 132's sta_act_script.c: runs the program's scripts, up to 10 at once, each in a VM with the commands of
// script_command.c, and the tasks the commands start. Type and field names are pokebw2's

#include "types.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nnsys/fnd.h"
#include "struct_decls.h"
#include "system/vm.h"

#define STA_SCRIPT_MAX 10

// StaActScript.flags
#define STA_SCRIPT_FLAG_END 0x1
// Runs in step with the other synced scripts, waiting for them at STA_SCRIPT_FLAG_WAIT_SYNC
#define STA_SCRIPT_FLAG_SYNC 0x2
#define STA_SCRIPT_FLAG_WAIT_SYNC 0x4
// The script owns its file, which is freed with it
#define STA_SCRIPT_FLAG_OWN_FILE 0x8

typedef struct {
    StaActScriptSys *sys;
    VM *vm;
    const void *data;
    void *file;
    // Frames the script has run
    u32 frame;
    // Frames until the script runs again
    u16 wait;
    u16 flags;
    // The Pokémon the script moves
    u8 pokeNo;
    u8 index;
} StaActScript;

// A task a script command started, and its work, which is freed with it
typedef struct {
    NNSFndLink link;
    void *work;
    TCB *tcb;
} StaActScriptTask;

struct StaActScriptSys {
    HeapID heapId;
    TCBManager *tcbMgr;
    void *tcbWork;
    StaActScript *scripts[STA_SCRIPT_MAX];
    StaActing *stage;
    NNSFndList tasks;
};

// pokebw2's StaActScript_InitSystem, StaActScript_TermSystem, StaActScript_UpdateSystem, StaActScript_CreateScript,
// StaActScript_CreateScriptFile, StaActScript_GetScriptNum, StaActScript_AddTask and StaActScript_DelTask
StaActScriptSys *func_ov132_021faaf4(HeapID heapId, StaActing *stage);
void func_ov132_021fab58(StaActScriptSys *sys);
void func_ov132_021fabd8(StaActScriptSys *sys);
StaActScript *func_ov132_021fac74(StaActScriptSys *sys, const void *data, BOOL sync);
// Runs the script at an index of a file of scripts, a table of their offsets, which the script frees
void func_ov132_021fad0c(StaActScriptSys *sys, u32 *file, u32 index, u8 pokeNo);
u8 func_ov132_021fad7c(StaActScriptSys *sys);
StaActScriptTask *func_ov132_021fada0(StaActScriptSys *sys, TCBFunc func, void *work, u32 priority);
void func_ov132_021fadf8(StaActScriptSys *sys, StaActScriptTask *task);

// script_command.c's commands (pokebw2's STA_SCRIPT_COMMANDS)
extern VMCommand STA_SCRIPT_COMMANDS[];
#define STA_SCRIPT_COMMAND_COUNT 54

#endif // BWPORT_APP_MUSICAL_STA_ACT_SCRIPT_H
