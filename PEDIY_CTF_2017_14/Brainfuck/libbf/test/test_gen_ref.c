#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  unsigned int* data_ptr = calloc(32768, sizeof(unsigned int));
  data_ptr += 1024;
  data_ptr[58] = 0;
  data_ptr[34] = 0;
  data_ptr[7] = 0;
  data_ptr[14] = 0;
  data_ptr[15] = 0;
  data_ptr[16] = 0;
  data_ptr[17] = 0;
  data_ptr[27] = 0;
  data_ptr[10] = 0;
  data_ptr[8] = 0;
  data_ptr[9] = 0;
  data_ptr[11] = 0;
  data_ptr[12] = 0;
  data_ptr[13] = 0;
  data_ptr[18] = 0;
  data_ptr[19] = 0;
  data_ptr[24] = 0;
  data_ptr[6] = 104;
  putchar(data_ptr[6]);
  data_ptr[6] = 101;
  putchar(data_ptr[6]);
  data_ptr[6] = 108;
  putchar(data_ptr[6]);
  data_ptr[6] = 108;
  putchar(data_ptr[6]);
  data_ptr[6] = 111;
  putchar(data_ptr[6]);
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[47] = 13;
  data_ptr[48] = 4;
  data_ptr[49] = 13;
  data_ptr[50] = 4;
  data_ptr[51] = 0;
  data_ptr[52] = 13;
  data_ptr[41] = 5;
  data_ptr[6] = 43;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = 5;
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = 5;
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = 5;
  data_ptr[20] = data_ptr[2];
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = 5;
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = 5;
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = 5;
  data_ptr[20] = data_ptr[2];
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = 12;
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = 12;
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = 12;
  data_ptr[20] = data_ptr[2];
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = 92345;
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[25] = 0;
  data_ptr[21] = 92345;
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = 92345;
  data_ptr[20] = data_ptr[2];
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[6] = 84;
  putchar(data_ptr[6]);
  data_ptr[6] = 121;
  putchar(data_ptr[6]);
  data_ptr[6] = 112;
  putchar(data_ptr[6]);
  data_ptr[6] = 101;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 97;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 99;
  putchar(data_ptr[6]);
  data_ptr[6] = 97;
  putchar(data_ptr[6]);
  data_ptr[6] = 114;
  putchar(data_ptr[6]);
  data_ptr[6] = 97;
  putchar(data_ptr[6]);
  data_ptr[6] = 99;
  putchar(data_ptr[6]);
  data_ptr[6] = 116;
  putchar(data_ptr[6]);
  data_ptr[6] = 101;
  putchar(data_ptr[6]);
  data_ptr[31] = 100;
  data_ptr[6] = 114;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 58;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[45] = getchar();
  putchar(data_ptr[45]);
  data_ptr[45] = 10;
  putchar(data_ptr[45]);
  data_ptr[39] = 1;
  data_ptr[46] = 0;
  data_ptr[53] = 0;
  data_ptr[33] = 0;
  data_ptr[36] = 1;
  do
  {
    data_ptr[13] = data_ptr[53];
    data_ptr[11] = data_ptr[53];
    data_ptr[12] = 10;
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[33] = 1;
      data_ptr[36] = 0;
    }
  }
  while (data_ptr[36] != 0);
  data_ptr[37] = data_ptr[33];
  while(data_ptr[37] != 0)
  {
    data_ptr[17] = data_ptr[53];
    data_ptr[16] = 5;
    data_ptr[46] = 1;
    data_ptr[15] = 0;
    while(data_ptr[14] != 0)
    {
      data_ptr[14] -= 1;
      data_ptr[16] = 0;
      data_ptr[15] = data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[15] != 0)
      {
        data_ptr[15] = 0;
        data_ptr[46] -= 1;
      }
    }
    data_ptr[16] -= data_ptr[17];
    data_ptr[17] = 0;
    if(data_ptr[16] != 0)
    {
      data_ptr[16] = 0;
      data_ptr[46] -= 1;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[39] = 0;
      data_ptr[36] = 0;
    }
    data_ptr[36] = data_ptr[39];
    if(data_ptr[36] != 0)
    {
      data_ptr[4] = 0;
      data_ptr[3] = data_ptr[53];
      data_ptr[25] = data_ptr[1];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] = 10;
      putchar(data_ptr[6]);
      data_ptr[36] = 0;
    }
    data_ptr[36] = data_ptr[39];
    if(data_ptr[36] != 0)
    {
      data_ptr[53] += 1;
      data_ptr[36] = 0;
    }
    data_ptr[33] = 0;
    data_ptr[36] = data_ptr[39];
    if(data_ptr[36] != 0)
    {
      data_ptr[13] = data_ptr[53];
      data_ptr[11] = data_ptr[53];
      data_ptr[12] = 10;
      data_ptr[46] = 0;
      data_ptr[9] += 1;
      data_ptr[8] += 1;
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      while(data_ptr[10] != 0)
      {
        data_ptr[10] -= 1;
        data_ptr[9] -= 1;
        data_ptr[8] -= 1;
        data_ptr[46] += 1;
        if(data_ptr[12] != 0)
        {
          data_ptr[12] = 0;
          data_ptr[46] -= 1;
        }
      }
      while(data_ptr[9] != 0)
      {
        data_ptr[9] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        data_ptr[8] -= data_ptr[10];
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
      while(data_ptr[8] != 0)
      {
        data_ptr[8] -= 1;
        while(data_ptr[13] != 0)
        {
          data_ptr[13] -= 1;
          data_ptr[12] -= 1;
          data_ptr[11] = data_ptr[12];
          data_ptr[10] += 1;
          if(data_ptr[11] != 0)
          {
            data_ptr[11] = 0;
            data_ptr[10] -= 1;
          }
          if(data_ptr[10] != 0)
          {
            data_ptr[13] = 0;
            data_ptr[46] += data_ptr[10];
            data_ptr[10] = 0;
          }
        }
      }
      data_ptr[27] = data_ptr[46];
      data_ptr[46] = 1;
      if(data_ptr[27] != 0)
      {
        data_ptr[27] = 0;
        data_ptr[46] = 0;
      }
      data_ptr[36] = data_ptr[46];
      if(data_ptr[36] != 0)
      {
        data_ptr[46] = 1;
        data_ptr[33] = 1;
        data_ptr[36] = 0;
      }
    }
    data_ptr[37] = data_ptr[33];
  }
  data_ptr[39] = 1;
  data_ptr[37] = 1;
  do
  {
    data_ptr[13] = data_ptr[53];
    data_ptr[11] = data_ptr[53];
    data_ptr[12] = 10;
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[7] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[9] -= data_ptr[10];
    data_ptr[8] -= data_ptr[10];
    data_ptr[7] -= data_ptr[10];
    data_ptr[10] = 0;
    while(data_ptr[7] != 0)
    {
      data_ptr[7] -= 1;
      data_ptr[13] -= 1;
      data_ptr[11] = data_ptr[13];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      while(data_ptr[10] != 0)
      {
        data_ptr[10] -= 1;
        data_ptr[9] -= 1;
        data_ptr[8] -= 1;
        data_ptr[46] += 1;
        if(data_ptr[12] != 0)
        {
          data_ptr[12] = 0;
          data_ptr[46] -= 1;
        }
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[17] = data_ptr[53];
      data_ptr[16] = 5;
      data_ptr[46] = 1;
      data_ptr[15] = 0;
      while(data_ptr[14] != 0)
      {
        data_ptr[14] -= 1;
        data_ptr[16] = 0;
        data_ptr[15] = data_ptr[17];
        data_ptr[17] = 0;
        if(data_ptr[15] != 0)
        {
          data_ptr[15] = 0;
          data_ptr[46] -= 1;
        }
      }
      data_ptr[16] -= data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[16] != 0)
      {
        data_ptr[16] = 0;
        data_ptr[46] -= 1;
      }
      data_ptr[36] = data_ptr[46];
      if(data_ptr[36] != 0)
      {
        data_ptr[39] = 0;
        data_ptr[36] = 0;
      }
      data_ptr[36] = data_ptr[39];
      if(data_ptr[36] != 0)
      {
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 15;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 11;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 17;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 8;
        putchar(data_ptr[6]);
        data_ptr[6] = 100;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 39;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 16;
        putchar(data_ptr[6]);
        data_ptr[6] = 32;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = 97;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 1;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[25] = 0;
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 10;
        putchar(data_ptr[6]);
        data_ptr[36] = 0;
      }
    }
    data_ptr[36] = data_ptr[39];
    if(data_ptr[36] != 0)
    {
      data_ptr[36] = 0;
    }
    data_ptr[33] = 0;
    data_ptr[36] = data_ptr[39];
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[33] = 1;
      data_ptr[36] = 0;
    }
    data_ptr[37] = data_ptr[33];
  }
  while (data_ptr[37] != 0);
  data_ptr[39] = 1;
  data_ptr[38] = 1;
  data_ptr[53] = 0;
  data_ptr[13] = 0;
  data_ptr[12] = 10;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] += 1;
  data_ptr[8] += 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[27] = data_ptr[46];
  data_ptr[46] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[46] = 0;
  }
  data_ptr[37] = data_ptr[46];
  while(data_ptr[37] != 0)
  {
    data_ptr[38] = 1;
    data_ptr[17] = data_ptr[53];
    data_ptr[16] = 5;
    data_ptr[46] = 1;
    data_ptr[15] = 0;
    while(data_ptr[14] != 0)
    {
      data_ptr[14] -= 1;
      data_ptr[16] = 0;
      data_ptr[15] = data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[15] != 0)
      {
        data_ptr[15] = 0;
        data_ptr[46] -= 1;
      }
    }
    data_ptr[16] -= data_ptr[17];
    data_ptr[17] = 0;
    if(data_ptr[16] != 0)
    {
      data_ptr[16] = 0;
      data_ptr[46] -= 1;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[38] = 0;
      data_ptr[36] = 0;
    }
    data_ptr[36] = data_ptr[38];
    if(data_ptr[36] != 0)
    {
      data_ptr[17] = data_ptr[53];
      data_ptr[16] = 5;
      data_ptr[46] = 1;
      data_ptr[15] = 0;
      while(data_ptr[14] != 0)
      {
        data_ptr[14] -= 1;
        data_ptr[16] = 0;
        data_ptr[15] = data_ptr[17];
        data_ptr[17] = 0;
        if(data_ptr[15] != 0)
        {
          data_ptr[15] = 0;
          data_ptr[46] -= 1;
        }
      }
      data_ptr[16] -= data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[16] != 0)
      {
        data_ptr[16] = 0;
        data_ptr[46] -= 1;
      }
      data_ptr[36] = data_ptr[46];
      if(data_ptr[36] != 0)
      {
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 15;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 11;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 17;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 8;
        putchar(data_ptr[6]);
        data_ptr[6] = 100;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 39;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 16;
        putchar(data_ptr[6]);
        data_ptr[6] = 32;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = 97;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 1;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[25] = 0;
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 10;
        putchar(data_ptr[6]);
        data_ptr[36] = 0;
      }
      data_ptr[4] = 0;
      data_ptr[3] = data_ptr[53];
      data_ptr[25] = data_ptr[1];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] = 10;
      putchar(data_ptr[6]);
    }
    data_ptr[53] += 1;
    data_ptr[13] = data_ptr[53];
    data_ptr[11] = data_ptr[53];
    data_ptr[12] = 10;
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[37] = data_ptr[46];
  }
  data_ptr[38] = 1;
  data_ptr[46] = 0;
  data_ptr[53] = 0;
  data_ptr[37] = 1;
  do
  {
    data_ptr[38] = 1;
    data_ptr[53] += 1;
    data_ptr[17] = data_ptr[53];
    data_ptr[16] = 5;
    data_ptr[46] = 1;
    data_ptr[15] = 0;
    while(data_ptr[14] != 0)
    {
      data_ptr[14] -= 1;
      data_ptr[16] = 0;
      data_ptr[15] = data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[15] != 0)
      {
        data_ptr[15] = 0;
        data_ptr[46] -= 1;
      }
    }
    data_ptr[16] -= data_ptr[17];
    data_ptr[17] = 0;
    if(data_ptr[16] != 0)
    {
      data_ptr[16] = 0;
      data_ptr[46] -= 1;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[36] = 0;
      data_ptr[33] = 0;
      data_ptr[37] = 0;
      data_ptr[38] = 0;
    }
    data_ptr[36] = data_ptr[38];
    if(data_ptr[36] != 0)
    {
      data_ptr[17] = data_ptr[53];
      data_ptr[16] = 5;
      data_ptr[46] = 1;
      data_ptr[15] = 0;
      while(data_ptr[14] != 0)
      {
        data_ptr[14] -= 1;
        data_ptr[16] = 0;
        data_ptr[15] = data_ptr[17];
        data_ptr[17] = 0;
        if(data_ptr[15] != 0)
        {
          data_ptr[15] = 0;
          data_ptr[46] -= 1;
        }
      }
      data_ptr[16] -= data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[16] != 0)
      {
        data_ptr[16] = 0;
        data_ptr[46] -= 1;
      }
      data_ptr[36] = data_ptr[46];
      if(data_ptr[36] != 0)
      {
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 15;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 11;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 17;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 8;
        putchar(data_ptr[6]);
        data_ptr[6] = 100;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 39;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 16;
        putchar(data_ptr[6]);
        data_ptr[6] = 32;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 4;
        putchar(data_ptr[6]);
        data_ptr[6] = 97;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 12;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[6] += 1;
        putchar(data_ptr[6]);
        data_ptr[6] = data_ptr[31];
        data_ptr[25] = 0;
        data_ptr[6] += 10;
        putchar(data_ptr[6]);
        data_ptr[6] = 10;
        putchar(data_ptr[6]);
        data_ptr[36] = 0;
      }
      data_ptr[4] = 0;
      data_ptr[3] = data_ptr[53];
      data_ptr[25] = data_ptr[1];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[53];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] = 10;
      putchar(data_ptr[6]);
    }
    data_ptr[13] = data_ptr[53];
    data_ptr[11] = data_ptr[53];
    data_ptr[12] = 10;
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[7] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[9] -= data_ptr[10];
    data_ptr[8] -= data_ptr[10];
    data_ptr[7] -= data_ptr[10];
    data_ptr[10] = 0;
    while(data_ptr[7] != 0)
    {
      data_ptr[7] -= 1;
      data_ptr[13] -= 1;
      data_ptr[11] = data_ptr[13];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      while(data_ptr[10] != 0)
      {
        data_ptr[10] -= 1;
        data_ptr[9] -= 1;
        data_ptr[8] -= 1;
        data_ptr[46] += 1;
        if(data_ptr[12] != 0)
        {
          data_ptr[12] = 0;
          data_ptr[46] -= 1;
        }
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[37] = data_ptr[46];
  }
  while (data_ptr[37] != 0);
  data_ptr[38] = 1;
  data_ptr[17] = 0;
  data_ptr[46] = 1;
  data_ptr[15] = 0;
  data_ptr[14] += 1;
  while(data_ptr[14] != 0)
  {
    data_ptr[14] -= 1;
    data_ptr[15] = data_ptr[17];
    if(data_ptr[15] != 0)
    {
      data_ptr[15] = 0;
      data_ptr[46] -= 1;
    }
  }
  data_ptr[16] = -1 * data_ptr[17];
  if(data_ptr[16] != 0)
  {
    data_ptr[16] = 0;
    data_ptr[46] -= 1;
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[15] = 0;
  data_ptr[46] = 1;
  data_ptr[17] = 0;
  data_ptr[6] = 49;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 49;
  putchar(data_ptr[6]);
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[37] = 13;
  do
  {
    data_ptr[49] -= 1;
    data_ptr[50] -= 1;
    data_ptr[17] = data_ptr[50];
    data_ptr[46] = 1;
    data_ptr[15] = 0;
    data_ptr[14] += 1;
    while(data_ptr[14] != 0)
    {
      data_ptr[14] -= 1;
      data_ptr[15] = data_ptr[17];
      data_ptr[17] = 0;
      if(data_ptr[15] != 0)
      {
        data_ptr[15] = 0;
        data_ptr[46] -= 1;
      }
    }
    data_ptr[16] = -1 * data_ptr[17];
    data_ptr[17] = 0;
    if(data_ptr[16] != 0)
    {
      data_ptr[16] = 0;
      data_ptr[46] -= 1;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[51] += 1;
      data_ptr[52] = data_ptr[49];
      data_ptr[50] = data_ptr[48];
      data_ptr[36] = 0;
    }
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 4;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 1;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 8;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 8;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[25] = 0;
    data_ptr[6] += 11;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[37] = data_ptr[49];
  }
  while (data_ptr[37] != 0);
  data_ptr[6] = 113;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[51];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[51];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[51];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 44;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 114;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[52];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[52];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[52];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[21] = 13;
  data_ptr[20] = data_ptr[48];
  data_ptr[44] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[44] += data_ptr[18];
      data_ptr[20] = data_ptr[48];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[21] = 13;
  data_ptr[42] = 13;
  data_ptr[20] = data_ptr[48];
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[42] = data_ptr[21];
      data_ptr[20] = data_ptr[48];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[6] = 113;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[44];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[44];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[44];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 44;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[31] = 100;
  data_ptr[30] = 1;
  data_ptr[6] = 114;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[42];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[23] = 2;
  data_ptr[22] = 1;
  do
  {
    data_ptr[23] -= 1;
    data_ptr[22] += data_ptr[22];
  }
  while (data_ptr[23] != 0);
  data_ptr[42] = data_ptr[22] * 4;
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[42];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[23] = 2;
  data_ptr[22] = 1;
  do
  {
    data_ptr[23] -= 1;
    data_ptr[22] += data_ptr[22];
  }
  while (data_ptr[23] != 0);
  data_ptr[21] = 4;
  data_ptr[20] = data_ptr[22];
  data_ptr[42] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[42] += data_ptr[18];
      data_ptr[20] = data_ptr[22];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[42];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[44] = 4;
  data_ptr[23] = 3;
  data_ptr[22] = 1;
  do
  {
    data_ptr[23] -= 1;
    data_ptr[22] += data_ptr[22];
  }
  while (data_ptr[23] != 0);
  data_ptr[21] = 4;
  data_ptr[20] = data_ptr[22];
  data_ptr[42] = 0;
  do
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[42] += data_ptr[18];
      data_ptr[20] = data_ptr[22];
      data_ptr[18] = 0;
    }
  }
  while (data_ptr[21] != 0);
  data_ptr[4] = 0;
  data_ptr[3] = data_ptr[42];
  data_ptr[1] = 10;
  data_ptr[0] = 1000;
  data_ptr[2] = 10000;
  data_ptr[25] = 0;
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = 10000;
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[42];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = 10;
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[5] = data_ptr[6];
  if(data_ptr[5] != 0)
  {
    data_ptr[4] = 1;
    data_ptr[5] = 0;
  }
  data_ptr[5] = data_ptr[4];
  if(data_ptr[5] != 0)
  {
    data_ptr[0] = 48;
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[5] = 0;
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  data_ptr[6] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[6] += data_ptr[18];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] += 48;
  putchar(data_ptr[6]);
  data_ptr[21] = data_ptr[3];
  data_ptr[20] = data_ptr[2];
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[3] = data_ptr[21];
      data_ptr[20] = data_ptr[2];
      data_ptr[18] = 0;
    }
  }
  data_ptr[0] = data_ptr[2];
  data_ptr[21] = data_ptr[2];
  data_ptr[20] = data_ptr[1];
  data_ptr[2] = 0;
  while(data_ptr[21] != 0)
  {
    data_ptr[21] -= 1;
    data_ptr[20] -= 1;
    data_ptr[19] = data_ptr[20];
    data_ptr[18] += 1;
    if(data_ptr[19] != 0)
    {
      data_ptr[19] = 0;
      data_ptr[18] -= 1;
    }
    if(data_ptr[18] != 0)
    {
      data_ptr[2] += data_ptr[18];
      data_ptr[20] = data_ptr[1];
      data_ptr[18] = 0;
    }
  }
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[54] = 1;
  data_ptr[13] = 1;
  data_ptr[12] = 10;
  data_ptr[46] = 0;
  data_ptr[9] += 1;
  data_ptr[8] += 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[27] = data_ptr[46];
  data_ptr[46] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[46] = 0;
  }
  data_ptr[37] = data_ptr[46];
  while(data_ptr[37] != 0)
  {
    data_ptr[44] = 0;
    data_ptr[25] = data_ptr[54];
    if(data_ptr[25] != 0)
    {
      data_ptr[44] = data_ptr[54] * data_ptr[25];
      data_ptr[25] = 0;
    }
    data_ptr[4] = 0;
    data_ptr[3] = data_ptr[44];
    data_ptr[25] = data_ptr[1];
    data_ptr[2] = 0;
    if(data_ptr[25] != 0)
    {
      data_ptr[2] = data_ptr[1] * data_ptr[25];
      data_ptr[25] = 0;
    }
    data_ptr[25] = data_ptr[2];
    data_ptr[2] = 0;
    if(data_ptr[25] != 0)
    {
      data_ptr[2] = data_ptr[1] * data_ptr[25];
      data_ptr[25] = 0;
    }
    data_ptr[0] = data_ptr[2];
    data_ptr[25] = data_ptr[2];
    data_ptr[2] = 0;
    if(data_ptr[25] != 0)
    {
      data_ptr[2] = data_ptr[1] * data_ptr[25];
      data_ptr[25] = 0;
    }
    data_ptr[21] = data_ptr[44];
    data_ptr[20] = data_ptr[2];
    data_ptr[6] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[6] += data_ptr[18];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[5] = data_ptr[6];
    if(data_ptr[5] != 0)
    {
      data_ptr[4] = 1;
      data_ptr[5] = 0;
    }
    data_ptr[5] = data_ptr[4];
    if(data_ptr[5] != 0)
    {
      data_ptr[0] = 48;
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[5] = 0;
    }
    data_ptr[21] = data_ptr[44];
    data_ptr[20] = data_ptr[2];
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[3] = data_ptr[21];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[0] = data_ptr[2];
    data_ptr[21] = data_ptr[2];
    data_ptr[20] = data_ptr[1];
    data_ptr[2] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[2] += data_ptr[18];
        data_ptr[20] = data_ptr[1];
        data_ptr[18] = 0;
      }
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    data_ptr[6] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[6] += data_ptr[18];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[5] = data_ptr[6];
    if(data_ptr[5] != 0)
    {
      data_ptr[4] = 1;
      data_ptr[5] = 0;
    }
    data_ptr[5] = data_ptr[4];
    if(data_ptr[5] != 0)
    {
      data_ptr[0] = 48;
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[5] = 0;
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[3] = data_ptr[21];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[0] = data_ptr[2];
    data_ptr[21] = data_ptr[2];
    data_ptr[20] = data_ptr[1];
    data_ptr[2] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[2] += data_ptr[18];
        data_ptr[20] = data_ptr[1];
        data_ptr[18] = 0;
      }
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    data_ptr[6] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[6] += data_ptr[18];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[5] = data_ptr[6];
    if(data_ptr[5] != 0)
    {
      data_ptr[4] = 1;
      data_ptr[5] = 0;
    }
    data_ptr[5] = data_ptr[4];
    if(data_ptr[5] != 0)
    {
      data_ptr[0] = 48;
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[5] = 0;
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[3] = data_ptr[21];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[0] = data_ptr[2];
    data_ptr[21] = data_ptr[2];
    data_ptr[20] = data_ptr[1];
    data_ptr[2] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[2] += data_ptr[18];
        data_ptr[20] = data_ptr[1];
        data_ptr[18] = 0;
      }
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    data_ptr[6] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[6] += data_ptr[18];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[5] = data_ptr[6];
    if(data_ptr[5] != 0)
    {
      data_ptr[4] = 1;
      data_ptr[5] = 0;
    }
    data_ptr[5] = data_ptr[4];
    if(data_ptr[5] != 0)
    {
      data_ptr[0] = 48;
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[5] = 0;
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[3] = data_ptr[21];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[21] = data_ptr[2];
    data_ptr[20] = data_ptr[1];
    data_ptr[2] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[2] += data_ptr[18];
        data_ptr[20] = data_ptr[1];
        data_ptr[18] = 0;
      }
    }
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    data_ptr[6] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[6] += data_ptr[18];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[6] += 48;
    putchar(data_ptr[6]);
    data_ptr[21] = data_ptr[3];
    data_ptr[20] = data_ptr[2];
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[3] = data_ptr[21];
        data_ptr[20] = data_ptr[2];
        data_ptr[18] = 0;
      }
    }
    data_ptr[0] = data_ptr[2];
    data_ptr[21] = data_ptr[2];
    data_ptr[20] = data_ptr[1];
    data_ptr[2] = 0;
    while(data_ptr[21] != 0)
    {
      data_ptr[21] -= 1;
      data_ptr[20] -= 1;
      data_ptr[19] = data_ptr[20];
      data_ptr[18] += 1;
      if(data_ptr[19] != 0)
      {
        data_ptr[19] = 0;
        data_ptr[18] -= 1;
      }
      if(data_ptr[18] != 0)
      {
        data_ptr[2] += data_ptr[18];
        data_ptr[20] = data_ptr[1];
        data_ptr[18] = 0;
      }
    }
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[54] += 1;
    data_ptr[13] = data_ptr[54];
    data_ptr[11] = data_ptr[54];
    data_ptr[12] = 10;
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[37] = data_ptr[46];
  }
  data_ptr[13] = 2;
  data_ptr[12] = 2;
  data_ptr[46] = 0;
  data_ptr[9] += 1;
  data_ptr[8] += 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 2;
  data_ptr[12] = 2;
  data_ptr[46] = 0;
  data_ptr[7] += 1;
  data_ptr[11] = 0;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] -= data_ptr[10];
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 2;
  data_ptr[12] = 2;
  data_ptr[46] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 2;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 1;
  data_ptr[12] = 2;
  data_ptr[46] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 1;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 1;
  data_ptr[12] = 0;
  data_ptr[46] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[11] = 0;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    while(data_ptr[12] != 0)
    {
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 0;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    while(data_ptr[12] != 0)
    {
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 61;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 2;
  data_ptr[12] = 2;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 2;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 50;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 1;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 1;
  data_ptr[12] = 0;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      while(data_ptr[12] != 0)
      {
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[10] += 1;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 0;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[10] += 1;
  data_ptr[9] = (-1 * data_ptr[10] + 1);
  data_ptr[8] = (-1 * data_ptr[10] + 1);
  data_ptr[7] = (-1 * data_ptr[10] + 1);
  data_ptr[10] = 0;
  while(data_ptr[7] != 0)
  {
    data_ptr[7] -= 1;
    data_ptr[13] -= 1;
    data_ptr[11] = data_ptr[13];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      while(data_ptr[12] != 0)
      {
        data_ptr[46] -= 1;
      }
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 62;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 0;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    while(data_ptr[12] != 0)
    {
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[27] = data_ptr[46];
  data_ptr[46] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[46] = 0;
  }
  data_ptr[36] = data_ptr[46];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 60;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 33;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 60;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 58;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 78;
    putchar(data_ptr[6]);
    data_ptr[6] = 79;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[13] = 0;
  data_ptr[12] = 1;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[27] = data_ptr[46];
  data_ptr[46] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[46] = 0;
  }
  data_ptr[36] = data_ptr[46];
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = 48;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 60;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = 49;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[33] = 0;
  data_ptr[45] = 2;
  data_ptr[17] = 1;
  data_ptr[16] = 2;
  data_ptr[44] = 1;
  data_ptr[15] = 0;
  while(data_ptr[14] != 0)
  {
    data_ptr[14] -= 1;
    data_ptr[16] = 0;
    data_ptr[15] = data_ptr[17];
    data_ptr[17] = 0;
    if(data_ptr[15] != 0)
    {
      data_ptr[15] = 0;
      data_ptr[44] -= 1;
    }
  }
  data_ptr[16] -= data_ptr[17];
  data_ptr[17] = 0;
  if(data_ptr[16] != 0)
  {
    data_ptr[16] = 0;
    data_ptr[44] -= 1;
  }
  data_ptr[27] = data_ptr[44];
  data_ptr[43] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[43] = 0;
  }
  data_ptr[6] = 49;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 33;
  putchar(data_ptr[6]);
  data_ptr[6] = 61;
  putchar(data_ptr[6]);
  data_ptr[6] = 32;
  putchar(data_ptr[6]);
  data_ptr[6] = 50;
  putchar(data_ptr[6]);
  data_ptr[6] = 10;
  putchar(data_ptr[6]);
  data_ptr[32] = 2;
  data_ptr[55] = 2;
  data_ptr[46] = 0;
  data_ptr[35] = 1;
  do
  {
    data_ptr[46] = 0;
    data_ptr[27] = 0;
    data_ptr[32] = 0;
    data_ptr[34] = 0;
    data_ptr[36] = data_ptr[33];
    data_ptr[35] = 1;
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[34] = 1;
      data_ptr[36] = 0;
      data_ptr[35] = 0;
    }
    if(data_ptr[35] != 0)
    {
      data_ptr[46] = 0;
      data_ptr[34] = 0;
      data_ptr[35] = 0;
    }
  }
  while (data_ptr[35] != 0);
  data_ptr[36] = data_ptr[34];
  if(data_ptr[36] != 0)
  {
    data_ptr[44] = 48;
    data_ptr[43] = data_ptr[55];
    data_ptr[43] += 48;
    data_ptr[6] = data_ptr[43];
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
  }
  data_ptr[53] = 0;
  data_ptr[56] = 1;
  data_ptr[32] = 5;
  data_ptr[13] = 0;
  data_ptr[12] = 5;
  data_ptr[46] = 0;
  data_ptr[11] = 0;
  data_ptr[9] = 1;
  data_ptr[8] = 1;
  data_ptr[10] += 1;
  while(data_ptr[10] != 0)
  {
    data_ptr[10] -= 1;
    data_ptr[9] -= 1;
    data_ptr[8] -= 1;
    data_ptr[46] += 1;
    if(data_ptr[12] != 0)
    {
      data_ptr[12] = 0;
      data_ptr[46] -= 1;
    }
  }
  while(data_ptr[9] != 0)
  {
    data_ptr[9] -= 1;
    data_ptr[11] = data_ptr[12];
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    data_ptr[8] -= data_ptr[10];
    data_ptr[46] += data_ptr[10];
    data_ptr[10] = 0;
  }
  while(data_ptr[8] != 0)
  {
    data_ptr[8] -= 1;
    while(data_ptr[13] != 0)
    {
      data_ptr[13] -= 1;
      data_ptr[12] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      if(data_ptr[10] != 0)
      {
        data_ptr[13] = 0;
        data_ptr[46] += data_ptr[10];
        data_ptr[10] = 0;
      }
    }
  }
  data_ptr[27] = data_ptr[46];
  data_ptr[46] = 1;
  if(data_ptr[27] != 0)
  {
    data_ptr[27] = 0;
    data_ptr[46] = 0;
  }
  data_ptr[37] = data_ptr[46];
  while(data_ptr[37] != 0)
  {
    data_ptr[46] = 48;
    data_ptr[45] = data_ptr[53];
    data_ptr[45] += 48;
    data_ptr[32] = data_ptr[45];
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 11;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 4;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 11;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[25] = 0;
    data_ptr[6] += 4;
    putchar(data_ptr[6]);
    data_ptr[6] = 32;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[45];
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[53] += 1;
    data_ptr[56] += 1;
    data_ptr[36] = data_ptr[56];
    data_ptr[35] = 1;
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 5;
      data_ptr[32] = 5;
      data_ptr[36] = 0;
      data_ptr[35] = 0;
    }
    if(data_ptr[35] != 0)
    {
      data_ptr[46] = 10;
      data_ptr[32] = 10;
      data_ptr[35] = 0;
    }
    data_ptr[13] = data_ptr[53];
    data_ptr[11] = data_ptr[53];
    data_ptr[12] = data_ptr[32];
    data_ptr[46] = 0;
    data_ptr[9] += 1;
    data_ptr[8] += 1;
    data_ptr[10] += 1;
    if(data_ptr[11] != 0)
    {
      data_ptr[11] = 0;
      data_ptr[10] -= 1;
    }
    while(data_ptr[10] != 0)
    {
      data_ptr[10] -= 1;
      data_ptr[9] -= 1;
      data_ptr[8] -= 1;
      data_ptr[46] += 1;
      if(data_ptr[12] != 0)
      {
        data_ptr[12] = 0;
        data_ptr[46] -= 1;
      }
    }
    while(data_ptr[9] != 0)
    {
      data_ptr[9] -= 1;
      data_ptr[11] = data_ptr[12];
      data_ptr[10] += 1;
      if(data_ptr[11] != 0)
      {
        data_ptr[11] = 0;
        data_ptr[10] -= 1;
      }
      data_ptr[8] -= data_ptr[10];
      data_ptr[46] += data_ptr[10];
      data_ptr[10] = 0;
    }
    while(data_ptr[8] != 0)
    {
      data_ptr[8] -= 1;
      while(data_ptr[13] != 0)
      {
        data_ptr[13] -= 1;
        data_ptr[12] -= 1;
        data_ptr[11] = data_ptr[12];
        data_ptr[10] += 1;
        if(data_ptr[11] != 0)
        {
          data_ptr[11] = 0;
          data_ptr[10] -= 1;
        }
        if(data_ptr[10] != 0)
        {
          data_ptr[13] = 0;
          data_ptr[46] += data_ptr[10];
          data_ptr[10] = 0;
        }
      }
    }
    data_ptr[27] = data_ptr[46];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[37] = data_ptr[46];
  }
  data_ptr[33] = 0;
  data_ptr[36] = data_ptr[55];
  if(data_ptr[36] != 0)
  {
    data_ptr[27] = data_ptr[55];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[36] = data_ptr[46];
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[33] = 1;
      data_ptr[36] = 0;
    }
  }
  data_ptr[36] = data_ptr[33];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[46] = 1;
    data_ptr[34] = 1;
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[33] = 0;
    data_ptr[36] = data_ptr[55];
    if(data_ptr[36] != 0)
    {
      data_ptr[27] = data_ptr[55];
      data_ptr[46] = 1;
      if(data_ptr[27] != 0)
      {
        data_ptr[27] = 0;
        data_ptr[46] = 0;
      }
      data_ptr[36] = data_ptr[46];
      if(data_ptr[36] != 0)
      {
        data_ptr[46] = 1;
        data_ptr[33] = 1;
        data_ptr[36] = 0;
      }
    }
    data_ptr[27] = data_ptr[33];
    data_ptr[46] = 1;
    if(data_ptr[27] != 0)
    {
      data_ptr[27] = 0;
      data_ptr[46] = 0;
    }
    data_ptr[36] = data_ptr[46];
    data_ptr[35] = 1;
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[34] = 1;
      data_ptr[36] = 0;
      data_ptr[35] = 0;
    }
    if(data_ptr[35] != 0)
    {
      data_ptr[46] = 0;
      data_ptr[34] = 0;
      data_ptr[35] = 0;
    }
  }
  data_ptr[36] = data_ptr[34];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[46] = 1;
    data_ptr[34] = 1;
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[36] = data_ptr[55];
    data_ptr[35] = 1;
    if(data_ptr[36] != 0)
    {
      data_ptr[46] = 1;
      data_ptr[34] = 1;
      data_ptr[36] = 0;
      data_ptr[35] = 0;
    }
    if(data_ptr[35] != 0)
    {
      data_ptr[46] = 0;
      data_ptr[34] = 0;
      data_ptr[35] = 0;
    }
  }
  data_ptr[36] = data_ptr[34];
  data_ptr[35] = 1;
  if(data_ptr[36] != 0)
  {
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 2;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[6] += 11;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[25] = 0;
    data_ptr[6] += 11;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[36] = 0;
    data_ptr[35] = 0;
  }
  if(data_ptr[35] != 0)
  {
    data_ptr[6] = 98;
    putchar(data_ptr[6]);
    data_ptr[6] = 97;
    putchar(data_ptr[6]);
    data_ptr[6] = data_ptr[31];
    data_ptr[25] = 0;
    data_ptr[6] += 14;
    putchar(data_ptr[6]);
    data_ptr[6] = 10;
    putchar(data_ptr[6]);
    data_ptr[35] = 0;
  }
  data_ptr[46] = 2;
  data_ptr[45] = 3;
  data_ptr[44] = 6;
  data_ptr[25] = 0;
  data_ptr[57] = 6;
  data_ptr[37] = 1;
  do
  {
    data_ptr[45] = data_ptr[57];
    data_ptr[45] -= 1;
    data_ptr[44] = 1;
    data_ptr[46] = data_ptr[45];
    data_ptr[46] += 1;
    data_ptr[37] = data_ptr[46];
    while(data_ptr[37] != 0)
    {
      data_ptr[4] = 0;
      data_ptr[3] = data_ptr[57];
      data_ptr[25] = data_ptr[1];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[25] = data_ptr[2];
      data_ptr[2] = 0;
      if(data_ptr[25] != 0)
      {
        data_ptr[2] = data_ptr[1] * data_ptr[25];
        data_ptr[25] = 0;
      }
      data_ptr[21] = data_ptr[57];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[57];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[5] = data_ptr[6];
      if(data_ptr[5] != 0)
      {
        data_ptr[4] = 1;
        data_ptr[5] = 0;
      }
      data_ptr[5] = data_ptr[4];
      if(data_ptr[5] != 0)
      {
        data_ptr[0] = 48;
        data_ptr[6] += 48;
        putchar(data_ptr[6]);
        data_ptr[5] = 0;
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      data_ptr[6] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[6] += data_ptr[18];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] += 48;
      putchar(data_ptr[6]);
      data_ptr[21] = data_ptr[3];
      data_ptr[20] = data_ptr[2];
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[3] = data_ptr[21];
          data_ptr[20] = data_ptr[2];
          data_ptr[18] = 0;
        }
      }
      data_ptr[0] = data_ptr[2];
      data_ptr[21] = data_ptr[2];
      data_ptr[20] = data_ptr[1];
      data_ptr[2] = 0;
      while(data_ptr[21] != 0)
      {
        data_ptr[21] -= 1;
        data_ptr[20] -= 1;
        data_ptr[19] = data_ptr[20];
        data_ptr[18] += 1;
        if(data_ptr[19] != 0)
        {
          data_ptr[19] = 0;
          data_ptr[18] -= 1;
        }
        if(data_ptr[18] != 0)
        {
          data_ptr[2] += data_ptr[18];
          data_ptr[20] = data_ptr[1];
          data_ptr[18] = 0;
        }
      }
      data_ptr[6] = 10;
      putchar(data_ptr[6]);
      data_ptr[58] = 8;
      data_ptr[57] -= 1;
      data_ptr[45] = data_ptr[57];
      data_ptr[45] -= 1;
      data_ptr[57] = data_ptr[45];
      data_ptr[45] -= 1;
      data_ptr[46] = data_ptr[45];
      data_ptr[46] += 1;
      data_ptr[37] = data_ptr[46];
    }
    data_ptr[46] = 0;
  }
  while (data_ptr[37] != 0);
  data_ptr[46] = (1 + data_ptr[57]);
  data_ptr[37] = data_ptr[46];
  while(data_ptr[37] != 0)
  {
    data_ptr[46] = (1 + data_ptr[57]);
    data_ptr[37] = data_ptr[46];
  }
  data_ptr[55] = 4;
  data_ptr[57] = 2;
  data_ptr[37] = 2;
  do
  {
    data_ptr[37] = data_ptr[57];
  }
  while (data_ptr[37] != 0);
  data_ptr[46] = 5;
  data_ptr[45] = data_ptr[57];
  data_ptr[45] += 5;
  data_ptr[28] = 0;
  data_ptr[57] = data_ptr[45];
  data_ptr[29] = 0;
  return 0;
}
