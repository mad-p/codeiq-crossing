#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 400000

int n;
int perm[MAX_POINTS];
char visited[MAX_POINTS];

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

long long int count() {
  long long int c = 0;
  int p, q;
  for (p=1; p<n; p++) {
    for (q=0; q < p; q++)
      if (perm[q] > perm[p])
        c++;
  }
  return c;
}

int main(int argc, char **argv) {
  read_file(argv[1]);
  // printf("%d\n", n);
  long long int c = count();
  printf("%lld\n", c);
  return 0;
}
