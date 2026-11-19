#include "global.h"

#include "frontier_data.h"
#include "unk_0202D230.h"
#include "unk_02030A98.h"
#include "unk_02066EDC.h"

u32 sub_020672D8(SaveData *saveData) {
    u32 v0;
    FrontierData *frontierData;
    u32 v1;
    u8 v4;
    u8 v3;
    u32 v2;

    v1 = (u32)FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), 0, 0xFF);
    v0 = v1;
    if (v1 < 0x14U) {
        return 0U;
    }
    frontierData = Save_FrontierData_Get(saveData);
    v4 = (u8)sub_0202D5DC(frontierData, 0xDU, 0U);
    v3 = (u8)sub_0202D5DC(frontierData, 0U, 0U);
    v2 = (u8)sub_0202D5DC(frontierData, 1U, 0U);
    sub_0202D5DC(frontierData, 0xEU, 0U);
    sub_0202D5DC(frontierData, 2U, 0U);
    sub_0202D5DC(frontierData, 3U, 0U);
    if ((v4 != 0) && (v3 != 0) && (v2 != 0)) {
        return 0U;
    }
    if (v4 == 0) {
        sub_0202D5DC(frontierData, 0xDU, 1U);
        return 1U;
    }
    if (v0 < 0x32U) {
        return 0U;
    }
    if (v3 == 0) {
        sub_0202D5DC(frontierData, 0U, 1U);
        return 2U;
    }
    if ((v0 < 0x64U) || (v2 != 0)) {
        return 0U;
    }
    sub_0202D5DC(frontierData, 1U, 1U);
    return 3U;
}
