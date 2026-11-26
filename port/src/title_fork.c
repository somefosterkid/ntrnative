// The title snapshot (PLAN B2; tools/route.py, on unless ROUTE_SNAPSHOT=0; Linux only): a leg starts from a game that has already
// booted to the title instead of replaying the boot's ~6,300 (Platinum) / ~7,700 (HG/SS) frames.
//
// BWPORT_FORK_AT=N alone: at the game's V-blank wait N the process forks, and the game goes on in the child; the parent waits
// for it and ends with its status (the prototype: proves a forked game plays on exactly as one that never forked).
//
// BWPORT_FORK_SERVER=SOCK (with BWPORT_FORK_AT=N): the fork server. The exe the leg starts is only a client: before anything
// else (a constructor) it connects to SOCK and hands over its working directory, its environment and its stdin/stdout/stderr,
// waits, and exits with the leg's status. When nobody listens at SOCK it starts the server first (the same exe and arguments,
// in BWPORT_FORK_SERVER_DIR if set, SDL_VIDEODRIVER=offscreen and LP_NUM_THREADS=0: no X connection or raster thread to share
// with its children; its log is SOCK.log) and waits for it. The server boots to V-blank N and then serves: one fork per
// client; the child takes the client's stdio, environment and working directory, opens its read-only files again (its own
// offsets), reads its own save (PortGame.saveReload), moves the pilot into its own BWPORT_PILOT (PortRoute_PilotRebase) and
// plays the leg. The server answers each client with its child's exit status, kills a child whose client is gone, and ends
// after BWPORT_FORK_IDLE seconds (default 60) with no client. A server that does not come up in 120 s: the client boots by
// itself (said on stderr). One server per game and set of boot-time options: the caller picks SOCK by them (route.py).
//
// The child has only the forking thread (the game's, which also runs libntr's frame loop: SIM_Render is called from
// OS_WaitIrq), so the threads the game needs are made again: libntr's two PXI threads (a loop on a semaphore each: drained
// first, so none is inside a message), the ARM7's alarm threads that ran (pxi.c's win_snd_alarms) and every OS thread libntr
// made before the fork (libntr-osfork.patch: the ARM7's WM request thread, CARD's and CTRDG's task threads; all asleep at the
// fork, each restarted from its function; without the WM one a WM request after the fork was never answered and Platinum's
// comm system went another way, ver43 timed out). The game threads' own hosts are made again by main_threads.c. The sound callback is
// held off across the fork (ab_lock: it may not be inside acore then); the child plays no sound: SDL's audio device is closed
// before the fork (its thread, SDLAudioP1, runs the callback under SDL's audio lock: a fork while it held it left the lock taken
// for good in the child, whose first SIM_Audio_FrameTick then waited in SDL_LockAudio: the random hangs one frame after the
// fork), which the live clock does not need (sound time is the game's frames: docs/acore.md "The live clock"). SDL's timer
// thread is ended the same way (SDL_AddTimer in the child starts a new one).
// The child writes its pid to $BWPORT_FORK_PIDFILE: tools/port_run.py's frame watch follows it.
#include <nitro.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__linux__) && !defined(__ANDROID__)   // (POSIX headers only here: MinGW has no sys/wait.h)
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include "port_game.h"
#include "port_route.h"

