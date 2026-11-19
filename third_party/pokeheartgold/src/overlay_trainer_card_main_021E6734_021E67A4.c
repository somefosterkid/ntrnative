#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "trainer_card_main.h"
#include "unk_02005D10.h"
#include "gf_gfx_planes.h"

void ov51_021E66C0(void *arg0);

void ov51_021E6D44(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 *signature);

void ov51_021E6C6C(void *arg0);

void ov51_021E71D0(TrainerCardMain *, Window *);

u32 ov51_021E67A4(TrainerCardMain *cardMain) {
    u8 uVar1;
    s32 iVar2;
    switch (cardMain->subState) {
    case 0:
        ov51_021E77A0(cardMain, 0);
        cardMain->subState = cardMain->subState + 1;
        break;

    case 1:
        uVar1 = TextPrinterCheckActive(cardMain->textPrinterId);
        if (uVar1 != 0) {
            return 0;
        }
        ov51_021E7804(cardMain, 0);
        ov51_021E6E10(cardMain, 1);
        cardMain->subState = cardMain->subState + 1;
        break;

    case 2:
        iVar2 = ov51_021E786C(cardMain, 0);
        if (iVar2 < 0) {
            break;
        }
        if (iVar2 == 0) {
            ov51_021E6E10(cardMain, 0);
            return 1;
        }
        if (cardMain->card->signatureExists) {
            ov51_021E77A0(cardMain, 1);
        } else {
            return 2;
        }
        cardMain->subState = cardMain->subState + 1;
        break;

    case 3:
        uVar1 = TextPrinterCheckActive(cardMain->textPrinterId);
        if (uVar1 != 0) {
            return 0;
        }
        ov51_021E7804(cardMain, 1);
        cardMain->subState = cardMain->subState + 1;
        break;

    case 4:
        iVar2 = ov51_021E786C(cardMain, 1);
        switch (iVar2) {
        case 0:
            ov51_021E6E10(cardMain, 0);
            return 1;
        case 1:
            return 2;
        }
    }

    return 0;
}

u32 ov51_021E6888(TrainerCardMain *cardMain) {
    u8 bVar1;
    u32 uVar2;

    bVar1 = cardMain->subState;
    uVar2 = 0;
    switch (bVar1) {
    case 0:
        cardMain->flipSpeed = 8;
        cardMain->scaleX = 0x1000;
        cardMain->scaleY = 0x1000;
        cardMain->scaleX = cardMain->scaleX + 0x80;
        cardMain->scaleY = cardMain->scaleY + 0x80;
        PlaySE(0x695);
        cardMain->subState = cardMain->subState + 1;
        break;
    case 1:
        cardMain->scaleX = cardMain->scaleX - (2 << (0xc - cardMain->flipSpeed));
        if (cardMain->scaleX <= 0) {
            cardMain->scaleX = 0x24;
            cardMain->subState = cardMain->subState + 1;
        }
        cardMain->flipSpeed = cardMain->flipSpeed + -1;
        if (cardMain->flipSpeed <= 1) {
            cardMain->flipSpeed = 1;
        }
        break;
    case 2:
        cardMain->showingBack = cardMain->showingBack ^ 1;
        ov51_021E66C0(cardMain);
        cardMain->subState = cardMain->subState + 1;
        break;
    case 3:
        if (cardMain->showingBack == 0) {
            ov51_021E6C6C(cardMain);
            ov51_021E76EC(cardMain, 1);
        } else {
            ov51_021E6D44(cardMain->bgConfig, GF_BG_LYR_SUB_3, cardMain->signaturePixels);
            if (cardMain->isConnected == 0) {
                ov51_021E76EC(cardMain, 0);
            }
        }
        cardMain->subState = cardMain->subState + 1;
        break;
    case 4:
        ov51_021E71D0(cardMain, cardMain->windows);
        cardMain->subState = cardMain->subState + 1;
        break;
    case 5:
        cardMain->flipSpeed = cardMain->flipSpeed + 1;
        if (cardMain->flipSpeed > 8) {
            cardMain->flipSpeed = 8;
        }
        cardMain->scaleX = cardMain->scaleX + (2 << (0xc - cardMain->flipSpeed));
        if (cardMain->scaleX >= 0x1000) {
            cardMain->scaleX = 0x1000;
            cardMain->scaleX = 0x1000;
            cardMain->scaleY = 0x1000;
            uVar2 = 1;
        }
    default:
        break;
    }
    cardMain->scaleChanged = 1;
    return uVar2;
}
