// TwlSDK's twl/ltdmain_begin.h: the code and data after it go in the LTD autoload's .ltdmain (DSi-only main memory),
// up to twl/ltdmain_end.h (twl/section.h's LTDMAIN section)
#pragma define_section LTDMAIN ".ltdmain" ".ltdmain.bss" abs32 RWX
#pragma section LTDMAIN begin
