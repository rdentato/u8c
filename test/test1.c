#include <stdio.h>
#include "u8c.h"
#include "dbg.h"

char *s1;
char *s[] = {"Aa", "èa", "会員", "𧀀𧀍" };

int main(int argc, char *argv[])
{
  int l,slen,l2;
  int c;

  for (int k=0; k<sizeof(s)/sizeof(char *);k++) {
    s1=s[k];
    l=u8next(s1,&c);
    dbgchk(l == k+1,"Expected length: %d, got %d",k+1,l);
    slen = u8strlen(s1);
    dbgchk( slen == 2,"String len (in codepoints) should be 2, got %d",slen);
    l2=u8next(s1);
    dbgchk( l == l2,"Calling u8next with ch == NULL failed. Expecting %d got %d",l,l2);
  }
}
