#include <stdio.h>
#include "u8c.h"
#include "dbg.h"


int main(int argc, char *argv[])
{ 
  int ret;
  int cs, ct;
  char *s, *t;
  char buf[10];

  dbgmsg("TEST: Latin");
  s="ABC"; t="ABC";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  s="ABc"; t="aBC";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  dbgmsg("TEST: Cyrillic");
  s="ЖДБ"; t="ЖДБ";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  s="ЖдБ"; t="жДб";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  s="Ж"; t="ж";
  cs = u8codepoint(s);
  ct = u8codepoint(t);
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s' %06X (%06X) != %06X (%06X)",s,t,cs,u8fold(cs),ct,u8fold(ct));

  s="ԐЂѓ"; t="ԑђЃ";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  dbgmsg("TEST: Greek");
  s="ΔΘΞ"; t="ΔΘΞ";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  s="ΔΘξ"; t="δθΞ";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  s="ἈἴὈ"; t="ἀἼὀ";
  ret = u8stricmp(s,t);
  dbgchk(ret == 0,"'%s' != '%s'",s,t);

  u8encode(0x2031,buf);
  cs = u8codepoint(buf);
  dbgchk(cs == 0x2031,"Wrong encoding!");
  t = "\u2031";
  dbgchk(strcmp(t,buf)==0,"\\u failed %02X %02x",t[0],t[1]);

  return 0;
}
