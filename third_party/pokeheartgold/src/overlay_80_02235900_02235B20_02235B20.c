#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_system.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "frontier_data.h"
#include "heap.h"
#include "overlay_80_022384D8.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "scrcmd_20.h"
#include "unk_0202D230.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "unk_0204A3F4.h"
#include "unk_0205BFF0.h"
#include "unk_02096910.h"
#include "use_item_on_mon.h"

struct FrontierData;

struct FrontierLaunchArgs;

struct FrontierSave;

struct FrontierScriptContext;

struct FrontierSystem;

struct MsgData;

struct SaveData;

BOOL FrtCmd_178(FrontierScriptContext *ctx);

BOOL FrtCmd_178(FrontierScriptContext *ctx) {
    u8 sel = *ctx->scriptPtr++;
    u8 arg = *ctx->scriptPtr++;
    ctx->scriptPtr++;
    u16 *out = FrontierScript_ReadVarPtr(ctx);
    u8 *data = Frontier_GetData(ctx->frontierSystem->unk0);
    FrontierLaunchArgs *launchArgs = Frontier_GetLaunchArgs(ctx->frontierSystem->unk0);
    switch (sel) {
    case 0:
        sub_02096910(data);
        break;
    case 1:
        *out = *(u16 *)(data + 0x86);
        break;
    case 2:
        if (arg == 1) {
            *out = 0;
            if (*(u16 *)(data + 0x76) == *(u16 *)(data + 0x86) || *(u16 *)(data + 0x76) == *(u16 *)(data + 0x88)) {
                *out = *out + 1;
            }
            if (*(u16 *)(data + 0x78) == *(u16 *)(data + 0x86) || *(u16 *)(data + 0x78) == *(u16 *)(data + 0x88)) {
                *out = *out + 2;
            }
        } else if (arg == 5) {
            if (*(u16 *)(data + 0x76) == *(u16 *)(data + 0x86)) {
                *out = 0;
            } else {
                *out = 1;
            }
        } else if (arg == 4 || arg == 6) {
            *out = 0;
            if (*(u16 *)(data + 0x76) == *(u16 *)(data + 0x86) || *(u16 *)(data + 0x76) == *(u16 *)(data + 0x88)) {
                *out = *out + 1;
            }
            if (*(u16 *)(data + 0x78) == *(u16 *)(data + 0x86) || *(u16 *)(data + 0x78) == *(u16 *)(data + 0x88)) {
                *out = *out + 2;
            }
        }
        break;
    case 3:
        *out = FrontierSave_GetStat(Save_Frontier_GetStatic(launchArgs->saveData), 0x6a, sub_0205C268(0x6a));
        break;
    case 4: {
        FrontierSave *fs = Save_Frontier_GetStatic(launchArgs->saveData);
        int stat = sub_0205C11C(3);
        *out = FrontierSave_GetStat(fs, stat, sub_0205C268(sub_0205C11C(3)));
        break;
    }
    case 5:
        *out = *(u16 *)(data + arg * 2 + 0x76);
        break;
    case 6:
        sub_0204F878(launchArgs->saveData, sub_02030C5C(launchArgs->saveData), 3);
        break;
    case 7:
        data[0xa0] = arg;
        break;
    case 8:
        *out = *(u16 *)(data + 0x74);
        break;
    case 9:
        *out = data[0xa0];
        break;
    case 10:
        *out = data[0x71];
        break;
    case 11: {
        int idx;
        u16 stat;
        *out = 1;
        idx = ov80_022385D8(data[0xa0]);
        stat = FrontierSave_GetStat(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx));
        if (stat != *(u16 *)(data + 0x72)) {
            sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx), 0);
            idx = ov80_02238610(data[0xa0]);
            sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx), 0);
            *out = 0;
        }
        break;
    }
    case 12: {
        int idx = ov80_02238610(data[0xa0]);
        *out = FrontierSave_GetStat(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx));
        if (*out == 0) {
            idx = ov80_022385D8(data[0xa0]);
            sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx), 0);
            if (data[0xa0] == 1) {
                sub_0202D57C(Save_FrontierData_Get(*(SaveData **)data), 6, 2);
            }
            if (data[0xa0] == 4) {
                sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), 0x88, sub_0205C268(0x88), 0);
                sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), 0x89, sub_0205C268(0x89), 0);
            }
            if (data[0xa0] == 2) {
                sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), sub_0205C048(0, 3), sub_0205C268(sub_0205C048(0, 3)), 0);
            }
            if (data[0xa0] == 3) {
                sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), sub_0205C048(1, 3), sub_0205C268(sub_0205C048(1, 3)), 0);
            }
        } else {
            sub_02031108(Save_Frontier_GetStatic(*(SaveData **)data), idx, sub_0205C268(idx), 0);
        }
        break;
    }
    case 13: {
        int i;
        for (i = 0; i < 2; i++) {
            data[0xa1 + i] = 0;
        }
        data[0x9f] = 0;
        break;
    }
    }
    return FALSE;
}
