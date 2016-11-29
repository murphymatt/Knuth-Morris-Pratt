#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *compute_prefix_function(char *P) {

  int m = strlen(P);
  int *pi = malloc(sizeof(int) * m);
  pi[0] = -1;
  int k = -1;
  int q;

  for (q = 1; q < m; q++) {
    while (k > -1 && P[k+1] != P[q]) {
      k = pi[k];
    }
    if (P[k+1] == P[q]) {
      k++;
    }
    pi[q] = k;
  }

  for (q = 0; q < m; q++) { printf("%d ", pi[q]); }
  printf("\n");
  
  return pi;
}

void kmp_matcher(char *T, char *P) {

  printf("T:  %s\n", T);
  printf("P:  %s\n", P);

  int comp = 0;
  int n = strlen(T);
  int m = strlen(P);
  int *pi = compute_prefix_function(P);
  int q = -1, i, wCounter;
  
  for (i = 0; i < n; i++) {
    // if match doesnt match text and not prefix counter
    //   set back match to previous prefix value
    wCounter = 0;
    while (q > -1 && P[q+1] != T[i]) {
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
    if (P[q+1] == T[i]) {
      q++;
    }
    //    printf("%d ", q);
    // if match len is pattern len
    //   set back match len to prefix len
    if (q == m-1) {
      printf("Pattern matching occurs with shift %d.\n", i-(m-1));
      q = pi[q];
    }
  }
  
  printf("Length T:  %d\n", n);
  printf("Length P:  %d\n", m);
  printf("Character comparisons:  %d\n\n", comp);
}

int main(int argc, char *argv[]) {

  char *tests[][2] = {
    {"banana", "ana"},
    {"ababaca", "aba"},
    {"ababaca", "a"},
    {"the ball rolled down the hill", "ll"},
    {"aaaaaaaaaa", "a"},
    {"aaaaaaaaaa", "aa"},
    {"aaaaaaaaaa", "aaaaaaaaaa"},
    {"abcd", ""}
  };

  for (int i = 0; i < 8; i++) {
    kmp_matcher(tests[i][0], tests[i][1]);
  }
  
  return 0;
}
