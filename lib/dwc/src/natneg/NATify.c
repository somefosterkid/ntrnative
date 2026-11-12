// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/natneg/NATify.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include <string.h>
// Before dwc/ext.h: MWCC builds func_ov009_02198a0c's copies of the servers' packets in place, as the ROM does, only when <string.h>
// declares memcpy
#include "dwc/NATify.h"
#include "dwc/ext.h"
#include "dwc/natneg.h"
#include "dwc/gsDebug.h"
#include "dwc/gsPlatformSocket.h"
#include "gs/nonport.h"
#include "arith.h"
#include "mem.h"

// GameSpy's NAT detection (NATify.c). The ROM names neither the file nor its functions: the file, func_ov009_02198850,
// GetLocalIP, func_ov009_02198a0c, func_ov009_02198e74 and func_ov009_02198e80 are GameSpy's, as pret's pokerevo Wii build has them
// (NATify.s), and GetLocalPort, func_ov009_0219898c and the statics are named after the later SDK

int func_ov009_02198a0c(SOCKET sock, NAT *nat);

// pokebw2's NatifyThink


const char *func_ov009_02198850(unsigned int ip, unsigned short port, char string[22]) {
    static char strAddressArray[2][22];
    static int nIndex;
    char *strAddress;

    if (string != NULL) {
        strAddress = string;
    } else {
        nIndex ^= 1;
        strAddress = strAddressArray[nIndex];
    }
    if (ip != 0) {
        SOCInAddr inAddr;

        inAddr.addr = ip;
        if (port != 0) {
            OS_SPrintf(strAddress, "%s:%d", func_ov009_0215fa9c(inAddr), port);
        } else {
            OS_SPrintf(strAddress, "%s", func_ov009_0215fa9c(inAddr));
        }
    } else if (port != 0) {
        OS_SPrintf(strAddress, ":%d", port);
    } else {
        strAddress[0] = '\0';
    }
    return strAddress;
}

static gsi_bool gotERT1;
static gsi_bool gotERT2;
static gsi_bool gotERT3;
static gsi_bool gotADDRESS1a;
// pokebw2's gotADDRESS1b
static gsi_bool data_ov009_021ae44c;
static gsi_bool gotADDRESS2;
static gsi_bool gotADDRESS3;

u32 func_ov009_021988f4(void)
{
    int i;
    SOCHostEnt *phost;
    u32 *addr;
    int localip = 0;

    phost = getlocalhost();
    if (phost == NULL) {
        return 0;
    }
    for (i = 0;; i++) {
        if (phost->addrList[i] == NULL) {
            break;
        }
        addr = (u32 *)phost->addrList[i];
        if (*addr == 0x100007f) { // 127.0.0.1, in network order
            continue;
        }
        localip = *addr;
        if (IsPrivateIP((SOCInAddr *)addr)) {
            return localip;
        }
    }
    return localip;
}

u16 func_ov009_02198958(int sock)
{
    SOCSockAddrIn addr;
    int len = sizeof(addr);
    int ret = getsockname(sock, (SOCKADDR *)&addr, &len);

    if (ret == -1) {
        return 0;
    }
    return addr.port;
}

void func_ov009_0219898c(const AddressMapping *mapping) {
    if (mapping == NULL) {
        gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment,
                      "ERROR: Port mapping not available.");
    } else {
        gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment, "Discovered map: [%s] -> [%s]\n",
                      func_ov009_02198850(mapping->privateIp, mapping->privatePort, NULL),
                      func_ov009_02198850(mapping->publicIp, mapping->publicPort, NULL));
    }
}

