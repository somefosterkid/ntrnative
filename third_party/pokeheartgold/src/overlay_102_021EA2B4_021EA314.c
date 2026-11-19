#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu_cursor.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "yes_no_prompt.h"
#include "overlay_102_easy_chat_sentence_view.h"

u32 *ov102_021E8F68(struct EasyChatApp *app);

EasyChatSentenceView *ov102_021EA314(struct UnkStruct_ov20_021D2128_t *parent, struct EasyChatApp *app, struct EasyChatWordList *wordList) {
    EasyChatSentenceView *view;

    view = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatSentenceView));
    view->parent = parent;
    view->app = app;
    view->wordList = wordList;
    view->ignoreTouchFlag = ov102_021E8F68(view->app);
    view->sprites[0] = NULL;
    view->sprites[1] = NULL;
    view->sprites[2] = NULL;
    view->sprites[3] = NULL;
    view->sprites[4] = NULL;
    view->sprites[5] = NULL;
    view->string = String_New(0x80U, HEAP_ID_35);
    view->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x11B, HEAP_ID_35);
    view->cursor = ListMenuCursorNew(HEAP_ID_35);
    view->scrollX = 0;
    view->scrollY = 0;
    return view;
}

void ov102_021EA380(EasyChatSentenceView *view) {
    MsgData *msgData;
    Sprite *sprite;
    Sprite *v0;
    Sprite *v1;
    Sprite *v2;
    Sprite *v3;
    Sprite *v4;
    String *string;
    struct ListMenuCursor *cursor;

    cursor = view->cursor;
    if (cursor != NULL) {
        DestroyListMenuCursorObj(cursor);
    }
    sprite = view->sprites[0];
    if (sprite != NULL) {
        Sprite_Delete(sprite);
    }
    v0 = view->sprites[1];
    if (v0 != NULL) {
        Sprite_Delete(v0);
    }
    v1 = view->sprites[2];
    if (v1 != NULL) {
        Sprite_Delete(v1);
    }
    v2 = view->sprites[3];
    if (v2 != NULL) {
        Sprite_Delete(v2);
    }
    v3 = view->sprites[4];
    if (v3 != NULL) {
        Sprite_Delete(v3);
    }
    v4 = view->sprites[5];
    if (v4 != NULL) {
        Sprite_Delete(v4);
    }
    msgData = view->msgData;
    if (msgData != NULL) {
        DestroyMsgData(msgData);
    }
    string = view->string;
    if (string != NULL) {
        String_Delete(string);
    }
    RemoveWindow(&view->windows[0]);
    RemoveWindow(&view->windows[1]);
    RemoveWindow(&view->windows[2]);
    RemoveWindow(&view->windows[3]);
    RemoveWindow(&view->windows[4]);
    YesNoPrompt_Destroy(view->yesNoPrompt);
    Heap_Free(view);
}
