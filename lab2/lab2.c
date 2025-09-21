#include <stdio.h>
#include <string.h>

int main() {
  char input[256];

  while (1) {
    printf("Enter programs to run.\n>");
    if (fgets(input, sizeof(input), stdin) == NULL) {
      break; // EOF (Ctrl+D)
    }

    // Remove newline
    input[strcspn(input, "\n")] = '\0';
    printf("You typed: %s\n", input);
  }
  return 0;
}
