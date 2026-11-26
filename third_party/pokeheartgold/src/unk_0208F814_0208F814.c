#include "global.h"

struct CommCmdTable;

void sub_0208F828(int param0, int param1, void *param2, void *param3);

void sub_0208F8BC(int param0, int param1, void *param2, void *param3);

void sub_0208F8E4(int param0, int param1, void *param2, void *param3);

void sub_0208F918(int param0, int param1, void *param2, void *param3);

void sub_0208F960(int param0, int param1, void *param2, void *param3);

void sub_020960D4(int param0, int param1, void *param2, void *param3);

void sub_0208FA54(int param0, int param1, void *param2, void *param3);

void sub_0209610C(int param0, int param1, void *param2, void *param3);

void sub_02096110(int param0, int param1, void *param2, void *param3);

void sub_0208FA90(int param0, int param1, void *param2, void *param3);

void sub_0208FAA8(int param0, int param1, void *param2, void *param3);

typedef int (*UnkFuncPtr_02032868)(void);

typedef u8 *(*UnkFuncPtr_0203290C)(int, void *, int);

typedef void (*UnkFuncPtr_020F8E60)(int, int, void *, void *);

typedef struct CommCmdTable {
    UnkFuncPtr_020F8E60 unk_00;
    UnkFuncPtr_02032868 unk_04;
    UnkFuncPtr_0203290C unk_08;
} CommCmdTable;

void sub_0203410C(const CommCmdTable *param0, int param1, void *param2);

int CommPacketSizeOf_Nothing(void);

int CommPacketSizeOf_NetId(void);

void sub_02099510(int param0, int param1, void *param2, void *param3);

int sub_02091598(void);

int ov96_021ED748(void);

int sub_02099544(void);


int sub_020961E0(void);

int sub_020961E4(void);

void sub_0208FAEC(int param0, int param1, void *param2, void *param3);

extern const CommCmdTable _021054B4[110];

void sub_0208F814(void *param0);

void sub_0208F814(void *param0) {
    int v0 = sizeof(_021054B4) / sizeof(CommCmdTable);
    sub_0203410C(_021054B4, v0, param0);
}
