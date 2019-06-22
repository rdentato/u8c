#include <stdio.h>
#include "u8c.h"
#include "dbg.h"

#define MAX_BUF_SIZE 20000

char *s1;
char  buf[MAX_BUF_SIZE];

int main(int argc, char *argv[])
{
   int l;
   int c;

   char *p;

   FILE *f;

   *buf = 0;
   int times;

   f=fopen("text1.txt","rb");
   dbgchk(f,"Unable to open text file!");
   if (f) {
      c = fread(buf,1,MAX_BUF_SIZE,f);
      dbgchk(f>0,"Read: %d bytes",c);
      fclose(f);
   }
   if (*buf) {
     times = 100000;
     dbgmsg("Timing u8next() (%d times)",times);
     dbgclk {
       for (int k=0; k<times; k++) {
         p = buf;
         while (*p) {
           l=u8next(p,&c);
           // printf("%2d %06X\n",l,c);
           p+=l;
         }
       }
     }
     dbgmsg("Timing u8next_FAST() (%d times)",times);
     dbgclk {
       for (int k=0; k<times; k++) {
         p = buf;
         while (*p) {
           l=u8next_FAST(p,&c);
           // printf("%2d %06X\n",l,c);
           p+=l;
         }
       }
     }
   }
}
