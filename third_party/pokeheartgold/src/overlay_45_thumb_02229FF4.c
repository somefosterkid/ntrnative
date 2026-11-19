#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PasswordWordBank;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

UnkStruct_ov66_0223177C *ov45_0222D860(u32 heapID);

void ov45_0222E5D4(u32 heapID, SaveData *saveData, u32 param2, const UnkStruct_ov66_022324F0 *param3, void *param4);

void ov45_0222EE20(const UnkStruct_ov66_022589B4 *param0, u32 param1, void *param2);

typedef struct PasswordWordBank PasswordWordBank;

void ov45_0222B2B4(s32 param0, const void *param1, void *param2, BOOL param3);

void ov45_0222B470(s32 param0, void *param1);

void ov45_0222B530(s32 param0, const void *param1, void *param2);

void ov45_0222B5A0(PPW_LOBBY_TIME_EVENT param0, void *param1);

void ov45_0222B75C(const void *param0, u32 param1, void *param2);

void ov45_0222B8A0(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov45_0222BC3C(UnkStruct_ov66_0222FB64 *param0);

void ov45_0222BCC8(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov45_0222BD30(UnkStruct_ov66_0222FC58 *param0);

void ov45_0222BD40(UnkStruct_ov66_0222FC68 *param0);

void ov45_0222C388(UnkStruct_ov66_022302B0 *param0);

void ov45_0222C8AC(UnkStruct_ov66_022307D4 *param0);

void ov45_0222C978(UnkStruct_ov66_022308A0 *param0, u32 param1);

void ov45_0222CB44(UnkStruct_ov66_02230A6C *param0, const SaveData *saveData);

extern const UnkStruct_ov66_022589B4 ov45_02254AC4[8];

WiFiPlazaSystem *ov45_02229FF4(SaveData *saveData, enum HeapID heapID);

struct WiFiPlazaQuestion;
void ov45_0222AB0C(const WiFiPlazaProfile *profile, UnkStruct_ov66_0222E908 *param1);
void ov45_0222D890(UnkStruct_ov66_0223177C *param0);
void ov45_0222E688(void);
void ov45_0222EE80(void);
typedef struct WiFiPlazaQuestion WiFiPlazaQuestion;
WiFiPlazaQuestion *sub_020318E8(SaveData *saveData);
void sub_020318FC(WiFiPlazaQuestion *wiFiPlazaQuestion, u32 questionNumber);
void sub_02031900(WiFiPlazaQuestion *wiFiPlazaQuestion, u32 selectedAnswer);
void ov45_0222BCD8(UnkStruct_ov66_0222FBF0 *param0);
void ov45_0222C994(UnkStruct_ov66_022308A0 *param0);
void ov45_0222A0F0(WiFiPlazaSystem *param0);

WiFiPlazaSystem *ov45_02229FF4(SaveData *saveData, enum HeapID heapID) {
    WiFiPlazaSystem *v0 = Heap_Alloc(heapID, sizeof(WiFiPlazaSystem));
    memset(v0, 0, sizeof(WiFiPlazaSystem));

    v0->saveData = saveData;
    v0->heapID = heapID;

    {
        UnkStruct_ov66_022324F0 v1;

        v1.unk_00 = ov45_0222B2B4;
        v1.unk_04 = ov45_0222B470;
        v1.unk_08 = ov45_0222B530;
        v1.unk_0C = ov45_0222B5A0;
        v1.unk_10 = ov45_0222B75C;

        ov45_0222E5D4(heapID, v0->saveData, sizeof(WiFiPlazaProfile), &v1, v0);
    }

    {
        int v2;

        v0->unk_04.unk_00 = ov45_0222D860(heapID);

        for (v2 = 0; v2 < 4; v2++) {
            v0->unk_04.trainerInfos[v2] = PlayerProfile_New(heapID);
        }
    }

    ov45_0222B8A0(&v0->unk_108, v0->saveData, heapID);
    ov45_0222BD40(&v0->unk_1C0);
    ov45_0222BD30(&v0->unk_1E8);
    ov45_0222EE20(ov45_02254AC4, 8, v0);
    ov45_0222BC3C(&v0->unk_1FC);
    ov45_0222C388(&v0->unk_20C);
    ov45_0222C8AC(&v0->unk_3AB);
    ov45_0222C978(&v0->unk_3E4, heapID);
    ov45_0222CB44(&v0->unk_4BC, v0->saveData);
    ov45_0222BCC8(&v0->unk_508, heapID);

    return v0;
}

void ov45_0222A0F0(WiFiPlazaSystem *param0) {
    {
        WiFiPlazaQuestion *wiFiPlazaQuestion;
        UnkStruct_ov66_0222E908 v1;

        ov45_0222AB0C(&param0->unk_108.profile, &v1);

        wiFiPlazaQuestion = sub_020318E8(param0->saveData);

        sub_020318FC(wiFiPlazaQuestion, v1.questionNumber);
        sub_02031900(wiFiPlazaQuestion, v1.selectedAnswer);
    }

    ov45_0222BCD8(&param0->unk_508);
    ov45_0222C994(&param0->unk_3E4);

    {
        ov45_0222EE80();
    }

    {
        int v2;

        ov45_0222D890(param0->unk_04.unk_00);

        for (v2 = 0; v2 < 4; v2++) {
            Heap_Free(param0->unk_04.trainerInfos[v2]);
        }
    }

    ov45_0222E688();
    Heap_Free(param0);
}
