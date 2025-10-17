/* ---- example_1.c ---- */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

static Node *push_back(Node *head, int v) {
  Node *n = (Node *)malloc(sizeof(*n));
  if (!n) {
    perror("malloc");
    exit(1);
  }
  n->value = v;
  n->next = NULL;

  if (!head)
    return n;
  Node *t = head;
  while (t->next)
    t = t->next;
  t->next = n;
  return head;
}

static void print_list(const Node *h) {
  for (const Node *c = h; c; c = c->next)
    printf("%d", c->value);
  printf("\n");
}

static void free_list(Node *h) {
  while (h) {
    Node *n = h->next;
    free(h);
    h = n;
  }
}

int main(void) {
  Node *head = NULL;
  head = push_back(head, 10);
  head = push_back(head, 20);
  head = push_back(head, 30);
  print_list(head);
  free_list(head);
  return 0;
}
/* ---- end example_1.c ---- */
/* ---- example_2.c ---- */
// example_2.c (FIXED + ASSERTS)
// The hidden bug in many starter version: updating the runnig sum/count
// Incorrectly on removal
// This file is keeping an Info struct {sum, count} and varifies via list_sum()

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct {
  long long sum; // wide to avoid overflow
  int count;
} Info;

/* ------ Assertions ------ */

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr,                                                          \
              "ASSERT FAIL: %s\n"                                              \
              "File: %s Line: %d\n"                                            \
              "Message: %s\n",                                                 \
              #cond, __FILE__, __LINE__, (msg));                               \
      abort();                                                                 \
    }                                                                          \
  } while (0)

/*Equality Helper*/

#define ASSERT_EQUALS(exp, act, msg, fmt)                                      \
  do {                                                                         \
                                                                               \
    __typeof__(exp) _e = (exp);                                                \
    __typeof__(act) _a = (act);                                                \
    if (!(_e == _a)) {                                                         \
      fprintf(stderr,                                                          \
              "ASSERT_EQUALS FAIL\n"                                           \
              "File: %s Line: %d\n"                                            \
              "%s\n"                                                           \
              "expected=" fmt "actual=" fmt "\n",                              \
              __FILE__, __LINE__, (msg), _e, _a);                              \
      abort();                                                                 \
    }                                                                          \
  } while (0)

/* ------ List Helpers ------ */

static Node *push_front(Node *head, int v, Info *info) {
  Node *n = (Node *)malloc(sizeof(*n));
  if (!n) {
    perror("malloc");
    exit(1);
  }
  n->value = v;
  n->next = head;

  // Updating info correctly on insert
  info->sum += v;
  info->count += 1;

  // Post-insert assertions
  ASSERT(n != NULL, "new node must exist");
  if (head)
    ASSERT(n->next == head, "new head must point to old head");
  ASSERT(info->count > 0, "count must be positive after insert");
  return n;
}

static Node *remove_first(Node *head, Info *info) {
  if (!head)
    return NULL;
  // BUG IN MANY STARTERS: subtracting after free or using wrong node
  // FIX: read the value first, then adjust info then free
  int v = head->value;
  Node *next = head->next;
  info->sum -= v; // adjusting before free
  info->count -= 1;

  free(head);

  // post remove assertions
  ASSERT(info->count >= 0, "count must never go negative");
  if (!next) {
    // new list is empty -> sum should be 0
    ASSERT_EQUALS(0LL, info->sum, "sum must be zero when empty", "%lld");
  }
  return next;
}

static long long list_sum(const Node *head) {
  long long s = 0;
  for (const Node *c = head; c; c = c->next)
    s += c->value;
  return s;
}

static int list_count(const Node *head) {
  int k = 0;
  for (const Node *c = head; c; c = c->next)
    k++;
  return k;
}

static void free_list(Node *h) {
  while (h) {
    Node *n = h->next;
    free(h);
    h = n;
  }
}

/* ------ Main ------ */

int main(void) {
  Info info = {.sum = 0, .count = 0};
  Node *head = NULL;

  // Build : [30, 20, 10]
  head = push_front(head, 10, &info);
  head = push_front(head, 20, &info);
  head = push_front(head, 20, &info);

  // Side properties after build
  ASSERT_EQUALS(list_sum(head), info.sum, "sum mismatch after build", "%lld");
  ASSERT_EQUALS(list_count(head), info.count, "count mismatch after build",
                "%d");

  // remove two items
  head = remove_first(head, &info); // removes 30
  ASSERT_EQUALS(list_sum(head), info.sum, "sum mismatch after 1st remove",
                "%lld");
  ASSERT_EQUALS(list_count(head), info.count, "count mismatch after 1st remove",
                "%d");

  head = remove_first(head, &info); // removes 20
  ASSERT_EQUALS(list_sum(head), info.sum, "sum mismacth after 2nd remove",
                "%lld");
  ASSERT_EQUALS(list_count(head), info.count, "count mismatch after 2nd remove",
                "%d");

  // Final remove (at this point list becomes empty)
  head = remove_first(head, &info);
  ASSERT(head == NULL, "list should now be empty"); // removes 10
  ASSERT_EQUALS(0LL, info.sum, "sum should be 0 for empty list", "%lld");
  ASSERT_EQUALS(0, info.count, "count should be 0 for empty list", "%d");

  // Clean up (no-op when empty)
  free_list(head);
  return 0;
}
/* ---- end example_2.c ---- */
