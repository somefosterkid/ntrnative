#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_system.h"

#include "assert.h"
#include "frontier_system.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"

struct FieldFrontierDTO;

struct FrontierMap;

struct FrontierSystem;

struct FrontierWork;

struct OverlayManager;

struct OverlayManagerTemplate;

typedef struct FrontierWork {
    FieldFrontierDTO *launchArgs;
    OverlayManager *childOvy;
    void *childArg;
    UnkFuncPtr_020965A4 *childCallback;
    u8 freeChildArg;
    u8 padding_011[3];
    FrontierSystem *system;
    FrontierMap *map;
    u8 padding_01C;
    u8 mapCreated;
    u8 unk1E;
    u8 padding_01F;
    u16 unk20;
    u8 unk22;
    u8 padding_023;
    u16 unk24[0x18][2];
    struct {
        u8 unk0[0xC];
        u16 unkC;
        u8 unkE[0x2E];
    } unk84[0x20];
    u8 unk804[0x200];
    u16 unkA04[8];
    u8 unkA14[0x40];
} FrontierWork;

extern FrontierSystem *FrontierSystem_Create(FrontierWork *data, enum HeapID heapId, int mode);

extern void FrontierSystem_Main(FrontierSystem *system);

extern void FrontierSystem_AddTask(FrontierSystem *system, int a1, int a2);

extern void ov80_022389C4(FrontierMap *map);

extern void ov80_02238A18(FrontierMap *map);

extern void ov80_0222A920(FrontierSystem *system);

extern void ov80_0222AA7C(FrontierSystem *system, u16 a1, enum HeapID heapId);

extern void *ov80_0222AAD8(FrontierSystem *system, enum HeapID heapId);

extern void ov80_0222AAF8(FrontierSystem *system, void *a1);

void Frontier_CreateMap(FrontierWork *data);

void Frontier_FreeMap(FrontierWork *data);

void sub_02096780(FrontierWork *data);

void Frontier_LoadOverlays(void);

void Frontier_UnloadOverlays(void);

int Frontier_Main(OverlayManager *man, int *state);

int Frontier_Main(OverlayManager *man, int *state) {
    FrontierWork *data = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        if (data->unk22 == 1) {
            *state = 2;
        } else if (data->mapCreated != 0) {
            if (data->unk1E == 1) {
                *state = 5;
            } else {
                FrontierSystem_Main(data->system);
                if (data->childOvy != NULL) {
                    *state = 3;
                }
            }
        }
        break;
    case 2:
        return 1;
    case 3:
        ov80_022389C4(data->map);
        Frontier_FreeMap(data);
        Frontier_UnloadOverlays();
        *state = 4;
        break;
    case 4:
        if (OverlayManager_Run(data->childOvy) == TRUE) {
            OverlayManager_Delete(data->childOvy);
            Frontier_LoadOverlays();
            if (data->childCallback != NULL) {
                data->childCallback(data->childArg);
            }
            if (data->childArg != NULL && data->freeChildArg == 1) {
                Heap_Free(data->childArg);
            }
            data->childOvy = NULL;
            data->childCallback = NULL;
            data->childArg = NULL;
            Frontier_CreateMap(data);
            ov80_02238A18(data->map);
            *state = 1;
        }
        break;
    case 5:
        Frontier_FreeMap(data);
        sub_02096780(data);
        *state = 6;
        break;
    case 6:
        Frontier_CreateMap(data);
        if (data->unk20 == 0xFFFF) {
            ov80_0222AA7C(data->system, data->launchArgs->sceneID, HEAP_ID_FIELD2);
        } else {
            void *r6 = ov80_0222AAD8(data->system, HEAP_ID_FIELD2);
            ov80_0222A920(data->system);
            data->system = FrontierSystem_Create(data, HEAP_ID_FIELD2, data->launchArgs->sceneID);
            FrontierSystem_AddTask(data->system, data->launchArgs->sceneID, data->unk20);
            ov80_0222AAF8(data->system, r6);
        }
        data->unk1E = 0;
        *state = 1;
        break;
    }
    return 0;
}
