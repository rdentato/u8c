#include <stdio.h>
#include "u8c.h"
#include "dbg.h"

char *s1;
char *s[] = {"Aa", "èa", "会員", "𧀀𧀍"   };
int  cp[] = {0x41, 0xE8, 0x4F1A, 0x27000 };

int main(int argc, char *argv[])
{
   int l;
   int c;

  for (int k=0; k<sizeof(s)/sizeof(char *);k++) {
    s1=s[k];
   l=u8next(s1,&c);
   dbgchk(l==k+1,"Wrong length: expected %d, got %d",k+1,l);
   dbgchk(c==cp[k],"Wrong code: expected %06X, got %06X",cp[k],c);
   l=u8next_FAST(s1,&c);
   dbgchk(l==k+1,"(FAST) Wrong length: expected %d, got %d",k+1,l);
   dbgchk(c==cp[k],"(FAST) Wrong code: expected %06X, got %06X",cp[k],c);
  }

}
