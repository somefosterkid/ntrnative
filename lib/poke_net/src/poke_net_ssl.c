// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/poke_net/src/poke_net_ssl.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The SSL settings of the GDS client's connection: the certificate authority that the server's certificate must come
// from, and the settings of a connection, which the socket library reads. The ROM names nothing here: the file's name
// and its functions' are guesses.

#include "types.h"
#include "poke_net_internal.h"
#include "dwc/ext.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/std.h"

static int func_ov125_021d8b3c(int result, POKE_NET_SSL_CONNECTION *connection, int level);
void func_ov125_021d8bc0(POKE_NET_SSL_CONNECTION *connection);
void func_ov125_021d8be4(POKE_NET_SSL_AUTH_CALLBACK callback);
void func_ov125_021d8bf8(POKE_NET_SSL_CA_INFO **caInfo, int count);

// The public key of Nintendo of America's certificate authority: the exponent 65537 and the modulus
// pokebw2's sPOKE_NET_SSL_CaExponent
static u8 data_ov125_021d8d34[] = {0x01, 0x00, 0x01};

// pokebw2's sPOKE_NET_SSL_CaModulo
static u8 data_ov125_021d8da8[] = {
    0xb4, 0xa8, 0x0f, 0xe3, 0x80, 0x02, 0xa7, 0xad, 0xfb, 0x59, 0x9d, 0xe1, 0x92, 0x9d, 0x19, 0x7b,
    0x62, 0x00, 0x96, 0xce, 0x13, 0x2e, 0x5b, 0x83, 0xc0, 0x33, 0x26, 0xa6, 0x39, 0x34, 0xac, 0x8e,
    0xe7, 0x7f, 0x69, 0x7e, 0xd4, 0xc0, 0x7c, 0xb7, 0x60, 0x86, 0xaf, 0xb6, 0x45, 0x06, 0x36, 0xe4,
    0x5d, 0xea, 0xed, 0x15, 0x54, 0x36, 0x1d, 0x57, 0xb6, 0x9e, 0x0e, 0x07, 0x43, 0x62, 0x10, 0xee,
    0x35, 0xc2, 0x41, 0x34, 0x38, 0xd0, 0xdd, 0x52, 0xae, 0x86, 0xfd, 0x0c, 0x9d, 0x29, 0xfa, 0x3e,
    0xb6, 0xbb, 0xd9, 0x51, 0x38, 0x26, 0x24, 0x87, 0xa2, 0x20, 0x37, 0x4e, 0x8e, 0xbc, 0xd1, 0x04,
    0x5b, 0x98, 0x18, 0x7e, 0xa8, 0x54, 0xfd, 0xd0, 0xbf, 0xa6, 0x1b, 0x48, 0x0e, 0xe6, 0x19, 0xb4,
    0xdf, 0xc4, 0x3f, 0x8a, 0x19, 0xce, 0xa0, 0x14, 0x23, 0xb1, 0x47, 0x12, 0x2c, 0x1e, 0x8c, 0xa7,
};

// pokebw2's sPOKE_NET_SSL_CaDn
static char data_ov125_021d8d50[] =
    "US, Washington, Nintendo of America Inc., NOA, Nintendo Class 2 CA, ca@noa.nintendo.com";

// pokebw2's sPOKE_NET_SSL_CaInfo
static POKE_NET_SSL_CA_INFO data_ov125_021d8d3c = {
    data_ov125_021d8d50,
    sizeof(data_ov125_021d8da8),
    data_ov125_021d8da8,
    sizeof(data_ov125_021d8d34),
    data_ov125_021d8d34,
};

// pokebw2's sPOKE_NET_SSL_CaInfoList
static POKE_NET_SSL_CA_INFO *data_ov125_021d8d38[] = {
    &data_ov125_021d8d3c,
};

// The settings that connections start from: the default ones unless others were set
// pokebw2's sPOKE_NET_SSL_DefaultConnection
static POKE_NET_SSL_CONNECTION data_ov125_021daaac;
// pokebw2's sPOKE_NET_SSL_Entropy
static u32 data_ov125_021daa8c[8];
// pokebw2's sPOKE_NET_SSL_Connection
static POKE_NET_SSL_CONNECTION *data_ov125_021daa88;

// Lets every certificate that the library's own check let through pass
// pokebw2's POKE_NET_SSL_AuthCallback
static int func_ov125_021d8b3c(int result, POKE_NET_SSL_CONNECTION *connection, int level) {
    return 0;
}

void func_ov125_021d8b44(void) {
    OS_GetLowEntropyData(data_ov125_021daa8c);
    func_ov009_02162248(data_ov125_021daa8c, 0x20);
    if (data_ov125_021daa88 != NULL) {
        return;
    }
    MI_CpuFill8(&data_ov125_021daaac, 0, sizeof(POKE_NET_SSL_CONNECTION));
    data_ov125_021daaac.ServerName = NULL;
    func_ov125_021d8bc0(&data_ov125_021daaac);
    func_ov125_021d8bf8(data_ov125_021d8d38, 1);
    func_ov125_021d8be4(func_ov125_021d8b3c);
}


void func_ov125_021d8bc0(POKE_NET_SSL_CONNECTION *connection) {
    if (connection != NULL) {
        data_ov125_021daa88 = connection;
    } else {
        data_ov125_021daa88 = &data_ov125_021daaac;
    }
}


void func_ov125_021d8be4(POKE_NET_SSL_AUTH_CALLBACK callback) {
    data_ov125_021daa88->AuthCallback = callback;
}

void func_ov125_021d8bf8(POKE_NET_SSL_CA_INFO **caInfo, int count) {
    data_ov125_021daa88->CaInfo = caInfo;
    data_ov125_021daa88->CaInfoCount = count;
}

void func_ov125_021d8c14(const char *serverName, POKE_NET_SSL_CONNECTION *connection) {
    MI_CpuCopy8(data_ov125_021daa88, connection, sizeof(POKE_NET_SSL_CONNECTION));
    if (serverName == NULL) {
        return;
    }
    connection->ServerName = func_ov125_021d7c7c(STD_GetStringLength(serverName) + 1);
    STD_CopyString(connection->ServerName, serverName);
}


void func_ov125_021d8c5c(POKE_NET_SSL_CONNECTION *connection) {
    if (connection->ServerName != NULL) {
        func_ov125_021d7c90(connection->ServerName);
        connection->ServerName = NULL;
    }
}




