#include "global.h"

#include "string_util.h"
#include "unk_02031B0C.h"

s32 sub_02032158(ApricornRecord *arg0, ApricornRecord *arg1);

s32 sub_02032158(ApricornRecord *arg0, ApricornRecord *arg1) {
    if ((s32)arg0->trainerId != (s32)arg1->trainerId) {
        return 0;
    }
    if (arg0->gender != arg1->gender) {
        return 0;
    }
    if (arg0->version != arg1->version) {
        return 0;
    }
    if (arg0->language != arg1->language) {
        return 0;
    }
    if (StringNotEqual(arg0->name, arg1->name) == 0) {
        return 1;
    }
    return 0;
}