int func_ov009_02198a0c(SOCKET sock, NAT *nat) {
    static char data[0x200];
    int length;
    gsi_u8 type;
    NatNegPacket packet;
    SOCSockAddrIn from;
    int fromSize = sizeof(SOCSockAddrIn);

    if (gotERT1 && gotERT2 && gotERT3 && gotADDRESS1a && data_ov009_021ae44c && gotADDRESS2 && gotADDRESS3) {
        return gsi_false;
    }
    if (sock != -1) {
        while (CanReceiveOnSocket(sock)) {
            length = recvfrom(sock, data, sizeof(data), 0, (SOCKADDR *)&from, &fromSize);
            if (length == -1) {
                gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Notice, "RECV SOCKET ERROR: %d\n",
                              GOAGetLastError(sock));
                break;
            }
            if (memcmp(data, NNMagicData, NATNEG_MAGIC_LEN) != 0) {
                return gsi_true;
            }
            type = data[7];
            if (length < 21) {
                return gsi_true;
            }
            if (type == 2) {
                memcpy(&packet, data, 21);
                memcpy(&packet.Packet.init.localip, &data[15], 4);
                memcpy(&packet.Packet.init.localport, &data[19], 2);
                switch (packet.Packet.init.porttype) {
                case 1:
                    gotERT1 = gsi_true;
                    gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment,
                                  "(1) Got the solicited ERT from: %s\n",
                                  func_ov009_02198850(from.addr.addr, NN_SWAP16(from.port), NULL));
                    break;
                case 2:
                    nat->ipRestricted = gsi_false;
                    gotERT2 = gsi_true;
                    gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment,
                                  "(2) Got the unsolicited (address) ERT from: %s\n",
                                  func_ov009_02198850(from.addr.addr, NN_SWAP16(from.port), NULL));
                    break;
                case 3:
                    nat->portRestricted = gsi_false;
                    gotERT3 = gsi_true;
                    gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment,
                                  "(3) Got the unsolicited (port) ERT from: %s\n",
                                  func_ov009_02198850(from.addr.addr, NN_SWAP16(from.port), NULL));
                    break;
                }
            } else if (type == 11) {
                memcpy(&packet, data, 21);
                memcpy(&packet.Packet.init.localip, &data[15], 4);
                memcpy(&packet.Packet.init.localport, &data[19], 2);
                packet.cookie = NN_SWAP32(packet.cookie);
                switch (packet.cookie) {
                case 0:
                    gotADDRESS1a = gsi_true;
                    break;
                case 3:
                    data_ov009_021ae44c = gsi_true;
                    break;
                case 1:
                    gotADDRESS2 = gsi_true;
                    break;
                case 2:
                    gotADDRESS3 = gsi_true;
                    break;
                }
                nat->mappings[packet.cookie].privateIp = func_ov009_021988f4();
                nat->mappings[packet.cookie].privatePort = NN_SWAP16(func_ov009_02198958(sock));
                nat->mappings[packet.cookie].publicIp = packet.Packet.init.localip;
                nat->mappings[packet.cookie].publicPort = NN_SWAP16(packet.Packet.init.localport);
                gsDebugFormat(GSIDebugCat_NatNeg, GSIDebugType_Network, GSIDebugLevel_Comment,
                              "Got ADDRESS REPLY %d from: %s\n", packet.cookie,
                              func_ov009_02198850(from.addr.addr, NN_SWAP16(from.port), NULL));
                func_ov009_0219898c(&nat->mappings[packet.cookie]);
            }
            if (sock == -1) {
                break;
            }
        }
    }
    return gsi_true;
}




int func_ov009_02198e74(SOCKET sock, NAT *nat) {
    return func_ov009_02198a0c(sock, nat);
}

gsi_bool func_ov009_02198e80(NAT *nat) {
    gsi_bool portRestricted;
    gsi_bool ipRestricted;

    nat->natType = 6;
    nat->promiscuity = 4;
    nat->qr2Compatible = gsi_true;
    if (nat->mappings[0].publicIp == 0 || nat->mappings[1].publicIp == 0 || nat->mappings[2].publicIp == 0) {
        return gsi_false;
    }
    portRestricted = nat->portRestricted;
    if (!portRestricted && !nat->ipRestricted && nat->mappings[0].publicIp == nat->mappings[0].privateIp) {
        nat->natType = 0;
    } else if (nat->mappings[0].publicIp == nat->mappings[0].privateIp) {
        nat->natType = 1;
    } else {
        ipRestricted = nat->ipRestricted;
    if (!ipRestricted && !portRestricted &&
                   abs(nat->mappings[2].publicPort - nat->mappings[1].publicPort) >= 1) {
            nat->natType = 5;
            nat->promiscuity = 0;
        } else if (ipRestricted && !portRestricted &&
                   abs(nat->mappings[2].publicPort - nat->mappings[1].publicPort) >= 1) {
            nat->natType = 5;
            nat->promiscuity = 2;
        } else if (!ipRestricted && portRestricted &&
                   abs(nat->mappings[2].publicPort - nat->mappings[1].publicPort) >= 1) {
            nat->natType = 5;
            nat->promiscuity = 3;
        } else if (ipRestricted && portRestricted &&
                   abs(nat->mappings[2].publicPort - nat->mappings[1].publicPort) >= 1) {
            nat->natType = 5;
            nat->promiscuity = 1;
        } else if (portRestricted) {
            nat->natType = 4;
        } else if (ipRestricted && !portRestricted) {
            nat->natType = 3;
        } else if (!ipRestricted && !portRestricted) {
            nat->natType = 2;
        } else {
            nat->natType = 6;
        }
    }
    if (nat->mappings[0].publicPort == nat->mappings[0].privatePort &&
        nat->mappings[1].publicPort == nat->mappings[1].privatePort &&
        nat->mappings[2].publicPort == nat->mappings[2].privatePort) {
        nat->mappingScheme = 1;
    } else if (nat->mappings[0].publicPort == nat->mappings[1].publicPort &&
               nat->mappings[1].publicPort == nat->mappings[2].publicPort) {
        nat->mappingScheme = 2;
    } else if (nat->mappings[0].publicPort == nat->mappings[0].privatePort &&
               nat->mappings[2].publicPort - nat->mappings[1].publicPort == 1) {
        nat->mappingScheme = 4;
    } else if (nat->mappings[2].publicPort - nat->mappings[1].publicPort == 1) {
        nat->mappingScheme = 3;
    } else {
        nat->mappingScheme = 0;
    }
    if (nat->mappings[3].publicPort != 0 && nat->mappings[0].publicPort != nat->mappings[3].publicPort) {
        nat->qr2Compatible = gsi_false;
    }
    return gsi_true;
}


