#include "spinlang.hpp"

bool TokenizeStyleString(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    if (*p == '"')
    {
        p++;

        while (p < in_end)
        {
            // handle end of string
            if (*p == '"')
            {
                out_begin = in_begin;
                out_end   = p + 1;
                return true;
            }

            // handle escape character for "
            if (*p == '\\' && p + 1 < in_end && p[1] == '"')
                p++;

            p++;
        }
    }

    return false;
}

bool TokenizeCharacterLiteral(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    if (*p == '\'')
    {
        p++;

        // handle escape characters
        if (p < in_end && *p == '\\')
            p++;

        if (p < in_end)
            p++;

        // handle end of character literal
        if (p < in_end && *p == '\'')
        {
            out_begin = in_begin;
            out_end   = p + 1;
            return true;
        }
    }

    return false;
}

bool TokenizeIdentifier(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_')
    {
        p++;

        while ((p < in_end) &&
               ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_'))
            p++;

        out_begin = in_begin;
        out_end   = p;
        return true;
    }

    return false;
}

bool TokenizeNumber(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p               = in_begin;

    const bool startsWithNumber = *p >= '0' && *p <= '9';

    if (*p != '+' && *p != '-' && !startsWithNumber)
        return false;

    p++;

    bool hasNumber = startsWithNumber;

    while (p < in_end && (*p >= '0' && *p <= '9'))
    {
        hasNumber = true;

        p++;
    }

    if (hasNumber == false)
        return false;

    bool isFloat  = false;
    bool isHex    = false;
    bool isBinary = false;

    if (p < in_end)
    {
        if (*p == '.')
        {
            isFloat = true;

            p++;

            while (p < in_end && (*p >= '0' && *p <= '9'))
                p++;
        }
        else if (*p == 'x' || *p == 'X')
        {
            // hex formatted integer of the type 0xef80

            isHex = true;

            p++;

            while (p < in_end && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')))
                p++;
        }
        else if (*p == 'b' || *p == 'B')
        {
            // binary formatted integer of the type 0b01011101

            isBinary = true;

            p++;

            while (p < in_end && (*p >= '0' && *p <= '1'))
                p++;
        }
    }

    if (isHex == false && isBinary == false)
    {
        // floating point exponent
        if (p < in_end && (*p == 'e' || *p == 'E'))
        {
            isFloat = true;

            p++;

            if (p < in_end && (*p == '+' || *p == '-'))
                p++;

            bool hasDigits = false;

            while (p < in_end && (*p >= '0' && *p <= '9'))
            {
                hasDigits = true;

                p++;
            }

            if (hasDigits == false)
                return false;
        }

        // single precision floating point type
        if (p < in_end && *p == 'f')
            p++;
    }

    if (isFloat == false)
    {
        // integer size type
        while (p < in_end && (*p == 'u' || *p == 'U' || *p == 'l' || *p == 'L'))
            p++;
    }

    out_begin = in_begin;
    out_end   = p;
    return true;
}

bool TokenizePunctuation(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    (void)in_end;

    switch (*in_begin)
    {
    case '[':
    case ']':
    case '{':
    case '}':
    case '!':
    case '%':
    case '^':
    case '&':
    case '*':
    case '(':
    case ')':
    case '-':
    case '+':
    case '=':
    case '~':
    case '|':
    case '<':
    case '>':
    case '?':
    case ':':
    case '/':
    case ';':
    case ',':
    case '.':
        out_begin = in_begin;
        out_end   = in_begin + 1;
        return true;
    }

    return false;
}

TextEditor::LanguageDefinition createPromelaLanguage()
{
    // FIXME: Make sure and select with 1..10 works correctly
    TextEditor::LanguageDefinition promela;
    promela.mName     = "Promela";
    promela.mKeywords = {
        "active", "assert",   "atomic", "bool",   "break", "byte",   "case",    "chan",   "d_step",   "do",    "else",
        "else",   "eval",     "false",  "fi",     "goto",  "hidden", "if",      "inline", "int",      "local", "mtype",
        "od",     "proctype", "return", "select", "short", "skip",   "timeout", "true",   "unsigned", "xr",    "xs",
    };

    promela.mTokenize = [](const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end,
                           TextEditor::PaletteIndex &paletteIndex) -> bool {
        paletteIndex = TextEditor::PaletteIndex::Max;

        while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
            in_begin++;

        if (in_begin == in_end)
        {
            out_begin    = in_end;
            out_end      = in_end;
            paletteIndex = TextEditor::PaletteIndex::Default;
        }
        else if (TokenizeStyleString(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::String;
        else if (TokenizeCharacterLiteral(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::CharLiteral;
        else if (TokenizeIdentifier(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Identifier;
        else if (TokenizeNumber(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Number;
        else if (TokenizePunctuation(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Punctuation;

        return paletteIndex != TextEditor::PaletteIndex::Max;
    };
    promela.mCommentStart      = "/*";
    promela.mCommentEnd        = "*/";
    promela.mSingleLineComment = "//";
    return promela;
}
