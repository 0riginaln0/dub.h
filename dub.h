/****************************************************************************************
 *   "THE GUITAR-WARE LICENSE" (Revision 1):                                            *
 *                       <ggooddmitriy@gmail.com> wrote this file.                      *
 *   As long as you retain this notice you can do whatever you want with this stuff.    *
 *   If we meet some day, and you think this stuff is worth it,                         *
 *   you can buy me a guitar strings in return.                                         *
 *                                                                             Ryzh     *
 *//////////////////////////////////////////////////////////////////////////////////////

#ifndef DUB_H
#define DUB_H

#include <stdbool.h>
#include <string.h>

typedef struct {
  int argc;
  char** argv;
  int i;
} Dub;

Dub   dub_create(int argc, char** argv); // Create a Dub instance
bool  dub_end(Dub* dub);  // Have you reached the end? (No more args to parse)
char* dub_peek(Dub* dub); // Get the current argument. (NULL when at the end)
void  dub_next(Dub* dub); // Move to the next argument. (dub->i++)

// Functions below advance to the next argument when the match/parse succeeded.
bool dub_match(Dub* dub, char* s);
bool dub_parse_integer(Dub* dub, int* out);
bool dub_parse_unsigned_integer(Dub* dub, unsigned int* out);
bool dub_parse_port(Dub* dub, unsigned int* out);
bool dub_parse_email(Dub* dub, char** out);
bool dub_parse_string(Dub* dub, char** out);

#ifdef DUB_IMPLEMENTATION

///* Internal */

static int dub_internal_ascii_digit_to_int(char c) {
  return c - 48;
}

static bool dub_internal_is_integer(char* s, int* out) {
  // Pointer to string is NULL
  if (s == NULL) return false;
  // The string content is 0 length
  if (*s == '\0') return false;

  char* cursor = s;
  int x = 0;
  int sign = 1;
  bool some_digit_met = false;
  bool error_during_parsing = false;
  bool sign_met = false;

  while (true) {
    char c = *cursor;
    if ('0' <= c && c <= '9') {
      some_digit_met = true;
      x = x * 10;
      x = x + dub_internal_ascii_digit_to_int(c);
      cursor++;
      continue;
    }
    // End of string
    if (c == '\0') break;
    if (c == '-' || c == '+') {
      if (sign_met) {
        error_during_parsing = true;
        break;
      }
      switch (c) {
        case '-': sign = -1; break;
        case '+': sign = 1;  break;
      }
      sign_met = true;
      cursor++;
      continue;
    }
    // Unexpected char
    error_during_parsing = true;
    break;
  }

  if (error_during_parsing) return false;
  if (sign_met) x = x * sign;
  *out = x;
  return true;
}

static bool dub_internal_is_unsigned_integer(char* s, unsigned int* out) {
  // Pointer to string is NULL
  if (s == NULL) return false;
  // The string content is 0 length
  if (*s == '\0') return false;

  char* cursor = s;
  unsigned int x = 0;
  bool some_digit_met = false;
  bool error_during_parsing = false;

  while (true) {
    char c = *cursor;
    if ('0' <= c && c <= '9') {
      some_digit_met = true;
      x = x * 10;
      x = x + dub_internal_ascii_digit_to_int(c);
      cursor++;
      continue;
    }
    // End of string
    if (c == '\0') break;
    // Unexpected char
    error_during_parsing = true;
    break;
  }

  if (error_during_parsing) return false;
  *out = x;
  return true;
}

/*
  Email is validated positively if:
    - contains @
    - 1 <= username length <= 64
    - 3 <= domain length <= 255
*/
bool dub_internal_is_email(char* s, char** out) {
  // Pointer to string is NULL
  if (s == NULL) return false;
  // The string content is 0 length
  if (*s == '\0') return false;

  char* cursor = s;
  bool error_during_parsing = false;
  bool at_symbol_found = false;

  int username_length = 0;
  int domain_length = 0;

  while (true) {
    char c = *cursor;

    if (c == '\0') {
      if (domain_length < 3) error_during_parsing = true;
      break;
    }

    if (c == '@') {
      if (at_symbol_found) {
        error_during_parsing = true;
        break;
      }

      if (username_length < 1) {
        error_during_parsing = true;
        break;
      }

      at_symbol_found = true;
      cursor++;
      continue;
    }

    if (!at_symbol_found) {
      username_length++;
      if (username_length > 64) {
        error_during_parsing = true;
        break;
      }
      cursor++;
      continue;
    }

    if (at_symbol_found) {
      domain_length++;
      if (domain_length > 255) {
        error_during_parsing = true;
        break;
      }
      cursor++;
      continue;
    }
  }

  if (error_during_parsing) return false;
  *out = s;
  return true;
}

  /* Internal *///

Dub dub_create(int argc, char** argv) {
  return (Dub){
    .argc = argc,
    .argv = argv,
    .i = 1
  };
}

bool dub_end(Dub* dub) {
  return dub->i >= dub->argc;
}

char* dub_peek(Dub* dub) {
  if (dub_end(dub)) return NULL;
  return dub->argv[dub->i];
}

void dub_next(Dub* dub) {
  if (!dub_end(dub)) dub->i++;
}

bool dub_match(Dub* dub, char* s) {
  if (dub_end(dub)) return false;
  bool match = strcmp(dub_peek(dub), s) == 0;
  if (match) dub->i++;
  return match;
}

bool dub_parse_integer(Dub* dub, int* out) {
  if (dub_end(dub)) return false;

  char* s = dub_peek(dub);
  int i;
  if (!dub_internal_is_integer(s, &i)) return false;
  *out = i;
  dub->i++;
  return true;
}

bool dub_parse_unsigned_integer(Dub* dub, unsigned int* out) {
  if (dub_end(dub)) return false;

  char* s = dub_peek(dub);
  unsigned int i;
  if (!dub_internal_is_unsigned_integer(s, &i)) return false;
  *out = i;
  dub->i++;
  return true;
}

bool dub_parse_port(Dub* dub, unsigned int* out) {
  if (dub_end(dub)) return false;

  char* s = dub_peek(dub);
  unsigned int i;
  if (!dub_internal_is_unsigned_integer(s, &i)) return false;
  if (65535 < i) return false;
  *out = i;
  dub->i++;
  return true;
}

bool dub_parse_email(Dub* dub, char** out) {
  if (dub_end(dub)) return false;

  char* s = dub_peek(dub);
  char* email;
  if (!dub_internal_is_email(s, &email)) return false;
  *out = email;
  dub->i++;
  return true;
}

bool dub_parse_string(Dub* dub, char** out) {
  if (dub_end(dub)) return false;

  char* s = dub_peek(dub);
  *out = s;
  dub->i++;
  return true;
}

#endif /* DUB_IMPLEMENTATION */
#endif /* DUB_H */
