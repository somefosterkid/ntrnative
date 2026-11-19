#ifndef POKEHEARTGOLD_MAIL_MESSAGE_TYPES_H
#define POKEHEARTGOLD_MAIL_MESSAGE_TYPES_H

// MailMessage on its own (also reached through pokemon_types_def.h), so headers such as
// union_room_manager.h can embed one without pulling in every Pokemon type.

#define MAILMSG_BANK_NONE  (0xFFFF)
#define MAILMSG_FIELDS_MAX (2)

typedef struct MailMessage {
    u16 msg_bank;
    u16 msg_no;
    u16 fields[MAILMSG_FIELDS_MAX];
} MailMessage;

#endif // POKEHEARTGOLD_MAIL_MESSAGE_TYPES_H
