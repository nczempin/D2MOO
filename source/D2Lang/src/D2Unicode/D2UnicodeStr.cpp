/**
 * D2MOO
 * Copyright (c) 2020-2022  The Phrozen Keep community
 *
 * This file belongs to D2MOO.
 * File authors: Mir Drualga, Lectem
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <D2Unicode.h>

#include <assert.h>
#include <ctype.h>

#include "Fog.h"

BOOL __fastcall Unicode::isWordEnd(const Unicode* str, size_t index) {
  if (index == 0) {
    return FALSE;
  }

  return ::isalnum(str[index].ch)
      && !::isalnum(str[index + 1].ch);
}

void __cdecl Unicode::sprintf(
    int buffer_size,
    Unicode* buffer,
    const Unicode* format,
    ...) {

  if (format == NULL) {
    return;
  }

  int i_buffer = 0;
  buffer[0].ch = L'\0';

  va_list args;
  va_start(args, format);

  Unicode percent_sign[2] = { L'%' };

  for (int i_format = 0; format[i_format].ch != L'\0'; i_format += 2) {
    unsigned short conversion_specifier = L'\0';
    Unicode* strstr_result = Unicode::strstr(&format[i_format], percent_sign);
    if (strstr_result != NULL) {
      conversion_specifier = strstr_result[1];
    }

    /*
     * Copy format into buffer up to where the % is found, or if % not
     * found, then copy the remaining string.
     */
    while (&format[i_format] != strstr_result
        && format[i_format].ch != L'\0'
        && i_buffer < buffer_size) {
      
      buffer[i_buffer] = format[i_format];

      ++i_format;
      ++i_buffer;
    }

    if (i_buffer >= buffer_size) {
      buffer[i_buffer - 1].ch = L'\0';
      va_end(args);
      return;
    }

    if (format[i_format].ch == L'\0') {
      buffer[i_buffer].ch = L'\0';
      va_end(args);
      return;
    }

    D2_ASSERT(strstr_result != NULL);

    switch (conversion_specifier) {
      case L'\0': {
        /*
         * No conversion specifier found or % is the last character.
         */
        Unicode::strcpy(&buffer[i_buffer], percent_sign);
        va_end(args);
        return;
      }

      case L'%': {
        if (i_buffer + 1 >= buffer_size) {
          va_end(args);
          return;
        }

        Unicode::strcpy(&buffer[i_buffer], percent_sign);
        ++i_buffer;

        break;
      }

      case L'd':
      case L'u': {
        char itoa_buffer[16];

        if (conversion_specifier == L'd') {
          ::_itoa(va_arg(args, int), itoa_buffer, 10); // NOLINT(clang-diagnostic-deprecated-declarations)
        } else {
          ::_ultoa(va_arg(args, unsigned int), itoa_buffer, 10); // NOLINT(clang-diagnostic-deprecated-declarations)
        }

        Unicode itoa_unicode[15];
        Unicode::toUnicode(itoa_unicode, itoa_buffer, 15);

        int itoa_length;
        for (itoa_length = 0;
            itoa_unicode[itoa_length].ch != L'\0';
            ++itoa_length) {
          /* Left empty on purpose. */
        }

        if ((i_buffer + itoa_length + 1) >= buffer_size) {
          va_end(args);
          return;
        }

        Unicode::strcpy(&buffer[i_buffer], itoa_unicode);
        i_buffer += itoa_length;

        break;
      }

      case L's': {
        const Unicode* arg_string = va_arg(args, const Unicode*);
        if (arg_string == NULL || arg_string[0].ch == L'\0') {
          buffer[i_buffer] = L'\0';
          va_end(args);
          return;
        }

        const int arg_s_length = Unicode::strlen(arg_string);

        if (arg_s_length == 0 || (i_buffer + arg_s_length) >= buffer_size) {
          Unicode::strncat(
              &buffer[i_buffer],
              arg_string,
              buffer_size - (i_buffer + 1));
          va_end(args);
          return;
        }

        for (int i_arg_s = 0; ; ++i_arg_s) {
          buffer[i_buffer + i_arg_s] = arg_string[i_arg_s];

          if (arg_string[i_arg_s + 1] == L'\0') {
            break;
          }
        }

        i_buffer += arg_s_length;

        break;
      }

      default: {
        FOG_DisplayHalt(
            "Unknown format specifier in Unicode::sprintf",
            __FILE__,
            __LINE__);
        exit(-1);
      }
    }
  }

  va_end(args);
}

Unicode* __fastcall Unicode::strcat(Unicode* dest, const Unicode* src) {
  size_t i = 0;
  while (dest[i].ch != L'\0') {
    ++i;
  }

  Unicode::strcpy(&dest[i], src);

  return dest;
}