// libntr (include/simulator/sim.h, libraries/sim7/src/pxi.c)
typedef struct PortSndAlarm {
    u32 alarmNo;
    u32 time;
    u32 period;
    u32 id;
    pthread_t thread;
    u8 isRunning;
} PortSndAlarm;
extern PortSndAlarm win_snd_alarms[32];
extern int SIM_procPxiThread(void *arg);
extern struct SDL_Thread *SIM_CreateHostThread(int (*fn)(void *), const char *name, void *data);   // libntr os_thread.c: 8 MB stack
extern int SIM_procPxi7to9Thread(void *arg);
extern void *SIM_sndAlarm(void *arg);
extern void SIM_PxiDrain(void);
extern int SIM_OsForkPrepare(void (*skip)(void *));   // libntr-osfork.patch: every OS thread asleep, the wait lock held over the fork
extern void SIM_OsForkParent(void);
extern int SIM_OsForked(void (*skip)(void *), char *names, size_t size, unsigned *waitMs);   // the child: the OS threads made again
void (*PORT_ThreadHostEntry(void))(void *);   // main_threads.c: its game threads' hosts (made again by PORT_ThreadsForked's caller)
extern void ab_lock(void);     // port/bridge/acore_bridge.c
extern void ab_unlock(void);
extern void bwport_vcore_fork_prepare(void);   // port/bridge/vcore_bridge.c: the frame's worker joined before the fork
extern void bwport_vcore_forked(void);         // and in the child: the renderer's thread pools start empty
extern void bwport_gpu_fork_prepare(void);     // port/bridge/gpu_bridge.c: the GPU path's stage-1 worker, the same
extern char **environ;

static char sStartDir[4096];   // the server's working directory at its start (the leg's is the client's)
void PORT_HangWatchChild(void);   // hang_watch.c
int PORT_ThreadsForked(void);     // main_threads.c: the game lock and the game threads' slots made again


// the process's threads by name (/proc/self/task/*/comm) into buf: the server's at its fork point, which its children lack
static void ThreadNames(char *buf, size_t size) {
    DIR *d = opendir("/proc/self/task");
    struct dirent *e;
    size_t used = 0;

    buf[0] = '\0';
    while (d != NULL && (e = readdir(d)) != NULL) {
        char path[96], name[64] = "";
        FILE *f;

        if (e->d_name[0] == '.') {
            continue;
        }
        snprintf(path, sizeof(path), "/proc/self/task/%s/comm", e->d_name);
        if ((f = fopen(path, "r")) != NULL) {
            if (fgets(name, sizeof(name), f) != NULL) {
                name[strcspn(name, "\n")] = '\0';
            }
            fclose(f);
        }
        used += (size_t)snprintf(buf + used, used < size ? size - used : 0, "%s%s", used ? ", " : "", name);
        if (used >= size) {
            break;
        }
    }
    if (d != NULL) {
        closedir(d);
    }
}

static int sForkBusy;   // OS threads not asleep at the fork (SIM_OsForkPrepare gave up after 1 s)

static int ForkNow(void) {
    pid_t pid;

    sForkBusy = SIM_OsForkPrepare(PORT_ThreadHostEntry());
    pid = fork();
    if (pid != 0) {
        SIM_OsForkParent();
    }
    return pid;
}

static void ChildThreads(void) {
    char osNames[512];
    unsigned osWait;
    int nos = SIM_OsForked(PORT_ThreadHostEntry(), osNames, sizeof(osNames), &osWait);   // (first: the OS locks made again)
    int i, n = 0, lost = PORT_ThreadsForked();
    const char *pidfile = getenv("BWPORT_FORK_PIDFILE");

    if (pidfile != NULL && *pidfile) {   // tools/port_run.py's frame watch reads this process's V-blank count from now on
        FILE *f = fopen(pidfile, "w");

        if (f != NULL) {
            fprintf(f, "%d\n", (int)getpid());
            fclose(f);
        }
    }
    bwport_vcore_forked();   // (the renderer's worker threads were the server's: a frame given to one never ended)
    SIM_CreateHostThread(SIM_procPxiThread, "PXI", NULL);
    SIM_CreateHostThread(SIM_procPxi7to9Thread, "PXI7to9", NULL);
    for (i = 0; i < 32; i++) {
        if (win_snd_alarms[i].isRunning) {
            pthread_create(&win_snd_alarms[i].thread, NULL, SIM_sndAlarm, &win_snd_alarms[i]);
            n++;
        }
    }
    PORT_HangWatchChild();
    fprintf(stderr, "fork: child %d at V-blank %u: the game lock, PXI threads, the hang watch and %d alarm threads made again "
            "(%d game threads lost); %d OS threads made again (%d not asleep at the fork, %u ms waited: %s)\n",
            (int)getpid(), (unsigned)OS_GetVBlankCount(), n, lost, nos, sForkBusy, osWait, osNames);
}

