#include "global.h"

typedef int (*UnkFuncPtr_02032868)(void);
typedef u8 *(*UnkFuncPtr_0203290C)(int, void *, int);
typedef void (*UnkFuncPtr_020F8E60)(int, int, void *, void *);
typedef struct CommCmdTable {
    UnkFuncPtr_020F8E60 unk_00;
    UnkFuncPtr_02032868 unk_04;
    UnkFuncPtr_0203290C unk_08;
} CommCmdTable;
extern const CommCmdTable Unk_ov116_02267740[5];

int sub_02057798(void);

int sub_02057798(void) {
    return sizeof(Unk_ov116_02267740) / sizeof(CommCmdTable);
}
