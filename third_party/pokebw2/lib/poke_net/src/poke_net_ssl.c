// The SSL settings of the GDS client's connection: CA records used by certificate checking and the template from
// which the socket library's connection starts. The ROM names nothing here: the file's name and its functions' are
// guesses.

#include "types.h"
#include "dwc/ext.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "poke_net_internal.h"

static int POKE_NET_SSL_AuthCallback(int result, CPSSslConnection *connection, int level);

// The public key of Nintendo of America's certificate authority: the exponent 65537 and the modulus
static u8 sPOKE_NET_SSL_CaExponent[] = { 0x01, 0x00, 0x01 };

static u8 sPOKE_NET_SSL_CaModulo[] = {
#include "asset/poke_net_ssl_sPOKE_NET_SSL_CaModulo.inc"
};

static char sPOKE_NET_SSL_CaDn[] = {
#include "asset/poke_net_ssl_sPOKE_NET_SSL_CaDn.inc"
};

static CPSCaInfo sPOKE_NET_SSL_CaInfo = {
    sPOKE_NET_SSL_CaDn,       sizeof(sPOKE_NET_SSL_CaModulo), sPOKE_NET_SSL_CaModulo, sizeof(sPOKE_NET_SSL_CaExponent),
    sPOKE_NET_SSL_CaExponent,
};

static CPSCaInfo *sPOKE_NET_SSL_CaInfoList[] = {
    &sPOKE_NET_SSL_CaInfo,
};

// The settings that connections start from: the default ones unless others were set
static CPSSslConnection sPOKE_NET_SSL_DefaultConnection;
static u32 sPOKE_NET_SSL_Entropy[8];
static CPSSslConnection *sPOKE_NET_SSL_Connection;

// Returns zero for every supplied certificate-check result; SSL replaces its result with this return value.
// Earlier parser failures exit before this callback.
static int POKE_NET_SSL_AuthCallback(int result, CPSSslConnection *connection, int level) {
    return 0;
}

// Seeds the random numbers of the secure connections, and makes the default settings the first time
void POKE_NET_SSL_Setup(void) {
    OS_GetLowEntropyData(sPOKE_NET_SSL_Entropy);
    CPS_SslAddRandomSeed(sPOKE_NET_SSL_Entropy, sizeof(sPOKE_NET_SSL_Entropy));
    if (sPOKE_NET_SSL_Connection != NULL) {
        return;
    }
    MI_CpuFill8(&sPOKE_NET_SSL_DefaultConnection, 0, sizeof(CPSSslConnection));
    sPOKE_NET_SSL_DefaultConnection.hostname = NULL;
    POKE_NET_SSL_SetConnection(&sPOKE_NET_SSL_DefaultConnection);
    POKE_NET_SSL_SetCaInfo(sPOKE_NET_SSL_CaInfoList, NELEMS(sPOKE_NET_SSL_CaInfoList));
    POKE_NET_SSL_SetAuthCallback(POKE_NET_SSL_AuthCallback);
}

// Retains the supplied template for later StartConnection calls; NULL selects the static default.
void POKE_NET_SSL_SetConnection(CPSSslConnection *connection) {
    if (connection != NULL) {
        sPOKE_NET_SSL_Connection = connection;
    } else {
        sPOKE_NET_SSL_Connection = &sPOKE_NET_SSL_DefaultConnection;
    }
}

void POKE_NET_SSL_SetAuthCallback(POKE_NET_SSL_AUTH_CALLBACK callback) {
    sPOKE_NET_SSL_Connection->validate_certificate = callback;
}

// Borrows the list, records and key bytes; stores pointer/count without copying or validating their contents.
void POKE_NET_SSL_SetCaInfo(CPSCaInfo **caInfo, int count) {
    sPOKE_NET_SSL_Connection->named_params = caInfo;
    sPOKE_NET_SSL_Connection->named_param_count = count;
}

// Copies the whole template, retaining its pointer fields without duplicating their storage.
// A non-NULL serverName replaces the copied name pointer with a separately allocated copy.
void POKE_NET_SSL_StartConnection(const char *serverName, CPSSslConnection *connection) {
    MI_CpuCopy8(sPOKE_NET_SSL_Connection, connection, sizeof(CPSSslConnection));
    if (serverName == NULL) {
        return;
    }
    connection->hostname = POKE_NET_AllocFunc(STD_GetStringLength(serverName) + 1);
    STD_CopyString(connection->hostname, serverName);
}

void POKE_NET_SSL_EndConnection(CPSSslConnection *connection) {
    if (connection->hostname == NULL) {
        return;
    }
    POKE_NET_FreeFunc(connection->hostname);
    connection->hostname = NULL;
}