static int WriteAll(int fd, const void *buf, size_t n) {
    const char *p = buf;

    while (n > 0) {
        ssize_t w = write(fd, p, n);

        if (w < 0 && errno == EINTR) {
            continue;
        }
        if (w <= 0) {
            return -1;
        }
        p += w;
        n -= (size_t)w;
    }
    return 0;
}

static int ReadAll(int fd, void *buf, size_t n) {
    char *p = buf;

    while (n > 0) {
        ssize_t r = read(fd, p, n);

        if (r < 0 && errno == EINTR) {
            continue;
        }
        if (r <= 0) {
            return -1;
        }
        p += r;
        n -= (size_t)r;
    }
    return 0;
}

static int SockAddr(const char *path, struct sockaddr_un *a) {
    memset(a, 0, sizeof(*a));
    a->sun_family = AF_UNIX;
    if (strlen(path) >= sizeof(a->sun_path)) {
        fprintf(stderr, "fork: socket path too long: %s\n", path);
        return -1;
    }
    strcpy(a->sun_path, path);
    return 0;
}

// ---- the child: the leg's process, made from the server at the fork point

// every read-only regular file the server had open, opened again on the same descriptor at the same offset: a forked file
// description's offset is shared with the server's other children (/proc/self/fd/N also opens a file deleted since)
static int ReopenReadOnly(void) {
    DIR *d = opendir("/proc/self/fd");
    struct dirent *e;
    int n = 0;

    if (d == NULL) {
        return 0;
    }
    while ((e = readdir(d)) != NULL) {
        int fd = atoi(e->d_name), fl, nfd;
        struct stat st;
        char path[64];
        off_t off;

        if (e->d_name[0] < '0' || e->d_name[0] > '9' || fd <= 2 || fd == dirfd(d)) {
            continue;
        }
        fl = fcntl(fd, F_GETFL);
        if (fl < 0 || (fl & O_ACCMODE) != O_RDONLY || fstat(fd, &st) != 0 || !S_ISREG(st.st_mode)) {
            continue;
        }
        off = lseek(fd, 0, SEEK_CUR);
        snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
        nfd = open(path, fl);
        if (nfd < 0) {
            continue;
        }
        lseek(nfd, off, SEEK_SET);
        dup2(nfd, fd);
        fcntl(fd, F_SETFD, fcntl(nfd, F_GETFD));
        close(nfd);
        n++;
    }
    closedir(d);
    return n;
}

// msg: the client's working directory, then its environment, each string NUL-ended
static void Adopt(char *msg, size_t len, const int *fds) {
    char *p = msg, *end = msg + len, now[4096], dir[8192];
    size_t sl = strlen(sStartDir);
    int i, nf;

    for (i = 0; i < 3; i++) {
        dup2(fds[i], i);
        close(fds[i]);
    }
    clearenv();
    p += strlen(p) + 1;
    while (p < end) {   // (the strings live as long as the process: putenv keeps them)
        putenv(p);
        p += strlen(p) + 1;
    }
    // the server's own chdir since its start (the launcher's game folder) made again from the client's directory
    if (getcwd(now, sizeof(now)) == NULL) {
        now[0] = '\0';
    }
    snprintf(dir, sizeof(dir), "%s%s", msg, strncmp(now, sStartDir, sl) == 0 ? now + sl : "");
    if (chdir(dir) != 0) {
        fprintf(stderr, "fork: chdir %s failed: %s\n", dir, strerror(errno));
        _exit(126);
    }
    nf = ReopenReadOnly();
    ab_unlock();
    ChildThreads();
    if (PORT_Game->saveReload != NULL) {
        PORT_Game->saveReload();
    }
    PortRoute_PilotRebase(getenv("BWPORT_PILOT"));
    fprintf(stderr, "fork: leg in %s: %d read-only files opened again, save read again%s\n", dir, nf,
            PORT_Game->saveReload != NULL ? "" : " (none: this game has no saveReload)");
}

// ---- the server: parked at the fork point, one child per client

