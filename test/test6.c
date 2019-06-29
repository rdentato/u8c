#include <stdio.h>
#include "u8c.h"
#include "dbg.h"

char *s1;
char *s[] = {"Aa", "èa", "会員", "𧀀𧀍"   };
int  cp[] = {0x41, 0xE8, 0x4F1A, 0x27000 };

int main(int argc, char *argv[])
{
  char buf[8+1]; // Pretend it's just 8 bytes long!
  char *t="kk2";

  strcpy(buf,"ab");
  strcat(buf,"cd");

  dbgchk(strcmp(buf,"abcd") == 0,"strcpy/cat error!! '%s'",buf);

  buf[6] = 'A';
  if (argc < 100)  t = "ef";
  strncat(buf,t,2);
  dbgchk(strncmp(buf,"abcdef",6) == 0,"strcpy/cat error!! '%s'",buf);
  dbgchk(buf[6] == '\0',"strncat() does not terminate");

  buf[8] = 'B';
  strcpy(buf,"abcd");
  if (argc < 100)  t = "efgh";
  strncat(buf,t,4);
  dbgchk(strncmp(buf,"abcdefgh",8) == 0,"strcpy/cat error!! '%s'",buf);
  dbgchk(buf[8] == '\0',"strncat() did not null terminate");

  u8strcpy(buf,"ab");
  u8strcat(buf,"cd");

  dbgchk(u8strcmp(buf,"abcd") == 0,"u8strcpy/cat error!! '%s'",buf);

  buf[6] = 'A';
  if (argc < 100)  t = "ef";
  u8strncat(buf,t,2);
  dbgchk(u8strncmp(buf,"abcdef",6) == 0,"u8strcpy/cat error!! '%s'",buf);
  dbgchk(buf[6] == '\0',"u8strncat() does not terminate");

  buf[7] = 'B';
  u8strcpy(buf,"abcd");
  if (argc < 100)  t = "efgh";
  u8strncat(buf,t,4);
  dbgchk(u8strncmp(buf,"abcdefgh",8) == 0,"u8strcpy/cat error!! '%s'",buf);
  dbgchk(buf[7] != '\0',"strncat() does not terminate %02X",(unsigned char)buf[7]);
  dbgchk(buf[8] == '\0',"strncat() does not terminate %02X",(unsigned char)buf[8]);
}
