// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/mailview.h: the lines in use so far.
#ifndef BWPORT_FIELD_MAILVIEW_H
#define BWPORT_FIELD_MAILVIEW_H

#include "types.h"
#include "field/mailsys.h"

// pokebw2's MailView_Unload
BOOL func_ov136_021c4d14(MailSysWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MailView_Load
// Returns TRUE once everything is loaded
BOOL func_ov136_021c4bfc(MailSysWork *work);

#endif // BWPORT_FIELD_MAILVIEW_H
