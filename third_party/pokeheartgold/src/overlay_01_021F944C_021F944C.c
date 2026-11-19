#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"
#include "gf_3d_loader.h"

struct MapObject;
struct GF_2DGfxRawResMan;
struct ResourceHeap;
struct TextureResourceManager;
struct UnkStruct_ov5_021ED0A4_t;
struct UnkStruct_ov5_021ED110;
typedef struct MapObject MapObject;
typedef struct TextureResourceManager TextureResourceManager;
extern const int ov01_02206CF0[];
extern const int ov01_02207260[];
void ov01_021F9698(UnkStruct_ov5_021ED0A4 *param0, int param1);
void ov01_021FA2EC(UnkStruct_ov5_021ED0A4 *param0, int param1);
void ov01_021FA854(UnkStruct_ov5_021ED0A4 *param0, int param1, int param2);

void ov01_021F944C(UnkStruct_ov5_021ED0A4 *param0, MapObjectManager *param1, int param2, int param3, int param4, int param5, const int *param6, int param7);

void ov01_021F944C(UnkStruct_ov5_021ED0A4 *param0, MapObjectManager *param1, int param2, int param3, int param4, int param5, const int *param6, int param7) {
    param0->unk_104 = param1;

    ov01_021FA2EC(param0, param2);
    ov01_021FA854(param0, param2, param7);
    ov01_021F9808(param0, 8, 4, 8, 4, param4, param5);
    ov01_021F9698(param0, param2);
    ov01_021F9A8C(param0, param6, ov01_02207260, ov01_02206CF0);
}
