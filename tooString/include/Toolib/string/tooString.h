// Markus Borris, 2011-2016
// This file is part of Toolib library. Open source.

//! If you expected this file to define yet another custom string type, you are fortunately wrong.
//! Instead it serves just as a documentation of guidelines of how to deal with strings in your
//! program.
/**
    (1) By default use std::string everywhere.
        BUT, be aware of
        (i)   You take care that std:string is understood to contain UTF-8 everywhere, except
              you make it very explicit that you expect a local encoding (the only other one
              suitable for std::string) at a certain place.
        (ii)  Know that you can't use length, size, index and iterators to refer to characters;
              these work bytewise instead. Unless it is inherently clear that ASCII is the only
              possible content.

    (2) Write literals using u8"..." if your compiler supports it. Otherwise restrict yourself
        to ASCII (the 7bit thing for char numbers 0..127 resp. 0x0..0x7f, not the 8bit ANSI)
        and write "...".
        If ASCII isn't sufficient for you special character, encode it (bytewise) in UTF-8,
        e.g. "\xc3\xa4" for German "ae". Note that the compiler reads everything that is 0..f
        after the \x and interprets as hex, which as soon as it becomes larger than 0...0ff
        doesn't compile. Solution: use "\xc3\xa4""hnlich" instead of "\xc3\xa4hnlich".
        Hint: use http://www.utf8-zeichentabelle.de
        
    (3) So always ask youself:
        (i) Can I safely pass my UTF-8 string somewhere, or do I need to convert to something the
            receiver understands?
        (ii) Before assigning to my UTF-8 string, what's the source encoding, do I need to convert?
        
    (4) Tools to a basic extent for
            conversions (local, utf8, utf16, utf32) <-> (local, utf8, utf16, utf32),
            counting,
            iterating,
            verifying utf8,
        can be found in this lib or its sdks.

    (5) Whenever you need other encodings, use
        std::u16string for UTF16 and u"...",
        std::u32string for UTF32 and U"...",
        and std::string for local encodings only as little and as explicitly as possible.

        */
//! \file

#ifndef TOOSTRING_H_edkrhcxt87e3nzt837t4jxgfw8t428
#define TOOSTRING_H_edkrhcxt87e3nzt837t4jxgfw8t428

#include "Toolib/PPDEFS.H"

//! Including this header regardless of whether it used here. You will need it in combination with the
//! proposed usage of std::string as UTF-8-everywhere.
#include "../../../sdks/utf8cpp/source/utf8.h"

//#######################################################################################################
//! Under Windows make sure you compile everything using UNICODE-setting - otherwise API
//! restricts everything to ASCII 7bit (perhaps ANSI 8bit and local encodings).
#if TOO_OS_WINDOWS
#if !defined(UNICODE) && !defined(_UNICODE)
#error \
    "Under Windows your code has to be compiled with Unicode setting - according to a desired choice of encoding >= UTF8"
#endif
#endif

#endif
