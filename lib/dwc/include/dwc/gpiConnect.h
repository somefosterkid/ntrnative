// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/func_ov009_0218a624.h, with Black's names.
#ifndef BWPORT_DWC_GPICONNECT_H
#define BWPORT_DWC_GPICONNECT_H

#include "dwc/gpi.h"
#include "dwc/nonport.h"

// The login of GameSpy's GP to the connection manager (func_ov009_0218a624.c), in overlay 11 (see dwc/gp.h for the names)

GPResult func_ov009_0218a624(GPConnection *connection, const char nick[GP_NICK_LEN], const char uniquenick[GP_UNIQUENICK_LEN],
                    const char email[GP_EMAIL_LEN], const char password[GP_PASSWORD_LEN],
                    const char authtoken[GP_AUTHTOKEN_LEN], const char partnerchallenge[GP_PARTNERCHALLENGE_LEN],
                    const char cdkey[GP_CDKEY_LEN], GPEnum firewall, GPIBool newuser, GPEnum blocking,
                    GPCallback callback, void *param);
GPResult func_ov009_0218ae9c(GPConnection *connection, GPIOperation *operation, const char *input);
GPResult func_ov009_0218b5a0(GPConnection *connection);
void func_ov009_0218b760(GPConnection *connection, GPIBool tellServer);
// Handles the server's "\\rar\\", which gives the namespace and the partner of the login
int func_ov009_0218b914(GPConnection *connection, const char *input);

#endif // BWPORT_DWC_GPICONNECT_H
