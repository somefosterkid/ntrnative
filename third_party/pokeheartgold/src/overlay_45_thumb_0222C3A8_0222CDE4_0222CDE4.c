#include "global.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveData;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_02230C90_t;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UnkStruct_ov66_02230C90_t UnkStruct_ov66_02230C90;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct {
    void *unk_00;
} UnkStruct_ov66_02230E4C;

struct UnkStruct_ov66_02230C90_t {
    OverlayManager *appMan;
    UnkStruct_ov66_02230E4C unk_04[13];
    u8 unk_38;
    u8 unk_39;
    u16 heapID;
    WiFiPlazaSystem *unk_3C;
    SaveData *saveData;
    UnkStruct_ov66_0222DCE0_sub1 *unk_44;
};

void ov45_0222CF00(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222CFF4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_02230FA0(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D0FC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_0223108C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_02231134(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_022311D4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_0223127C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_02231300(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222D44C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov66_022313AC(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, u32 heapID);

void ov45_0222CDE4(UnkStruct_ov66_02230C90 *param0, u32 param1, u32 heapID);

void ov45_0222CDE4(UnkStruct_ov66_02230C90 *param0, u32 param1, u32 heapID) {
    extern void (*ov45_02254E88[])(UnkStruct_ov66_02230C90 *, UnkStruct_ov66_02230E4C *, u32);

    ((param1 < 13) ? (void)0 : GF_AssertFail());
    ov45_02254E88[param1](param0, &param0->unk_04[param1], heapID);
}

void ov66_02230E4C(UnkStruct_ov66_02230E4C *param0);

void ov66_02230F44(UnkStruct_ov66_02230E4C *param0);

void ov66_02230FE4(UnkStruct_ov66_02230E4C *param0);

void ov66_02231058(UnkStruct_ov66_02230E4C *param0);

void ov66_022310C4(UnkStruct_ov66_02230E4C *param0);

void ov66_02231164(UnkStruct_ov66_02230E4C *param0);

void ov66_0223120C(UnkStruct_ov66_02230E4C *param0);

void ov66_022312A8(UnkStruct_ov66_02230E4C *param0);

void ov66_02231338(UnkStruct_ov66_02230E4C *param0);

void ov66_022313E8(UnkStruct_ov66_02230E4C *param0);

void ov45_0222CE0C(UnkStruct_ov66_02230C90 *param0, u32 param1);

void ov45_0222CE0C(UnkStruct_ov66_02230C90 *param0, u32 param1) {
    extern void (*_02254E20[])(UnkStruct_ov66_02230E4C *);

    ((param1 < 13) ? (void)0 : GF_AssertFail());
    _02254E20[param1](&param0->unk_04[param1]);
}

void ov66_02230E68(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02230F50(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02230FF0(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02231064(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_022310D8(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02231178(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02231220(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov45_0222D3B0(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov45_0222D3C4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02231344(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_02231350(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov66_022313F4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

void ov45_0222CE2C(UnkStruct_ov66_02230C90 *param0, u32 param1);

void ov45_0222CE2C(UnkStruct_ov66_02230C90 *param0, u32 param1) {
    extern void (*ov45_02254E54[])(UnkStruct_ov66_02230C90 *, UnkStruct_ov66_02230E4C *);

    ((param1 < 13) ? (void)0 : GF_AssertFail());
    ov45_02254E54[param1](param0, &param0->unk_04[param1]);

    param0->unk_38 = param1;
}

int ov66_02230E90(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02230F7C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02231000(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02231074(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02231104(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_022311A4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_0223124C(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_022312B4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02231370(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov66_02231404(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1);

int ov45_0222CE54(UnkStruct_ov66_02230C90 *param0, u32 param1);

int ov45_0222CE54(UnkStruct_ov66_02230C90 *param0, u32 param1) {
    extern int (*ov45_02254EBC[])(UnkStruct_ov66_02230C90 *, UnkStruct_ov66_02230E4C *);

    ((param1 < 13) ? (void)0 : GF_AssertFail());
    return ov45_02254EBC[param1](param0, &param0->unk_04[param1]);
}
