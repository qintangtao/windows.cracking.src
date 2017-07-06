#include <stdlib.h>

int main()
{
  printf("hello\n");
  /* bonjour
  tres cher */
  /* float f; */
  int test1 = 13;
  int test2 = 4;
  int tmp1 = test1;
  int tmp2 = test2;
  int q = 0;
  int r = test1;
  int i;
  
  printf("+%d=%d\n", 5, +5);
  printf("%d %d\n", 12, 92345);
  printf("Type a caracter : ");
  putchar(getchar());
  putchar('\n');
  
  for(i=0;i<10;i++)
  {
    if (i == 5)
    {
      break;
    }
    printf("%d\n", i);
  }
  
  do
  {
    if (i <= 10)
    {
      if (i == 5)
        break;
      printf("shouldn't happen\n");
    }
  } while(1);
  
  for(i=0;i<10;i++)
  {
    if (i == 5)
    {
      continue;
    }
    if (i == 5)
    {
      printf("shouldn't happen\n");
    }
    printf("%d\n", i);
  }
  i = 0;
  do
  {
    i++;
    if (i == 5)
    {
      while(1) break;
      continue;
    }
    if (i == 5)
    {
      printf("shouldn't happen\n");
    }
    printf("%d\n", i);
  } while(i <= 10);

  if (0 == 0) printf("0==0\n");
  if (0 == 1) printf("0==1\n");
  if (1 == 0) printf("1==0\n");
  if (1 == 1) printf("1==1\n");
  while(tmp1) 
  {
    tmp1--;
    tmp2--;
    if (tmp2 == 0)
    {
      q++;
      r = tmp1;
      tmp2 = test2;
    }
    printf("hello\n");
  }
  printf("q = %d, r= %d\n", q, r);
  printf("q = %d, r= %d\n", (test1 / test2), (test1 % test2));
  printf("%d\n", 4 << 2);
  printf("%d\n", 4 >> 2);
  printf("%d\n", 4 >> 3);

  int square = 1;
  for(;square<10;square++)
  {
    printf("%d\n", square * square);
  }

  int a = (1);// ? 2 : 3; // c++ comment
  a = a + a;
  if (a >= a) printf("a >= a\n");
  if (a > a) printf("a > a !!!\n"); else printf("a > a : NO\n");
  if (2 >= 2) printf("2 >= 2\n");
  if (2 >= 1) printf("2 >= 1\n");
  if (1 >= 2) printf("1 >= 2 !!!\n"); else printf("1 >= 2 : NO\n");
  if (1 >= 1) printf("1 >= 1\n");
  if (1 >= 0) printf("1 >= 0\n");
  if (0 >= 0) printf("0 >= 0\n");
  if (0 >= 1) printf("0 >= 1 !!!\n"); else printf("0 >= 1 : NO\n");
  if (2 > 2) printf("2 > 2 !!!\n"); else printf("2 > 2 : NO\n");
  if (2 > 1) printf("2 > 1\n");
  if (1 > 1) printf("1 > 1 !!!\n"); else printf("1 > 1 : NO\n");
  if (1 > 0) printf("1 > 0\n");
  if (0 > 1) printf("0 > 1 !!!\n"); else printf("0 > 1 : NO\n");
  if (0 > 0) printf("0 > 0 !!!\n"); else printf("0 > 0 : NO\n");
  if (0 < 0) printf("0 < 0 !!!\n"); else printf("0 < 0 : NO\n");
  if (0 < 1) printf("0 < 1\n");
  while(--a && 0) ;
  do
  {
  } while(0 && --a);
  //for(;a;a=0);
  
  a = (1 != 2);
  //if (1 != 2)
    printf("1 != 2\n");
  
  a = ((1) ? 2 : 3);
  if (0 || 0 || (1 && (0 || ((!2) ? 1 : 0))))
    printf("%c\n", a + 48);
  
  int c = 0;
  c = 0;
  for(i=0;i<(++c ? 5 : 10);i++)
  {
    printf("oh oh %c\n", (1) ? i + 48 : 0);
  }
  
  
  if ((a && !a) || !(a && !a) || a)
  {
    printf("foo\n");
  }
  else
    printf("bar\n");
  
  {
    a++;
    int a = 2*3;
    /* f(1, a, 2); */
    do
    for (a;(a-1)+1;a=--a-1) {
      printf("%d\n", a);
      int a = 3;
      a += 5;
    }
    while(0);
    while(a+1);
    a += 5;
  }
  a = 3;
  {
    a++;
    int a = 2;
    for (a+1;0;a++) {
      int a = 3;
      a += 5;
    }
    for(a;a;a);
    a += 5;
  }
  /*
  int b = a += 2, f, g= 3 & 4 +5;
  int d=!!(34- -33*-(--f + main(5) - 6)+5+a++ <= 3);
  int c = (a * b * d)/(d=a=3);
  int e = d <= 25;
  int z;
  z = 33;
  z = 34;
  z++ + 3;
  ++(z) + 3;
  while (e) {}
  while(e);;
  {
    int c = 3;
  }
  if (a != 3)
  { printf("foo"); }
  else if (z==2)
  {printf("bar");}
  else if (z==3);
  if (a)
    if (b)
      ;
    else
      ;
  else
    ;
  while(e)
  {
    e++;
    e=0;
  }
  if (a) do if (z);  while(0);
  0 <= 1;
  for(a=1;a>=5;a++) { printf("foo"); } 
  //for(a=1,c=2, putchar(3);a>=5;a++) {} 

  putchar(c + 48);
  putchar(10);
  putchar('a');
  putchar(32 + 'b');
  printf("hello");
  putchar(10+3 ? 10 ? 1 : 2 : 3);
  a = 3 ? 2 : 1;*/
}
