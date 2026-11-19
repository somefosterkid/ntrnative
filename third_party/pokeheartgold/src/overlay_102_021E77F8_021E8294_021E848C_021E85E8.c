#include "global.h"
#include "overlay_102_pmsinput_work.h"

struct ButtonMan;

struct PMSData;

struct PMSIParam;

struct PMSISearch;

struct PMSInputData;

struct PMSInputView;

struct PMSInputWork;

struct TCBManager;

typedef struct ButtonMan ButtonMan;

typedef struct PMSData PMSData;

typedef struct PMSInputData PMSInputData;

typedef struct PMSInputView PMSInputView;

typedef struct PMSIParam PMSIParam;

typedef struct PMSISearch PMSISearch;

typedef struct TCBManager TCBManager;

struct PMSData {

    u16 type;
    u16 id;
    u16 words[2];
};

typedef BOOL (*PMSInputMainProc)(PMSInputWork *wk, u32 *seq);

typedef void (*PMSInputSubProc)(PMSInputWork *wk, u32 *seq);

typedef void (*PMSInputKTChangeFunc)(PMSInputWork *wk, u32 *seq);

typedef BOOL (*PMSInputCursorFunc)(PMSInputWork *wk);

typedef struct {
    s16 type;
    s8 id;
    s8 count;
} PMSInputSentenceWork;

typedef struct {
    u16 top;
    u16 scrollMax;
    u16 wordMax;
    u8 x;
    u8 y;
    int scrollVector;
    u16 onBack;
    u16 selected;
} PMSInputWordWin;

typedef struct {
    u8 pos;
    u8 max;
} PMSInputMenu;

BOOL func_ov110_021beb78(PMSInputWork *wk);

BOOL func_ov110_021bec20(PMSInputWork *wk);

BOOL ov102_021E85E8(PMSInputWork *wk);

BOOL ov102_021E85E8(PMSInputWork *wk) {
    extern const PMSInputCursorFunc _021EC5D8[];

    return _021EC5D8[wk->categoryMode](wk);
}
