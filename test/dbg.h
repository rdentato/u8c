/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**
**  *** DEBUG AND TESTING MACROS ***
**
**  dbgmsg(char *, ...)        --> Prints a message on stderr (works as printf(...)).
**                                 If DEBUG is not defined, do nothing.
**
**  dbgchk(test, char *, ...)  --> Perform the test. If test fails prints a message on
**                                 stderr (works as printf(...)).
**                                 If DEBUG is not defined, do nothing.
**
**  dbgclk { ... }             --> Measure and prints time needed to execute the block
**
**  _dbgmsg(char *, ...)       --> Do nothing. Used to disable the debug message.
**  _dbgchk(test, char *, ...) --> Do nothing. Used to disable the debug message.
**  _dbgclk { ... }            --> Execute the block but don't measure time.
**
**  Note that NDEBUG has precedence over DEBUG 
*/

#ifndef DBG_H__
#define DBG_H__

#ifdef DBG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#ifdef NDEBUG
#ifdef DEBUG
#undef DEBUG
#endif
#endif

#ifdef DEBUG
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define dbgexp(x) x

#define dbgmsg(...)   ((fflush(stdout), fprintf(stderr,__VA_ARGS__), \
                        fprintf(stderr," \x9%s:%d\n",__FILE__,__LINE__), \
                        fflush(stderr)))
#define dbg0(x,...)   (x)
#define dbgchk(e,...)  do {int e_=!(e); errno = 0;\
                          fflush(stdout); /* Ensure dbg message appears *after* pending stdout prints */ \
                          fprintf(stderr,"%s: (%s) \x9%s:%d\n",(e_?"FAIL":"PASS"),#e,__FILE__,__LINE__); \
                          if (e_ && *(dbgexp(dbg0(__VA_ARGS__)))) {  \
                            fprintf(stderr,"    : " __VA_ARGS__); fputc('\n',stderr);\
                          } \
                          fflush(stderr); errno = e_; \
                      } while(0) 

#define dbgclk        for (clock_t dbg_clk = clock(); \
                           dbg_clk != (clock_t)-1; \
                           dbgmsg("TIME: %ld/%ld sec.", \
                                   (long int)(clock()-dbg_clk), (long int)CLOCKS_PER_SEC), \
                              dbg_clk = (clock_t)-1 )
#define dbgblk(x) do { x  ; errno = 0;} while (0);

#else
#define dbgmsg(...)
#define dbgchk(e,f,...)
#define dbgclk
#define dbgblk(x)
#endif

#define _dbgmsg(...)
#define _dbgchk(e,f,...)
#define _dbgclk 
#define _dbgblk(x)

#endif // __DBG_H__

/*<<<>>>*/
/*  ************ TESTS STATISTICS *************
**  Compile with:
**     cp dbg.h dbgstat.c; cc -DDBGSTAT -Wall -o dbgstat dbgstat.c
*/

#ifdef DBGSTAT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 1024
char s[MAXLEN];

int main(int argc, char *argv[])
{
  int n_fail = 0, n_pass = 0;
  char *p;

  while ((p=fgets(s,MAXLEN,stdin))) {
         if (strncmp(p,"PASS: ",6)==0) n_pass++; 
    else if (strncmp(p,"FAIL: ",6)==0) n_fail++; 
  }

  printf("FAIL: %d\nPASS: %d\n",n_fail,n_pass);
  exit(n_fail != 0 || n_pass == 0);
}

#endif // DBGSTAT

/*  ************ TEST SUITE *************
**  Compile with:
**     cp dbg.h dbgtest.c; cc -DDEBUG -DDBGTEST -Wall -o dbgtest dbgtest.c
*/

#ifdef DBGTEST
int main(int argc, char *argv[])
{
  int x;
  dbgmsg("Test %s (argc: %d)","message 1",argc);

  x=0;
  dbgmsg("TEST: (1>x) with x=%d",x);
  dbgchk(1>x,"x=%d\n",x);

  x=1;
  dbgmsg("TEST: (1>x) with x=%d",x);
  dbgchk(1>x,"x=%d\n",x);

  x=2;
  dbgmsg("TEST: (1>x) with x=%d",x);
  dbgchk(1>x,"x=%d\n",x);

  dbgblk({
    int e = errno;
    if (e) {
      dbgmsg("Sigh it failed :(%d)",e);
    }
  })

  x=3;
  dbgmsg("TEST: (1>x) with x=%d (no message on fail)",x);
  dbgchk(1>x,"");

  _dbgblk({
    int e = errno;
    if (e) {
      dbgmsg("Sigh it failed: (%d) but I'll never know",e);
    }
  })

  x = 100000;
  dbgmsg("Testing count to %d",x);
  dbgclk {
    for (int k=0; k<x; k++);
  }

  x = 100000000;
  dbgmsg("Testing count to %d",x);
  dbgclk {
    for (int k=0; k<x; k++) ;
  }
}

#endif // DBG_TEST

