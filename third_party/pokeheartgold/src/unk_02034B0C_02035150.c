#include "global.h"

#include "comm_server_client.h"
#include "error_handling.h"
#include "pokemon_types_def.h"

int sub_02035150(int param0);

int sub_02035150(int param0) {
    int i, v1 = 0;

    for (i = 0; i < 16; i++) {
        if (_021D4134.sCommServerClient->unk_D44[i] != 0) {
            if (v1 == param0) {
                return i;
            }
            v1++;
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}
