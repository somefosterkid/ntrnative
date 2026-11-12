// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/mailsys.h: the lines in use so far.
#ifndef BWPORT_FIELD_MAILSYS_H
#define BWPORT_FIELD_MAILSYS_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "system/app_taskmenu.h"
#include "system/pms_data.h"
#include "system/printsys.h"
#include "struct_decls.h"

#define MAIL_WORD_COUNT 3
typedef struct MailSysWork MailSysWork;

// Black 2's; twinned code confirms the offsets of mode, selected, cursor, trainerId, unk09, unk0A, design, name, words,
// hasPreset and word, and its size in Black
// What a mail screen shows, and what it returns: the mail as a copy that the player edits
typedef struct MailEdit {
    // The MAIL_PARAM_MODE_* on entry. Then what the screen chose: a sentence from 0 to 2 or the word 3 to input,
    // MAIL_EDIT_DONE to keep the mail, or MAIL_EDIT_CANCEL
    u16 mode;
    // The sentence or word that the input screen edits, and the cursor to come back to
    u8 selected;
    u8 cursor;
    u32 trainerId;
    u8 unk08;
    u8 unk09;
    u8 unk0A;
    u8 design;
    StrBuf *name;
    u8 unk10[6];
    PMSData words[MAIL_WORD_COUNT];
    // Whether the design gives the sentence a preset one to start from
    u8 hasPreset[MAIL_WORD_COUNT];
    u8 unk31;
    // The mail's easy word
    u16 word;
} MailEdit;

// Black 2's, not checked against Black
// A touch or key mode change, called with the mail screen's work
typedef BOOL (*MailSysModeFunc)(MailSysWork *work);

typedef struct {
    BmpWin *win;
    // Whether its text is drawn
    u8 drawn;
} MailSysBmpWin;

// Black 2's; twinned code confirms the offsets of heapId, seq, subSeq, keyMode, phase, initialPhase, confirming,
// textSpeed, cursor, savedCursor, blinkLevel, blinkFalling, blinkTarget, blinkCurrent, edit, msgData, font, unk28,
// msgBuf, palFade, onKeyMode, onTouchMode, windows, entries, printQueue, printStream, clUnit, tcbBuffer, tcbMgr,
// blinkTask, tcbEx, vblankTask, menuRes, menu, menuItems, confirmButton, cancelButton, buttonItems, pmsDraw, pmsWord,
// pmsClUnit and pmsQueue, and its size in Black
// The work of the mail screen's proc (mailsys.c's MailSys_EditInit), whose screens mailview.c sets up
struct MailSysWork {
    u32 heapId;
    // The step of MailSys_Update
    u32 seq;
    // The step of the phase or of the loading
    u16 subSeq;
    // Whether the keys drive the cursor rather than the touch screen
    u8 keyMode;
    // MAIL_PHASE_*
    u8 phase;
    u8 initialPhase;
    u32 confirming;
    u8 unk14;
    s8 textSpeed;
    u8 cursor;
    u8 savedCursor;
    // The cursor's pulse, and whether it fades back
    u8 blinkLevel;
    u8 blinkFalling;
    u8 blinkTarget;
    u8 blinkCurrent;
    MailEdit *edit;
    MsgData *msgData;
    Font *font;
    u32 unk28[3];
    StrBuf *msgBuf;
    PaletteFade *palFade;
    MailSysModeFunc onKeyMode;
    MailSysModeFunc onTouchMode;
    BmpWin *windows[7];
    MailSysBmpWin entries[6];
    u8 unk90[8];
    PrintQueue *printQueue;
    PrintStream *printStream;
    ClActUnit *clUnit;
    u8 unkA4[0x50];
    void *tcbBuffer;
    TCBManager *tcbMgr;
    TCB *blinkTask;
    TCBExManager *tcbEx;
    TCB *vblankTask;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    AppTaskMenuItem menuItems[2];
    AppTaskMenuWin *confirmButton;
    AppTaskMenuWin *cancelButton;
    AppTaskMenuItem buttonItems[2];
    PMSDraw *pmsDraw;
    PMSData pmsWord;
    ClActUnit *pmsClUnit;
    PrintQueue *pmsQueue;
};

// The phases: the mail only shown, and the mail written
#define MAIL_PHASE_VIEW 0

#define MAIL_PHASE_WRITE 1

// pokebw2's MailSys_UpdateBlink
// The task that pulses the cursor's palette
void func_ov136_021c4b88(TCB *tcb, void *data);

#define HEAPID_MAIL 0x63
#define HEAPID_MAIL_SCREEN 0x64
#define MAIL_EDIT_DONE 4
#define MAIL_EDIT_CANCEL 0xffff

// pokebw2's MailSys_ProcInit
BOOL func_ov136_021c3e3c(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's MailSys_ProcExit
BOOL func_ov136_021c3f98(GameProc *proc, u32 *state, void *param, void *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MailSys_OnKeyMode
BOOL func_ov136_021c4534(MailSysWork *work);
// pokebw2's MailSys_OnTouchMode
BOOL func_ov136_021c4528(MailSysWork *work);
// pokebw2's func_ov136_021c3e9c
BOOL func_ov136_021c3e9c(GameProc *proc, u32 *state, void *param, void *work);

// The mail screen's proc, which the field app call runs
extern const GameProcFunctions MAILSYS_PROC_FUNCTIONS;

// The proc for the field app call's table (pokebw2's MailSys_GetProcFunctions): MWCC counts this read, ahead of the
// table's definition, as a reference, which puts the table among mailsys.c's other .rodata objects as in the ROM
static inline const GameProcFunctions *MailSys_GetProcFunctions(void) {
    return &MAILSYS_PROC_FUNCTIONS;
}

#endif // BWPORT_FIELD_MAILSYS_H
