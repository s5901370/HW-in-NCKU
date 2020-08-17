#include <stdio.h>

int main()
{
  unsigned char c;

  while(scanf("%u", &c) != EOF) {
    printf("%u ",c);
  }

  //scanf("%u", &c);
  printf("hello %u",c);
  fflush(stdout);
  return 0;
}
