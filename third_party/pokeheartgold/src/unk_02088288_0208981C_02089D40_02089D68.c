#include "global.h"

#include "bg_window.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void sub_0208A950(PokemonSummary *arg0);

void sub_0208B5A8(PokemonSummary *arg0);

void sub_0208B9C8(PokemonSummary *arg0);

void sub_0208BCD4(PokemonSummary *arg0);

void sub_0208C2A0(PokemonSummary *arg0);

void sub_0208CBD4(void *arg0);

void sub_02089F98(PokemonSummary *);

s32 sub_0208B448(void *);

s32 sub_0208B89C(void *);

s32 sub_0208C42C(void *);

s32 sub_0208C4E0(void *);

void sub_02089D68(PokemonSummary *arg0, s8 arg1);

void sub_02089D68(PokemonSummary *arg0, s8 arg1) {
    if (arg0->page != arg1) {
        sub_0208C4E0(arg0);
        arg0->page = arg1;
        sub_0208B448(arg0);
        sub_0208B5A8(arg0);
        sub_0208B89C(arg0);
        sub_0208B9C8(arg0);
        sub_0208C2A0(arg0);
        sub_0208BCD4(arg0);
        sub_0208C42C(arg0);
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 3U, 0x12U, 0x14U, 0U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
        FillBgTilemapRect(arg0->bgConfig, 4U, 0U, 0U, 3U, 0x12U, 0x14U, 0U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
        sub_0208CBD4(arg0);
        sub_02089F98(arg0);
        if (arg0->args->unk12 == 2) {
            sub_0208A950(arg0);
        }
    }
}
