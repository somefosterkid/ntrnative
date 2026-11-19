#include "global.h"

#include "constants/items.h"

#include "heap.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "string_util.h"
#include "unk_02031B0C.h"

s32 sub_02031D6C(ApricornJuice *a0);

s32 sub_02032158(void *arg0, void *arg1);

void sub_020321A0(SaveApricornBox *box, void *slotData, int count, int startIdx);

void sub_020321A0(SaveApricornBox *box, void *slotData, int count, int startIdx) {
    u8 foundMatch;
    s32 validSlotCount = 0;
    u32 outerCount = 0;
    ApricornJuice *unkPtr = &box->juiceRecords[0].data;
    while ((s32)outerCount < 3) {
        if (!sub_02031D6C(unkPtr)) {
            break;
        }
        outerCount++;
        validSlotCount++;
        unkPtr = (ApricornJuice *)((u8 *)unkPtr + 0x20);
    }

    int loopI = 0;
    ApricornRecord *inSlot = (ApricornRecord *)slotData;
    while (loopI < count) {
        if (loopI != startIdx) {
            if (sub_02031D6C(&inSlot->data)) {
                foundMatch = 0;
                u32 matchPos = 0;
                ApricornRecord *searchSlot = &box->juiceRecords[0];
                u32 j = foundMatch;
                while ((s32)j < 3) {
                    if (sub_02032158(inSlot, searchSlot)) {
                        matchPos = (u8)j;
                        foundMatch = 1;
                        break;
                    }
                    j++;
                    searchSlot++;
                }

                u8 ip;
                if (validSlotCount >= 3) {
                    ip = 2;
                    foundMatch = 1;
                } else if (foundMatch) {
                    ip = (u8)(validSlotCount - 1);
                } else {
                    ip = (u8)validSlotCount;
                    validSlotCount++;
                }

                if (foundMatch && matchPos < ip) {
                    s32 shiftEnd = validSlotCount - 1;
                    if ((s32)matchPos < shiftEnd) {
                        u8 *shiftPtr = (u8 *)box + matchPos * 0x20;
                        s32 shiftPos = matchPos;
                        do {
                            ApricornRecord *dst2 = (ApricornRecord *)(shiftPtr + 0x20);
                            ApricornRecord *src2 = (ApricornRecord *)(shiftPtr + 0x40);
                            *dst2 = *src2;
                            shiftPtr += 0x20;
                            shiftPos++;
                        } while (shiftPos < shiftEnd);
                    }
                }

                ApricornRecord *ipSlot = (ApricornRecord *)((u8 *)box + 0x20 + ip * 0x20);
                *ipSlot = *inSlot;
            }
        }
        inSlot++;
        loopI++;
    }
}
