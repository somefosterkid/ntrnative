#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"

extern u32 ov86_021E7E9C;

u32 ov86_021E7984(FrontierRecordsApp *p);

u32 ov86_021E7984(FrontierRecordsApp *p) {
    u32 in;
    s32 r;

    in = GridInputHandler_HandleInput_NoHold(p->inputHandler);
    switch (in) {
    case 8:
    case 0xfffffffe:
        PlaySE(0x5dd);
        ov86_021E71C0(p);
        p->nextState = 2;
        return 9;
    case 0xfffffffc:
        break;
    case 0xfffffffd:
        PlaySE(0x5dc);
        break;
    case 0xffffffff:
        r = GridInputHandler_GetNextInput(p->inputHandler);
        switch (r) {
        case 0:
        case 2:
        case 4:
        case 6:
            if ((gSystem.newKeys & PAD_KEY_LEFT) && p->lastPage != 0) {
                p->page = p->page - 1;
                if (p->page < 0) {
                    p->page = p->lastPage;
                }
                PlaySE(0x5dc);
                ov86_021E7B68(p);
                ov86_021E7CF8(p);
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            if ((gSystem.newKeys & PAD_KEY_RIGHT) && p->lastPage != 0) {
                p->page = p->page + 1;
                if (p->page > p->lastPage) {
                    p->page = 0;
                }
                PlaySE(0x5dc);
                ov86_021E7B68(p);
                ov86_021E7CF8(p);
            }
            break;
        }
        break;
    default:
        if (in + p->page * 8 >= (u32)p->lists[p->category].count) {
            PlaySE(0x5f2);
            break;
        }
        {
            p->cursor = in;
            PlaySE(0x5dd);
            ov86_021E78B8(p, in);
            p->nextState = 7;
            return 9;
        }
        PlaySE(0x5f2);
        break;
    }
    r = TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)&ov86_021E7E9C);
    switch (r) {
    case 0:
        if (p->lastPage != 0) {
            p->page = p->page - 1;
            if (p->page < 0) {
                p->page = p->lastPage;
            }
            PlaySE(0x5e0);
            ov86_021E792C(p, 0x22, 0x34);
            p->nextState = 6;
            return 9;
        }
        break;
    case 1:
        if (p->lastPage != 0) {
            p->page = p->page + 1;
            if (p->page > p->lastPage) {
                p->page = 0;
            }
            PlaySE(0x5e0);
            ov86_021E792C(p, 0x26, 0x34);
            p->nextState = 6;
            return 9;
        }
        break;
    }
    return 5;
}
