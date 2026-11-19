#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "overlay_102_easy_chat_sentence_view.h"

typedef u16 charcode_t;
charcode_t *MsgArray_SkipControlCode(const charcode_t *cstr);
void CopyU16ArrayToStringN(String *dst, const charcode_t *src, u32 num);

void ov102_021EAC70(EasyChatTemplateReader *reader) {
    String_Delete(reader->string);
}

int ov102_021EAC7C(EasyChatTemplateReader *param0, String *param1) {
    const u16 *v0 = param0->cur;
    int v1;

    switch (param0->state) {
    case 0:
        while (param0->state == 0) {
            switch (*(param0->cur)) {
            case 0xe000:
                param0->state = 2;
                break;
            case 0xffff:
                param0->state = 3;
                break;
            case 0xfffe:
                param0->state = 1;
                break;
            default:
                param0->cur++;
                break;
            }
        }

        CopyU16ArrayToStringN(param1, v0, (param0->cur - v0) + 1);
        return 0;
    case 1:
        param0->cur = MsgArray_SkipControlCode(param0->cur);
        v1 = 1;
        break;
    case 2:
        param0->cur++;
        v1 = 2;
        break;
    case 3:
    default:
        return 3;
    }

    switch (*(param0->cur)) {
    case 0xe000:
        param0->state = 2;
        break;
    case 0xffff:
        param0->state = 3;
        break;
    case 0xfffe:
        param0->state = 1;
        break;
    default:
        param0->state = 0;
        break;
    }

    return v1;
}

void ov102_021EAD48(const EasyChatSentenceViewPos *src, EasyChatSentenceViewPos *dst) {
    dst->x = src->x - (96 / 2);
    dst->y = src->y - (16 / 2);
}

void ov102_021EAD5C(const EasyChatSentenceViewPos *src, EasyChatSentenceViewPos *dst) {
    dst->x = src->x + 3 * 8;
    dst->y = src->y + 1 * 8;
}

void ov102_021EAD70(Window *window, const EasyChatSentenceViewPos *pos) {
    EasyChatSentenceViewPos v0;

    ov102_021EAD48(pos, &v0);
    FillWindowPixelRect(window, 14, v0.x, v0.y, 96, 16);
}
