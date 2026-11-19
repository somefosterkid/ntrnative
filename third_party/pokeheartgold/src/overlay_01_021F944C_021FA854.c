#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "billboard.h"
#include "overlay_01_021FC4C4.h"
#include "sys_task.h"
#include "map_object.h"
#include "gf_3d_loader.h"

struct MapObject;
struct MapObjectManager;
struct GF_2DGfxRawResMan;
struct ResourceHeap;
struct SysTask;
struct SysTaskQueue;
struct TextureResourceManager;
struct UnkStruct_ov5_021ED0A4_t;
struct UnkStruct_ov5_021ED110;
typedef struct MapObject MapObject;
typedef struct TextureResourceManager TextureResourceManager;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);
SysTask * SysTask_CreateOnVBlankQueue(SysTaskFunc func, void *data, u32 priority);
SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);
void ov01_021FA564(SysTask *param0, void *param1);
void ov01_021FA6E0(SysTask *param0, void *param1);
void ov01_021FA71C(SysTask *param0, void *param1);
void ov01_021FA7F8(SysTask *param0, void *param1);

void ov01_021FA854(UnkStruct_ov5_021ED0A4 *param0, int param1, int param2);

void ov01_021FA854(UnkStruct_ov5_021ED0A4 *param0, int param1, int param2) {
    u32 v0, v1, v2, v3, v4;
    UnkStruct_ov5_021EDF3C *v5;
    u8 *v6;

    v4 = (sizeof(UnkStruct_ov5_021EE294)) * param1;
    v3 = (sizeof(UnkStruct_ov5_021EDFBC)) * param1;
    v2 = (sizeof(UnkStruct_ov5_021EE134)) * param1;
    v1 = (sizeof(UnkStruct_ov5_021EDF3C)) + v2 + v3 + v4;
    v5 = Heap_Alloc(HEAP_ID_FIELD1, v1);

    ((v5 != 0L) ? (void)0 : GF_AssertFail());
    memset(v5, 0, v1);

    v5->unk_00 = param1;
    v5->unk_02 = param2;

    v6 = (u8 *)v5 + (sizeof(UnkStruct_ov5_021EDF3C));
    v5->unk_08 = (void *)v6;

    v6 += v4;
    v5->unk_0C = (void *)v6;

    v6 += v3;
    v5->unk_10 = (void *)v6;

    param0->unk_100 = v5;

    v0 = MapObjectManager_GetPriority(param0->unk_104);
    v5->unk_18 = SysTask_CreateOnMainQueue(ov01_021FA564, param0, v0 + 1);
    v5->unk_1C = SysTask_CreateOnMainQueue(ov01_021FA7F8, param0, v0 + 2);
    v5->unk_20 = SysTask_CreateOnVBlankQueue(ov01_021FA6E0, param0, 0xff);
    v5->unk_24 = SysTask_CreateOnVWaitQueue(ov01_021FA71C, param0, 0xff);
}
