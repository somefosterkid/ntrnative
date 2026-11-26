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
    SaveData *saveData;
    WiFiPlazaSystem *unk_04;
    BOOL unk_08;
} UnkStruct_ov66_02231300;


int ov46_02258DA8(OverlayManager *appMan, int *param1);


FS_EXTERN_OVERLAY(overlay67);

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

void ov45_0222CDC4(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, const OverlayManagerTemplate *param2);

void ov45_0222D434(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, BOOL param2);

void ov45_0222D434(UnkStruct_ov66_02230C90 *param0, UnkStruct_ov66_02230E4C *param1, BOOL param2) {
    FS_EXTERN_OVERLAY(overlay67);

    extern const OverlayManagerTemplate ov45_02254B04;

    {
        UnkStruct_ov66_02231300 *v1;

        v1 = param1->unk_00;
        v1->unk_08 = param2;
    }

    ov45_0222CDC4(param0, param1, &ov45_02254B04);
}
