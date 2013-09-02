#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX (512 * 1024)

int counts[MAX];
int max = 1;

int count_and_add(const int n) {
  int c = 0;
  int m = n;
  while (n >= max) max <<= 1;
  while (m < max) {
    if (m >= n) {
      c += counts[m];
    }
    if (m <= n) {
      counts[m]++;
    }
    int k = m & (m - 1);
    m = k | ((m - k) << 1);
  }
  return c;
}

uint64_t count(const char *filename) {
  char line[BUFSIZ];
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror(filename);
    exit(1);
  }

  uint64_t c = 0;
  while (fgets(line, sizeof line, fp)) {
    int x = atoi(line);
    c += count_and_add(x);
  }
  fclose(fp);
  return c;
}

int main(int argc, char **argv) {
  time_t st = time(NULL);

  uint64_t c = count(argv[1]);
  printf("%lld,", c);

  time_t en = time(NULL);
  printf("%d\n", (int)(en - st));
  return 0;
}