Unicode* __fastcall Unicode::strchr(const Unicode* str, Unicode ch) {
  for (size_t i = 0; str[i].ch != L'\0'; ++i) {
    if (str[i].ch == ch.ch) {
      return (Unicode*)&str[i];
    }
  }

  return NULL;
}

int __fastcall Unicode::strcmp(const Unicode* str1, const Unicode* str2) {
  /*
   * This loop does not run beyond the end of either string. If the
   * end of only one string is reached, then a comparison between '\0'
   * to a different character is made, and a return is guaranteed to
   * happen.
   */
  for (size_t i = 0; (str1[i].ch != L'\0') || (str2[i].ch != L'\0'); ++i) {
    if (str1[i].ch < str2[i].ch) {
      return -1;
    } else if (str1[i].ch > str2[i].ch) {
      return 1;
    }
  }

  return 0;
}

Unicode* __fastcall Unicode::strcpy(Unicode* dest, const Unicode* src) {
  size_t i = 0;
  do {
    dest[i].ch = src[i].ch;
  } while (src[i++].ch != L'\0');

  return dest;
}

int __fastcall Unicode::stricmp(const Unicode* str1, const Unicode* str2) {
  /*
   * This loop does not run beyond the end of either string. If the
   * end of only one string is reached, then a comparison between '\0'
   * to a different character is made, and a return is guaranteed to
   * happen.
   */
  for (size_t i = 0; (str1[i].ch != L'\0') || (str2[i].ch != L'\0'); ++i) {
    Unicode ch1_upper = str1[i].toUpper();
    Unicode ch2_upper = str2[i].toUpper();

    if (ch1_upper.ch < ch2_upper.ch) {
      return -1;
    } else if (ch1_upper.ch > ch2_upper.ch) {
      return 1;
    }
  }

  return 0;
}

int __fastcall Unicode::strnicmp(const Unicode* str1, const Unicode* str2, size_t count) {
    /*
     * This loop does not run beyond the end of either string. If the
     * end of only one string is reached, then a comparison between '\0'
     * to a different character is made, and a return is guaranteed to
     * happen.
     */
    for (size_t i = 0; ((str1[i].ch != L'\0') || (str2[i].ch != L'\0')) && count != 0; ++i, count--) {
        Unicode ch1_upper = str1[i].toUpper();
        Unicode ch2_upper = str2[i].toUpper();

        if (ch1_upper.ch < ch2_upper.ch) {
            return -1;
        }
        else if (ch1_upper.ch > ch2_upper.ch) {
            return 1;
        }
    }

    return 0;
}

int __fastcall Unicode::strlen(const Unicode* str) {
  if (str == NULL) {
    return 0;
  }

  int i = 0;
  while ((str++)->ch != L'\0') {
    ++i;
  }

  return i;
}

Unicode* __fastcall Unicode::strncat(
    Unicode* dest,
    const Unicode* src,
    int count) {
  int i_dest = 0;
  while (dest[i_dest].ch != L'\0') {
    ++i_dest;
  }

  int i_src;
  for (i_src = 0; i_src != count && src[i_src].ch != L'\0'; ++i_src) {
    dest[i_dest + i_src] = src[i_src];
  }

  dest[i_dest + i_src] = L'\0';

  return dest;
}

int __fastcall Unicode::strncmp(
    const Unicode* str1,
    const Unicode* str2,
    size_t count) {
  /*
   * Vanilla bug: If one string is a prefix of the other string, then
   * the loop ends early and 0 is returned.
   */
  for (size_t i = 0;
      (str1[i].ch != L'\0') && (str2[i].ch != L'\0') && (i < count);
      ++i) {
    if (str1[i].ch < str2[i].ch) {
      return -1;
    } else if (str1[i].ch > str2[i].ch) {
      return 1;
    }
  }

  return 0;
}

/**
 * This implementation outputs the same binary from 1.00.
 *
 * 1.00: D2Lang.0x1000114A (#10042)
 * 1.10: D2Lang.0x6FC11460 (#10044)
 * 1.11: D2Lang.0x6FC0A900 (#10044)
 * 1.13c: D2Lang.0x6FC0B0D0 (#10044)
 * ?strncpy@Unicode@@SIPAU1@PAU1@PBU1@H@Z
 */
