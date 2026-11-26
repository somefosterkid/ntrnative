#ifndef POKEHEARTGOLD_POKEATHLON_H
#define POKEHEARTGOLD_POKEATHLON_H

#include "constants/heap.h"

#include "pokeathlon/pokeathlon_save.h"

#include "comm_cmd_manager.h"
#include "overlay_manager.h"
#include "sys_task.h"
#include "player_data.h"
#include "save.h"

typedef struct PokeathlonCourseData PokeathlonCourseData;

typedef struct PokeathlonCmdBuf {
    u8 *sendData;
    u8 *recvData;
    s32 size;
    u32 active;
} PokeathlonCmdBuf;

// Function pointer types for state management
typedef BOOL (*PokeathlonStateFunc)(PokeathlonCourseData *, u8 *);
typedef BOOL (*PokeathlonStateHandlerFunc)(PokeathlonCourseData *, int);

// Per-event handler table (0x14 bytes, in asm data): funcs[1] is run by ov96_021E67AC, funcs[2] (may be NULL) and
// funcs[3] are the setup/cleanup of the event, flag selects the data copy step of ov96_021E6F2C.
typedef struct PokeathlonEventHandlers {
    PokeathlonStateHandlerFunc funcs[4];
    s32 flag;
} PokeathlonEventHandlers;

// State info structure (8 bytes)
struct Sprite;
struct SysTask;

// One of the 3 sprite slots of the course data (ov96_021E64F8 fills it, ov96_021E81D8 is the task).
typedef struct PokeathlonCourseSpriteSlot {
    s32 active;           // 0x0
    void *unk_4;          // 0x4
    struct Sprite *sprite; // 0x8
    struct SysTask *task; // 0xC
} PokeathlonCourseSpriteSlot; // size 0x10

typedef struct PokeathlonStateInfo {
    const PokeathlonStateFunc *ptr; // 0x00 - Pointer to state function table
    u8 field_04;      // 0x04 - Field at offset 4 (initialized to 1)
    u8 stateArgsBase; // 0x05 - Base for state function args
    u8 stateIndex;    // 0x06 - State function index
    u8 field_07;      // 0x07 - Field at offset 7
} PokeathlonStateInfo;

// Five stat-table indices of one monster (ov96_021E60D8 returns a pointer to them).
typedef struct PokeathlonMonKinds {
    u8 statIdx[5];
} PokeathlonMonKinds;

// One monster of a participant (0x28 bytes, 3 per participant).
typedef struct PokeathlonMonEntry {
    u16 species;               // 0x00
    u16 unk_2;                 // 0x02
    s32 unk_4;                 // 0x04
    PokeathlonMonKinds kinds;  // 0x08
    u8 filler_D[3];            // 0x0D
    u8 unk_10;                 // 0x10
    u8 unk_11;                 // 0x11
    u8 filler_12[0x16];        // 0x12
} PokeathlonMonEntry;

// Participant data structure (0x7C = 124 bytes each)
typedef struct PokeathlonParticipantData {
    u8 field_00[0x04];
    union {
        u8 field_04;
        PokeathlonMonEntry mons[3]; // 0x04
    };
} PokeathlonParticipantData;

// 0x14-byte performance record read from NARC_poketool_personal_performance (ov96_021E604C).
typedef struct PokeathlonMonPerf {
    u8 unk_0[5];  // 0x00
    u8 unk_5;     // 0x05
    u8 unk_6;     // 0x06
    u8 unk_7;     // 0x07
    u8 unk_8;     // 0x08
    u8 unk_9[0xB]; // 0x09
} PokeathlonMonPerf;

// Hud kept in the course data by ov96_021E61D8 / ov96_021E637C / ov96_021E62AC (0x20 bytes).
typedef struct PokeathlonCourseHud {
    struct Sprite *unk_0; // 0x00
    u8 unk_4;             // 0x04
    u8 unk_5;             // 0x05
    s16 unk_6;            // 0x06
    struct Sprite *unk_8; // 0x08
    struct Sprite *unk_C; // 0x0C
    u8 filler_10[0x10];   // 0x10
} PokeathlonCourseHud;

// Field data structure (0x60 = 96 bytes each)
typedef struct PokeathlonFieldData {
    u8 data[0x60]; // Opaque data for now
} PokeathlonFieldData;

// Pokeathlon course arguments structure
typedef struct PokeathlonCourseArgs {
    SaveData *saveData; // 0x000 - Save data pointer
    union {
        u32 mode;        // 0x004 - Mode flag (1 = special mode) - accessed as u32
        u8 modeBytes[4]; // 0x004-0x007 - Individual byte access: [0]=0x4, [1]=0x5, [2]=0x6, [3]=0x7
    };
    u8 field_8;        // 0x008
    u8 field_9;        // 0x009
    u8 filler_A[4];    // 0x00A
    u8 shouldFreeHeap; // 0x00E - Flag: if 0, free heap allocations in Exit
    // ... more fields to be discovered
} PokeathlonCourseArgs;

