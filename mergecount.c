#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_POINTS 400000
int n = 0;
int perm[MAX_POINTS];
int buff[MAX_POINTS];

// Count inversions by merging a[l..m], a[m..r] into b[l..r]
// Range starting is inclusive, while end is exclusive
uint64_t merge_count(int a[],
                     int l, int m, int r,
                     int b[]) {
  int *p = a + l;  int *pe = a + m;
  int *q = a + m;  int *qe = a + r;
  int *s = b + l;
  uint64_t c = 0;
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
  return c;
}

// Count inversions in a[st..en] (st inclusive, en exclusive)
// At the same time, sort a[st..en] inplace, by using b[st..en] as a buffer.
uint64_t count(int a[], int st, int en, int b[]) {
  if (st + 1 == en) {
    return 0ull;
  } else if (st + 2 == en) {
    if (a[st] < a[st+1]) {
      return 0ull;
    } else {
      int tmp = a[st+1];
      a[st+1] = a[st];
      a[st] = tmp;
      return 1ull;
    }
  }
  int md = (st + en) / 2;
  uint64_t result = count(a, st, md, b);
  result += count(a, md, en, b);
  result += merge_count(a, st, md, en, b);
  memcpy(&a[st], &b[st], (en - st) * sizeof(int));
  return result;
}

void read_file(char *filename) {
  char buf[BUFSIZ];
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror(filename);
    exit(1);
  }

  while (fgets(buf, sizeof buf, fp)) {
    perm[n++] = atoi(buf) - 1;
  }
  fclose(fp);
}

int main(int argc, char **argv) {
  time_t st = time(NULL);

  read_file(argv[1]);

  uint64_t c = count(perm, 0, n, buff);
  printf("%lld,", c);
  
  time_t en = time(NULL);
  printf("%d\n", (int)(en - st));

  return 0;
}