typedef struct {
    int conn;
    pid_t pid;
} Leg;

static int RecvLeg(int conn, char **msg, size_t *len, int *fds) {
    u32 n;
    struct msghdr mh;
    struct iovec iov = {&n, sizeof(n)};
    char cbuf[CMSG_SPACE(3 * sizeof(int))];
    struct cmsghdr *c;

    memset(&mh, 0, sizeof(mh));
    mh.msg_iov = &iov;
    mh.msg_iovlen = 1;
    mh.msg_control = cbuf;
    mh.msg_controllen = sizeof(cbuf);
    if (recvmsg(conn, &mh, MSG_WAITALL) != (ssize_t)sizeof(n)) {
        return -1;
    }
    c = CMSG_FIRSTHDR(&mh);
    if (c == NULL || c->cmsg_type != SCM_RIGHTS || c->cmsg_len != CMSG_LEN(3 * sizeof(int))) {
        return -1;
    }
    memcpy(fds, CMSG_DATA(c), 3 * sizeof(int));
    *msg = malloc(n + 1);
    if (*msg == NULL || ReadAll(conn, *msg, n) != 0) {
        return -1;
    }
    (*msg)[n] = '\0';
    *len = n;
    return 0;
}

static void Serve(const char *sock) {
    struct sockaddr_un a;
    Leg legs[256];
    int nlegs = 0, lfd, i, idle = 60;
    const char *e = getenv("BWPORT_FORK_IDLE");
    time_t last = time(NULL);
    const char *ow = getenv("BWPORT_FORK_CTL");   // route.py's pid: the server ends with it (it is its own session, no parent-death signal)
    pid_t ctl = ow != NULL ? (pid_t)atoi(ow) : 0;

    if (e != NULL && atoi(e) > 0) {
        idle = atoi(e);
    }
    lfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (lfd < 0 || SockAddr(sock, &a) != 0) {
        _exit(2);
    }
    unlink(sock);
    if (bind(lfd, (struct sockaddr *)&a, sizeof(a)) != 0 || listen(lfd, 64) != 0) {
        fprintf(stderr, "fork: server: cannot listen at %s: %s\n", sock, strerror(errno));
        _exit(2);
    }
    {   // SOCK.pid: the caller ends the server with it (route.py, when its legs are done)
        char pidf[4200];
        FILE *f;

        snprintf(pidf, sizeof(pidf), "%s.pid", sock);
        if ((f = fopen(pidf, "w")) != NULL) {
            fprintf(f, "%d\n", (int)getpid());
            fclose(f);
        }
    }
    {
        char names[2048];

        ThreadNames(names, sizeof(names));
        fprintf(stderr, "fork: server %d at V-blank %u listens at %s; its threads: %s\n", (int)getpid(),
                (unsigned)OS_GetVBlankCount(), sock, names);
    }
    fflush(stderr);
    for (;;) {
        struct pollfd pf[257];
        pid_t pid;
        int status;

        pf[0].fd = lfd;
        pf[0].events = POLLIN;
        for (i = 0; i < nlegs; i++) {
            pf[1 + i].fd = legs[i].conn;
            pf[1 + i].events = POLLIN;
        }
        poll(pf, 1 + nlegs, 100);
        if (ctl > 1 && kill(ctl, 0) != 0 && errno == ESRCH) {   // the controlling process is gone: no one will stop this server
            for (i = 0; i < nlegs; i++) {
                kill(legs[i].pid, SIGKILL);
            }
            unlink(sock);
            fprintf(stderr, "fork: server %d: controller %d gone, done\n", (int)getpid(), (int)ctl);
            _exit(0);
        }
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {   // a leg ended: its status to its client
            for (i = 0; i < nlegs; i++) {
                if (legs[i].pid == pid) {
                    int rc = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + (WIFSIGNALED(status) ? WTERMSIG(status) : 0);

                    WriteAll(legs[i].conn, &rc, sizeof(rc));
                    close(legs[i].conn);
                    legs[i] = legs[--nlegs];
                    pf[1 + i].revents = 0;
                    break;
                }
            }
        }
        for (i = 0; i < nlegs; i++) {   // a client gone (killed): its leg too
            if (pf[1 + i].fd == legs[i].conn && (pf[1 + i].revents & (POLLIN | POLLHUP | POLLERR))) {
                kill(legs[i].pid, SIGKILL);
            }
        }
        if (nlegs > 0) {
            last = time(NULL);
        }
        if (pf[0].revents & POLLIN) {
            int conn = accept(lfd, NULL, NULL), fds[3];
            char *msg;
            size_t len;

            if (conn >= 0) {
                fcntl(conn, F_SETFD, FD_CLOEXEC);
            }
            if (conn < 0) {
                continue;
            }
            last = time(NULL);
            if (nlegs >= 256 || RecvLeg(conn, &msg, &len, fds) != 0) {
                close(conn);
                continue;
            }
            fflush(NULL);
            pid = ForkNow();
            if (pid == 0) {
                close(lfd);
                for (i = 0; i < nlegs; i++) {
                    close(legs[i].conn);
                }
                close(conn);
                Adopt(msg, len, fds);
                return;   // the leg plays on from this V-blank
            }
            for (i = 0; i < 3; i++) {
                close(fds[i]);
            }
            free(msg);
            if (pid < 0) {
                close(conn);
                continue;
            }
            legs[nlegs].conn = conn;
            legs[nlegs].pid = pid;
            nlegs++;
        }
        if (nlegs == 0 && time(NULL) - last >= idle) {
            unlink(sock);
            fprintf(stderr, "fork: server %d: no client for %d s, done\n", (int)getpid(), idle);
            _exit(0);
        }
    }
}

