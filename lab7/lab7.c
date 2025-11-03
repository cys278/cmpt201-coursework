// lab7.c - CMPT 201 Lab 7: MapReduce mini

#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_INPUTS = 1024, MAX_LINE_NUMS = 1024 };

typedef struct {
  int line_number; // 1,2,3...
  int value;       // raw used input
} Input;

typedef struct {
  int line_number;   // preserved key (line number)
  int doubled_value; // value *2
} IntermediateInput;

typedef struct {
  int doubled_value;               // the grouped key
  int line_numbers[MAX_LINE_NUMS]; // all line numbers that produced it
  int line_count;                  // how many
} Output;

/* ------ Map ------*/

/* Populate intermediate_input from input:
 * double_value=input->value *2
 * line_number = input->line_number
 * */

void map(Input *input, IntermediateInput *intermediate_input) {
  // Ensuring pointers are valid
  assert(input != NULL && intermediate_input != NULL);

  intermediate_input->line_number = input->line_number;
  intermediate_input->doubled_value = input->value * 2;
}

/* ------ Group ------*/
/*
 * For one IntermediateInput:
 * If its doubled_value already exists in output[0..*result_count-1],
 * append its line_number to that Output's line_numbers.
 * Otherwise create a new Output entry.
 *
 * result_count is in/out: increment when we add a new group
 * */

void groupByKey(IntermediateInput input, Output output[], int *result_count) {
  assert(output != NULL && result_count != NULL);
  assert(*result_count >= 0 && *result_count <= MAX_INPUTS);

  // 1) Search for existing group with same doubled_value
  for (int i = 0; i < *result_count; i++) {
    if (output[i].doubled_value == input.doubled_value) {
      // Append line number to this existing group
      assert(output[i].line_count < MAX_LINE_NUMS);
      output[i].line_numbers[output[i].line_count++] = input.line_number;
      return;
    }
  }

  // 2) Not found - create new group at the end
  assert(*result_count < MAX_INPUTS);
  Output *slot = &output[*result_count];
  slot->doubled_value = input.doubled_value;
  slot->line_count = 0;
  assert(slot->line_count < MAX_LINE_NUMS);
  slot->line_numbers[slot->line_count++] = input.line_number;

  (*result_count)++;
}

/*------ Reduce ------*/

/* Print one Output in the exact format:
 * (value, [a,b,c])
 * */

void reduce(Output output) {
  printf("(%d, [", output.doubled_value);
  for (int i = 0; i < output.line_count; i++) {
    if (i > 0)
      printf(", ");
    printf("%d", output.line_numbers[i]);
  }
  printf("])\n");
}

/* ------ Helpers / main ------ */

static bool line_is_end(const char *s) {
  // Accept "end" with or without trailing newline/spaces
  if (!s)
    return false;
  while (*s == ' ' || *s == '\t')
    s++;
  return strncmp(s, "end", 3) == 0 &&
         (s[3] == '\n' || s[3] == '\0' || s[3] == ' ' || s[3] == '\t');
}

int main(void) {
  Input inputs[MAX_INPUTS];
  IntermediateInput mids[MAX_INPUTS];
  Output outputs[MAX_INPUTS];

  int n_inputs = 0;

  printf("Enter values (one per line). Type 'end' to finish: \n");

  char buf[128];
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    if (line_is_end(buf))
      break;

    // Parse integer on the line
    char *endp = NULL;
    errno = 0;
    long v = strtol(buf, &endp, 10);
    if (errno != 0 || endp == buf) {
      // Not a number; ignore and keep reading
      fprintf(stderr, "Ignoring invalid input: %s", buf);
      continue;
    }

    assert(n_inputs < MAX_INPUTS);
    inputs[n_inputs].line_number = n_inputs + 1; // 1-based
    inputs[n_inputs].value = (int)v;
    n_inputs++;
  }

  // Map
  for (int i = 0; i < n_inputs; i++) {
    map(&inputs[i], &mids[i]);
  }

  // Group
  int n_groups = 0;
  for (int i = 0; i < n_inputs; i++) {
    groupByKey(mids[i], outputs, &n_groups);
  }

  // Reduce (print results in insertion order)
  for (int i = 0; i < n_groups; i++) {
    reduce(outputs[i]);
  }

  return 0;
}
