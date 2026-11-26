#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "heap.h"

extern u8 ov01_02209B70[4][0xF000];

MapLoadBuffers *ov01_021FAC44(const BOOL allocBDHCFiles);

void ov01_021FACF8(const unsigned int index, const int mapModelFileSize, const int bdhcFileSize, MapLoadBuffers *buffers);

MapLoadBuffers *ov01_021FAC44(const BOOL allocBDHCFiles) {
    u8 i;
    int bdhcFileSize;
    MapLoadBuffers *buffers = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapLoadBuffers));

    for (i = 0; i < 4; i++) {
        buffers->primary[i] = (void *)&ov01_02209B70[i];

        if (buffers->primary[i] == 0L) {
            return 0L;
        }

        if (allocBDHCFiles) {
            buffers->secondary[i] = Heap_Alloc(HEAP_ID_FIELD1, 0x9000);

            if (buffers->secondary[i] == 0L) {
                return 0L;
            }

            bdhcFileSize = 0x9000;
        } else {
            buffers->secondary[i] = 0L;
            bdhcFileSize = 0;
        }

        ov01_021FACF8(i, 0xF000, bdhcFileSize, buffers);
    }

    return buffers;
}
