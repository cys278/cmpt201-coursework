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
