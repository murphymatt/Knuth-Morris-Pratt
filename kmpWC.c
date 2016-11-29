#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define linked list structure to hold values
typedef struct node {
  int val;
  struct node * next;
} node_t;

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

void push(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}

typedef struct kmp_ret {
  node_t * matches;
  int comparisons;
} kmp_ret_t;

int *compute_prefix_function(char *P) {

  int m = strlen(P);
  int *pi = malloc(sizeof(int) * m);
  pi[0] = -1;
  int k = -1;
  int q;

  for (q = 1; q < m; q++) {
    while (k > -1 && P[k+1] != P[q] && P[k+1] != '?') {
      k = pi[k];
    }
    if (P[q] == P[k+1] || P[k+1] == '?') {
      k++;
    }    
    pi[q] = k;
  }

  printf("pi =  { ");
  for (q = 0; q < m; q++) { printf("%d ", pi[q]); }
  printf("}\n");
  
  return pi;
}

int compute_wildcard_suffix(char *P) {
  int i, m = strlen(P), count = 0;
  for (i = m-1; i >= 0; i--) {
    if (P[i] != '?') { break; }
    count++;
  }
  // case (all WC) -> no backshift necessary
  if (count == m) { return 0; }
  return count;
}

kmp_ret_t *kmp_matcher(char *T, char *P) {

  kmp_ret_t *ret = malloc(sizeof(kmp_ret_t));
  ret->matches = malloc(sizeof(node_t));
  ret->matches->val = -1;
  ret->matches->next = NULL;
  
  int comp = 0;
  int n = strlen(T);
  int m = strlen(P);
  
  printf("T:  %s, T.length = %d\n", T, n);
  printf("P:  %s, T.length = %d\n", P, m);
  
  int *pi = compute_prefix_function(P);
  int q = -1, i, wCounter;
  int suffix_wildcards = compute_wildcard_suffix(P);
  printf("Wildcard chars:  %d\n", suffix_wildcards);

  for (i = 0; i < n; i++) {
    // if match doesnt match text and not prefix counter
    //   set back match to previous prefix value
    wCounter = 0;
    while (q > -1 && (P[q+1] != T[i] && P[q+1] != '?')) {
      wCounter++;
      q = pi[q];
    }

    if (wCounter < 1) {
      comp++;
    } else {
      comp += wCounter;
    }
      
    // increase match counter
    comp++;
    if (P[q+1] == T[i] || P[q+1] == '?') {
      q++;
    }

    // if match len is pattern len
    //   set back match len to prefix len
    if (q == m-1) {
      push(ret->matches, i-(m-1));
      printf("Pattern matching occurs with shift %d.\n", i-(m-1));
      q = pi[q];
      i -= suffix_wildcards;
    }
  }

  pop(&(ret->matches));
  ret->comparisons = comp;
  printf("Character comparisons:  %d\n\n", ret->comparisons);
  return ret;
}

int main(int argc, char *argv[]) {
  
  char *tests[][2] = {
    {"banana", "ana"},
    {"ababaca", "a?a"},
    {"ababaaca", "a??"},
    {"ababaca", "?a"},
    {"big bubble blowing baby", "b??"},
    {"big bubble blowing baby", "g?b"},
    {"aaaaa", "a?"},
    {"abababa", "?"},
    {"", "abc"},
    {"aaaaaaaaaa", "abcabcabc"},
    {"aaaaaaaa", "a????"},
    {"aabaabaab", "aaa"}
  };

  for (int i = 0; i < 12; i++) {
    printf("Test case %d:\n", i);
    kmp_matcher(tests[i][0], tests[i][1]);
  }
  
  return 0;
}
