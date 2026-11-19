#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov49_0225A840(UnkStruct_ov69_0225D35C *param0);

void ov49_0225A840(UnkStruct_ov69_0225D35C *param0) {
    DoScheduledBgGpuUpdates(param0->unk_00);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}
