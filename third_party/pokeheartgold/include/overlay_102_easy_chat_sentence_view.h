#ifndef POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_SENTENCE_VIEW_H
#define POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_SENTENCE_VIEW_H

#include "bg_window.h"
#include "filesystem.h"
#include "touchscreen.h"

struct EasyChatApp;
struct EasyChatWordList;
struct ListMenuCursor;
struct MsgData;
struct Sprite;
struct String;
struct UnkStruct_ov20_021D2128_t;
struct YesNoPrompt;

// Reads a message template (text runs, word boxes, line breaks) from a String.
typedef struct EasyChatTemplateReader {
    struct String *string;
    const u16 *cur;
    int state; // 0: text run, 1: control code, 2: line break, 3: end
} EasyChatTemplateReader;

typedef struct EasyChatSentenceViewPos {
    s16 x;
    s16 y;
} EasyChatSentenceViewPos;

// The sentence / word box screen of the Easy Chat input (parent slot +0x1E0).
typedef struct EasyChatSentenceView {
    /* 0x000 */ struct UnkStruct_ov20_021D2128_t *parent;
    /* 0x004 */ struct EasyChatApp *app;
    /* 0x008 */ struct EasyChatWordList *wordList;
    /* 0x00C */ Window windows[5];
    /* 0x05C */ Window *textWindow;
    /* 0x060 */ struct Sprite *sprites[6];
    /* 0x078 */ struct MsgData *msgData;
    /* 0x07C */ struct String *string;
    /* 0x080 */ struct ListMenuCursor *cursor;
    /* 0x084 */ EasyChatSentenceViewPos wordPos[2];
    /* 0x08C */ s32 wordBoxCount;
    /* 0x090 */ s32 tileBase;
    /* 0x094 */ EasyChatTemplateReader reader;
    /* 0x0A0 */ u16 palette[10][16];
    /* 0x1E0 */ s16 scrollX;
    /* 0x1E2 */ s16 scrollY;
    /* 0x1E4 */ u8 frame;
    /* 0x1E5 */ u8 dir;
    /* 0x1E8 */ struct YesNoPrompt *yesNoPrompt;
    /* 0x1EC */ u16 frameTile;
    /* 0x1EE */ u16 yesNoTile;
    /* 0x1F0 */ u32 *ignoreTouchFlag;
} EasyChatSentenceView; // size 0x1F4 on DS

EasyChatSentenceView *ov102_021EA314(struct UnkStruct_ov20_021D2128_t *parent, struct EasyChatApp *app, struct EasyChatWordList *wordList);
void ov102_021EA380(EasyChatSentenceView *view);
void ov102_021EA41C(EasyChatSentenceView *view, NARC *narc);
void ov102_021EA644(void *view);
void ov102_021EA678(EasyChatSentenceView *view, s32 enable);
void ov102_021EA71C(EasyChatSentenceView *view, u8 dir);
s32 ov102_021EA754(EasyChatSentenceView *view);
void ov102_021EA80C(EasyChatSentenceView *view, NARC *narc);
void ov102_021EA874(EasyChatSentenceView *view);
void ov102_021EA8C0(EasyChatSentenceView *view);
void ov102_021EA920(EasyChatSentenceView *view);
void ov102_021EAA3C(EasyChatSentenceView *view);
void ov102_021EAB10(const EasyChatSentenceView *view, TouchscreenHitbox *hitbox, s32 idx);
s32 ov102_021EAB30(EasyChatSentenceView *view, Window *window);
void ov102_021EAC20(EasyChatSentenceView *view);
void ov102_021EAC44(EasyChatTemplateReader *reader, EasyChatSentenceView *view);
void ov102_021EAC70(EasyChatTemplateReader *reader);
int ov102_021EAC7C(EasyChatTemplateReader *reader, struct String *dest);
void ov102_021EAD48(const EasyChatSentenceViewPos *src, EasyChatSentenceViewPos *dst);
void ov102_021EAD5C(const EasyChatSentenceViewPos *src, EasyChatSentenceViewPos *dst);
void ov102_021EAD70(Window *window, const EasyChatSentenceViewPos *pos);
void ov102_021EAD98(EasyChatSentenceView *view, Window *window, EasyChatSentenceViewPos *pos, u16 ecWord);
s32 ov102_021EADF0(const EasyChatSentenceView *view);
void ov102_021EADF8(EasyChatSentenceView *view, s32 idx);
void ov102_021EAE40(EasyChatSentenceView *view, u32 mode);
void ov102_021EAF44(EasyChatSentenceView *view);
void ov102_021EAF50(EasyChatSentenceView *view);
void ov102_021EAF5C(EasyChatSentenceView *view, s32 flag);
void ov102_021EAF7C(EasyChatSentenceView *view);
void ov102_021EAFAC(EasyChatSentenceView *view);
struct Sprite *ov102_021EAFEC(const EasyChatSentenceView *view);
void ov102_021EAFF0(EasyChatSentenceView *view, s32 idx);
void ov102_021EB088(EasyChatSentenceView *view, s32 flag);
u32 ov102_021EB130(EasyChatSentenceView *view);
void ov102_021EB170(EasyChatSentenceView *view, u32 idx);

#endif // POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_SENTENCE_VIEW_H
