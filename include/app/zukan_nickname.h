// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_nickname.h: the lines in use so
// far.
#ifndef BWPORT_APP_ZUKAN_NICKNAME_H
#define BWPORT_APP_ZUKAN_NICKNAME_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct ZukanNickname ZukanNickname;

// What the player answered
#define ZUKAN_NICKNAME_NO 0

#define ZUKAN_NICKNAME_YES 1

// pokebw2's ZukanNickname_Start
// Draws the window and starts the question
void func_ov207_0220b908(ZukanNickname *nickname);

// Black 2's, not checked against Black
// Not yet
#define ZUKAN_NICKNAME_UNANSWERED 2

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZukanNickname_Create
// boxMessage is the message about the box (see ZukanTorokuParam), or NULL for none
ZukanNickname *func_ov207_0220b55c(HeapID heapId, PartyPkm *pkm, StrBuf *boxMessage, BoxSaveAccessor *boxes, u32 box,
                                    ClActUnit *unit, Font *font, PrintQueue *printQueue, GameData *gameData);
// pokebw2's ZukanNickname_Free
void func_ov207_0220b6e4(ZukanNickname *nickname);
// pokebw2's ZukanNickname_Update
// Returns whether the question has been answered and its messages are done
BOOL func_ov207_0220b72c(ZukanNickname *nickname);
// pokebw2's ZukanNickname_GetAnswer
u32 func_ov207_0220b940(ZukanNickname *nickname);

#endif // BWPORT_APP_ZUKAN_NICKNAME_H