// ---- the client: the leg's exe, before anything else runs

static int Connect(const char *sock) {
    struct sockaddr_un a;
    int fd;

    if (SockAddr(sock, &a) != 0) {
        return -1;
    }
    fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd >= 0 && connect(fd, (struct sockaddr *)&a, sizeof(a)) == 0) {
        return fd;
    }
    if (fd >= 0) {
        close(fd);
    }
    return -1;
}

static void StartServer(const char *sock, char **argv) {
    pid_t pid = fork();

    if (pid == 0) {
        setsid();
        if (fork() == 0) {
            char log[4200];
            const char *dir = getenv("BWPORT_FORK_SERVER_DIR");
            int fd;

            snprintf(log, sizeof(log), "%s.log", sock);
            fd = open(log, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                dup2(fd, 1);
                dup2(fd, 2);
            }
            fd = open("/dev/null", O_RDONLY);
            if (fd >= 0) {
                dup2(fd, 0);
            }
            if (dir != NULL && *dir && chdir(dir) != 0) {
                fprintf(stderr, "fork: server: chdir %s failed\n", dir);
                _exit(2);
            }
            setenv("BWPORT_FORK_ISSERVER", "1", 1);
            setenv("SDL_VIDEODRIVER", "offscreen", 1);
            setenv("LP_NUM_THREADS", "0", 1);
            setenv("BWPORT_HANGWATCH", "0", 1);   // (hang_watch.c: the server waits in Serve with no frames; each child is watched by its caller)
            unsetenv("BWPORT_FORK_PIDFILE");
            execv("/proc/self/exe", argv);
            _exit(127);
        }
        _exit(0);
    }
    if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
}