Unicode* __fastcall Unicode::strncpy(
    Unicode* dest, const Unicode* src, int count)
{
  // Copy src into dest, up to count characters.
  Unicode* current_dest = dest;
  for (; (count--) != 0 && src->ch != L'\0'; ++current_dest, ++src)
  {
    current_dest->ch = src->ch;
  }

  // Fill remaining characters as null-terminator.
  for (; count != -1; ++current_dest, --count)
  {
    current_dest->ch = L'\0';
  }

  return dest;
}

Unicode* __fastcall Unicode::strstr(
    const Unicode* str,
    const Unicode* substr) {
  if (str == NULL || substr == NULL) {
    return NULL;
  }

  size_t i_str;
  for (i_str = 0; ; ++i_str) {
    for (; str[i_str].ch != substr[0].ch; ++i_str) {
      if (str[i_str].ch == L'\0') {
        return NULL;
      }
    }

    if (str[i_str].ch == '\0') {
      return NULL;
    }

    size_t i_substr;
    for (i_substr = 0;
        str[i_str + i_substr].ch == substr[i_substr].ch;
        ++i_substr) {
      if (str[i_str + i_substr].ch == L'\0') {
        break;
      }

      if (substr[i_substr].ch == L'\0') {
        return (Unicode*)&str[i_str];
      }
    }

    if (substr[i_substr].ch == L'\0') {
      return (Unicode*)&str[i_str];
    }
  }

  return (Unicode*)&str[i_str];
}

// 1.12a: D2Lang.[0x6FC0FBB0 to 0x6FC0FDB0)
static const unsigned short sgCollationTable[256] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x40, 0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x10F,
  0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117, 0x118, 0x119, 0x11A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
  0x60, 0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x10F,
  0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117, 0x118, 0x119, 0x11A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x113, 0x8B, 0x120, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x113, 0x9B, 0x120, 0x9D, 0x9E, 0x119,
  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
  0x100, 0x100, 0x100, 0x100, 0x11E, 0x11D, 0x11B, 0x102, 0x104, 0x104, 0x104, 0x104, 0x108, 0x108, 0x108, 0x108,
  0x121, 0x10E, 0x10F, 0x10F, 0x10F, 0x10F, 0x11F, 0xD7, 0x11C, 0x115, 0x115, 0x115, 0x115, 0x119, 0x122, 0x113,
  0x100, 0x100, 0x100, 0x100, 0x11E, 0x11D, 0x11B, 0x102, 0x104, 0x104, 0x104, 0x104, 0x108, 0x108, 0x108, 0x108,
  0x121, 0x10E, 0x10F, 0x10F, 0x10F, 0x10F, 0x11F, 0xF7, 0x11C, 0x115, 0x115, 0x115, 0x115, 0x119, 0x122, 0x119,
};

// 1.12a: D2Lang.0x6FC011A0 (#10037)
int __fastcall Unicode::strcoll(const Unicode* str1, const Unicode* str2) {
  return Unicode::strncoll(str1, str2, 0x7FFFFFFF);
}

// 1.12a: D2Lang.0x6FC0B7A0 (#10043)
int __fastcall Unicode::strncoll(const Unicode* str1, const Unicode* str2, int count) {
  int i = 0;
  for (; str1[i].ch != L'\0' && str2[i].ch != L'\0' && i < count; ++i) {
    unsigned short ch1 = str1[i].ch;
    unsigned short ch2 = str2[i].ch;
    int w1, w2;
    if (ch1 < 0x100 && ch2 < 0x100) {
      w1 = sgCollationTable[ch1];
      w2 = sgCollationTable[ch2];
    } else {
      w1 = ch1;
      w2 = ch2;
    }
    int diff = w1 - w2;
    if (diff != 0) {
      return diff;
    }
  }

  if (i < count && str1[i].ch != str2[i].ch) {
    return (str1[i].ch != L'\0') ? 1 : -1;
  }

  return Unicode::strncmp(str1, str2, count);
}

// 1.12a: D2Lang.0x6FC0ACB0 (#10047)
Unicode* __fastcall Unicode::strstri(const Unicode* str, const Unicode* substr) {
  if (str == NULL || substr == NULL) {
    return NULL;
  }

  size_t i_str;
  for (i_str = 0; ; ++i_str) {
    for (; str[i_str].toUpper().ch != substr[0].toUpper().ch; ++i_str) {
      if (str[i_str].ch == L'\0') {
        return NULL;
      }
    }

    if (str[i_str].ch == L'\0') {
      return NULL;
    }

    size_t i_substr;
    for (i_substr = 0;
        str[i_str + i_substr].toUpper().ch == substr[i_substr].toUpper().ch;
        ++i_substr) {
      if (str[i_str + i_substr].ch == L'\0') {
        break;
      }

      if (substr[i_substr].ch == L'\0') {
        return (Unicode*)&str[i_str];
      }
    }

    if (substr[i_substr].ch == L'\0') {
      return (Unicode*)&str[i_str];
    }
  }
}

