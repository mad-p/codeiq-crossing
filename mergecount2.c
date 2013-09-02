#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_POINTS 400000
int perm[MAX_POINTS];
int buff[MAX_POINTS];
int runs[MAX_POINTS];

// Count inversions by merging runs in perm[].
// Use buf[] as a buffer.
// Beginning of runs are listed in runs[] and updated in place.
uint64_t merge_count(const int n, int *perm, int *buf, int runs[]) {
  uint64_t c = 0;
  int *in = perm; int *out = buf;
  while (runs[1] < n) {
    int rin = 0, rout = 1;
    int *s = out;
    while (s < out + n) {
      // Take two runs and merge, repeat.
      if (runs[rin+1] == n) {
        // If we have left only one run, leave it unmerged.
        int *p = in + runs[rin];  int *pe = in + n;
        while (p < pe) *s++ = *p++;
        runs[rout] = n;
        break;
      }
      // Now we are sure that we have two runs.
      int *p = in + runs[rin];  int *pe = in + runs[rin+1];
      int *q = pe;              int *qe = in + runs[rin+2];
      runs[rout++] = runs[rin += 2];
      while (p < pe || q < qe) {
        if (p == pe) {              // already done in left
          while (q < qe) *s++ = *q++;
          break;
        } else if (q == qe) {       // already done in right
          while (p < pe) *s++ = *p++;
          break;
        } else if (*p < *q) {       // not inverted
          *s++ = *p++;
        } else {                    // inverted
          *s++ = *q++;
          c += pe - p;
        }
      }
    }
    // Swap buffer
    int *tmp = in;
    in = out;
    out = tmp;
  }
  return c;
}

// Find beginning of runs while reading.
// Return number of data points.
int read_file(char *filename) {
  char line[BUFSIZ];
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror(filename);
    exit(1);
  }

  int n = 0;
  int r = 0;
  int last = MAX_POINTS;
  while (fgets(line, sizeof line, fp)) {
    int x = perm[n] = atoi(line) - 1;
    if (x < last) {
      runs[r++] = n;
    }
    last = x;
    n++;
  }
  fclose(fp);
  runs[r] = n;
  return n;
}

int main(int argc, char **argv) {
  time_t st = time(NULL);

  int n = read_file(argv[1]);

  uint64_t c = merge_count(n, perm, buff, runs);
  printf("%lld,", c);
  
  time_t en = time(NULL);
  printf("%d\n", (int)(en - st));

  return 0;
}
