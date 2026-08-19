#include <stdlib.h>
#include <stdio.h>

// Configure:
//   - maximum command length
//   - executable name
//   - commands list

#define MAX_CMD_LENGTH 256

#if defined(_WIN32) || defined(_WIN64)
  #define EXECUTABLE_NAME "dub.exe"
#else
  #define EXECUTABLE_NAME "./dub"
#endif

int main() {
  // Write here the commands which will traverse all your CLI app tree
  const char* commands[] = {
    "version",
    "plus 5 10",
    "help"
  };
  int num_commands = sizeof(commands) / sizeof(commands[0]);
  char system_cmd[MAX_CMD_LENGTH];
  for (int i = 0; i < num_commands; i++) {
    snprintf(system_cmd, sizeof(system_cmd), "%s %s", EXECUTABLE_NAME, commands[i]);
    int result = system(system_cmd);
    printf("%d %d\n", i, result);
  }
  return 0;
}