// Main state machine states
typedef enum PokeathlonCourseMainState {
    POKEATHLON_STATE_IDLE = 0,         // Idle state - waiting for transitions
    POKEATHLON_STATE_START_TRANSITION, // Start transition
    POKEATHLON_STATE_WAIT_TRANSITION,  // Wait for transition to complete
    POKEATHLON_STATE_START_EXIT,       // Start exit transition (for type 0x10)
    POKEATHLON_STATE_EXIT_SEQUENCE     // Exit sequence
} PokeathlonCourseMainState;

// State machine structure (overlays stateArgsPtr at 0x3B4)
// This struct maps to a u32 array view for code generation compatibility
typedef struct PokeathlonCourseState {
    PokeathlonStateInfo *argsPtr; // [0] 0x3B4 - Pointer to state data pointer
    u32 transitionType;           // [1] 0x3B8 - Transition type (0x10 = exit)
    u32 mainState;                // [2] 0x3BC - Main state machine state
    u32 exitFlag;                 // [3] 0x3C0 - Exit flag
} PokeathlonCourseState;

// Unknown sub-structure at offset 0x974 (0x74 bytes)
typedef struct PokeathlonUnkSubStruct_974 {
    u8 data[0x74]; // Opaque data for now
} PokeathlonUnkSubStruct_974;

// The data copy area (PokeathlonCourseData + 0x28C, 0x128 bytes) is 7 records of 0x28 bytes:
//   +0x00 PokeathlonTouchRecord, +0x28 record 1 (word array), +0x50..+0xEF four per-participant records,
//   +0xF0 PokeathlonTally. Each minigame reads the tally record through its own view of the bytes, so it is
//   a union of array views.
typedef struct PokeathlonTouchRecord {
    s32 touching; // 0x00
    s8 touchX;    // 0x04
    s8 touchY;    // 0x05
    u8 filler_06[2];
    s8 unk_08;    // 0x08
    s8 unk_09;    // 0x09
    u8 filler_0A[0x1E];
} PokeathlonTouchRecord; // size 0x28

typedef struct PokeathlonTallyPair {
    s32 word0;
    s32 word1;
} PokeathlonTallyPair;

typedef union PokeathlonTally {
    PokeathlonTallyPair pairs[5];
    u8 u8s[0x28];
    u16 u16s[0x14];
    u32 u32s[10];
} PokeathlonTally; // size 0x28

typedef struct PokeathlonRecord1 {
    s32 words[10];
} PokeathlonRecord1; // size 0x28

// Network/local message system of the course (ov96_021E8770 allocates it).
// One queued message: payloads up to 0x26 bytes are copied into data, larger ones are
// referenced (or copied by the table's alloc callback) through ptr.
typedef struct Ov96Msg {
    s32 active;
    s32 cmd;
    u8 data[0x28];
    s32 size;
    void *ptr;
} Ov96Msg; // size 0x38 on the DS

typedef struct Ov96Handlers {
    Ov96Msg msgs[8];
    const CommCmdTable *cmdTable;
    int cmdCount;
    void *userData;
    SysTask *task;
} Ov96Handlers; // size 0x1D0 on the DS

typedef struct Ov96System {
    u16 isLocal;
    u16 sizeCheck;
    Ov96Handlers *handlers;
} Ov96System;

BOOL ov96_021E8914(Ov96Handlers *handlers, s32 cmd, void *data, s32 size);
BOOL ov96_021E8988(Ov96Handlers *handlers, s32 cmd, void *data, s32 size);

void ov96_021E87B0(Ov96System *system, s32 mode);

