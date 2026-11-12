// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/ext/ext_keycontrol.h, as it is (tools/port_lib.py)
#ifndef EXT_KEYCONTROL_H_
#define EXT_KEYCONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u16 key;
	u16 count;
} EXTKeys;

void EXT_AutoKeys(const EXTKeys * sequence, u16 * cont, u16 * trig);

#ifdef __cplusplus
}
#endif

#endif
