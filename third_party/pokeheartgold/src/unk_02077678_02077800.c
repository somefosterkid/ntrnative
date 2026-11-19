#include "global.h"

#include "error_handling.h"

extern s32 sMoveCategoryGfxCharFileIds;

s32 GetMoveCategoryIconGfxCharFileId(int moveCategory);

enum MoveClass {
    CLASS_PHYSICAL = 0,
    CLASS_SPECIAL = 1,
    CLASS_STATUS = 2,
};
extern __attribute__((aligned(4))) const u8 sMoveCategoryGfxPlttOverrides[3];
typedef struct WiFiPlazaQuestion {
    u32 questionNumber;
    u32 selectedAnswer;
} WiFiPlazaQuestion;
u8 GetMoveCategoryIconGfxPlttOverride(enum MoveClass moveCat);
int GetMoveCategoryIconGfxNarcId(void);

s32 GetMoveCategoryIconGfxCharFileId(int moveCategory) {
    if ((u32)moveCategory >= 3U) {
        GF_AssertFail();
    }
    return *(s32 *)(&sMoveCategoryGfxCharFileIds + moveCategory);
}

u8 GetMoveCategoryIconGfxPlttOverride(enum MoveClass moveCat) {
    ((moveCat < (sizeof(sMoveCategoryGfxPlttOverrides) / sizeof(*(sMoveCategoryGfxPlttOverrides)))) ? (void)0 : GF_AssertFail());
    return sMoveCategoryGfxPlttOverrides[moveCat];
}

int GetMoveCategoryIconGfxNarcId(void) {
    return sizeof(WiFiPlazaQuestion);
}
