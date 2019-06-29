#include <stdio.h>
#include "u8c.h"
#include "dbg.h"

char *s1;
char *s[] = {"Aa", "èa", "会員", "𧀀𧀍"   };
int  cp[] = {0x41, 0xE8, 0x4F1A, 0x27000 };

int main(int argc, char *argv[])
{
  int k;
  char buf[8];
  char *t;

  t = "bèa";
  buf[0] = '\0';
  u8strncpy(buf,t,8);
  for (k=0; k<5; k++) {
    dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  }

  t = "bèa";
  buf[0] = '\0';
  u8strncpy(buf,t,3);
  for (k=0; k<3; k++) {
    dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  }

  t = "bèa";
  buf[0] = '\0';
  u8strncpy(buf,t,2);
  k=0; dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  k=1; dbgchk(buf[k] != t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
       dbgchk(buf[1] == '\0',"u8nstrcpy failed at %d (%02X",k,buf[1],t[1]);
  
  t = "èa";
  buf[0] = '\0';
  u8strncpy(buf,t,2);
  k=0; dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  k=1; dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);

  t = "èa";
  buf[0] = '\0';
  u8strncpy(buf,t,1);
  k=0; dbgchk(buf[k] != t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
       dbgchk(buf[0] == '\0',"u8nstrcpy failed at %d (%02X",k,buf[0],t[0]);
  

  t = "𧀀𧀍";
  _dbgblk({
    for (k=0;k<8;k++)  printf("%02X ",(unsigned char)t[k]);
    printf("\n");
  })

  buf[0] = '\0';
  u8strncpy(buf,t,8);
  for (k=0; k<8; k++) {
    dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  }
  
  buf[0] = '\0';
  u8strncpy(buf,t,7);
  for (k=0; k<4; k++) {
    dbgchk(buf[k] == t[k],"u8nstrcpy failed at %d (%02X != %02X)",k,buf[k],t[k]);
  }
  dbgchk(buf[4] == '\0',"u8nstrcpy failed at %d (%02X",k,buf[4],t[4]);

  buf[0] = '\0';
  u8strncpy(buf,t,3);
  dbgchk(buf[0] == '\0',"u8nstrcpy failed at %d (%02X",k,buf[0],t[0]);
}
