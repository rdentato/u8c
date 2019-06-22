#include <stdio.h>
#include <string.h>

#include "u8c.h"
#include "dbg.h"

int main(int argc, char *argv[])
{
  int l,l2;
  int c;
  char buf[8];
  char *s1;
  char *s[] = {"Aa", "èa", "会員", "𧀀𧀍" };

  for (int k=0; k<sizeof(s)/sizeof(char *);k++) {
    s1=s[k];
 
    l=u8next(s1,&c);
    dbgchk(l == k+1,"Expected length: %d, got %d",k+1,l);
    l2=u8encode(buf,c);
    dbgchk(l == l2,"Length should be %d, got %d",l,l2);
    dbgchk(strncmp(s1,buf,l) == 0,"Encoding error");
    dbgblk ({
      if (errno) {
        dbgmsg("    : len=%d [S %02X %02X %02X %02X] [E %02X %02X %02X %02X]",l2,s1[0],s1[1],s1[2],s1[3],buf[0],buf[1],buf[2],buf[3]);
      }
    })
    dbgchk(buf[l]=='\0',"Encoding not 0 terminated!" );
  } 
}