__attribute__((constructor(101))) static void ForkClient(int argc, char **argv) {
    const char *sock = getenv("BWPORT_FORK_SERVER");
    char lock[4200], cwd[4096], *buf, **e;
    size_t len;
    int fd = -1, lk, i, rc, fds[3] = {0, 1, 2};
    struct msghdr mh;
    struct iovec iov;
    char cbuf[CMSG_SPACE(3 * sizeof(int))];
    struct cmsghdr *c;
    u32 n;

    (void)argc;
    if (getcwd(sStartDir, sizeof(sStartDir)) == NULL) {
        sStartDir[0] = '\0';
    }
    if (sock == NULL || !*sock || getenv("BWPORT_FORK_ISSERVER") != NULL) {
        return;
    }
    snprintf(lock, sizeof(lock), "%s.lock", sock);
    lk = open(lock, O_RDWR | O_CREAT | O_CLOEXEC, 0644);
    if (lk >= 0) {
        flock(lk, LOCK_EX);   // one client starts the server; the others wait here and then find it
    }
    fd = Connect(sock);
    if (fd < 0) {
        StartServer(sock, argv);
        for (i = 0; i < 1200 && fd < 0; i++) {
            usleep(100000);
            fd = Connect(sock);
        }
    }
    if (lk >= 0) {
        close(lk);
    }
    if (fd < 0) {
        fprintf(stderr, "fork: no server at %s (its log: %s.log): this exe boots by itself\n", sock, sock);
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        cwd[0] = '\0';
    }
    len = strlen(cwd) + 1;
    for (e = environ; *e != NULL; e++) {
        len += strlen(*e) + 1;
    }
    buf = malloc(len);
    if (buf == NULL) {
        _exit(125);
    }
    n = 0;
    strcpy(buf, cwd);
    n += strlen(cwd) + 1;
    for (e = environ; *e != NULL; e++) {
        strcpy(buf + n, *e);
        n += strlen(*e) + 1;
    }
    memset(&mh, 0, sizeof(mh));
    iov.iov_base = &n;
    iov.iov_len = sizeof(n);
    mh.msg_iov = &iov;
    mh.msg_iovlen = 1;
    mh.msg_control = cbuf;
    mh.msg_controllen = sizeof(cbuf);
    c = CMSG_FIRSTHDR(&mh);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(fds));
    memcpy(CMSG_DATA(c), fds, sizeof(fds));
    if (sendmsg(fd, &mh, 0) != (ssize_t)sizeof(n) || WriteAll(fd, buf, n) != 0) {
        fprintf(stderr, "fork: the server at %s did not take the leg\n", sock);
        _exit(125);
    }
    if (ReadAll(fd, &rc, sizeof(rc)) != 0) {
        fprintf(stderr, "fork: the server at %s ended before the leg's status came\n", sock);
        _exit(125);
    }
    _exit(rc);
}

void PORT_ForkFrame(void) {
    static int sState;   // 0: not read yet, 1: on, -1: off or done
    static u32 sFrame;
    pid_t pid;
    int status = 0;
    const char *sock;

    if (sState == 0) {
        const char *e = getenv("BWPORT_FORK_AT");

        sFrame = e != NULL ? (u32)strtoul(e, NULL, 10) : 0;
        sState = sFrame > 0 ? 1 : -1;
    }
    if (sState <= 0 || OS_GetVBlankCount() < sFrame) {
        return;
    }
    sState = -1;
    SIM_PxiDrain();
    SDL_CloseAudio();   // (the header: no SDL-owned thread may hold an SDL lock at the fork)
    if (SDL_WasInit(SDL_INIT_TIMER)) {
        SDL_QuitSubSystem(SDL_INIT_TIMER);
    }
    bwport_vcore_fork_prepare();
    bwport_gpu_fork_prepare();
    ab_lock();
    fflush(NULL);
    sock = getenv("BWPORT_FORK_SERVER");
    if (sock != NULL && *sock && getenv("BWPORT_FORK_ISSERVER") != NULL) {
        Serve(sock);   // returns in a child only, its leg adopted
        return;
    }
    pid = ForkNow();
    if (pid < 0) {
        ab_unlock();
        fprintf(stderr, "fork: failed; the game goes on in this process\n");
        return;
    }
    if (pid == 0) {
        ab_unlock();
        ChildThreads();
        return;
    }
    fprintf(stderr, "fork: parent %d waits for child %d\n", (int)getpid(), (int)pid);
    fflush(stderr);
    while (waitpid(pid, &status, 0) < 0) {
    }
    _exit(WIFEXITED(status) ? WEXITSTATUS(status) : 128 + (WIFSIGNALED(status) ? WTERMSIG(status) : 0));
}
#else
void PORT_ForkFrame(void) {
}
#endif