// Pokeathlon course data structure (0xD70 = 3440 bytes)
struct PokeathlonCourseData {
    u32 field_0[4];                            // 0x000
    u8 filler_10[0x14C];                       // 0x010
    u8 field_15C;                              // 0x15C
    u8 filler_15D[0x83];                       // 0x15D
    const PokeathlonEventHandlers *state;      // 0x1E0
    void *heapAllocPtr4;                       // 0x1E4 - Heap allocated pointer
    u8 filler_1E8[0x5];                        // 0x1E8
    u8 field_1ED;                              // 0x1ED
    u8 participantCount;                       // 0x1EE - Number of active participants (1-4)
    u8 field_1EF;                              // 0x1EF
    u32 currentParticipantIndex;               // 0x1F0
    u32 field_1F4;                             // 0x1F4
    PokeathlonCourseArgs *args;                // 0x1F8
    s32 unk_1FC;
    s32 unk_200_PokeathlonCourseData;
    u8 unk_204[0x6C];
    void *field_270;                           // 0x270
    u8 filler_274[0x8];                        // 0x274
    u32 field_27C;                             // 0x27C
    OverlayManager *subOverlay;                // 0x280
    enum HeapID heapId;                        // 0x284
    Ov96System *system;                        // 0x288
    u8 dataCopySource[0x28];                   // 0x28C - Source buffer for exit data copy
    u8 dataCopyBuffer1[0x28];                  // 0x2B4 - Destination buffer 1
    u8 dataCopyBuffer2[0x28];                  // 0x2DC - Destination buffer 2
    u8 filler_304[0xA0];                       // 0x304
    PokeathlonCmdBuf field_3A4;                // 0x3A4
    PokeathlonCourseState courseState;         // 0x3B4 - State machine structure (16 bytes: 4 fields x 4 bytes)
    PokeathlonStateInfo stateInfo;             // 0x3C4 - State information (8 bytes)
    PokeathlonStateInfo subStateInfo;          // 0x3CC
    u8 filler_3D4[4];                          // 0x3D4
    u32 field_3D8[4];                          // 0x3D8 - Array of 4 u32s (16 bytes)
    u32 field_3E8;                             // 0x3E8
    PokeathlonParticipantData participants[4]; // 0x3EC - 4 participant data elements (0x7C each = 0x1F0 bytes)
    PlayerProfile *playerProfiles;             // 0x5DC - Player profiles pointer (heap allocated, 4 profiles)
    u16 field_5E0[4][2];                       // 0x5E0
    u16 field_5F0[4][2];                       // 0x5F0
    u8 filler_600[0x14];                       // 0x600
    void *graphicsSystem;                      // 0x614 - Graphics/UI subsystem pointer
    PokeathlonMonPerf monPerf[4][3];           // 0x618 [participant][mon]
    PokeathlonCourseHud hud;                   // 0x708
    u8 unk_728;                                // 0x728
    u8 unk_729;                                // 0x729
    u8 maxParticipants;                        // 0x72A - Maximum participant limit (3 or 4)
    u8 filler_72B[1];                          // 0x72B
    PokeathlonFieldData fieldData[6];          // 0x72C - Array of PokeathlonFieldData (6 * 0x60 = 0x1E0 bytes)
    u8 filler_96C[8];                          // 0x96C
    PokeathlonUnkSubStruct_974 field_974[4];   // 0x974 - Array of 4 unknown sub-structures (0x74 bytes each = 0x1D0 bytes)
    u8 filler_B44[0x60];                       // 0xB44
    u8 field_BA4[0x180];                       // 0xBA4
    s32 frameCounter;                          // 0xD24 - Increments every 0x708 frames (max 0xEA5F)
    u16 frameTimer;                            // 0xD28 - Frame counter, resets at 0x708
    u16 filler_D2A;                            // 0xD2A - Alignment padding
    u32 counterEnabled;                        // 0xD2C - Flag to enable frame counters
    s32 unk_D30;
    PokeathlonCourseSpriteSlot spriteSlots[3]; // 0xD34
    void *heapAllocPtr2;                       // 0xD64 - Heap allocated pointer (freed in Exit if mode == 0)
    void *heapAllocPtr3;                       // 0xD68 - Heap allocated pointer (freed in Exit if mode == 0)
    u32 unk_D6C;
};

BOOL PokeathlonCourse_Init(OverlayManager *manager, int *state);
BOOL PokeathlonCourse_Main(OverlayManager *manager, int *state);
BOOL PokeathlonCourse_Exit(OverlayManager *manager, int *state);

