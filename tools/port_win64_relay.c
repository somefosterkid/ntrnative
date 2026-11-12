// A stdio <-> TCP relay for debugging bwport.exe from WSL (tools/port_win64.sh gdb). Windows' gdbserver serves on a
// port only (no stdio), and WSL's NAT network can't reach the Windows host's 127.0.0.1 (nor its other addresses
// through the firewall), so gdb in WSL talks to gdbserver through this, which WSL starts as a Windows process:
//   (gdb) target remote | port_win64_relay.exe 2345
// Built by tools/port_win64.sh: x86_64-w64-mingw32-gcc -O2 -o port_win64_relay.exe port_win64_relay.c -lws2_32
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

static SOCKET sock;

static DWORD WINAPI StdinToSocket(LPVOID arg) {
    char buf[4096];
    DWORD n;
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

    (void)arg;
    while (ReadFile(in, buf, sizeof(buf), &n, NULL) && n > 0) {
        if (send(sock, buf, (int)n, 0) != (int)n) {
            break;
        }
    }
    shutdown(sock, SD_SEND);
    return 0;
}

int main(int argc, char **argv) {
    WSADATA wsa;
    struct sockaddr_in addr = {0};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[4096];
    int n, tries;

    if (argc < 2 || WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "usage: port_win64_relay PORT (connects to 127.0.0.1:PORT)\n");
        return 2;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // gdbserver may still be starting: try for 10 s
    for (tries = 0;; tries++) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            break;
        }
        closesocket(sock);
        if (tries == 100) {
            fprintf(stderr, "port_win64_relay: nothing on 127.0.0.1:%s\n", argv[1]);
            return 1;
        }
        Sleep(100);
    }
    CreateThread(NULL, 0, StdinToSocket, NULL, 0, NULL);
    while ((n = recv(sock, buf, sizeof(buf), 0)) > 0) {
        DWORD written;
        if (!WriteFile(out, buf, (DWORD)n, &written, NULL)) {
            break;
        }
    }
    return 0;
}
