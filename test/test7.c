#include <stdio.h>
#include "u8u.h"
#include "dbg.h"


int main(int argc, char *argv[])
{ 
  int ret;
  int cs, ct;
  char *s, *t;

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

  "pippo\U002301topolino"

  return 0;
}
