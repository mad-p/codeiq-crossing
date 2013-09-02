#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_POINTS 400000
#define BITS_PER_SLOT 64
#define SLOTS (MAX_POINTS/BITS_PER_SLOT)

int n;
uint64_t bits[SLOTS];
unsigned char counts[SLOTS];

inline int bitcount(uint64_t b) {
  return __builtin_popcountll(b);
}

inline int bitset(int x) {
  int slot = x / BITS_PER_SLOT;
  int bit = x & (BITS_PER_SLOT-1);
  bits[slot] |= (uint64_t)1 << bit;
  counts[slot] = bitcount(bits[slot]);
  if (x > n) n = x;
}

// count number of bits set in the range min..max (both end inclusive)
inline uint64_t bitcount_range(int min, int max) {
  if (min > max) return 0;
  int nslot = min / BITS_PER_SLOT;
  int nbit  = min & (BITS_PER_SLOT-1);
  uint64_t nmask = (- (((uint64_t)1) << nbit));
  int xslot = max / BITS_PER_SLOT;
  int xbit  = max & (BITS_PER_SLOT-1);
  uint64_t xmask = xbit == 63 ? 0xffffffffffffffff : ((((uint64_t)1) << (xbit + 1)) - 1);
  uint64_t c = 0;
  int i;
  for (i = nslot + 1; i < xslot; i++) c += counts[i];
  if (nslot == xslot) {
    c += bitcount(bits[nslot] & nmask & xmask);
  } else {
    c += bitcount(bits[nslot] & nmask);
    c += bitcount(bits[xslot] & xmask);
  }
  return c;
}

int main(int argc, char **argv) {
  time_t st = time(NULL);

  char buf[BUFSIZ];
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror(argv[1]);
    exit(1);
  }

  uint64_t c = 0;
  while (fgets(buf, sizeof buf, fp)) {
    int x = atoi(buf) - 1;
    if (x < n) {
      uint64_t cnt = bitcount_range(x + 1, n);
      c += cnt;
    }
    bitset(x);
  }
  printf("%llu,", c);

  time_t en = time(NULL);
  printf("%d\n", (int)(en - st));

  return 0;
}
