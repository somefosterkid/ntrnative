#include "global.h"
#include "constants/heap.h"
#include "overlay_103_unk_struct_ov70_022630_a4_t.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"

s32 ov103_021EDA70(void *, s8, s32);

s32 ov103_021EE8A8(u8 *, s32);

void ov103_021ED93C(void *arg0);

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_022630A4_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);
typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;
typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;
void ov103_021EDA84(UnkStruct_ov70_022630A4 *param0, u32 param1);
void ov103_021ED954(UnkStruct_ov70_022630A4 *param0);

s32 ov103_021ED960(MailBoxWork *work);
s32 ov103_021EDA14(void *, s32);
void ov103_021ED97C(MailBoxWork *work);
s32 ov103_021EDE7C(void *);
s32 ov103_021EE860(u8 *param_1);
void ov103_021ED99C(void *arg0);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov103_021EDC68(void *, s8, s16, u32);
#else
s32 ov103_021EDC68(void *, s32, u16, u32);
#endif
s32 ov103_021ED9AC(MailBoxWork *work);

void ov103_021ED93C(void *arg0) {
    ov103_021EE8A8(arg0, 2);
    ov103_021EDA70(arg0, 0, 0x13);
}

void ov103_021ED954(UnkStruct_ov70_022630A4 *param0) {
    ov103_021EDA84(param0, 1);
}

s32 ov103_021ED960(MailBoxWork *work) {
    if (work->partyResult != 6) {
        work->nextState = 0x15;
        return 0;
    }
    work->childProc = 2;
    return 6;
}

void ov103_021ED97C(MailBoxWork *work) {
    ov103_021EE0CC(work->view, 2, 0);
    ov103_021EDB60(work->view, 1);
    ov103_021EDA14(work, 0x16);
}

void ov103_021ED99C(void *arg0) {
    ov103_021EE860(arg0);
    ov103_021EDE7C(arg0);
}

s32 ov103_021ED9AC(MailBoxWork *work) {
    if (work->mailGiven == 0) {
        work->nextState = 0x15;
        return 0;
    }
    ov103_021EDC68(work, 0xB, work->item, work->partyPos);
    work->childProc = 3;
    return 6;
}
