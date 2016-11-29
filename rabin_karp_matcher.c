#include <stdio.h>
#include <math.h>
#include <string.h>

void rabin_karp_matcher(char* T, char* P, int d, int q) {
  int n = strlen(T), m = strlen(P);
  int h = ((int) pow(d, m-1)) % q;
  int t[n];
  int p = 0;
  t[0] = 0;

  int i;
  for (i = 1; i < m; i++) {
    p    = (d * p + P[i]) % q;
    t[0] = (d * t[0] + T[i]) % q;
  }

  int s;
  for (s = 0; s < n-m; s++) {
    if (p == t[s]) {
      char* tSub;
      strncpy(tSub, T+s, m);
      if (!memcmp(tSub, P, m)) {
	printf("Pattern occurs with shift %d\n", s);
      }
    }
    if (s < n - m) {
      t[s+1] = (d * (t[s] - T[s+1] * h) + T[s + m + 1]) % q;
    }
  }
}

int main() {
  char* text    = "Connor Mulligan is a ginger";
  char* pattern = "Mulligan";
  rabin_karp_matcher(text, pattern, 26, 13);
  return 0;
}
