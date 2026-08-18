# dub.h - Routing tree CLA parser

This library helps to parse and handle Command line arguments.

The feature of Dub is that commands routing and handling are integrated. You can execute arbitrary code at any point or branch during the routing process.

The library provides two types of functions:
- Non-moving the current argument pointer:
  ```
  dub_create dub_end dub_peek
  ```
- Moving the current argument pointer:
  ```
  dub_next
  dub_match
  dub_parse_*
  ```

`dub_parse_*` and `dub_match` will move the argument pointer only when the parsing/matching is successfull.


# API
```c
Dub   dub_create(int argc, char** argv); // Create a Dub instance
bool  dub_end(Dub* dub);  // Have you reached the end? (No more args to parse)
char* dub_peek(Dub* dub); // Get the current argument. (NULL when at the end)
void  dub_next(Dub* dub); // Move to the next argument. (dub->i++)

bool dub_match(Dub* dub, char* s);
bool dub_parse_integer(Dub* dub, int* out);
bool dub_parse_unsigned_integer(Dub* dub, unsigned int* out);
bool dub_parse_port(Dub* dub, unsigned int* out);
bool dub_parse_email(Dub* dub, char** out);
bool dub_parse_string(Dub* dub, char** out);
```

# Usage

## Python http.server & calendar larping

```sh
python -m http.server
python -m http.server 9292
python -m http.server 9292 --bind 127.0.0.1
python -m calendar
```

```c
#include <stdio.h>
#include <time.h>

#define DUB_IMPLEMENTATION
#include "dub.h"

int main(int argc, char** argv) {
  Dub dub = dub_create(argc, argv);

  char* host = "0.0.0.0";
  unsigned int port = 8000;

  if (dub_match(&dub, "-m")) {
    if (dub_match(&dub, "http.server")) {
      dub_parse_port(&dub, &port);
      if (dub_match(&dub, "--bind")) {
        if (!dub_parse_string(&dub, &host)) {
          printf("Invalid bind option. expected `--bind 127.0.0.1`");
          return -1;
        }
      }
      printf("Serving HTTP on %s port %d (http://%s:%d/) ...\n", host, port, host, port);
      return 0;  
    }
    if (dub_match(&dub, "calendar")) {
      time_t t = time(NULL);
      struct tm *today = localtime(&t);
      printf("Today's Date: %04d-%02d-%02d\n", 
            today->tm_year + 1900, 
            today->tm_mon + 1, 
            today->tm_mday);
      return 0;
    }
    printf("No module named %s", dub_peek(&dub));
    return -1;
  }

  if (!dub_end(&dub)) printf("Unknow command: %s\n", dub_peek(&dub));
  return -1;
}
```

## Hello world:

```
dub version
dub plus 7 8
dub help
dub -h
dub --help
```

```c
#include <stdio.h>

#define DUB_IMPLEMENTATION
#include "dub.h"

int main(int argc, char** argv) {
  Dub dub = dub_create(argc, argv);

  if (dub_match(&dub, "version")) {
    printf("Dub version: 1.0.0\n");
  }
  else if (dub_match(&dub, "plus")) {
    int a, b;
    if (dub_parse_integer(&dub, &a) && dub_parse_integer(&dub, &b))
      printf("%d + %d = %d\n", a, b, a + b);
    else
      printf("Bro, I need two numbers. Try `dub plus 2 2`\n");
  }
  else if (dub_match(&dub, "help") || dub_match(&dub, "-h") || dub_match(&dub, "--help")) {
    printf("Help yourself!\n");
  }
  else {
    printf(
      "Usage:\n"
      "version\n"
      "plus <int> <int>\n"
      "help --help -h\n"
    );
  }

  return 0;
}
```

## Parsing optional flags in any order

```c
#include <stdio.h>

#define DUB_IMPLEMENTATION
#include "dub.h"

int main(int argc, char** argv) {
  Dub dub = dub_create(argc, argv);

  bool verbose = false;
  bool ignore_case = false;
  bool first_try = false;

  // dub -v -f -i
  while (!dub_end(&dub)) {
    /**/ if (dub_match(&dub, "-v")) verbose = true;
    else if (dub_match(&dub, "-i")) ignore_case = true;
    else if (dub_match(&dub, "-f")) first_try = true;
    else break;
  }

  printf("I'll do my job and:\n");
  if (verbose) printf("\tbe verbose\n");
  if (ignore_case) printf("\tignore case\n");
  if (first_try) printf("\tsucceed in first try!\n");

  return 0;
}
```

# Inspiration

Dub was inspired by [Roda](https://github.com/jeremyevans/roda)

