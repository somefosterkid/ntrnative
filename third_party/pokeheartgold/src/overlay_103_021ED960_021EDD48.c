#include "global.h"

#include "heap.h"
#include "overlay_103_mail_box_work.h"

s32 ov103_021EDD48(MailBoxWork *arg0);

s32 ov103_021EDD48(MailBoxWork *arg0) {
    Heap_Free(arg0->childParam);
    return 8;
}
