#include "global.h"

struct LocalMapObject;

typedef struct LocalMapObject LocalMapObject;

void *sub_0205F3BC(LocalMapObject *mapObj);

int MapObject_CheckSingleMovement(const LocalMapObject *mapObj);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s8 unk_04;
    s8 unk_05;
    s8 unk_06;
    s8 unk_07;
    s8 unk_08;
} UnkStruct_0206762C;

int sub_02063A94(LocalMapObject *param0);

int sub_02063AC8(LocalMapObject *param0);

int sub_02063CB4(LocalMapObject *param0);

int sub_02063CB4(LocalMapObject *param0) {
    UnkStruct_0206762C *v0 = sub_0205F3BC(param0);

    switch (v0->unk_00) {
    case 0:
        if (sub_02063A94(param0) == 1) {
            v0->unk_00++;
        }

        break;
    case 1:
        if (sub_02063AC8(param0) == 0) {
            break;
        }

        v0->unk_02++;

        if (v0->unk_02 < v0->unk_03) {
            v0->unk_00 = 0;
            break;
        }

        v0->unk_00++;
    case 2:
        if (MapObject_CheckSingleMovement(param0) == 1) {
            break;
        }

        v0->unk_00++;
        v0->unk_02 = 0;
        v0->unk_01 = 0;
    case 3:
        return 1;
    }

    return 0;
}
