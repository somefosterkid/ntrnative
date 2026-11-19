#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222BC84(const UnkStruct_ov66_0222FB64 *param0);

void ov45_0222A520(WiFiPlazaSystem *param0, BOOL param1);

void ov45_0222A520(WiFiPlazaSystem *param0, BOOL param1) {
    if (param1 != param0->unk_1FC.unk_0D) {
        param0->unk_1FC.unk_0D = param1;
        ov45_0222BC84(&param0->unk_1FC);
    }
}
