#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_01_021F4704.h"

void ov01_021F49FC(NARC *narc, u32 fileId, u32 *header) {
    u32 *buf;

    if (narc != NULL) {
        buf = Heap_AllocAtEnd(HEAP_ID_FIELD1, 0x10U);
        NARC_ReadFromMember(narc, fileId, 0U, 0x10U, buf);
    } else {
        GF_AssertFail();
        buf = AllocAtEndAndReadFromNarcMemberByIdPair(NARC_fielddata_landdata_land_data, (s32)fileId, HEAP_ID_FIELD1, 0U, 0x10U);
    }
    header[0] = buf[0];
    header[3] = buf[1];
    header[1] = buf[2];
    header[2] = buf[3];
    Heap_Free(buf);
}
