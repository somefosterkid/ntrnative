// An integer division by zero does what the DS does, not crash (x86-64: Linux, Android's emulator, Windows).
//
// The game's C divides by zero on the DS too, quietly: MWCC compiles '/' and '%' to calls of _s32_div_f / _u32_div_f
// (Black's ARM9 0x0209c0a4 / 0x0209c2b0), which return at once when the divisor is 0 with r0 (the quotient) still the
// dividend and r1 (the remainder) 0. An x86 div / idiv traps instead (#DE, SIGFPE on Linux, EXCEPTION_INT_DIVIDE_BY_ZERO on
// Windows). Found 2026-11-13 in Elesa's battle (route leg b gym4-e): a battle effect's particle resource has a particle
// life time of 1 and a child resource, so each parent particle lives 1 frame and func_02056684 (spl_emit.c) divides
// 0xffff by lifeTime / 2 = 0 for each child it emits; the DS gets 0xffff. Which moves the battle shows depends on the
// environment's length (the route notes), so the leg crashed in one checkout's path and passed in another.
//
// The handler gives the trapping instruction the DS's result and steps over it: the quotient register keeps the dividend
// (AL, AX, EAX zero-extended, RAX) and the remainder register (AH, DX, EDX, RDX) is 0. The other #DE, INT_MIN / -1, gets
// the same: _s32_div_f returns INT_MIN with remainder 0 there. The first few are logged ("bwport: DS division by zero").
// An instruction it cannot decode (not F6/F7 /6 or /7) is left to crash as before. ARM64 (Mac, iOS, Android phones) does
// not trap: sdiv/udiv by 0 give 0 there, not the DS's dividend (no crash; a value that differs from the DS's).
#if defined(__x86_64__) && (defined(__linux__) || defined(_WIN32))
#include <stdint.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>
#ifndef REG_RIP   // glibc names them only with _GNU_SOURCE, which the build's prefix header has already decided
#define REG_RDX 12
#define REG_RAX 13
#define REG_RIP 16
#endif
#endif

static volatile int sDivLogged;

// The length of the div / idiv at pc and its operand size in bytes (1, 2, 4, 8); 0 when pc holds no div or idiv
static int DivDecode(const uint8_t *pc, int *size) {
    const uint8_t *p = pc;
    int opsize16 = 0;
    int rexW = 0;
    uint8_t modrm;
    int mod;
    int rm;
    int len;

    for (;;) {   // legacy prefixes
        uint8_t b = *p;

        if (b == 0x66) {
            opsize16 = 1;
        } else if (!(b == 0x67 || b == 0xf0 || b == 0xf2 || b == 0xf3 || b == 0x2e || b == 0x36 || b == 0x3e ||
                     b == 0x26 || b == 0x64 || b == 0x65)) {
            break;
        }
        p++;
        if (p - pc > 14) {
            return 0;
        }
    }
    if ((*p & 0xf0) == 0x40) {   // REX
        rexW = (*p & 8) != 0;
        p++;
    }
    if (*p != 0xf6 && *p != 0xf7) {
        return 0;
    }
    *size = *p == 0xf6 ? 1 : rexW ? 8 : opsize16 ? 2 : 4;
    p++;
    modrm = *p++;
    if (((modrm >> 3) & 7) < 6) {   // /6 div, /7 idiv
        return 0;
    }
    mod = modrm >> 6;
    rm = modrm & 7;
    len = (int)(p - pc);
    if (mod != 3) {
        if (rm == 4) {   // SIB
            uint8_t sib = *p;

            len++;
            if (mod == 0 && (sib & 7) == 5) {
                len += 4;
            }
        } else if (mod == 0 && rm == 5) {   // RIP-relative
            len += 4;
        }
        if (mod == 1) {
            len += 1;
        } else if (mod == 2) {
            len += 4;
        }
    }
    return len;
}

// The DS's result in the registers: quotient = dividend, remainder = 0
static void DivResult(int size, uint64_t *rax, uint64_t *rdx) {
    switch (size) {
    case 1:
        *rax &= ~(uint64_t)0xff00;   // AH = 0, AL keeps the dividend's low byte
        break;
    case 2:
        *rdx &= ~(uint64_t)0xffff;
        break;
    case 4:
        *rax &= 0xffffffffu;   // a 32-bit write zero-extends
        *rdx = 0;
        break;
    default:
        *rdx = 0;
        break;
    }
}

static void DivLog(uint64_t pc) {
    char msg[96] = "bwport: DS division by zero at 0x";
    char *s = msg + strlen(msg);
    int i;

    if (sDivLogged >= 8) {
        return;
    }
    sDivLogged++;
    for (i = 60; i >= 0; i -= 4) {
        *s++ = "0123456789abcdef"[(pc >> i) & 15];
    }
    memcpy(s, " (quotient = dividend, remainder 0)\n", 37);
#ifdef _WIN32
    {
        DWORD n;

        WriteFile(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)strlen(msg), &n, NULL);
    }
#else
    (void)!write(2, msg, strlen(msg));
#endif
}

#ifdef _WIN32
static LONG CALLBACK DivHandler(EXCEPTION_POINTERS *e) {
    CONTEXT *c = e->ContextRecord;
    DWORD code = e->ExceptionRecord->ExceptionCode;
    int size;
    int len;

    if (code != EXCEPTION_INT_DIVIDE_BY_ZERO && code != EXCEPTION_INT_OVERFLOW) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    len = DivDecode((const uint8_t *)c->Rip, &size);
    if (len == 0) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    DivLog(c->Rip);
    DivResult(size, (uint64_t *)&c->Rax, (uint64_t *)&c->Rdx);
    c->Rip += len;
    return EXCEPTION_CONTINUE_EXECUTION;
}

__attribute__((constructor)) static void DivInit(void) {
    AddVectoredExceptionHandler(1, DivHandler);
}
#else
static void DivHandler(int sig, siginfo_t *info, void *uctx) {
    ucontext_t *uc = uctx;
    greg_t *g = uc->uc_mcontext.gregs;
    int size;
    int len = 0;

    (void)sig;
    if (info->si_code == FPE_INTDIV || info->si_code == FPE_INTOVF) {
        len = DivDecode((const uint8_t *)g[REG_RIP], &size);
    }
    if (len == 0) {   // not a division: crash as before (the default action when the instruction runs again)
        signal(SIGFPE, SIG_DFL);
        return;
    }
    DivLog((uint64_t)g[REG_RIP]);
    DivResult(size, (uint64_t *)&g[REG_RAX], (uint64_t *)&g[REG_RDX]);
    g[REG_RIP] += len;
}

__attribute__((constructor)) static void DivInit(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = DivHandler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGFPE, &sa, NULL);
}
#endif
#endif

// MWCC's signed division helper, called by name where the decompiled C kept the call (Black's ov060 Funfest levels,
// ov118 trade demo; the map sweep: the Entralink zone stopped on "undefined function _s32_div_f"): the quotient in the
// low word, the remainder in the high one, as _u32_div_f (stub_b2w2.c). The DS's corner cases: a divisor of 0 gives the
// dividend and remainder 0, INT_MIN / -1 gives INT_MIN and 0 (the header above)
unsigned long long _s32_div_f(int dividend, int divisor) {
    int q = dividend, r = 0;

    if (divisor != 0 && !(divisor == -1 && dividend == (int)0x80000000)) {
        q = dividend / divisor;
        r = dividend % divisor;
    }
    return (unsigned long long)(unsigned)q | (unsigned long long)(unsigned)r << 32;
}
