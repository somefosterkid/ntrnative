// The geometry commands of our NitroSystem (lib/nnsys) on libntr. lib/nnsys writes them as the DS takes them: to the
// geometry FIFO (reg_G3X_GXFIFO = packed command word, then MIi_CpuSend32 of its parameters to the same port) and to
// the command ports (reg_G3_MTX_MODE = mode ...). On libntr those registers are plain variables: a store reaches
// nothing. port/include_nnsys/nnsys_lib.h (included only into lib/nnsys's own files) turns each register into a word
// of the list below, in the DS's packed format (a port write becomes its command word and its parameter), and
// MIi_CpuSend32 to the FIFO into words appended to it. The list goes to libntr's simulator as one command list
// (DRAW_CMD_G3_CMD_LIST, as a GX DMA's) before any other geometry traffic: libntr-nnsys.patch calls
// SIM_G3PendingHook at the top of SIM_HandleG3Command and in the G3X reads (clip and vector matrices, the stack
// levels, the test results), so the commands keep their order against libntr's own G3 functions and the reads see
// them done. The list only grows between those points (one command never splits across two lists).
#include <nitro.h>
#include <simulator/drawmsg.h>
#include <stdlib.h>
#include <string.h>

extern void (*SIM_G3PendingHook)(void);

static u32 *s_words;
static u32 s_count;
static u32 s_cap;
static u32 s_sink;  // the value written to a port whose command takes no parameter (the DS ignores it)

void PORT_NnsG3Flush(void);

static u32 *Stage(u32 n) {
    if (s_count + n > s_cap) {
        u32 cap = s_cap ? s_cap * 2 : 1024;
        while (cap < s_count + n) {
            cap *= 2;
        }
        s_words = realloc(s_words, cap * sizeof(u32));
        s_cap = cap;
    }
    if (s_count == 0) {
        SIM_G3PendingHook = PORT_NnsG3Flush;
    }
    u32 *w = &s_words[s_count];
    s_count += n;
    return w;
}

// Sends what is staged as one command list; SIM_HandleG3Command calls it first (the hook), so it comes back here
// with nothing staged
void PORT_NnsG3Flush(void) {
    if (s_count == 0) {
        return;
    }
    u32 bytes = s_count * sizeof(u32);
    u8 *list = malloc(bytes);  // the simulator frees a command list's data
    memcpy(list, s_words, bytes);
    s_count = 0;
    SIM_G3PendingHook = NULL;
    draw_msg_t msg;
    msg.type = DRAW_CMD_G3_CMD_LIST;
    msg.data.ptr = list;
    msg.size = bytes;
    SIM_HandleG3Command(&msg);
}

// reg_G3X_GXFIFO: the next word of the list
volatile u32 *PORT_NnsG3Fifo(void) {
    return Stage(1);
}

// reg_G3_<command>: the command's word, then its parameter (the commands lib/nnsys writes by port take at most one)
volatile u32 *PORT_NnsG3Port(u32 op, u32 numParams) {
    if (numParams == 0) {
        *Stage(1) = op;
        return &s_sink;
    }
    u32 *w = Stage(2);
    w[0] = op;
    return &w[1];
}

// MIi_CpuSend32 to &reg_G3X_GXFIFO (the destination expression staged a word, the last one: it is taken back) appends
// the words; any other destination is libntr's
void PORT_NnsCpuSend32(const void *src, volatile void *dest, u32 size) {
    if (s_count != 0 && dest == &s_words[s_count - 1]) {
        s_count--;
        u32 n = size / sizeof(u32);
        if (n != 0) {
            memcpy(Stage(n), src, n * sizeof(u32));
        }
        return;
    }
    MIi_CpuSend32(src, dest, size);
}

// The in-process restart (docs/launcher.md; port/src/machine.c with BWPORT_NNSYS): nothing staged survives it
void PORT_NnsReset(void) {
    s_count = 0;
    SIM_G3PendingHook = NULL;
}