BOOL PokeathlonCourse_RunStateFunc(PokeathlonCourseData *data);
BOOL PokeathlonCourse_RunSubStateLoop(PokeathlonCourseData *data);
void PokeathlonCourse_InitStateInfo(const PokeathlonStateFunc *funcTable, PokeathlonStateInfo *stateInfo);
void PokeathlonCourse_InitPlayerProfiles(PokeathlonCourseData *data);
PlayerProfile *PokeathlonCourse_GetPlayerProfile(PlayerProfile *profiles, int index);
u8 PokeathlonCourse_GetParticipantCount(PokeathlonCourseData *data);
PokeathlonParticipantData *PokeathlonCourse_GetParticipantData(PokeathlonCourseData *data, int index);
u8 *PokeathlonCourse_GetParticipantUnk04(PokeathlonCourseData *data, int index);
SaveData *PokeathlonCourse_GetSaveData(PokeathlonCourseData *data);
PokeathlonFieldData *PokeathlonCourse_GetFieldData(PokeathlonCourseData *data);
PokeathlonFieldData *PokeathlonCourse_GetFieldData_AtIndex(PokeathlonCourseData *data, int index);
void *PokeathlonCourse_GetFieldBA4(PokeathlonCourseData *data);
void *PokeathlonCourse_AllocPtr4FromHeap(PokeathlonCourseData *data, u32 size);
void PokeathlonCourse_FreePtr4HeapAlloc(PokeathlonCourseData *data);
void *PokeathlonCourse_GetHeapAllocPtr4(PokeathlonCourseData *data);
enum HeapID PokeathlonCourse_GetHeapID(PokeathlonCourseData *data);
u8 PokeathlonCourse_GetField1ED(PokeathlonCourseData *data);
void PokeathlonCourse_SetField1ED(PokeathlonCourseData *data, int param);
void PokeathlonCourse_IncrementField1ED(PokeathlonCourseData *data);
void PokeathlonCourse_SetField1F4(PokeathlonCourseData *data, int param);
void ov96_021E5E04(PokeathlonCourseData *data, u8 *param);
u32 PokeathlonCourse_GetField3D8_ForCurrentParticipant(PokeathlonCourseData *data);
u32 PokeathlonCourse_GetField3D8_AtIndex(PokeathlonCourseData *data, u8 index);
u32 ov96_021E5E7C(PokeathlonCourseData *data);
u32 PokeathlonCourse_GetCurrentParticipantIndex(PokeathlonCourseData *data);
u32 PokeathlonCourse_GetMode(PokeathlonCourseData *data);
u8 PokeathlonCourse_GetField1EF(PokeathlonCourseData *data);
void PokeathlonCourse_IncrementField1EF(PokeathlonCourseData *data);
void PokeathlonCourse_ResetField1EF(PokeathlonCourseData *data);
Ov96System *PokeathlonCourse_GetSystem(PokeathlonCourseData *data);
BOOL ov96_021E5F24(PokeathlonCourseData *data);
PlayerProfile *PokeathlonCourse_GetPlayerProfileFromData(PokeathlonCourseData *data, int index);
PokeathlonUnkSubStruct_974 *PokeathlonCourse_GetField974_AtIndex(PokeathlonCourseData *data, int index);
u8 *PokeathlonCourse_GetDataCopyArea(PokeathlonCourseData *data);
void PokeathlonCourse_ResetDataCopyArea(PokeathlonCourseData *data);
void PokeathlonCourse_SetField3A4(PokeathlonCourseData *data, u8 *a0, u8 *a1, s32 a2);
void PokeathlonCourse_ResetField3A4(PokeathlonCourseData *data);
u8 *PokeathlonCourse_GetField3A4(PokeathlonCourseData *data);
u32 PokeathlonCourse_GetUnkConstant4(void *data);
void PokeathlonCourse_SetField5E0_AtIndex(PokeathlonCourseData *data, int index, u16 value);
u16 PokeathlonCourse_GetField5F0_AtIndex(PokeathlonCourseData *data, int index);
void PokeathlonCourse_SetStateField07(PokeathlonCourseData *data, u32 value);
void PokeathlonCourse_SetStateField07_IfDifferent(PokeathlonCourseData *data, u8 value, u8 value2);
void PokeathlonCourse_SetStateTransitionType(PokeathlonCourseData *data, u32 transitionType);
void PokeathlonCourse_SetVBlankIntrCB(void *data);
void *PokeathlonCourse_GetGraphicsSystem(PokeathlonCourseData *data);
PokeathlonMonKinds *ov96_021E60D8(PokeathlonCourseData *data, s32 participant, s32 mon);

// Internal overlay 96 functions (stubs for now, in assembly)
void ov96_021E67AC(PokeathlonCourseData *data);
void ov96_021E75BC(void *);
void ov96_021E7F98(s32 frameCount, u32 maxValue, Pokeathlon_UnkSubStruct_B00 *result);
Ov96System *ov96_021E8770(const CommCmdTable *cmdTable, int cmdCount, PokeathlonCourseData *data, int a3, enum HeapID heapId);
Ov96Handlers *ov96_021E883C(const CommCmdTable *cmdTable, int cmdCount, void *userData, enum HeapID heapID);
BOOL ov96_021E87B4(s32 a0, void *a1, s32 a2, Ov96System *a3);
void ov96_021E8810(Ov96System *ptr);
BOOL ov96_021E8828(void *);
void ov96_021E6550(PokeathlonCourseData *data);
void ov96_021E65A4(PokeathlonCourseData *data);
void *ov96_021E8A20(void *ptr);
const CommCmdTable *ov96_021E8A24(void);
int ov96_021E8A2C(void);
void *ov96_021E92E0(enum HeapID heapId);
void ov96_021E9320(void *ptr);
u32 ov96_021E9A14(void);

#endif // POKEHEARTGOLD_POKEATHLON_H
