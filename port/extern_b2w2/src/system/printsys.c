// Black 2's and White 2's text printer (pokebw2's src/system/printsys.c compiled through this file). Test only: the route
// pilot's Wt token (port/src/route_test.c) waits until no text stream is printing; the wrappers keep the live streams in a
// short list (made by PrintStream_Create*, gone at PrintStream_Delete). The DS build never sees this file.
#define PrintStream_Create PrintStream_Create_game
#define PrintStream_CreateWithCallback PrintStream_CreateWithCallback_game
#define PrintStream_Delete PrintStream_Delete_game
#include "../../../../third_party/pokebw2/src/system/printsys.c"
#undef PrintStream_Create
#undef PrintStream_CreateWithCallback
#undef PrintStream_Delete

#include "port_route.h"

#define PORT_STREAMS 16
static PrintStream *sPortStreams[PORT_STREAMS];

// the streams now printing (PRINT_STREAM_RUNNING): route_test.c's Wt
int PortRoute_TextPrinting(void) {
    int n = 0;

    for (int i = 0; i < PORT_STREAMS; i++) {
        if (sPortStreams[i] != NULL && sPortStreams[i]->task != NULL && sPortStreams[i]->state == PRINT_STREAM_RUNNING) {
            n++;
        }
    }
    return n;
}

PrintStream *PrintStream_CreateWithCallback(BmpWin *window, s16 x, s16 y, const StrBuf *strbuf, Font *font, s32 wait,
                                            TCBExManager *tcbManager, u32 priority, HeapID heapId, u16 backgroundColor,
                                            PrintStreamCallback callback) {
    static int sNext;
    PrintStream *stream = PrintStream_CreateWithCallback_game(window, x, y, strbuf, font, wait, tcbManager, priority,
                                                              heapId, backgroundColor, callback);
    int i;

    for (i = 0; i < PORT_STREAMS && sPortStreams[i] != NULL; i++) {
    }
    if (i == PORT_STREAMS) {   // full (a stream never deleted): the oldest slot goes
        i = sNext;
        sNext = (sNext + 1) % PORT_STREAMS;
    }
    sPortStreams[i] = stream;
    return stream;
}

PrintStream *PrintStream_Create(BmpWin *window, s16 x, s16 y, const StrBuf *strbuf, Font *font, s32 wait,
                                TCBExManager *tcbManager, u32 priority, HeapID heapId, u16 backgroundColor) {
    return PrintStream_CreateWithCallback(window, x, y, strbuf, font, wait, tcbManager, priority, heapId,
                                          backgroundColor, NULL);
}

void PrintStream_Delete(PrintStream *stream) {
    for (int i = 0; i < PORT_STREAMS; i++) {
        if (sPortStreams[i] == stream) {
            sPortStreams[i] = NULL;
        }
    }
    PrintStream_Delete_game(stream);
}
