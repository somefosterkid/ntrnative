// port/include_plat/nitroWiFi/soc.h: NitroWiFi 2.1's spelling of the socket header (pret pokeplatinum's
// lib/gds), on the port's lib/nitrowifi: its SOC_* names (Black's lib/nitrowifi has these functions as func_ov009_*), as
// declarations only, since the online side is stubbed on the PC. The errors are Black's SOCL's.
#pragma once
// (#include_next: on a case-insensitive file system, macOS's, <nitrowifi/soc.h> is this file again)
#include_next <nitrowifi/soc.h>

#define SOC_PF_INET 2
#define SOC_SOCK_STREAM 1
#define SOC_F_GETFL 3
#define SOC_F_SETFL 4
#define SOC_O_NONBLOCK 4
#define SOC_EWOULDBLOCK (-6)
#define SOC_EINPROGRESS (-26)
#define SOC_EISCONN (-30)

int SOC_Socket(int domain, int type, int protocol);
int SOC_Connect(int sock, const SOCSockAddrIn *address);
int SOC_Send(int sock, const void *buffer, int size, int flags);
int SOC_Recv(int sock, void *buffer, int size, int flags);
int SOC_Shutdown(int sock, int how);
int SOC_Close(int sock);
int SOC_Fcntl(int sock, int command, int flags);
SOCHostEnt *SOC_GetHostByName(const char *host);
char *SOC_InetNtoA(SOCInAddr addr);
