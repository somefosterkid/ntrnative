#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "text.h"
#include "unk_02034354.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0247.h"
#include "constants/global.h"
#include "filesystem_files_def.h"

int ov74_0222E85C(int *orderNums);
int ov74_0222E8B4(WonderCardsAppData *appData, Window *window);
int ov74_0222E8B4(WonderCardsAppData *appData, Window *window) {

    int i;
    String *string;

    int numConnectedPlayers = 0;
    int numSlotsChanged = 0;

    for (i = 1; i < 4 + 1; i++) {
        PlayerProfile *tmpTrainerInfo = sub_02034818(i);

        if (tmpTrainerInfo == NULL) {
            if (appData->connTrainerInfos[i]) {
                numSlotsChanged++;
            }

            appData->connTrainerInfos[i] = NULL;
            appData->orderNumbers[i] = 0x7fff * 0x7fff;
        } else if (appData->connTrainerInfos[i] != tmpTrainerInfo) {
            numSlotsChanged++;
            numConnectedPlayers++;
            appData->connTrainerInfos[i] = tmpTrainerInfo;
            appData->orderNumbers[i] = appData->numPlayerConnections++;
        } else {
            numConnectedPlayers++;
        }
    }

    if (numSlotsChanged == 0) {
        return numConnectedPlayers;
    }

    int orderNumbers[4];
    int connectedPlayersNetIds[4];

    orderNumbers[0] = appData->orderNumbers[1], orderNumbers[1] = appData->orderNumbers[2], orderNumbers[2] = appData->orderNumbers[3], orderNumbers[3] = appData->orderNumbers[4];
    connectedPlayersNetIds[0] = ov74_0222E85C(orderNumbers);
    connectedPlayersNetIds[1] = ov74_0222E85C(orderNumbers);
    connectedPlayersNetIds[2] = ov74_0222E85C(orderNumbers);
    connectedPlayersNetIds[3] = ov74_0222E85C(orderNumbers);

    MessageFormat *strTemplate = MessageFormat_New(HEAP_ID_85);
    MsgData *msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msg_0247, HEAP_ID_85);
    int trainerInfoYOffset = 0;

    FillWindowPixelBuffer(window, 0);

    for (i = 0; i < numConnectedPlayers; i++) {
        PlayerProfile *tmpTrainerInfo = sub_02034818(connectedPlayersNetIds[i]);

        if (tmpTrainerInfo) {
            BufferPlayersName(strTemplate, 0, tmpTrainerInfo);
            string = ReadMsgData_ExpandPlaceholders(strTemplate, msgLoader, 54, HEAP_ID_85);

            if (PlayerProfile_GetTrainerGender(tmpTrainerInfo) == PLAYER_GENDER_MALE) {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0, trainerInfoYOffset, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
            } else {
                AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0, trainerInfoYOffset, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
            }

            String_Delete(string);
            BufferIntegerAsString(strTemplate, 0, PlayerProfile_GetTrainerID(tmpTrainerInfo) & 0xFFFF, 5, PRINTING_MODE_LEADING_ZEROS, 1);

            string = ReadMsgData_ExpandPlaceholders(strTemplate, msgLoader, 55, HEAP_ID_85);

            AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 80, trainerInfoYOffset, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
            String_Delete(string);

            trainerInfoYOffset += ((1) * 16) + 8;
        }
    }

    if (numConnectedPlayers) {
        CopyWindowToVram(window);
    }

    DestroyMsgData(msgLoader);
    MessageFormat_Delete(strTemplate);

    return numConnectedPlayers;
}
