#include <stdio.h>

#define DUB_IMPLEMENTATION
#include "dub.h"

int main(int argc, char** argv) {
  Dub dub = dub_create(argc, argv);

  FILE* help_file = NULL;
#ifdef HELPGEN
  help_file = fopen("dub_help.txt", "a");
  setvbuf(help_file, NULL, _IONBF, 0);
#endif

#include "dub_dsl.h"
  dub.desc_indent++;
  if (MATCH("version")) {
    DESC("version: Get the version of the Dub app", help_file);
    printf("Dub version: 1.0.0\n");
  }
  else if (MATCH("plus")) {
    DESC("plus: sums up two integers", help_file); dub.desc_indent++;
    int a, b;
    if (PARSE_INT(&a) && PARSE_INT(&b)) {
      DESC("[arg] a: integer", help_file);
      DESC("[arg] b: integer", help_file);
      printf("%d + %d = %d\n", a, b, a + b);
    }
    else {
      printf("Bro, I need two numbers. Try `dub plus 2 2`\n");
    }
  }
  else if (MATCH("help") || MATCH("-h") || MATCH("--help")) {
    DESC("help|-h|--help: Prints the help menu", help_file);
    printf(
      "PASTE HERE THE dub_help.txt content\n"
    );
  }
  else {
    printf("do you need help? write `dub help`\n");
  }
#include "dub_dsl.h"

  return 0;
}