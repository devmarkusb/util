//! \file

#ifndef LANGUAGES_H_sjdkghfnxz34gf7328f7gfr
#define LANGUAGES_H_sjdkghfnxz34gf7328f7gfr

#include "ul/assert.h"
#include "ul/error.h"
#include <string>
#include <vector>

namespace mb::ul::str
{
//! Language id type per ISO 639-1 plus an auto/systems choice.
using LangID_iso639_1 = std::string;

const LangID_iso639_1 auto_systems_language{"auto"};

//! Note: if you add to this, don't forget about the other functions in this file.
inline const std::vector<LangID_iso639_1>& getAllTheoreticallyPossibleLangIDs()
{
    static const std::vector<LangID_iso639_1> cachedval{
        "ar", // Arabic
        "bn", // Bengali, Bangla
        "de", // German
        "en", // English
        "es", // Spanish
        "fr", // French
        "hi", // Hindi
        "it", // Italian
        "ja", // Japanese
        "ko", // Korean
        "pt", // Portuguese
        "ru", // Russian
        "zh", // Chinese
    };
    return cachedval;
}

inline std::string getNativeLanguageName(const LangID_iso639_1& id)
{
    if (id == auto_systems_language)
    {
        // that's not what you might want; please handle the translation for that word yourself
        // (on library level I can't decide, whether you want 'auto' or 'systems default' or whatever)
        UL_ASSERT_THROW(false);
    }
    else if (id == "ar")
        return "\xD8\xA7\xD9\x84\xD8\xB9\xD8\xB1\xD8\xA8\xD9\x8A\xD8\xA9";
    else if (id == "bn")
        return "\xE0\xA6\xAC\xE0\xA6\xBE\xE0\xA6\x82\xE0\xA6\xB2\xE0\xA6\xBE";
    else if (id == "de")
        return "Deutsch";
    else if (id == "en")
        return "English";
    else if (id == "es")
        return "Espa\xC3\xB1"
               "ol";
    else if (id == "fr")
        return "Fran\xc3\xa7"
               "ais";
    else if (id == "hi")
        return "\xE0\xA4\xB9\xE0\xA4\xBF\xE0\xA4\xA8\xE0\xA5\x8D\xE0\xA4\xA6\xE0\xA5\x80";
    else if (id == "it")
        return "Italiano";
    else if (id == "ja")
        return "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E";
    else if (id == "ko")
        return "\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4";
    else if (id == "pt")
        return "Portugu\xC3\xAA"
               "s";
    else if (id == "ru")
        return "\xD0\xA0\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9";
    else if (id == "zh")
        return "\xE4\xB8\xAD\xE6\x96\x87";
    else
        throw ul::not_implemented{"unsupported or unknonwn ISO 639-1 language code"};
}

inline std::string getEnglishLanguageName(const LangID_iso639_1& id)
{
    if (id == auto_systems_language)
    {
        // that's not what you might want; please handle the translation for that word yourself
        // (on library level I can't decide, whether you want 'auto' or 'systems default' or whatever)
        UL_ASSERT_THROW(false);
    }
    else if (id == "ar")
        return "Arabic";
    else if (id == "bn")
        return "Bengali";
    else if (id == "de")
        return "German";
    else if (id == "en")
        return "English";
    else if (id == "es")
        return "Spanish";
    else if (id == "fr")
        return "French";
    else if (id == "hi")
        return "Hindi";
    else if (id == "it")
        return "Italian";
    else if (id == "ja")
        return "Japanese";
    else if (id == "ko")
        return "Korean";
    else if (id == "pt")
        return "Portuguese";
    else if (id == "ru")
        return "Russian";
    else if (id == "zh")
        return "Chinese";
    else
        throw ul::not_implemented{"unsupported or unknonwn ISO 639-1 language code"};
}
} // namespace mb::ul::str

#endif