// 1.12a: D2Lang.0x6FC0B120
static bool __fastcall StartsWithAnyChar(const char* szChars, const Unicode* str) {
  Unicode uChars[20] = {};
  Unicode::win2Unicode(uChars, szChars, 20);
  int nLen = Unicode::strlen(uChars);
  for (int i = 0; i < nLen; ++i) {
    if (str[0].ch == uChars[i].ch) {
      return true;
    }
  }
  return false;
}

// 1.12a: D2Lang.0x6FC0B220
static bool __fastcall EndsWithAnyChar(const char* szChars, const Unicode* str) {
  Unicode uChars[20] = {};
  Unicode::win2Unicode(uChars, szChars, 20);
  int nCharsLen = Unicode::strlen(uChars);
  int nStrLen = Unicode::strlen(str);
  if (nStrLen <= 0) {
    return false;
  }
  unsigned short lastCh = str[nStrLen - 1].ch;
  for (int i = 0; i < nCharsLen; ++i) {
    if (lastCh == uChars[i].ch) {
      return true;
    }
  }
  return false;
}

// 1.12a: D2Lang.0x6FC0B330 (#10018)
void __fastcall Unicode::Personalize(
    Unicode* dest,
    const Unicode* player_name,
    const Unicode* item_name,
    int max_length,
    ELANGUAGE language) {
  int nPlayerLen = Unicode::strlen(player_name);
  int nItemLen = Unicode::strlen(item_name);

  if (nPlayerLen + nItemLen + 5 > max_length) {
    if (nItemLen < max_length) {
      Unicode::strcpy(dest, item_name);
    }
    return;
  }

  Unicode uSep[10] = {};
  switch (language) {
    case LANGUAGE_ENGLISH:
    case LANGUAGE_ENGLISHKOR: {
      Unicode::strcpy(dest, player_name);
      const char* szSep = EndsWithAnyChar("s", player_name) ? "' " : "'s ";
      Unicode::win2Unicode(uSep, szSep, (int)::strlen(szSep) + 1);
      Unicode::strcat(dest, uSep);
      Unicode::strcat(dest, item_name);
      break;
    }
    case LANGUAGE_SPANISH: {
      Unicode::strcpy(dest, item_name);
      Unicode::win2Unicode(uSep, " de ", 5);
      Unicode::strcat(dest, uSep);
      Unicode::strcat(dest, player_name);
      break;
    }
    case LANGUAGE_GERMAN: {
      Unicode::strcpy(dest, player_name);
      const char* szSep = EndsWithAnyChar("sz", player_name) ? "' " : "s ";
      Unicode::win2Unicode(uSep, szSep, (int)::strlen(szSep) + 1);
      Unicode::strcat(dest, uSep);
      Unicode::strcat(dest, item_name);
      break;
    }
    case LANGUAGE_FRENCH: {
      Unicode::strcpy(dest, item_name);
      const char* szSep = StartsWithAnyChar("aeiou", player_name) ? " d'" : " de ";
      Unicode::win2Unicode(uSep, szSep, (int)::strlen(szSep) + 1);
      Unicode::strcat(dest, uSep);
      Unicode::strcat(dest, player_name);
      break;
    }
    case LANGUAGE_ITALIAN: {
      Unicode::strcpy(dest, item_name);
      Unicode::win2Unicode(uSep, " di ", 5);
      Unicode::strcat(dest, uSep);
      Unicode::strcat(dest, player_name);
      break;
    }
    case LANGUAGE_POLISH: {
      Unicode::strcpy(dest, item_name);
      Unicode uParen[2] = {};
      Unicode::win2Unicode(uParen, "(", 2);
      Unicode::strcat(dest, uParen);
      Unicode::strcat(dest, player_name);
      Unicode::win2Unicode(uParen, ")", 2);
      Unicode::strcat(dest, uParen);
      Unicode::win2Unicode(uParen, " ", 2);
      Unicode::strcat(dest, uParen);
      break;
    }
    default: {
      Unicode uParen[2] = {};
      Unicode::win2Unicode(uParen, "(", 2);
      Unicode::strcpy(dest, uParen);
      Unicode::strcat(dest, player_name);
      Unicode::win2Unicode(uParen, ")", 2);
      Unicode::strcat(dest, uParen);
      Unicode::win2Unicode(uParen, " ", 2);
      Unicode::strcat(dest, uParen);
      Unicode::strcat(dest, item_name);
      break;
    }
  }
}
